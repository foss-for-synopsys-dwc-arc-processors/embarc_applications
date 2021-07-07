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
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <title>饅丁蘑蕈註冊系統</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">  
</head>
<body>
<form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
    <div style="width: 30%;padding-top: 18%;margin: auto;overflow: hidden;">
        <h1>饅丁蘑蕈註冊系統</h1>    
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        帳號 : <input type"text" name="ac" pattern="^[A-Za-z0-9_]{1,10}$">
        <br>
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        密碼 : <input type="password" id="pw" name="pw" pattern="(?=.*\d)(?=.*[a-z]).{7,15}[^()/><\][\\\x22,;|]+" title="長度至少為8，須包含英文字及數字"onkeyup="validate()">
        <br>
        確認密碼 : <input type="password" id="repw" name="repw"  pattern="(?=.*\d)(?=.*[a-z]).{7,15}[^()/><\][\\\x22,;|]+" title="長度至少為8，須包含英文字及數字"onkeyup="validate()">
        <span id="tishi"></span>
        <br>        
        <br>
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type="submit" id="submit" value="送出" ID="submit_button" class="w3-button w3-black">
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type="reset" value="重填" class="w3-button w3-black"> 
    </div>    
</form>
</body>
    <script>
        function validate() 
        {
         var pw = document.getElementById("pw").value;
         var repw = document.getElementById("repw").value;
         if(pw == repw)
         {
            document.getElementById("tishi").innerHTML="<font color='green'> </font>";
            document.getElementById("submit").disabled = false;
         }
         else
         {
            document.getElementById("tishi").innerHTML="<font color='red'>密碼不相同</font>";
            document.getElementById("submit").disabled = true;
         }
        }
    </script>
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
            /*$like1 = $_POST["like1"];
            $like2 = $_POST["like2"];
            $like3 = $_POST["like3"];*/
        if (empty($result)) {
            $sql = "INSERT INTO `sign_data` (`account`, `Authority`, `password`) VALUES ('" . $account . "','0', '" . $password . "')";
            mysqli_query($con, $sql);       
                //echo $sql;
            echo '<script> alert("註冊成功!");</script>';
            header("Refresh:0; url=sign_in.php");            
        } 
        else {
                //若符合顯示帳號、密碼資訊
            echo '<script> alert("帳號已註冊，請更換!");</script>';                  
        }
    }
    ?>
</html>


<!--INSERT INTO `sign_data` (`account`, `Authority`, `password`, `like1`, `like2`, `like3`) VALUES ('account1', '0', '123456789', NULL, NULL, NULL);-->