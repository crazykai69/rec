<?php
 
  $fwc = 0;
  $ftl = 0;
  $ftr = 0;
  $fgz = 0;
  $rki = 0.0;
  $raz = 0.0;
  $lgf = "";

  if ( isset( $_GET["fwc"] ) ) $fwc = $_GET["fwc"];
  if ( isset( $_GET["ftl"] ) ) $ftl = $_GET["ftl"];
  if ( isset( $_GET["ftr"] ) ) $ftr = $_GET["ftr"];
  if ( isset( $_GET["fto"] ) ) $fto = $_GET["fto"];
  if ( isset( $_GET["ftw"] ) ) $ftw = $_GET["ftw"];
  if ( isset( $_GET["fgz"] ) ) $fgz = $_GET["fgz"];
  if ( isset( $_GET["raz"] ) ) $raz = $_GET["raz"];
  if ( isset( $_GET["rki"] ) ) $rki = $_GET["rki"];
  if ( isset( $_GET["lgf"] ) ) $lgf = $_GET["lgf"];

  $fn = "/var/tmp/stat.txt";

  echo( $fn . "<br>" );

  $myfile = fopen($fn, "w") or die("Unable to open file!");
  $txt = "fwc#$fwc#Fenster WC: " . wintx($fwc) . "\n";
  fwrite($myfile, $txt);
  echo $txt . "<br>";
  $txt = "ftl#$ftl#Terasse links: " . wintx($ftl) . "\n";
  fwrite($myfile, $txt);
  echo $txt . "<br>";
  $txt = "ftr#$ftr#Terasse rechts: " . wintx($ftr) . "\n";
  fwrite($myfile, $txt);
  echo $txt . "<br>";
  $txt = "fgz#$fgz#Gaestezimmer: " . wintx($fgz) . "\n";
  fwrite($myfile, $txt);
  echo $txt . "<br>";

  $txt = "ftw#$fgz#Terrasse West: " . wintx($ftw) . "\n";
  fwrite($myfile, $txt);
  $txt = "fto#$fgz#Terrasse Ost: " . wintx($fto) . "\n";
  fwrite($myfile, $txt);

  $txt = "rki#$rki#Rollladen Kind: " . round($rki*100,0) . " % offen #255#255#255#5\n";
  fwrite($myfile, $txt);
  echo $txt . "<br>";
  $txt = "raz#$raz#Rollladen AZ: " . round($raz*100,0) . " % offen #255#255#255#5\n";
  fwrite($myfile, $txt);
  echo $txt . "<br>";
  $txt = "lgf#$lgf#Licht Flutlicht: " . booltx($lgf) . "\n";
  fwrite($myfile, $txt);
  echo $txt . "<br>";
  fclose($myfile);

  if ($fwc == 0 && $ftl == 0 && $ftr == 0 && $fgz == 0 && $ftl==0 && $fto==0) 
  {
	$txt = "OK\n";
	$frm = "0#255#0#10\n";
  }
  else
  {
	$txt = "Offen\n";
	$frm = "255#0#0#12\n";
  }
  
  $fn = "/var/tmp/rt1.txt";
  $myfile = fopen($fn, "w") or die("Unable to open file!");
  fwrite($myfile, $frm);
  fwrite($myfile, $txt);
  fclose($myfile);


  function wintx( $nums )
  {
    if( $nums == 0 ) return "zu#0#255#0#5";
    if( $nums == 2 ) return "offen#255#0#0#5";
    if( $nums == 1 ) return "gekippt#255#0#0#5";

    return "";
  }

  function booltx( $b )
  {
    if( $b == "false" ) return "aus#0#255#0#5";
    if( $b == "true" ) return "an#255#0#0#5";

    return "";
  }

?> 
