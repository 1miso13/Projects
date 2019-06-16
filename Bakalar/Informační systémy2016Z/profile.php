<?


function profile($db)
{
	$nameErr = "";
	$mailErr = "";
	$Lerr = "";
	$Ferr = "";
	$passErr = "";
	
	$username = "";
	$password = "";
	$Meno = "";
	$Priezvisko = "";
	$titul = "";
	$date = "";
	$email = "";
	
	
	$result = NULL;
	
	if($_SESSION['TYPE'] === 1)
	{
		$query="SELECT * FROM Klient WHERE loginK='".$_SESSION['username']."'";
		$result = @mysql_query($query, $db);

		if(is_resource($result) and mysql_num_rows($result)>0)
		{
			while($data = mysql_fetch_array($result, MYSQL_ASSOC))
			{
				if($data["ID_K"] == $_SESSION["ID"])
				{
					$username = $data["loginK"];
					$Meno = $data["krstneMeno_K"];
					$password = $data["hesloK"];
					$Priezvisko = $data["priezvisko_K"];
					$titul = $data["titul"];
					$date = $data["datumNarodenia_K"];
					$email = $data["email"];
				}
			}	
		}
		else
		{
					?>
						<a class="err">Such user was not found.</a>
					<?
		}
	}
	else
	{
		$query="SELECT * FROM Uradnik WHERE loginU='".$_SESSION['username']."'";
			$result = @mysql_query($query, $db);
			if(is_resource($result) and mysql_num_rows($result)>0){ 
				while($data = mysql_fetch_array($result, MYSQL_ASSOC))
				{
					if($data["ID_U"] == $_SESSION["ID"])
					{
						$username = $data["loginU"];
						$password = $data["hesloU"];
						$Meno = $data["krstneMeno_U"];
						$Priezvisko = $data["priezvisko_U"];
						$titul = $data["titul"];
						$date = $data["datumNarodenia_U"];
						$email = $data["email"];
					}
				}	
			}
			else
			{
				?>
					<a class="err">Such user was not found.</a>
				<?
			}						
	}
					
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
	<?
				if((isset($_POST['pass']) && ($_POST['pass'] != "")))
				{
					if((isset($_POST['FName']) && ($_POST['FName'] != "")))
					{
						if((isset($_POST['LName']) && ($_POST['LName'] != "")))
						{
							if((isset($_POST['mail']) && ($_POST['mail'] != "")) && (filter_var($_POST['mail'], FILTER_VALIDATE_EMAIL)))
							{
								
								$result = "";
								if($_POST['mail'] != $email)
								{
									if($_SESSION['TYPE'] === 1)
									{
										$query="SELECT * FROM Klient WHERE email='".$_POST["mail"]."'";
										$result = @mysql_query($query, $db);										
									}
									else
									{
										$query="SELECT * FROM Uradnik WHERE email='".$_POST["mail"]."'";
										$result = @mysql_query($query, $db);
									}
								}
								if((is_resource($result) and mysql_num_rows($result)>0))
								{
									?><a class="err">The given E-mail already exists.<?
								}
								else
								{
									$query = "";
									if($_SESSION['TYPE'] === 1)
									{
										$password = $_POST["pass"];
										$Meno = $_POST["FName"];
										$Priezvisko = $_POST["LName"];
										$titul = $_POST["title"];
										$date = $_POST["date"];
										$email = $_POST["mail"];
										$query = "UPDATE Klient SET krstneMeno_K='".$_POST["FName"]."',priezvisko_K='".$_POST["LName"]."',
											email='".$_POST["mail"]."', titul='".$_POST["title"]."',hesloK='".$_POST["pass"]."',
											datumNarodenia_K='".$_POST["date"]."' WHERE ID_K='".$_SESSION["ID"]."'";
										$result=@mysql_query($query,$db);
										
										// addition of new things
										
										if(isset($_POST["Preukaz1"]))
										{
											$query="SELECT * FROM Klient_Preukaz WHERE ID_PR = 1 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)==0)
											{
												$query="INSERT INTO Klient_Preukaz(`ID_K`, `ID_PR`) VALUES ('".$_SESSION["ID"]."','1')";
												$result = @mysql_query($query,$db);
											}
										}
										else
										{
											$query="SELECT * FROM Klient_Preukaz WHERE ID_PR = 1 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)>0)
											{
												$query="DELETE FROM Klient_Preukaz WHERE ID_PR = 1 AND ID_K='".$_SESSION["ID"]."'";
												$result = @mysql_query($query,$db);
											}
										}
										/******************************************************************************************/
										if(isset($_POST["Preukaz2"]))
										{
											
											$query="SELECT * FROM Klient_Preukaz WHERE ID_PR = 2 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)==0)
											{
												$query="INSERT INTO Klient_Preukaz(`ID_K`, `ID_PR`) VALUES ('".$_SESSION["ID"]."','2')";
												$result = @mysql_query($query,$db);
											}
										}
										else
										{
											$query="SELECT * FROM Klient_Preukaz WHERE ID_PR = 2 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)>0)
											{
												$query="DELETE FROM Klient_Preukaz WHERE ID_PR = 2 AND ID_K='".$_SESSION["ID"]."'";
												$result = @mysql_query($query,$db);
											}
										}
										/******************************************************************************************/
										if(isset($_POST["Preukaz3"]))
										{
											$query="SELECT * FROM Klient_Preukaz WHERE ID_PR = 3 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)==0)
											{
												$query="INSERT INTO Klient_Preukaz(`ID_K`, `ID_PR`) VALUES ('".$_SESSION["ID"]."','3')";
												$result = @mysql_query($query,$db);
											}
										}
										else
										{
											$query="SELECT * FROM Klient_Preukaz WHERE ID_PR = 3 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)>0)
											{
												$query="DELETE FROM Klient_Preukaz WHERE ID_PR = 3 AND ID_K='".$_SESSION["ID"]."'";
												$result = @mysql_query($query,$db);
											}
										}
										/******************************************************************************************/
										if(isset($_POST["Preukaz4"]))
										{
											$query="SELECT * FROM Klient_Preukaz WHERE ID_PR = 4 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)==0)
											{
												$query="INSERT INTO Klient_Preukaz(`ID_K`, `ID_PR`) VALUES ('".$_SESSION["ID"]."','4')";
												$result = @mysql_query($query,$db);
											}
										}
										else
										{
											$query="SELECT * FROM Klient_Preukaz WHERE ID_PR = 4 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)>0)
											{
												$query="DELETE FROM Klient_Preukaz WHERE ID_PR = 4 AND ID_K='".$_SESSION["ID"]."'";
												$result = @mysql_query($query,$db);
											}
										}
										/******************************************************************************************/
										if(isset($_POST["Preukaz5"]))
										{
											$query="SELECT * FROM Klient_Preukaz WHERE ID_PR = 5 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)==0)
											{
												$query="INSERT INTO Klient_Preukaz(`ID_K`, `ID_PR`) VALUES ('".$_SESSION["ID"]."','5')";
												$result = @mysql_query($query,$db);
											}
										}
										else
										{
											$query="SELECT * FROM Klient_Preukaz WHERE ID_PR = 5 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)>0)
											{
												$query="DELETE FROM Klient_Preukaz WHERE ID_PR = 5 AND ID_K='".$_SESSION["ID"]."'";
												$result = @mysql_query($query,$db);
											}
										}
										/******************************************************************************************/
										//-----------------------------------------------------------------------------------------------	
																				
										if(isset($_POST["Certifikat1"]))
										{
											//echo "aloha1";
											/*$query="INSERT INTO Klient_Certifikat(`ID_K`, `ID_C`) VALUES ('".$_SESSION["ID"]."','1')";
											$result = @mysql_query($query,$db);*/
											$query="SELECT * FROM Klient_Certifikat WHERE ID_C = 1 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)==0)
											{
												$query="INSERT INTO Klient_Certifikat(`ID_K`, `ID_C`) VALUES ('".$_SESSION["ID"]."','1')";
												$result = @mysql_query($query,$db);
											}
										}
										else
										{
											$query="SELECT * FROM Klient_Certifikat WHERE ID_C = 1 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)>0)
											{
												$query="DELETE FROM Klient_Certifikat WHERE ID_C = 1 AND ID_K='".$_SESSION["ID"]."'";
												$result = @mysql_query($query,$db);
											}
										}
										/******************************************************************************************/
										if(isset($_POST["Certifikat2"]))
										{
											
											$query="SELECT * FROM Klient_Certifikat WHERE ID_C = 2 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)==0)
											{
												$query="INSERT INTO Klient_Certifikat(`ID_K`, `ID_C`) VALUES ('".$_SESSION["ID"]."','2')";
												$result = @mysql_query($query,$db);
											}
										}
										else
										{
											$query="SELECT * FROM Klient_Certifikat WHERE ID_C = 2 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)>0)
											{
												$query="DELETE FROM Klient_Certifikat WHERE ID_C = 2 AND ID_K='".$_SESSION["ID"]."'";
												$result = @mysql_query($query,$db);
											}
										}
										/******************************************************************************************/
										if(isset($_POST["Certifikat3"]))
										{
											$query="SELECT * FROM Klient_Certifikat WHERE ID_C = 3 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)==0)
											{
												$query="INSERT INTO Klient_Certifikat(`ID_K`, `ID_C`) VALUES ('".$_SESSION["ID"]."','3')";
												$result = @mysql_query($query,$db);
											}
										}
										else
										{
											$query="SELECT * FROM Klient_Certifikat WHERE ID_C = 3 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)>0)
											{
												$query="DELETE FROM Klient_Certifikat WHERE ID_C = 3 AND ID_K='".$_SESSION["ID"]."'";
												$result = @mysql_query($query,$db);
											}
										}
										/******************************************************************************************/
										if(isset($_POST["Certifikat4"]))
										{
											$query="SELECT * FROM Klient_Certifikat WHERE ID_C = 4 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)==0)
											{
												$query="INSERT INTO Klient_Certifikat(`ID_K`, `ID_C`) VALUES ('".$_SESSION["ID"]."','4')";
												$result = @mysql_query($query,$db);
											}
										}
										else
										{
											$query="SELECT * FROM Klient_Certifikat WHERE ID_C = 4 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)>0)
											{
												$query="DELETE FROM Klient_Certifikat WHERE ID_C = 4 AND ID_K='".$_SESSION["ID"]."'";
												$result = @mysql_query($query,$db);
											}
										}
										/******************************************************************************************/
										if(isset($_POST["Certifikat5"]))
										{
											$query="SELECT * FROM Klient_Certifikat WHERE ID_C = 5 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)==0)
											{
												$query="INSERT INTO Klient_Certifikat(`ID_K`, `ID_C`) VALUES ('".$_SESSION["ID"]."','5')";
												$result = @mysql_query($query,$db);
											}
										}
										else
										{
											$query="SELECT * FROM Klient_Certifikat WHERE ID_C = 5 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)>0)
											{
												$query="DELETE FROM Klient_Certifikat WHERE ID_C = 5 AND ID_K='".$_SESSION["ID"]."'";
												$result = @mysql_query($query,$db);
											}
										}
										/******************************************************************************************/
										//------------------------------------------------------------------------------------------------------
										
										if(isset($_POST["Skolenie1"]))
										{
											//echo "aloha1";
											/*$query="INSERT INTO Klient_Skolenie(`ID_K`, `ID_SK`) VALUES ('".$_SESSION["ID"]."','1')";
											$result = @mysql_query($query,$db);*/
											$query="SELECT * FROM Klient_Skolenie WHERE ID_SK = 1 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)==0)
											{
												$query="INSERT INTO Klient_Skolenie(`ID_K`, `ID_SK`) VALUES ('".$_SESSION["ID"]."','1')";
												$result = @mysql_query($query,$db);
											}
										}
										else
										{
											$query="SELECT * FROM Klient_Skolenie WHERE ID_SK = 1 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)>0)
											{
												$query="DELETE FROM Klient_Skolenie WHERE ID_SK = 1 AND ID_K='".$_SESSION["ID"]."'";
												$result = @mysql_query($query,$db);
											}
										}
										/******************************************************************************************/
										if(isset($_POST["Skolenie2"]))
										{
											$query="SELECT * FROM Klient_Skolenie WHERE ID_SK = 2 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)==0)
											{
												$query="INSERT INTO Klient_Skolenie(`ID_K`, `ID_SK`) VALUES ('".$_SESSION["ID"]."','2')";
												$result = @mysql_query($query,$db);
											}
										}
										else
										{
											$query="SELECT * FROM Klient_Skolenie WHERE ID_SK = 2 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)>0)
											{
												$query="DELETE FROM Klient_Skolenie WHERE ID_SK = 2 AND ID_K='".$_SESSION["ID"]."'";
												$result = @mysql_query($query,$db);
											}
										}
										/******************************************************************************************/
										if(isset($_POST["Skolenie3"]))
										{
											$query="SELECT * FROM Klient_Skolenie WHERE ID_SK = 3 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)==0)
											{
												$query="INSERT INTO Klient_Skolenie(`ID_K`, `ID_SK`) VALUES ('".$_SESSION["ID"]."','3')";
												$result = @mysql_query($query,$db);
											}
										}
										else
										{
											$query="SELECT * FROM Klient_Skolenie WHERE ID_SK = 3 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)>0)
											{
												$query="DELETE FROM Klient_Skolenie WHERE ID_SK = 3 AND ID_K='".$_SESSION["ID"]."'";
												$result = @mysql_query($query,$db);
											}
										}
										/******************************************************************************************/
										if(isset($_POST["Skolenie4"]))
										{
											$query="SELECT * FROM Klient_Skolenie WHERE ID_SK = 4 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)==0)
											{
												$query="INSERT INTO Klient_Skolenie(`ID_K`, `ID_SK`) VALUES ('".$_SESSION["ID"]."','4')";
												$result = @mysql_query($query,$db);
											}
										}
										else
										{
											$query="SELECT * FROM Klient_Skolenie WHERE ID_SK = 4 AND ID_K='".$_SESSION["ID"]."'";
											$result = @mysql_query($query, $db);
											if(mysql_num_rows($result)>0)
											{
												$query="DELETE FROM Klient_Skolenie WHERE ID_SK = 4 AND ID_K='".$_SESSION["ID"]."'";
												$result = @mysql_query($query,$db);
											}
										}
										/******************************************************************************************/
										
									}
									else
									{
										$password = $_POST["pass"];
										$Meno = $_POST["FName"];
										$Priezvisko = $_POST["LName"];
										$titul = $_POST["title"];
										$date = $_POST["date"];
										$email = $_POST["mail"];
										$query = "UPDATE Uradnik SET krstneMeno_U='".$_POST["FName"]."',priezvisko_U='".$_POST["LName"]."',hesloU='".$_POST["pass"]."',titul = '".$_POST["title"]."',email='".$_POST["mail"]."',
											datumNarodenia_U='".$_POST["date"]."' WHERE ID_U='".$_SESSION["ID"]."'";
									}
									$result = @mysql_query($query, $db);
									?>
									<a class="err">Changes were successful</a>
									<?
									
								}
							}
						}
					}
				}
				?>
	<form method="POST">
			Username: <? echo $username; ?><br>
			Password:<br> <input type="PASSWORD" name="pass" value="<?echo $password;?>"/><a class="err" >*<? echo $passErr; ?></a><br>
			First Name:<br> <input type="text" name="FName" value="<?php echo $Meno ?>"/><a class="err">*<? echo $Ferr; ?></a><br>
			Last Name:<br> <input type="text" name="LName" value="<?php echo $Priezvisko ?>"/><a class="err">*<? echo $Lerr; ?></a><br>
			Title:<br> <input type="text" name="title" value="<?php echo $titul ?>"/><br>
			Birthday:<br> <input type="text" name="date" value="<?php echo $date ?>"/><br>
			E-mail:<br> <input type="text" name="mail" value="<?php echo $email ?>"/><a class="err">*<? echo $mailErr; ?></a><br>
			<?// 1 ==klient , 2 == uradnik
			if($_SESSION['TYPE'] === 1)
			{
				$query="SELECT * FROM Klient WHERE loginK='".$_SESSION['username']."'";
				$result = @mysql_query($query, $db);
				$dataB = mysql_fetch_array($result, MYSQL_ASSOC);
				/************************************************************************/
				$queryX = "SELECT * FROM Klient_Preukaz WHERE ID_K ='".$data["ID_K"]."'";
				$queryY = "SELECT * FROM Klient_Certifikat WHERE ID_K ='".$data["ID_K"]."'";
				$queryZ = "SELECT * FROM Klient_Skolenie WHERE ID_K ='".$data["ID_K"]."'";
				
				$resultX = @mysql_query($query, $db);
				$resultY = @mysql_query($query, $db);
				$resultZ = @mysql_query($query, $db);
				
				
				if(is_resource($resultX) and mysql_num_rows($resultX)>0)
				{
					?>
					<h3>Licenses:</h3><br>
					<? 
						$query="SELECT * FROM Preukaz";
						$result = @mysql_query($query,$db);
					while($data = mysql_fetch_array($result, MYSQL_ASSOC)){?>
						<input type="checkbox" name="Preukaz<?echo $data["ID_PR"]?>"
						<?
						$queryA = "SELECT * FROM Klient_Preukaz WHERE ID_K ='".$dataB["ID_K"]."' AND ID_PR = '".$data["ID_PR"]."'";
						$resultA = @mysql_query($queryA,$db);
						if(is_resource($resultA) and mysql_num_rows($resultA)>0)
						{echo 'checked="checked"';}else{echo "";}
						?>><? echo $data["nazovPreukazu"]; ?> (<?echo $data["informaciaOPreukaze"]?>)<br>
					<?}
				}
				if(is_resource($resultY) and mysql_num_rows($resultY)>0)
				{?>
					<h3>Certificates:</h3><br>
					<? 
						$query="SELECT * FROM Certifikaty";
						$result = @mysql_query($query,$db);
					while($data = mysql_fetch_array($result, MYSQL_ASSOC)){?>
						<input type="checkbox" name="Certifikat<?echo $data["ID_C"]?>"
						<?
						$queryB = "SELECT * FROM Klient_Certifikat WHERE ID_K ='".$dataB["ID_K"]."' AND ID_C = '".$data["ID_C"]."'";
						$resultB = @mysql_query($queryB,$db);
						if(is_resource($resultB) and mysql_num_rows($resultB)>0)
						{echo 'checked="checked"';}else{echo "";}
						?>><? echo $data["nazovCertifikatu"]; ?> (<?echo $data["informaciaOCertifikatu"]?>)<br>
					<?}
				}
				if(is_resource($resultZ) and mysql_num_rows($resultZ)>0)
				{?>
					<h3>Courses:</h3><br>
					<? 
						$query="SELECT * FROM Skolenie";
						$result = @mysql_query($query,$db);
					while($data = mysql_fetch_array($result, MYSQL_ASSOC)){?>
						<input type="checkbox" name="Skolenie<?echo $data["ID_SK"]?>"
						<?
						$queryC = "SELECT * FROM Klient_Skolenie WHERE ID_K ='".$dataB["ID_K"]."' AND ID_SK = '".$data["ID_SK"]."'";
						$resultC = @mysql_query($queryC,$db);
						if(is_resource($resultC) and mysql_num_rows($resultC)>0)
						{echo 'checked="checked"';}else{echo "";}
						?>><? echo $data["nazovSkolenia"]; ?> (<?echo $data["informaciaOSkoleni"]?>)<br>
					<?}
				}
				?>
				<h4><input type="checkbox" name="news" value="Yes" checked="checked"/> I want to receive notifications <br></h4><?
			}
			?>
				<input type="submit">
	</form>		
<?		
	}
?>	
	