<?php 
// //$fp = fsockopen("127.0.0.1", 9527, $errno, $errstr, 5);//test
// if (!$fp) {
//     echo "$errstr ($errno)<br />\n";
// }
//fetch item data from mysql to the file
error_reporting(E_ALL & ~E_NOTICE);//取消notice顯示
//$fp = fsockopen("127.0.0.1", 9527, $errno, $errstr, 5);
$con = mysqli_connect("127.0.0.1", "peter1719", "Good870216", "projectarc");
if (!$con) {
    echo "連接錯誤";
    die('Error, insert query failed');
} else {
        //echo"連接成功!!";
}
mysqli_query($con, "set names utf8");

/*********************function*****************************/
function make_string($data)
{//make string 6 bits
    $size = strlen($data);
    $space_num = 6 - $size;
    $buffer = $data;
    for ($i = 0; $i < $space_num; $i++) {
        $buffer .= " ";
    }
    return $buffer;
}

function test_input($data)//處理資料，防止錯誤
{
    return $data;
}

function update($user, $reject, $con)//update the data base
{
    if($reject == 2)
    {
        $data = mysqli_query($con, "select * from item");
        $size = mysqli_num_rows($data);//陣列大小
        $myfile = fopen("com2arc.txt", "w") or die("Unable to open file!");
        $txt = "0\t0\t" . "$user" . "\n";
        fwrite($myfile, $txt);
        for ($i = 1; $i <= $size; $i++) {
            $rs = mysqli_fetch_row($data);
            $order = $rs[0];
            $name = make_string($rs[1]);
            $Price = $rs[2];
            $Type = make_string($rs[3]);
            $Exp_Date = $rs[4];
            $num = $rs[5];
            $txt = "$order" . "\t" . "$name" . "\t" . "$Price" . "\t" . "$Type" . "\t" . " $Exp_Date" . "\t" . "$num" . "\n";
            fwrite($myfile, $txt);
        }
        $fileData = file_get_contents("com2arc.txt");
        fclose($myfile);
        return $fileData;
    }
    else if ($reject == 1) {
        $data = mysqli_query($con, "select * from item");
        $size = mysqli_num_rows($data);//陣列大小
        $myfile = fopen("com2arc.txt", "w") or die("Unable to open file!");
        $txt = "0\t4\t" . "$user" . "\n";
        fwrite($myfile, $txt);
        for ($i = 1; $i <= $size; $i++) {
            $rs = mysqli_fetch_row($data);
            $order = $rs[0];
            $name = make_string($rs[1]);
            $Price = $rs[2];
            $Type = make_string($rs[3]);
            $Exp_Date = $rs[4];
            $num = $rs[5];
            $txt = "$order" . "\t" . "$name" . "\t" . "$Price" . "\t" . "$Type" . "\t" . " $Exp_Date" . "\t" . "$num" . "\n";
            fwrite($myfile, $txt);
        }
        $fileData = file_get_contents("com2arc.txt");
        fclose($myfile);
        return $fileData;
    } else {
        $sql = "SELECT * FROM `costomer` WHERE `Rfid` = \"" . $user . "\";";
        $search = mysqli_query($con, $sql);
        $result = mysqli_fetch_row($search);
        $like1 = $result[2];//1
        $like2 = $result[3];//0
        $like3 = $result[4];//0
        $like4 = $result[5];//0
        $like = array($like1, $like2, $like3, $like4);
        $array_size = $like1 + $like2 + $like3 + $like4 - 1 + 4;//4
        $recommand_weight = array();
        for ($i = 0; $i < 4; $i++) {
            for ($j = 0; $j < $like[$i] + 1; $j++) {//2+1+1+1=5
                array_push($recommand_weight, $i + 1);//11234
            }
        }
        $recommand_item = $recommand_weight[rand(0, $array_size)];

        $data = mysqli_query($con, "select * from item");
        $size = mysqli_num_rows($data);//陣列大小
        $myfile = fopen("com2arc.txt", "w") or die("Unable to open file!");
        $txt = "$recommand_item" . "\t3\t" . "$user" . "\n";
        fwrite($myfile, $txt);
        for ($i = 1; $i <= $size; $i++) {
            $rs = mysqli_fetch_row($data);
            $order = $rs[0];
            $name = make_string($rs[1]);
            $Price = $rs[2];
            $Type = make_string($rs[3]);
            $Exp_Date = $rs[4];
            $num = $rs[5];
            $txt = "$order" . "\t" . "$name" . "\t" . "$Price" . "\t" . "$Type" . "\t" . "$Exp_Date" . "\t" . "$num" . "\n";
            fwrite($myfile, $txt);
        }
        $fileData = file_get_contents("com2arc.txt");
        fclose($myfile);
        return $fileData;
    //$recommand = rand(1,4);
    }
}
/*********************************************************************************/
//echo "Loading... \n";

