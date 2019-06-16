<?

function addUradnik($db)
{
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
				$workDayErr = "The first work day is required";
			  }
			  else
			  {
				  if(!filter_mydate($_POST["workday"]))
				  {
					 $workDayErr = "Invalid date input";
				  }
			  }
			  
			  
			if (empty($_POST["birthday"])) {
				$birthdayErr = "The birthday is required";
			}
			else
			{
				if(!filter_mydate($_POST["birthday"]))
				  {
					 $birthdayErr = "Invalid date input";
				  }
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
		Username:<br> <input type="text" name="user" value="<?php echo isset($_POST['user']) ? $_POST['user'] : '' ?>"/><a class="err">*<? echo $nameErr; ?></a><br>
		Password:<br> <input type="PASSWORD" name="pass"/><a class="err">*<? echo $passErr; ?></a><br>
		Title:<br> <input type="text" name="title"/><a class="err"></a><br>
		First Name:<br> <input type="text" name="FName" value="<?php echo isset($_POST['FName']) ? $_POST['FName'] : '' ?>"/><a class="err">*<? echo $Ferr; ?></a><br>
		Last Name:<br> <input type="text" name="LName" value="<?php echo isset($_POST['LName']) ? $_POST['LName'] : '' ?>"/><a class="err">*<? echo $Lerr; ?></a><br>
		E-mail:<br> <input type="text" name="mail" placeholder="someone@email.com" value="<?php echo isset($_POST['mail']) ? $_POST['mail'] : '' ?>"/><a class="err">*<? echo $mailErr; ?></a><br>
		Wage:<br> <input type="text" name="mzda" value="<?php echo isset($_POST['mzda']) ? $_POST['mzda'] : '' ?>"/><a class="err">*<? echo $mzdaErr; ?></a><br>
		Day of Birth:<br> <input type="text" name="birthday" placeholder="YYYY-MM-DD" value="<?php echo isset($_POST['birthday']) ? $_POST['birthday'] : '' ?>"/><a class="err">*<? echo $birthdayErr; ?></a><br>
		Turn-out day:<br> <input type="text" name="workday" placeholder="YYYY-MM-DD" value="<?php echo isset($_POST['workday']) ? $_POST['workday'] : '' ?>"/><a class="err">*<? echo $workDayErr; ?></a><br>
		<input type="checkbox" name="typ" /> Set admin rights? <br>
			<input type="submit" name="add_uradnik_button" value="Add Worker">
		</form>
	<?
	if(isset($_POST['add_uradnik_button']))
		{	
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
								if((isset($_POST['mzda']) && ($_POST['mzda'] != "")) && is_numeric($_POST["mzda"]))
								{
									if((isset($_POST['birthday']) && ($_POST['birthday'] != "")) && filter_mydate($_POST["birthday"]))
									{
										if((isset($_POST['workday']) && ($_POST['workday'] != "")) && filter_mydate($_POST["workday"]))
										{
													$query="SELECT * FROM Uradnik WHERE loginK='".$_POST["user"]."'";
													$result = @mysql_query($query, $db);
													$queryA="SELECT * FROM Uradnik WHERE email='".$_POST["mail"]."'";
													$resultA= @mysql_query($queryA, $db);
													
													if(isset($_POST['typ']))
													{
														$typ = 2;
													}
													else
													{
														$typ = 1;
													}
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
														$query="INSERT INTO Uradnik (`krstneMeno_U`, `priezvisko_U`,`denNastupu`,`datumNarodenia_U`,`email`,`titul`,`mzda`,`typ`,`loginU`,`hesloU`) 
														VALUES 
														('".mysql_real_escape_string($_POST["FName"])."',
														'".mysql_real_escape_string($_POST["LName"])."',
														'".mysql_real_escape_string($_POST["workday"])."',
														'".mysql_real_escape_string($_POST["birthday"])."',
														'".mysql_real_escape_string($_POST["mail"])."',
														'".mysql_real_escape_string($_POST["title"])."',
														'".mysql_real_escape_string($_POST["mzda"])."',
														'".mysql_real_escape_string($typ)."',
														'".mysql_real_escape_string($_POST["user"])."',
														'".mysql_real_escape_string($_POST["pass"])."')
														
														";
														$result = @mysql_query($query,$db);
														echo "Worker was successfully added.";
														//header('Location: index.php?menu=4');
													}
										}
									}
								}
							}
						}
					}
				}
			}
		}
}
?>