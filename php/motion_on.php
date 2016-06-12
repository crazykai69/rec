<?php
echo( shell_exec('sudo /opt/rec/cprb2.sh') );
echo( shell_exec('sudo /opt/rec/on.sh') );
echo( 'Motion On triggered');

echo( shell_exec('/opt/rec/rmrb2.sh >/dev/null 2>/dev/null &') );
echo( 'RMRB2 triggered');
?>
