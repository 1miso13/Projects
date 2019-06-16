<?


function updateUradnik($db,$u)
{
	
	$queryA="SELECT * FROM Uradnik WHERE ID_U='".$u."'";
	$resultA = @mysql_query($queryA, $db);
	if(is_resource($resultA) and mysql_num_rows($resultA)>0)
	{
		$data = mysql_fetch_array($resultA, MYSQL_ASSOC);
				

		$typ = 0;
		
		$nameErr = "";
		$passErr = "";
		$mailErr = "";
		$Lerr = "";
		$Ferr = "";
		
		$workDayErr = "";
		$birthdayErr = "";
		$mzdaErr = "";

		if ($_SERVER["REQUEST_METHOD"] == "POST") 
			{
				if (empty($_POST["user"])) {
					$nameErr = "Username is required";
				  }
				
				if (empty($_POST["mzda"])) {
					$mzdaErr = "Wage is required";
				  }
				  else
				  {
					  if (!is_numeric($_POST["mzda"])) {
						$mzdaErr = "Wage must be a number";
					  }
				  }
				  if (empty($_POST["workday"])) {
					$workDayErr = "Turn-out day is required";
				  }
				  if (empty($_POST["birthday"])) {
					$birthdayErr = "Birthday is required";
				  }

				if (empty($_POST["mail"])) 
				{
					$mailErr = "E-mail is required";
				}
				else
				{
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
			Username: <? echo $data["loginU"];?><br>
			Password:<br> <input type="PASSWORD" name="pass" value="******"/><a class="err">*<? echo $passErr; ?></a><br>
			Title:<br> <input type="text" name="title" value="<? echo $data["titul"]; ?>"/><a class="err"></a><br>
			First Name:<br> <input type="text" name="FName" value="<?php echo $data["krstneMeno_U"]; ?>"/><a class="err">*<? echo $Ferr; ?></a><br>
			Last Name:<br> <input type="text" name="LName" value="<?php  echo $data["priezvisko_U"];  ?>"/><a class="err">*<? echo $Lerr; ?></a><br>
			E-mail:<br> <input type="text" name="mail" placeholder="someone@email.com" value="<?php echo $data["email"];  ?>"/><a class="err">*<? echo $mailErr; ?></a><br>
			Wage:<br> <input type="text" name="mzda" value="<?php  echo $data["mzda"];  ?>"/><a class="err">*<? echo $mzdaErr; ?></a><br>
			Day of Birth:<br> <input type="text" name="birthday" placeholder="YYYY-MM-DD" value="<?php  echo $data["datumNarodenia_U"];  ?>"/><a class="err">*<? echo $birthdayErr; ?></a><br>
			Turn-out day:<br> <input type="text" name="workday" placeholder="YYYY-MM-DD" value="<?php  echo $data["denNastupu"];  ?>"/><a class="err">*<? echo $workDayErr; ?></a><br>
			<?if($_SESSION["username"] != $data["loginU"]){?>
				<input type="checkbox" name="typ" /> Set admin rights? <br>
			<?}?>
				<input type="submit" name="submit_button" value="Update">
				<?if($_SESSION["username"] != $data["loginU"]){?>
					<input type="submit" name="delete_button" value="Delete">
				<?}?>
			</form>
		<?	
		
		if(isset($_POST['submit_button']))
		{			
			if((isset($_POST['pass']) && ($_POST['pass'] != "")))
			{
				if((isset($_POST['FName']) && ($_POST['FName'] != "")))
				{
					if((isset($_POST['LName']) && ($_POST['LName'] != "")))
					{
						if((isset($_POST['mail']) && ($_POST['mail'] != "")) && filter_var($_POST['mail'], FILTER_VALIDATE_EMAIL))
						{
							if((isset($_POST['mzda']) && ($_POST['mzda'] != "")) && is_numeric($_POST['mzda']))
							{
								if((isset($_POST['birthday']) && ($_POST['birthday'] != "")))
								{ 
									if((isset($_POST['workday']) && ($_POST['workday'] != "")))
									{ 
												if(isset($_POST['typ']))
												{
													$typ = 2;
												}
												else
												{
													$typ = 1;
												}

													$queryA="SELECT * FROM Uradnik WHERE email='".$_POST["mail"]."'";
													$resultA= @mysql_query($queryA, $db);

													if($_POST["mail"] != $data["email"] && (is_resource($resultA) and mysql_num_rows($resultA)>0))
													{
														
														if((is_resource($resultA) and mysql_num_rows($resultA)>0))
														{
															?><a class="err">The given E-mail already exists.</a><?
														}
													}
													else
													{
														//$query = "UPDATE Klient SET krstneMeno_K='".$_POST["FName"]."',priezvisko_K='".$_POST["LName"]."',
													//email='".$_POST["mail"]."', titul='".$_POST["title"]."',hesloK='".$_POST["pass"]."',
													//datumNarodenia_K='".$_POST["date"]."' WHERE ID_K='".$_SESSION["ID"]."'";
														$query="UPDATE Uradnik SET 
														krstneMeno_U='".mysql_real_escape_string($_POST["FName"])."', 
														priezvisko_U='".mysql_real_escape_string($_POST["LName"])."',
														denNastupu='".mysql_real_escape_string($_POST["workday"])."',
														datumNarodenia_U='".mysql_real_escape_string($_POST["birthday"])."',
														titul='".mysql_real_escape_string($_POST["title"])."',
														mzda='".mysql_real_escape_string($_POST["mzda"])."',
														typ='".mysql_real_escape_string($typ)."'													
														WHERE ID_U = '".$u."'"
														;
														$result = @mysql_query($query,$db);
														
														if($_POST["pass"] != $data["hesloU"])
														{
															$query = "UPDATE Uradnik SET 
																hesloU='".mysql_real_escape_string($_POST["pass"])."' 
																WHERE 
																	ID_U = '".$u."'";
															$result = @mysql_query($query,$db);
														}
														if($_POST["mail"] != $data["email"])
														{
															$query = "UPDATE Uradnik SET 
																email='".mysql_real_escape_string($_POST["mail"])."' 
																WHERE 
																	ID_U = '".$u."'";
															$result = @mysql_query($query,$db);
														}
														header( 'Location: index.php?menu=2' ) ;
													}
												
									}
								}
							}
						}
					}
				}
			}
		}
		else if(isset($_POST['delete_button']))
		{
			removeUradnik($db,$u);
		}
	}
}

function removeUradnik($db, $u)
{
	$query = "SELECT * FROM Uradnik WHERE ID_U = '".$u."'";
	$result = @mysql_query($query,$db);
	$data = mysql_fetch_array($result, MYSQL_ASSOC);
	if($data['typ'] == 2) // 2 is for admins
	{
		?><a class="err"> You can not delete an admin.</a><?
	}else{
	$query ="DELETE FROM Uradnik WHERE ID_U = '".$u."'";
	$result = @mysql_query($query,$db);
	header( 'Location: index.php?menu=2' ) ;
	}
}
?>