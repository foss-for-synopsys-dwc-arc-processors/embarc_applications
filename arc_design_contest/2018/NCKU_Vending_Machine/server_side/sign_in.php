<?php session_start(); ?>
<?php //連接上mysql
if (isset($_SESSION['username'])) {
    header("Refresh:0; url=item.php");
}
?>
<?php // 連上mysql
$con = mysqli_connect("127.0.0.1", "account", "password", "projectarc");
if (!$con) {
    echo "連接錯誤";
    die('Error, insert query failed');
} else {
        //echo"連接成功!!";
}
mysqli_query($con, "set names utf8");
?>
<?php
function test_input($data)//處理資料，防止錯誤
{
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}

if ($_SERVER["REQUEST_METHOD"] == "POST")//獲得html form post過來的值
{
    $account = test_input($_POST["ac"]);
    $pw = $_POST["pw"];
    $password = hash('sha256', $pw);
    $sql = "SELECT * FROM `sign_data` WHERE `account` = \"" . $account . "\";";
    $search = mysqli_query($con, $sql);
    $result = mysqli_fetch_array($search);
    if (empty($result)) {
                //沒有找到帳密
        echo "<script>alert(\"登入失敗\")</script>";
        header("Refresh:0");//重新整理
    } else {
        //找到帳密 
        $sql = "SELECT * FROM `sign_data` WHERE `account` LIKE \"" . $account . "\";";
        $search = mysqli_query($con, $sql);
        $result = mysqli_fetch_row($search);
        if ($result[2] == "$password")//密碼正確
        {
            echo "<script>messege(1)</script>";
            if ($result[1] == 1) {
                $_SESSION['username'] = $account;
                header("Refresh:0; url=item.php");
            } else {
                echo "<script>alert(\"權限不足\")</script>";
                header("Refresh:0");//重新整理
            }
        } else {
            echo "<script>alert(\"登入失敗\")</script>";
            header("Refresh:0");//重新整理
        }
    }
}
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <title>饅丁蘑蕈登入系統</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
    <script>
        function messege($e)
        {
            if($e==0)//fail
            {
                alert("帳號或密碼錯誤!");
            }
        }
    </script>  
</head>
<body>
<div style="width:100%;height:10vh">
    <div style="width: 50%;margin: 40vh auto;text-align:  center;">
    <h1>饅丁蘑蕈登入系統</h1>
    <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
        帳號 : <input type"text" name="ac" pattern="^[A-Za-z0-9_]{1,10}$">
        <br>
        密碼 : <input type="password" id="pw" name="pw" pattern="(?=.*\d)(?=.*[a-z]).{7,15}[^()/><\][\\\x22,;|]+" title="長度至少為8，須包含英文字及數字"onkeyup="validate()">
        <br>
        <input type="submit" id="submit" value="登入" ID="submit_button" class="w3-button w3-black">
        <input type="reset" value="重填" class="w3-button w3-black">
     </form>
    </div>
</div>
</body>
</html>

