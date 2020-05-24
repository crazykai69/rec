<?php
echo "<html><body>GetWeather V2<hr>";
$doc = new DOMDocument();

//shell_exec( 'sudo wget http://weather/livedata.htm' );

//$htmlLoaded = $doc->loadHTMLFile( 'livedata.htm' );

$htmlLoaded = $doc->load( 'http://192.168.178.41/livedata.htm' );

$url = "http://www.kai-merklein.de/avrtl/updweather.php?station=1";

if( $htmlLoaded )
{
  echo "html loaded<hr>";
  $atts = $doc->getElementsByTagName("input");

  foreach( $atts as $att )
  {
    $name  = $att->getAttribute( "name" );
    $value = $att->getAttribute( "value" );
	
	echo "$name: $value<br>";
	
	if( $name == "inTemp" ) $url = $url . "&it=". $value;
	if( $name == "outTemp" ) $url = $url . "&ot=". $value;
	if( $name == "inHumi" ) $url = $url . "&ih=". $value;
	if( $name == "outHumi" ) $url = $url . "&oh=". $value;
	if( $name == "windir" ) $url = $url . "&wd=". $value;
	if( $name == "avgwind" ) $url = $url . "&wf=". $value;
	if( $name == "gustspeed" ) $url = $url . "&gs=". $value;
	if( $name == "solarrad" ) $url = $url . "&so=". $value;
	if( $name == "rainofhourly" ) $url = $url . "&rh=". $value;
	if( $name == "rainofdaily" ) $url = $url . "&rd=". $value;
	if( $name == "RelPress" ) $url = $url . "&pr=". $value;
	
  }
  echo $url;
  
  echo file_get_contents( $url );
}
else echo ("failed to load html");
echo "</body></html>";

shell_exec( 'sudo rm livedata* ' );
shell_exec( 'sudo rm updweather* ' );

?>
