<?php session_start(); ?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <title>登出中...</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
    
</body>
</html>
<?php
//將session清空
unset($_SESSION['username']);
echo '登出中......';
header("Refresh:0; url=sign_in.php");
?>