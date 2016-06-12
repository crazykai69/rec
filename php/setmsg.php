 <?php
 
 $cr = 0;
 $cg = 0;
 $cb = 0;
 $s = 10;

 if ( isset( $_GET["r"] ) ) $cr = $_GET["r"];
 if ( isset( $_GET["g"] ) ) $cg = $_GET["g"];
 if ( isset( $_GET["b"] ) ) $cb = $_GET["b"];
 if ( isset( $_GET["s"] ) ) $s = $_GET["s"];

 $fnum = $_GET["fn"];
 $col = $_GET["c"];
 
 $msg = $_GET["m"];
 
 $fn = "/var/tmp/" . $fnum . ".txt";

echo( $fn ) . '<br>';

 
$frm = $cr . "#" . $cg . "#" . $cb . "#" . $s . "#green";

echo( $frm );

$myfile = fopen($fn, "w") or die("Unable to open file!");
$txt = $frm . "\n";
fwrite($myfile, $txt);
$txt = $msg . "\n";
fwrite($myfile, $txt);
fclose($myfile);
?> 
