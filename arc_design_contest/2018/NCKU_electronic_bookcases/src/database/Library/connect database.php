<?php
$localIP = getHostByName(getHostName());
define('DB_USER', /*depend on the server side */);
define('DB_PWD', /*depend on the server side */);
define('DB_HOST', $localIP);
define('DB_NAME', 'Library');

$dbcon=mysqli_connect(DB_HOST,DB_USER,DB_PWD,DB_NAME) or die('error mysql connect');

$GLOBALS['dbcon']=$dbcon;

?>

