<?
function register($db)
{
	$nameErr = "";
	$mailErr = "";
	$Lerr = "";
	$Ferr = "";
	$passErr = "";
	if ($_SERVER["REQUEST_METHOD"] == "POST") 
		{
			if (empty($_POST["user"])) {
				$nameErr = "Username is required";
			  } 

			if (empty($_POST["mail"])) 
			{
				$mailErr = "E-mail is required";
			}
			else{
				if(!filter_var($_POST['mail'], FILTER_VALIDATE_EMAIL))
				{
					$mailErr = "E-mail is incorrect";
				}
			}
			if (empty($_POST["FName"])) 
			{
				$Ferr = "First name is required";
		    } 
			if (empty($_POST["LName"])) 
			{
				$Lerr = "Last name is required";
		    } 			  
			if (empty($_POST["pass"])) 
			{
				$passErr = "Password is required";
			} 

		}
	?>
	<form method="POST">
		Username:<br> <input type="text" name="user" value="<?php echo isset($_POST['user']) ? $_POST['user'] : '' ?>"/><a class="err">*<? echo $nameErr; ?></a><br>
		Password:<br> <input type="PASSWORD" name="pass"/><a class="err">*<? echo $passErr; ?></a><br>
		First Name:<br> <input type="text" name="FName" value="<?php echo isset($_POST['FName']) ? $_POST['FName'] : '' ?>"/><a class="err">*<? echo $Ferr; ?></a><br>
		Last Name:<br> <input type="text" name="LName" value="<?php echo isset($_POST['LName']) ? $_POST['LName'] : '' ?>"/><a class="err">*<? echo $Lerr; ?></a><br>
		E-mail:<br> <input type="text" name="mail" value="<?php echo isset($_POST['mail']) ? $_POST['mail'] : '' ?>"/><a class="err">*<? echo $mailErr; ?></a><br>
		<input type="checkbox" name="news" value="Yes" checked="checked"/> I want to receive notifications <br>
		Licenses:<br>
		<? 
				$query="SELECT * FROM Preukaz";
				$result = @mysql_query($query,$db);
			while($data = mysql_fetch_array($result, MYSQL_ASSOC)){?>
				<input type="checkbox" name="Preukaz<?echo $data["ID_PR"]?>"><? echo $data["nazovPreukazu"]; ?> (<?echo $data["informaciaOPreukaze"]?>)<br>
		<?}?>
		Certificates:<br>
		<? 
				$query="SELECT * FROM Certifikaty";
				$result = @mysql_query($query,$db);
			while($data = mysql_fetch_array($result, MYSQL_ASSOC)){?>
				<input type="checkbox" name="Certifikat<?echo $data["ID_C"]?>"><? echo $data["nazovCertifikatu"]; ?> (<?echo $data["informaciaOCertifikatu"]?>)<br>
		<?}?>
		Courses:<br>
		<? 
				$query="SELECT * FROM Skolenie";
				$result = @mysql_query($query,$db);
			while($data = mysql_fetch_array($result, MYSQL_ASSOC)){?>
				<input type="checkbox" name="Skolenie<?echo $data["ID_SK"]?>"><? echo $data["nazovSkolenia"]; ?> (<?echo $data["informaciaOSkoleni"]?>)<br>
		<?}?>
			<input type="submit">
		</form>
	<?	
	if((isset($_POST['user']) && ($_POST['user'] != "")))
	{
		
		if((isset($_POST['pass']) && ($_POST['pass'] != "")))
		{
			if((isset($_POST['FName']) && ($_POST['FName'] != "")))
			{
				if((isset($_POST['LName']) && ($_POST['LName'] != "")))
				{
					if((isset($_POST['mail']) && ($_POST['mail'] != "")) && filter_var($_POST['mail'], FILTER_VALIDATE_EMAIL))
					{
						$query="SELECT * FROM Klient WHERE loginK='".$_POST["user"]."'";
						$result = @mysql_query($query, $db);
						$queryA="SELECT * FROM Klient WHERE email='".$_POST["mail"]."'";
						$resultA= @mysql_query($queryA, $db);

						if((is_resource($result) and mysql_num_rows($result)>0) || (is_resource($resultA) and mysql_num_rows($resultA)>0))
						{
							if((is_resource($result) and mysql_num_rows($result)>0))
							{
								?><a class="err">The given username already exists.</a><br><?
							}
							if((is_resource($resultA) and mysql_num_rows($resultA)>0))
							{
								?><a class="err">The given E-mail already exists.</a><?
							}
						}
						else
						{
							$query="INSERT INTO Klient (`krstneMeno_K`, `priezvisko_K`,`email`,`loginK`,`hesloK`) VALUES ('".mysql_real_escape_string($_POST["FName"])."','".mysql_real_escape_string($_POST["LName"])."','".mysql_real_escape_string($_POST["mail"])."','".mysql_real_escape_string($_POST["user"])."','".mysql_real_escape_string($_POST["pass"])."')";
							$result = @mysql_query($query,$db);
							
							
							$queryX="SELECT * FROM Klient WHERE loginK='".$_POST["user"]."'";
							$resultX = @mysql_query($queryX,$db);
							$dataX = mysql_fetch_array($resultX, MYSQL_ASSOC);
							
							if(isset($_POST["Preukaz1"]))
							{
								//echo "aloha1";
								$query="INSERT INTO Klient_Preukaz(`ID_K`, `ID_PR`) VALUES ('".$dataX["ID_K"]."','1')";
								$result = @mysql_query($query,$db);
							}
							if(isset($_POST["Preukaz2"]))
							{
								
								$query="INSERT INTO Klient_Preukaz(`ID_K`, `ID_PR`) VALUES ('".$dataX["ID_K"]."','2')";
								$result = @mysql_query($query,$db);
							}
							if(isset($_POST["Preukaz3"]))
							{
								//echo "aloha3";
								$query="INSERT INTO Klient_Preukaz(`ID_K`, `ID_PR`) VALUES ('".$dataX["ID_K"]."','3')";
								$result = @mysql_query($query,$db);
							}
							if(isset($_POST["Preukaz4"]))
							{
								//echo "aloha4	";
								$query="INSERT INTO Klient_Preukaz(`ID_K`, `ID_PR`) VALUES ('".$dataX["ID_K"]."','4')";
								$result = @mysql_query($query,$db);
							}
							if(isset($_POST["Preukaz5"]))
							{
								//echo "aloha4	";
								$query="INSERT INTO Klient_Preukaz(`ID_K`, `ID_PR`) VALUES ('".$dataX["ID_K"]."','5')";
								$result = @mysql_query($query,$db);
							}
							//-----------------------------------------------------------------------------------------------
							if(isset($_POST["Certifikat1"]))
							{
								//echo "aloha1";
								$query="INSERT INTO Klient_Certifikat(`ID_K`, `ID_C`) VALUES ('".$dataX["ID_K"]."','1')";
								$result = @mysql_query($query,$db);
							}
							if(isset($_POST["Certifikat2"]))
							{
								
								$query="INSERT INTO Klient_Certifikat(`ID_K`, `ID_C`) VALUES ('".$dataX["ID_K"]."','2')";
								$result = @mysql_query($query,$db);
							}
							if(isset($_POST["Certifikat3"]))
							{
								//echo "aloha3";
								$query="INSERT INTO Klient_Certifikat(`ID_K`, `ID_C`) VALUES ('".$dataX["ID_K"]."','3')";
								$result = @mysql_query($query,$db);
							}
							if(isset($_POST["Certifikat4"]))
							{
								//echo "aloha4	";
								$query="INSERT INTO Klient_Certifikat(`ID_K`, `ID_C`) VALUES ('".$dataX["ID_K"]."','4')";
								$result = @mysql_query($query,$db);
							}
							if(isset($_POST["Certifikat5"]))
							{
								//echo "aloha4	";
								$query="INSERT INTO Klient_Certifikat(`ID_K`, `ID_C`) VALUES ('".$dataX["ID_K"]."','5')";
								$result = @mysql_query($query,$db);
							}
							//------------------------------------------------------------------------------------------------------
							if(isset($_POST["Skolenie1"]))
							{
								//echo "aloha1";
								$query="INSERT INTO Klient_Skolenie(`ID_K`, `ID_SK`) VALUES ('".$dataX["ID_K"]."','1')";
								$result = @mysql_query($query,$db);
							}
							if(isset($_POST["Skolenie2"]))
							{
								
								$query="INSERT INTO Klient_Skolenie(`ID_K`, `ID_SK`) VALUES ('".$dataX["ID_K"]."','2')";
								$result = @mysql_query($query,$db);
							}
							if(isset($_POST["Skolenie3"]))
							{
								//echo "aloha3";
								$query="INSERT INTO Klient_Skolenie(`ID_K`, `ID_SK`) VALUES ('".$dataX["ID_K"]."','3')";
								$result = @mysql_query($query,$db);
							}
							if(isset($_POST["Skolenie4"]))
							{
								//echo "aloha4	";
								$query="INSERT INTO Klient_Skolenie(`ID_K`, `ID_SK`) VALUES ('".$dataX["ID_K"]."','4')";
								$result = @mysql_query($query,$db);
							}
							
							
							header('Location: index.php?menu=4');
						}

					}
				}
			}
		}
	}	
}
?>