<head>
<title>monitor soil humidity</title>
</head>
<?php
require ('../jpgraph/src/jpgraph.php');
require ('../jpgraph/src/jpgraph_line.php');
date_default_timezone('Asia/Tokyo');
$time = array();
$hum = array();
$server = "https;//****.firebaseio.com/";
$Date = date("Y/n/j");
$auth = ".json?auth=*****";
$url = $server.$Date.$auth;
$json = file_get_contents($url);
$json = mb_convert_encoding($json, 'UTF8', 'ASCII,JIS,UTF-8,EUC-JP,SJIS-WIN');
if($json === NULL){
?>
    <p>failed</p>
<?php
}else{
    $data = json_decode($json, true);
    $humidity = array_values($data);
    if($humidity === NULL){
?>
        <p> null humidity</p>
<?php
    }else{
        $graph = new Graph(900, 1024);
        $graph->SetScale("textlin");

        $lineplot = new LinePlot($humidity);
        $lineplot->SetColor("blue");

        $graph->Add($lineplot);
        $graph->Stroke("image/graph.png");
?>
        <img src="./image/graph.png">
        <tr>
        <table border="1">
        <th>time</th>
        <th>humidity</th>
        </tr>
<?php
        foreach($data as $key => $value){
            array_push($time, $key);
            array_push($hum, $value);
        }
        for($i = count($hum) - 10; $i < count($hum); $i++){

?>
            <tr>
            <td><?php echo $time[i]; ?></td>
            <td><?php echo $hum[i]; ?></td>
            </tr>
<?php
        }
    }
?>
    </table>
<?php
}
?>

