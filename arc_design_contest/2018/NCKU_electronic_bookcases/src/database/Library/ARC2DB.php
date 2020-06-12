<?php

#Session 類似buffer，用來儲存資料(全域變數)
session_start();

#debug files
/*
$myfile = fopen("for paste 2.txt", "a+") or die("Unable to open file!");
$current= fopen("debug.txt", "w") or die("Unable to open file!");

#GLOBALS  屬於全域變數
$GLOBALS['myfile'] = $myfile;
$GLOBALS['current'] = $current;
//*/







#connect to MySQL
include "connect database.php";

#Search ID 
function Search(){
	if(isset($_POST['SearchID'])){
		$ID=$_POST['SearchID'];
		$query="SELECT * FROM Book WHERE ID='$ID' ";
		$sqldata=mysqli_fetch_assoc(mysqli_query($GLOBALS['dbcon'],$query));

		if($sqldata['Location']!=NULL){

			/*
			$message="SearchID:".$ID." Found\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/

			echo $sqldata['Location'];
		}
		else if($ID==$sqldata['ID']){
			
			/*
			$message=$ID." Not in shelf\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/

			echo "E".-1;
		}
		else{
			
			/*
			$message=$ID." Not exist\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/

			echo "E". 0 . 0;
		}
	}

}

#Borrow Book
function Borrow(){
	if (isset($_POST['BorrowID'])&&isset($_POST['NewOwner'])) {
		$ID=$_POST['BorrowID'];
		$Owner=$_POST['NewOwner'];
		$query="SELECT * FROM Book WHERE ID='$ID' ";
		$sqldata=mysqli_fetch_assoc(mysqli_query($GLOBALS['dbcon'],$query));

		$ReservedBook="Book" . $ID;

		$query="SELECT priority,$ReservedBook FROM WaitingQueue WHERE priority=1 ";
		$sqldata2=mysqli_fetch_assoc(mysqli_query($GLOBALS['dbcon'],$query));

		
		
		if($sqldata['Owner']==NULL&&($sqldata2[$ReservedBook]==$Owner||$sqldata2[$ReservedBook]==NULL)&&$sqldata['ID']!=NULL&&$sqldata['Location']!=NULL){
			$message="BorrowID:".$ID."Found\n";
			date_default_timezone_set("Asia/Taipei");
			$Days=31;
			$TIME=date('Y-m-d H:i:s',strtotime('+ '.$Days.' days'));
			$CountBorrow=$sqldata['CountBorrow']+1;

			#
			if($sqldata2[$ReservedBook]==$Owner){
				$_SESSION['ID']=$ID;
				$_SESSION['User']=$Owner;
			}
			$query="UPDATE Book SET Location=NULL,Owner = '$Owner',ReturnTime = '$TIME',CountBorrow = '$CountBorrow' WHERE ID='$ID'";


			mysqli_query($GLOBALS['dbcon'],$query);

			/*
			$message=$ID."Lent out\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/

			echo "OOk";
			
			#Reservation Record
			if($sqldata2[$ReservedBook]==$Owner){
				date_default_timezone_set("Asia/Taipei");
				$TIME=date('Y-m-d H:i:s',time());
				$query="UPDATE ReservationRecord SET `Record$ID` =0,`Time$ID`='$TIME' WHERE id =(SELECT min(id) FROM (SELECT * FROM ReservationRecord) AS temp WHERE `Time$ID` is NULL)";
				mysqli_query($GLOBALS['dbcon'],$query);
				if(mysqli_affected_rows($GLOBALS['dbcon'])==0){
					$query="INSERT INTO ReservationRecord (`Record$ID`,`Time$ID`) VALUES (0,'$TIME')";
					mysqli_query($GLOBALS['dbcon'],$query);
				}
				
			}

		}
		elseif ($sqldata['ID']==NULL) {
			
			/*
			$message=$ID."Not Found\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/

			echo "E". 0 . 0;
		}
		elseif ($sqldata['Owner']!=NULL) {
			
			
			/*
			$message=$ID."Borrowed\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/

			echo "E". -1;
		}
		elseif ($sqldata2[$ReservedBook]!=$Owner&&$sqldata2[$ReservedBook]!=NULL) {
			
			
			/*
			$message=$ID."Already Reserved\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/

			echo "E". -2;
		}
	}
}


#Return the Book
function GiveBack(){

	if (isset($_POST['ReturnID'])&&$_POST['ReturnLocation']) {

		$ID=$_POST['ReturnID'];
		$Location=$_POST['ReturnLocation'];

		$query="SELECT * FROM Book WHERE ID='$ID' ";
		$sqldata=mysqli_fetch_assoc(mysqli_query($GLOBALS['dbcon'],$query));

		if($sqldata['ID']==NULL){
			$msNum=0 . 0;
			
			/*
			$message="E". $msNum."no such ID".$ID."\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/

			echo "E". $msNum;
		}

		elseif($sqldata['Location']!=NULL){
			$msNum=-1;
			
			
			/*
			$message="E". $msNum."Already Returned\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/

			echo "E". $msNum;
		}
		else {

			$query="SELECT * FROM Location WHERE Data='$Location'";
			$sqldata=mysqli_fetch_assoc(mysqli_query($GLOBALS['dbcon'],$query));
			if($sqldata['Data']==$Location){
				$query="UPDATE Book SET Location='$Location',Owner = NULL,ReturnTime = NULL WHERE ID='$ID'";
				mysqli_query($GLOBALS['dbcon'],$query);
			
				
				
			/*
			$message=$ID."Done GivenBack\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/

				echo "OOk";
			}
			else{
				
			/*
			$message=$ID."Return Failed No such Location\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/

				echo "E".-2;
			}
		}
	}
}



function NewReservation(){
	if(isset($_POST['Reservation'])&&isset($_POST['ReserveID'])) {
		$ID=$_POST['ReserveID'];
		$Wait=$_POST['Reservation'];
		$query="SELECT 	ID FROM Book WHERE ID='$ID' ";
		$sqldata=mysqli_fetch_assoc(mysqli_query($GLOBALS['dbcon'],$query));
		if($sqldata['ID']!=NULL){

			$query="SELECT 	* FROM WaitingQueue WHERE `Book$ID` ='$Wait' AND priority != 0 ";
			$result=mysqli_query($GLOBALS['dbcon'],$query);

			if(mysqli_num_rows($result)!=0){
				
				
			/*
			$message=$ID."Already reserved by".$Wait ."\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/

				echo "E". -1;
				return;
			}
			

			$query="UPDATE WaitingQueue SET `Book$ID` ='$Wait' WHERE priority =(SELECT min(priority) FROM (SELECT * FROM WaitingQueue) AS temp WHERE `Book$ID` is NULL)";
			mysqli_query($GLOBALS['dbcon'],$query);
			
			if(mysqli_affected_rows($GLOBALS['dbcon'])==0){
				$query="INSERT INTO WaitingQueue (`Book$ID`) VALUES ('$Wait')";
				mysqli_query($GLOBALS['dbcon'],$query);
			}

			#Reservation Record
			date_default_timezone_set("Asia/Taipei");
			$TIME=date('Y-m-d H:i:s',time());
			$query="UPDATE ReservationRecord SET `Record$ID` =1,`Time$ID`='$TIME' WHERE id =(SELECT min(id) FROM (SELECT * FROM ReservationRecord) AS temp WHERE `Time$ID` is NULL)";
			mysqli_query($GLOBALS['dbcon'],$query);
			if(mysqli_affected_rows($GLOBALS['dbcon'])==0){
				$query="INSERT INTO ReservationRecord (`Record$ID`,`Time$ID`) VALUES (1,'$TIME')";
				mysqli_query($GLOBALS['dbcon'],$query);
			}
			

			
			
			/*
			$message=$ID."Reserved by ".$Wait." Done\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/

			echo "OOk";
			
			
		}
		else{
			
			
			/*
			$message=$ID."Not exist\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/

			echo "E". 0 . 0;
		}
	}
}

function PopWaitingQueue(){
	$isCancel=FALSE;

	if(isset($_POST['CancelID'])&&isset($_POST['CancelReservation'])) {
		$_SESSION['ID']=$_POST['CancelID'];
		$_SESSION['User']=$_POST['CancelReservation'];
		$isCancel=TRUE;
	}

	if (isset($_SESSION['ID'])&&isset($_SESSION['User'])) {
		$ID=$_SESSION['ID'];
		$User=$_SESSION['User'];
		$query="SELECT priority FROM WaitingQueue WHERE `Book$ID`='$User'";
		$sqldata=mysqli_fetch_assoc(mysqli_query($GLOBALS['dbcon'],$query));
		$StartIndex=$sqldata['priority'];

		if($StartIndex==NULL){
			/*
			$message="No matched Reservation\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/
			unset($_SESSION['ID']);
			unset($_SESSION['User']);
			echo "E". 0 . 0 ;
			return;
		}


		if($isCancel){
			date_default_timezone_set("Asia/Taipei");
			$TIME=date('Y-m-d H:i:s',time());
			$query="UPDATE ReservationRecord SET `Record$ID` =-1,`Time$ID`='$TIME' WHERE id =(SELECT min(id) FROM (SELECT * FROM ReservationRecord) AS temp WHERE `Time$ID` is NULL)";
			mysqli_query($GLOBALS['dbcon'],$query);
			if(mysqli_affected_rows($GLOBALS['dbcon'])==0){
				$query="INSERT INTO ReservationRecord (`Record$ID`,`Time$ID`) VALUES (-1,'$TIME')";
				mysqli_query($GLOBALS['dbcon'],$query);
			}
			/*
			$message=$ID." Reservation Canceled\n";
			fwrite($GLOBALS['myfile'], $message);
			fwrite($GLOBALS['current'], $message);
			//*/
			echo "OOk";
		}
		

		$query="UPDATE WaitingQueue temp1 LEFT JOIN WaitingQueue temp2 ON temp1.priority=temp2.priority-1 SET temp1.`Book$ID` =temp2.`Book$ID` WHERE  temp2.priority >'$StartIndex' ";
		mysqli_query($GLOBALS['dbcon'],$query);

		
		$query="SELECT max(priority) AS priority FROM WaitingQueue";
		$sqldata=mysqli_fetch_assoc(mysqli_query($GLOBALS['dbcon'],$query));
		$EndPriority=$sqldata['priority'];

		$query="UPDATE WaitingQueue SET `Book$ID` =NULL WHERE priority ='$EndPriority'";
		mysqli_query($GLOBALS['dbcon'],$query);
		
		$query="SELECT * FROM WaitingQueue WHERE priority='$EndPriority'";
		$sqldata=mysqli_fetch_array(mysqli_query($GLOBALS['dbcon'],$query));


		#delete empty rows in DB
		$deleteRow=TRUE;
		for ($i=1; $i <sizeof($sqldata)>>1 ; $i++) { 
			if($sqldata[$i]!=NULL){
				$deleteRow=FALSE;
				break;
			}
		}

		if($deleteRow){
			$query="DELETE FROM WaitingQueue WHERE priority='$EndPriority'";
			mysqli_query($GLOBALS['dbcon'],$query);
		}

		$query="ALTER TABLE WaitingQueue AUTO_INCREMENT = 1";
		mysqli_query($GLOBALS['dbcon'],$query);
		unset($_SESSION['ID']);
		unset($_SESSION['User']);
	}
}


Search();
Borrow();
PopWaitingQueue();
NewReservation();
GiveBack();

/*
fclose($myfile);
fclose($current);
//*/

?>