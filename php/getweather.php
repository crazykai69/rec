<?php
echo "<html><body>GetWeather<hr>";
$doc = new DOMDocument();

shell_exec( 'sudo wget http://192.168.178.70/livedata.htm' );

$htmlLoaded = $doc->loadHTMLFile( 'livedata.htm' );

if( $htmlLoaded )
{
  echo "html loaded<hr>";
  $atts = $doc->getElementsByTagName("input");

  foreach( $atts as $att )
  {
//    echo $att->nodeValue, PHP_EOL;
    echo $att->getAttribute( "name" ) . ": ";
    echo $att->getAttribute( "value" ) . "<hr>";
  }
}
else echo ("failed to load html");
echo "</body></html>";

shell_exec( 'sudo rm livedata* ' );

?>