if (isset($_POST["post"]))//獲得html form post過來的值
{
    $read = $_POST["post"];
   // $read = "26.25\t1\t1\t1234\t2\t3\t4\t5";
    //echo $read;
    $txt = test_input($read);
    $myfile = fopen("arc2php.txt", "w") or die("Unable to open file!");
    fwrite($myfile, $txt);
    fclose($myfile);
    $myfile = fopen("arc2php.txt", "r");
    $value = fgets($myfile);//first line
    $value = rtrim($value);//取消每個字元後的空白
    $value = explode("\t", $value);//用tab分開字串
    $temp = $value[0];
    if ($temp == 00.00) {//初始化
        $data = mysqli_query($con, "select * from item");
        $size = mysqli_num_rows($data);//陣列大小
        $myfile = fopen("com2arc.txt", "w") or die("Unable to open file!");
        $txt = "0\t0\t0000\n";
        fwrite($myfile, $txt);
        for ($i = 1; $i <= $size; $i++) {
            $rs = mysqli_fetch_row($data);
            $order = $rs[0];
            $name = make_string($rs[1]);
            $Price = $rs[2];
            $Type = make_string($rs[3]);
            $Exp_Date = $rs[4];
            $num = $rs[5];
            $txt = "$order" . "\t" . "$name" . "\t" . "$Price" . "\t" . "$Type" . "\t" . " $Exp_Date" . "\t" . "$num" . "\n";
            fwrite($myfile, $txt);
        }
        $fileData = file_get_contents("com2arc.txt");
        echo $fileData;
        fclose($myfile);
    } else {
        /* $read = $_POST["post"];////從arc傳上來儲存
        $txt = test_input($read);
        $myfile = fopen("arc2php.txt", "w") or die("Unable to open file!");
        fwrite($myfile, $txt);
        echo $myfile;
        fclose($myfile);
        echo $read;
        echo "read done!!\n"; */

        /*******************更新database*********************/
        date_default_timezone_set("Asia/Taipei");//設定時區
        $Time = date("Y-m-d H:i:s");//24-hours format
        //echo $Time;
        $file = fopen("arc2php.txt", "r");
        $value = fgets($file);
        $value = rtrim($value);//取消每個字元後的空白            
        $value = explode("\t", $value);//用tab分開字串
        //echo $ID . "\n";
        $Temp = $value[0];
        //echo $Temp . "\n";
        $target_item = $value[1];
        $state = $value[2];
        $user = $value[3];
        $num = array($value[4], $value[5], $value[6], $value[7]);
        //顯示出文件內容
        //echo $value[0] . "\t" . $value[1] . "\t" . $value[2] . "\t" . $value[3] . "\t" . $value[4] . "\t" . $value[5] . "<br>";
        $data = mysqli_query($con, "SELECT * FROM `state`");
        $data = mysqli_query($con, "UPDATE `state` SET `Temperature` = '" . $Temp . "', `Time` = '" . $Time . "' WHERE `state`.`M_ID` = '1' ;");
        $data = mysqli_query($con, "SELECT * FROM `item`");
        for ($i = 1; $i <= 4; $i++) {
            $data = mysqli_query($con, "UPDATE `item` SET `Num` = '" . $num[$i - 1] . "' WHERE `item`.`序號` = " . $i . ";");
        }
        if($state == 0)
        {
            echo update($user, 2, $con);
        }
        /*****************判斷post的內容**************************** */
        /*temp target_item status user*/
        if ($state == 1)//1.uer exist? 2.判斷餘額是否足夠 
        {
            $sql = "SELECT * FROM `costomer` WHERE `Rfid` = \"" . $user . "\";";
            $search = mysqli_query($con, $sql);
            $result = mysqli_fetch_array($search);
            if (empty($result)) {//since rfid not found ,return reject at status
                echo update($user, 1, $con);
            } else {//having found rfid, then check balance
                //get balance from database
                $sql = "SELECT * FROM `costomer` WHERE `Rfid` = \"" . $user . "\";";
                $search = mysqli_query($con, $sql);
                $result = mysqli_fetch_row($search);

                $sql2 = "SELECT * FROM `item` WHERE `序號` = \"" . $target_item . "\";";
                $search2 = mysqli_query($con, $sql2);
                $result2 = mysqli_fetch_row($search2);
                $cost = $result2[2];//get item cost
                if ($result[6] < $cost)//have no enough, return reject at status
                {
                    echo update($user, 1, $con);
                } else {//return 3 at status and recomand item
                    echo update($user, 0, $con);
                }
            }
        } else if ($state == 9)//update sell data
        {
            //get balance from database
            //INSERT INTO `record` (`Buy_Time`, `Rfid`, `Item`, `Cost`, `Balance`) VALUES ('2018-05-31 10:27:26', '1234', 'Soda', '25', '50');
            //SELECT * FROM `item` WHERE `序號` = 1
            //UPDATE `costomer` SET `like2` = '0', `Balance` = '52' WHERE `costomer`.`Rfid` = 1234;
            $sql = "SELECT * FROM `costomer` WHERE `Rfid` = \"" . $user . "\";";
            $search = mysqli_query($con, $sql);
            $result = mysqli_fetch_row($search);
            $balance_old = $result[6];
            $liknum = $result[$target_item + 1] + 1;//get the old like counter

            $sql = "SELECT * FROM `item` WHERE `序號` = \"" . $target_item . "\";";
            $search = mysqli_query($con, $sql);
            $result = mysqli_fetch_row($search);
            $cost = $result[2];//get item cost
            $item = $result[1];//get name

            $balance_new = $balance_old - $cost;
            $sql = "INSERT INTO `record` (`Buy_Time`, `Rfid`, `Item`, `Cost`, `Balance`) VALUES('" . $Time . "', '" . $user . "', '" . $item . "', " . $cost . ", '" . $balance_new . "');";
            $data = mysqli_query($con, $sql);

            $like = "like" . (string)$target_item;
            $sql = "UPDATE `costomer` SET `" . $like . "` = '" . $liknum . "', `Balance` = '" . $balance_new . "' WHERE `costomer`.`Rfid` = " . $user . ";";
            $data = mysqli_query($con, $sql);
            echo update($user, 0, $con);
        }
       
        
    //關閉文件
        fclose($file);
    }
}
/* sleep(5);
header("Refresh:0");//重新整理 */
?>