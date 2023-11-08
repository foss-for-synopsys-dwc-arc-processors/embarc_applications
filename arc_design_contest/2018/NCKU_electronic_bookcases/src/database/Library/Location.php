<html>
<head>
<title>Library information</title>
<meta http-equiv="refresh" content="5">

<style type="text/css">
	table{
		border: 2px solid red;
		background-color: #FFC;
	}
	th{
		border-bottom: 5px solid #000;
	}
	td{
		border-bottom: 2px solid #666;
	}
</style>



</head>

<body>
	<?php
		#connect to MySQL
		include "connect database.php";
		


		function Display($Table){
			$query="SELECT * FROM $Table";
			$sqldata =mysqli_query($GLOBALS['dbcon'],$query) or die('error geting');

			echo "<h1>$Table";
			echo "<table>";
			echo "<tr>";
			while ($info= mysqli_fetch_field($sqldata)) {
				echo " <th>".$info->name."</th>";
			}
			echo "</tr>";
			
			///*
			while ($row=mysqli_fetch_array($sqldata)) {
				echo "<tr>";
				for ($i=0; $i <sizeof($row)>>1 ; $i++) { 
					echo "<td>".$row[$i]."</td>";
				}
				echo "</tr>";
			}
			echo "</table>";
			echo "</h1>";
		}

		

		Display("Location");


	?>
	
</body>