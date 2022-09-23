<!DOCTYPE html>
<html>
<head>
<title>FORMULAPHP</title>
</head>
<body>
<h1><?php echo "Hello World" ?></h1>
<pre>
<?php
echo "hola formula phpiana\n";
echo "-----REQUEST-----\n";
print_r($_REQUEST); 
echo "-----SERVER------\n";
print_r($_SERVER);
echo "-----POST file---\n";
print_r($_FILES);
/*echo "-----BODY--------\n";
$f = fopen( 'php://stdin', 'r' );
$i = 0;
while( ($line = fgets( $f )) && $i < 10) {
  echo $line;
  $i++;
}
fclose( $f );
 */
?> 
</pre>
</body>
</html>
