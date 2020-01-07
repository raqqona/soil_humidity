#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <String.h>
#define JST 3600* 9
WiFiClientSecure client;

const char* ssid ="****";
const char* password = "****";
const char* server = "tutorial-e5a73.firebaseio.com";
const char* firebase_auth = "*******"; // firebase's secret
String user_path = "******";

uint32_t WebGet_LastTime = 0;
struct tm timeInfo;
String year; 
String mon;
String day;


void setup(){
  Serial.begin(115200);
  while (!Serial);
  WiFi_conect();
  configTime(JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp"); //set NTP
  
  WebGet_LastTime = 200000;
}

void loop(){
  String now_time = create_time(); //get time
  int value = analogRead(35); //get humidity
  SendPatchRequest( now_time, String( value ) ); //send the data
  delay(280000);
}

//WiFiconnect function
void WiFi_conect(){
  WiFi.begin(ssid, password); //connect to WiFi
  while(WiFi.status() != WL_CONNECTED){ 
      Serial.print(".");
      delay(1000); // reconect 1 sencond later
  }
  Serial.println("WiFi connected");
  Serial.println("IP address");
  Serial.println(WiFi.localIP()); //esp32's localIPaddress
}

//get time
String create_time(){
  String now_time;
  getLocalTime(&timeInfo); //get time

  year = String(timeInfo.tm_year + 1900); //convert to A.D.
  mon = String(timeInfo.tm_mon + 1); //convert to real month
  day = String(timeInfo.tm_mday);

  if(int(timeInfo.tm_hour) / 10 == 0){ //add "0" when the sec is single digit
    String now_hour = "0" + String(timeInfo.tm_hour);
    now_time += now_hour;
  }else{
    String now_hour = String(timeInfo.tm_hour);
    now_time += now_hour;
  }
  now_time += ":";
  if(int(timeInfo.tm_min) / 10 == 0){ //add "0" when the sec is single digit
    String now_min = "0" + String(timeInfo.tm_min);
    now_time += now_min;
  }else{
    String now_min = String(timeInfo.tm_min);
    now_time += now_min;
  }
  now_time += ":";
  if(int(timeInfo.tm_sec) / 10 == 0){ //add "0" when the sec is single digit
    String now_sec= "0" + String(timeInfo.tm_sec);
    now_time += now_sec;
  }else{
    String now_sec = String(timeInfo.tm_sec);
    now_time += now_sec;
  }
  return now_time;
}

//write DB of firebase
void SendPatchRequest(String str1, String str2){
  if (!client.connect(server, 443)){ //connect DB
    Serial.println("Connection Failed!");
  }else{
    Serial.println("Connected to sucsess!");
    String URL;
    URL = "PATCH "; //http request method
    URL += String(user_path);
    URL += year + "/" + mon + "/" + day;
    URL += ".json?auth=";
    URL += String(firebase_auth);
    URL += " HTTP/1.1\r\n";

    String body = "{\"" + str1 + "\":\"" + str2 + "\"}";

    String head;
    head = "Host: ";
    head += String(server) + "\r\n";
    head += "Connection: keep-alive\r\n";
    head += "Content-Length: ";
    head += String(body.length()) + "\r\n";
    head += "\r\n";

    client.print(URL);
    client.print(head);
    client.print(body);

    Serial.println("####### Send HTTP Patch Request #######");
    Serial.print(URL);
    Serial.print(head);
    Serial.print(body);

    checkServerRespons();

    delay(2);
    client.stop();
    delay(2);
  }
}

//receive response
void checkServerRespons(){
  Serial.println("####### Firebase server HTTP Response #######");
  while(client.connected()){
    String res = client.readStringUntil('\n');
    Serial.println(res);
    if(res == "\r"){
      Serial.println("-------------- Response Headers Recived");
      break;
    }
  }

  while(client.available()){
    char c = client.read();
    Serial.print(c);
  }
  Serial.println("\r\n------------ Body Received");
}
