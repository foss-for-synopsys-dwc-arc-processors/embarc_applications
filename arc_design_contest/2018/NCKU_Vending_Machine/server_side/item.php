<?php session_start(); ?>
<?php //連接上mysql
if(isset($_SESSION['username'])) {
    $con = mysqli_connect("127.0.0.1", "account", "password", "projectarc");
    if (!$con) {
        echo "連接錯誤";
        die('Error, insert query failed');
    } else {
            //echo"連接成功!!";
    }
    mysqli_query($con, "set names utf8");
    $data = mysqli_query($con, "select * from item");
    $data3 = mysqli_query($con, "select * from state");
    $size = mysqli_num_rows($data);//陣列大小


    for ($i = 1; $i <= $size; $i++)//初始化，獲得資料庫數值
    {
        $idname = "name" . (string)$i;
        $idprice = "price" . (string)$i;
        $idtype = "type" . (string)$i;
        $idexp_date = "exp_date" . (string)$i;
        $idorder = "order" . (string)$i;
        $idnum = "num" . (string)$i;

        $rs = mysqli_fetch_row($data);
        $order[$idorder] = $rs[0];
        $name[$idname] = $rs[1];
        $Price[$idprice] = $rs[2];
        $Type[$idtype] = $rs[3];
        $Exp_Date[$idexp_date] = $rs[4];
        $num[$idnum] = $rs[5];
    }
    ///販賣機狀態初始化
        $rd = mysqli_fetch_row($data3);
        $M_ID = $rd[0];
        $State = $rd[1];
        $Temp = $rd[2];
        $Time = $rd[3];
    
    function test_input($data)//處理資料，防止錯誤
    {
        $data = trim($data);
        $data = stripslashes($data);
        $data = htmlspecialchars($data);
        return $data;
    }
        //UPDATE `item` SET `Name` = 'Soda2', `Price` = '252', `Type` = '飲料2', `Exp_Date` = '2018-03-26', `Num` = '52' WHERE `item`.`序號` = 1;
    if ($_SERVER["REQUEST_METHOD"] == "POST")//獲得html form post過來的值
    {
        for ($i = 1; $i <= $size; $i++) {
            $idname = "name" . (string)$i;
            $idprice = "price" . (string)$i;
            $idtype = "type" . (string)$i;
            $idexp_date = "exp_date" . (string)$i;
            $idorder = "order" . (string)$i;
            $idnum = "num" . (string)$i;
                
                //將資料傳至superGlobal        
            $name[$idname] = test_input($_POST[$idname]);
            $Price[$idprice] = test_input($_POST[$idprice]);
            $Type[$idtype] = test_input($_POST[$idtype]);
            $Exp_Date[$idexp_date] = test_input($_POST[$idexp_date]);
                //$Num = test_input($_POST["Num"]);	
            $data2 = mysqli_query($con, "UPDATE `item` SET `Name` = '" . $name[$idname] . "', `Price` = '" . $Price[$idprice] . "', `Type` = '" . $Type[$idtype] . "', `Exp_Date` = '" . $Exp_Date[$idexp_date] . "' WHERE `item`.`序號` = " . $i . ";");
        }
            // header("Refresh:0");    
    }
}
else
{
    header("Refresh:0; url=sign_in.php");
}
?>
<?php //連接上mysql
if (isset($_SESSION['username'])) {
?>
<!DOCTYPE html>
<html>
    <head>	
        <link rel="stylesheet" type="text/css" media="screen" href="item.css" />
        <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
        <script src="https://code.highcharts.com/highcharts.js"></script>
        <script src="https://code.highcharts.com/modules/exporting.js"></script>
        <script src="https://code.jquery.com/jquery-3.2.1.slim.min.js" integrity="sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN" crossorigin="anonymous"></script>
        <!--link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous"-->
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>饅丁蘑蕈倉儲系統</title>   
        <script>
        $(document).ready(function(){
            var time = new Date();
            var update = new Date("<?php echo $Time ?>");            
            var now = time.getTime();
            var update_time = update.getTime();
            <?php
            for ($i = 1; $i <= $size; $i++) {
                $idexp_date = "exp_date" . (string)$i;
                $idname = "name" . (string)$i;
                $idnum = "num" . (string)$i;
                ?>         
            var d = new Date("<?php echo $Exp_Date[$idexp_date] ?>");
            var n = d.getTime();
            if(now>n)                    
             $("#alert").append("<b>「"+"<?php echo $name[$idname] ?>"+"」</b>過期了，請更換!!<br>");
            if(<?php echo $num[$idnum] ?>==0)
             $("#alert").append("<b>「"+"<?php echo $name[$idname] ?>"+"」</b>已售完，請補貨!!<br>");
            <?php 
            } ?>//for 迴圈下標
            if(<?php echo $Temp ?>>40)
             $("#alert").append("<b>溫度超過40度，異常</b><br>");
            if(!<?php echo $State ?>)
             $("#alert").append("<b>機器狀態異常</b><br>");
            // console.log(time);
            // console.log(update);
            // console.log(now);
            // console.log(update_time);            
            // console.log(now-update_time);
            if((now-update_time)>900000){
                console.log(now-update_time);
             $("#alert").append("<b>機器超過15分鐘未更新，請聯絡管理員檢查</b><br>");
            }
        });
        </script>
    </head>

    <body>
    <!--html form回傳到自己的php-->
    <div class="w3-container" id="title"> 
        <h1><a name="home">饅丁蘑蕈倉儲系統</a></h1>
        <div id="user">
            <?php echo $_SESSION['username'] ?>
            <a href="logout.php"><button type="button" class="w3-button w3-black">登出</button></a>
        </div>
    </div>
    <div class="w3-panel w3-border-top w3-border-bottom">
    </div>
    <div class="w3-sidebar w3-bar-block w3-border-right " style="width:15%"> 
        <div id="link">
            <a href="#home" class="w3-bar-item w3-button">Home</a>
            <a href="#state"class="w3-bar-item w3-button">販賣機狀態</a>
            <a href="#analysis" class="w3-bar-item w3-button">分析</a>
            <a href="#record" class="w3-bar-item w3-button">購買紀錄</a>
        </div>
    </div>
    
    <div class="tables" style="margin-left:18%">
        <div id="state">
            <h2><a name="state">販賣機狀態</a></h2>
            <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
            <div class="grid-container">
            <?php
            for ($i = 1; $i <= $size; $i++) {
                $idname = "name" . (string)$i;
                $idprice = "price" . (string)$i;
                $idtype = "type" . (string)$i;
                $idexp_date = "exp_date" . (string)$i;
                $idorder = "order" . (string)$i;
                $idnum = "num" . (string)$i;
                $iditem = "item".(string)$i;
            ?>         
            <div class="<?php echo $iditem ?>">       	
                    <label>名稱</label><input type="text"   name="<?php echo $idname ?>"     value="<?php echo $name[$idname] ?>"   required maxlength="10">
                    <br>
                    <label>價格</label><input type="number" name="<?php echo $idprice ?>"    value="<?php echo $Price[$idprice] ?>"   required maxlength="10" min="0" max="500" >
                    <br>
                    <label>種類</label><input type="text"   name="<?php echo $idtype ?>"     value="<?php echo $Type[$idtype] ?>"   required maxlength="10">
                    <br>
                    <label>剩餘數量</label><?php echo $num[$idnum] ?>
                    <br>
                    <label>保存期限</label><input type="date" class="date"  name="<?php echo $idexp_date ?>" value="<?php echo $Exp_Date[$idexp_date] ?>"   required >
                    <br>
                   
            </div>
            <?php

        }//for loop下標 
        ?>
        <div class="item5">
            <p>通知</p>
            <p id="alert"></p>
        </div>
        <?php
        ?>
        <div class="item6">
           <p>狀態 <br>販賣機編號:<?php echo $M_ID ?> 狀態:
           <?php 
           if($State)
           echo "正常" ;
           else
            echo"機器異常" ;
             ?> 溫度:<?php echo $Temp ?> 時間:<?php echo $Time?></p>
        </div>
        
        </div>
            <input class="w3-button w3-black" type="submit" name="submit" value="更改" ID="submit_button"> 
            <!--input type="button" name="cancel" value="取消" ID="cancel_button"--> 
            <input  class="w3-button w3-black" type="reset" value="取消">
            
            
            
        </form>
        </div>
         <br>
        <div class="w3-panel w3-border-top w3-border-bottom">
        </div>
        <br>
<!--/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// -->
        <div id="analysis">
            <h2><a name="analysis">分析</a></h2>   
            <div class="w3-panel w3-border-top w3-border-bottom">
            </div>
            <div id="container" ></div>
        </div>
<!--/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// -->

            <?php //連接上mysql
            $data = mysqli_query($con, "SELECT * FROM `record` ORDER BY `record`.`Buy_Time` DESC");
            $size = mysqli_num_rows($data);//陣列大小
            $count = array("Soda" => 0, "Water" => 0, "Coffee" => 0, "Chips" => 0);
            for ($i = 1; $i <= $size; $i++)//初始化，獲得資料庫數值
            {
                $idaccount = "Buy_Time" . (string)$i;
                $idRfid = "Rfid" . (string)$i;
                $idItem = "Item" . (string)$i;
                $idCost = "Cost" . (string)$i;
                $idBalance = "Balance" . (string)$i;
                $rs = mysqli_fetch_row($data);
                $account[$idaccount] = $rs[1];
                $Rfid[$idRfid] = $rs[0];
                $Item[$idItem] = $rs[2];
                $Cost[$idCost] = $rs[3];
                $Balance[$idBalance] = $rs[4];
                if(isset($count[$rs[2]]))
                    $count[$rs[2]]++;
            }
            ?>
            <!--html form回傳到自己的php-->
        <div id="record">         
            <h2><a name="record">購買紀錄</a></h2>   
            <table border="1" class="w3-table" id="record">            
                <tr>
                    <td >	帳號	</td>
                    <td >	時間	</td>
                    <td >	商品	</td>
                    <td >	花費	</td>
                    <td >	餘額	</td>
                </tr>

            <?php
            for ($i = 1; $i <= $size; $i++) {
                $idaccount = "Buy_Time" . (string)$i;
                $idRfid = "Rfid" . (string)$i;
                $idItem = "Item" . (string)$i;
                $idCost = "Cost" . (string)$i;
                $idBalance = "Balance" . (string)$i;
                ?>
                <tr>	
                    <td><?php echo $account[$idaccount] ?></td>
                    <td><?php echo $Rfid[$idRfid] ?> </td>
                    <td><?php echo $Item[$idItem] ?></td>
                    <td><?php echo $Cost[$idCost] ?></td>
                    <td><?php echo $Balance[$idBalance] ?></td>
                </tr>
            <?php

        }//for loop下標 
        ?>
            </table>
        </div>
        <div class="w3-panel w3-border-top w3-border-bottom">
        </div>
    </div>		
    </body>
        <script>
        Highcharts.chart('container', {
            chart: {
                type: 'column'
            },
            title: {
                text: '銷售數量之分析'
            },
            xAxis: {
                categories: [
                    '品項'
                ],
                crosshair: false
            },
            yAxis: {
                min: 0,
                title: {
                    text: '銷售數量'
                }
            },
            series: [{
                name: 'Soda',
                data: [<?php echo $count['Soda']?>]

            }, {
                name: 'Water',
                data: [<?php echo $count['Water'] ?>]

            }, {
                name: 'Coffee',
                data: [<?php echo $count['Coffee'] ?>]

            }, {
                name: 'Chips',
                data: [<?php echo $count['Chips'] ?>]

            }]
        });
        /////////////////////////////////////////////////
        </script>
</html>
<?php
} else {
    header("Refresh:0; url=sign_in.php");
}
?>