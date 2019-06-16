<?


function addWork($db,$u)
{
	?>
	<h3>My job positions</h3>
	<?
	// vypis tabulky pozicii podla uradnika
	$first_row = true;
		$query="SELECT ID_PP as 'ID', datumPridania as 'Addition day',naplnPrace as 'Work description', miesto as 'Place', obsadenost as 'Number of candidates' 
				FROM Pracovne_pozicie
				WHERE ID_U = '".$_SESSION['ID']."'";
		$id = 0;
		$result = mysql_query($query,$db);
		echo '<table ID="tabulkaPP" cellspacing=0>';
		while ($row = mysql_fetch_assoc($result)) {
			if ($first_row) {
				$first_row = false;
				// Output header row from keys.
				echo '<tr>';
				foreach($row as $key => $field) {
					echo '<th>' . htmlspecialchars($key) . '</th>';
				}
				echo '</tr>';
			}
			$xx = 1;
			echo "<tr align=center>";
			foreach($row as $key => $field) {
				if($xx == 1)
				{$xx = 2; $id = htmlspecialchars($field);}
				
				echo "<td><a href='?menu=0&pp=".$id."' display:inline-block;><div>" . htmlspecialchars($field) . "</div></a></td>";
				
			}
			
			echo "<td><a href='removeWork.php?w=".$id."'> X</a></td>";
			echo '</tr>';
		}
		echo '</table>';
	?>	
	<h2> Add job position </h2>	
	<h3> Choose company </h3>
	<?
	// zaciatok tvorby novej pozicie
	
	$typ = 0;

	$query = "SELECT * FROM Firma";
	$result = @mysql_query($query, $db);
	
		
	?>
	<form method="POST">
	
		<select name="company">
			<option value="0">Add New Company</option>
			<? while($data = mysql_fetch_array($result, MYSQL_ASSOC)){?>
			<option value="<?echo $data['ID_F']?>"><?echo $data['nazovFirmy']?></option>
			<?}?>
		</select>
			<input type="submit" name="add_work_button" value="Choose"/>
		</form>
	<?	
		
		if(isset($_POST['add_work_button']))
		{
			if($_POST['company'] == 0)
			{
				
				$_SESSION["addcompany"]=1;
				
			}
			else
			{
				$_SESSION["addcompany"]=0;
				//pouzi existujucu firmu
				$query="SELECT ID_F as 'ID', nazovFirmy as 'Nazov',zodpovednaOsoba as 'Zodpovedna osoba', adresa as 'Adresa', sidloFirmy as 'Sidlo', pocetZamestnancov as 'Pocet zamestnancov' 
				FROM Firma
				WHERE ID_F = '".$_POST['company']."'";
				$result = @mysql_query($query, $db);
				$data = mysql_fetch_array($result, MYSQL_ASSOC);
				echo "Company name: ".$data['Nazov']."<br>";
				echo "Authorized person: ".$data['Zodpovedna osoba']."<br>";
				echo "Address: ".$data['Adresa']."<br>";
				echo "Company headquarters: ".$data['Sidlo']."<br>";
				echo "Worker count: ".$data['Pocet zamestnancov']."<br>";
				$_SESSION["selectCompID"] = $data["ID"];				
				?>
				<?
			}
		}
		
		
	if($_SESSION["addcompany"]==1)
	{
		addCompany($db);	
	}
	
	if($_SESSION["selectCompID"] != 0)
	{
		?>
		<h3>Job position</h3>
		<?
		addPosition($db,$u);	
	}
}

function updateWork($db,$pp)
{
	
	$queryA="SELECT * FROM Uradnik WHERE ID_U='".$u."'";
	$resultA = @mysql_query($queryA, $db);
	if(is_resource($resultA) and mysql_num_rows($resultA)>0)
	{
		$data = mysql_fetch_array($resultA, MYSQL_ASSOC);
				

		$typ = 0;
		
		$workDayErr = "";
		$birthdayErr = "";
		$passErr = "";
		$Ferr = "";
		$Lerr = "";
		$mailErr = "";
		$mzdaErr = "";
		//$adddayErr = "";
		//$popispraceErr = "";
		//$wageErr = "";
		//$placeErr = "";

		if ($_SERVER["REQUEST_METHOD"] == "POST") 
			{
				if (empty($_POST["workday"])) {
					$workDayErr = "Addition date is required";
				}
			    else
			    {
				  if(!filter_mydate($_POST["workday"]))
				  {
					 $workDayErr = "Invalid date input";
				  }
			    }
				
				if (empty($_POST["birthday"])) {
					$birthdayErr = "Birthday is required";
				}
			    else
			    {
				  if(!filter_mydate($_POST["birthday"]))
				  {
					 $birthdayErr = "Invalid date input";
				  }
			    }
				
				if (empty($_POST["FName"])) {
					$Ferr = "First name is required";
				  }
				  if (empty($_POST["LName"])) {
					$Lerr = "Last name is required";
				  }
				  if (empty($_POST["mail"])) {
					$mailErr = "E-mail is required";
				  }
				  else
				  {
					 if(!filter_var($_POST['mail'], FILTER_VALIDATE_EMAIL))
						{
							$mailErr = "E-mail is incorrect";
						} 
				  }
				   if (empty($_POST["mzda"])) {
					$wageErr = "Wage is required";
				  }
				  else
				  {
					  if (!is_numeric($_POST["mzda"])) {
						$wageErr = "Wage must be a number";
					  }
				  }
				 

			}
		?>
		<form method="POST">
			Username: <? echo $data["loginU"];?><br>
			Password:<br> <input type="PASSWORD" name="pass"/><a class="err">*<? echo $passErr; ?></a><br>
			Title:<br> <input type="text" name="title" value="<? echo $data["titul"]; ?>"/><a class="err"></a><br>
			First Name:<br> <input type="text" name="FName" value="<?php echo $data["krstneMeno_U"]; ?>"/><a class="err">*<? echo $Ferr; ?></a><br>
			Last Name:<br> <input type="text" name="LName" value="<?php  echo $data["priezvisko_U"];  ?>"/><a class="err">*<? echo $Lerr; ?></a><br>
			E-mail:<br> <input type="text" name="mail" placeholder="someone@email.com" value="<?php echo $data["email"];  ?>"/><a class="err">*<? echo $mailErr; ?></a><br>
			Wage:<br> <input type="text" name="mzda" value="<?php  echo $data["mzda"];  ?>"/><a class="err">*<? echo $mzdaErr; ?></a><br>
			Day of Birth:<br> <input type="text" name="birthday" placeholder="YYYY-MM-DD" value="<?php  echo $data["datumNarodenia_U"];  ?>"/><a class="err">*<? echo $birthdayErr; ?></a><br>
			Turn-out day:<br> <input type="text" name="workday" placeholder="YYYY-MM-DD" value="<?php  echo $data["denNastupu"];  ?>"/><a class="err">*<? echo $workDayErr; ?></a><br>
			<input type="checkbox" name="typ" /> Set admin rights? <br>
				<input type="submit" name="update_button" value="Update">
				<input type="submit" name="delete_button" value="Delete">				
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
								if((isset($_POST['birthday']) && ($_POST['birthday'] != "")) && filter_mydate($_POST["birthday"]))
								{ 
									if((isset($_POST['workday']) && ($_POST['workday'] != "")) && filter_mydate($_POST["workday"]))
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

function addPosition($db,$u)
{
	$workdayErr = "";
	$adddayErr = "";
	$popispraceErr = "";
	$wageErr = "";
	$placeErr = "";
	$descErr = "";

	if ($_SERVER["REQUEST_METHOD"] == "POST") 
		{
			if (empty($_POST["WorkDay"])) {
				$workdayErr = "Addition date is required";
			  }
			  else
			  {
				  if(!filter_mydate($_POST["Workday"]))
				  {
					 $workdayErr = "Invalid date input";
				  }
			  }
			
			if (empty($_POST["AddDay"])) {
				$adddayErr = "Addition day is required";
			  }
			  else
			  {
				  if(!filter_mydate($_POST["AddDay"]))
				  {
					 $adddayErr = "Invalid date input";
				  }
			  }
			  if (empty($_POST["WorkDesc"])) {
				$popispraceErr = "Work description is required";
			  }
			  if (empty($_POST["WorkPlace"])) {
				$placeErr = "Workplace is required";
			  }
			  if (empty($_POST["Wage"])) {
				$wageErr = "Wage is required";
			  }
			  else
			  {
				  if (!is_numeric($_POST["Wage"])) {
					$wageErr = "Wage must be a number";
				  }
			  }
			  if (empty($_POST["desc"])) {
				$descErr = "Position description is required";
			  }

		}
	?>
		<form method="POST">
			Date of addition:<br> <input type="text" name="AddDay" value="<?php echo isset($_POST['AddDay']) ? $_POST['AddDay'] : '' ?>"/><a class="err">*<? echo $adddayErr; ?></a><br>
			Date of Turn-out:<br> <input type="text" name="WorkDay" value="<?php echo isset($_POST['WorkDay']) ? $_POST['WorkDay'] : '' ?>"/><a class="err">*<? echo $workdayErr; ?></a><br>
			Work Description:<br> <input type="text" name="WorkDesc" value="<?php echo isset($_POST['WorkDesc']) ? $_POST['WorkDesc'] : '' ?>"/><a class="err">* <? echo $popispraceErr; ?></a><br>
			Workplace:<br> <input type="text" name="WorkPlace" value="<?php echo isset($_POST['WorkPlace']) ? $_POST['WorkPlace'] : '' ?>"/><a class="err">*<? echo $placeErr; ?></a><br>
			Wage:<br> <input type="text" name="Wage" value="<?php echo isset($_POST['Wage']) ? $_POST['Wage'] : '' ?>"/><a class="err">*<? echo $wageErr; ?></a><br>
			Title:<br> <input type="text" name="title" value="<?php echo isset($_POST['title']) ? $_POST['title'] : '' ?>"/><a class="err"></a><br>
			Position description:<br> <input type="text" name="desc" value="<?php echo isset($_POST['desc']) ? $_POST['desc'] : '' ?>"/><a class="err">*<? echo $descErr; ?></a><br>
			<? 
				$query="SELECT * FROM Benefity";
				$result = @mysql_query($query,$db);
			while($data = mysql_fetch_array($result, MYSQL_ASSOC)){?>
				<input type="checkbox" name="Benefity<?echo $data["ID_B"]?>"><? echo $data["popisBenefitu"]; ?><br>
			<?}?>
			<input type="submit" name="addCB" value="Add"/>
		</form>
	<?
	if(isset($_POST['addCB']))
		{	
			if((isset($_POST['WorkDay']) && ($_POST['WorkDay'] != "")) && filter_mydate($_POST["WorkDay"]))
			{
				if((isset($_POST['AddDay']) && ($_POST['AddDay'] != "")) && filter_mydate($_POST["AddDay"]))
				{
					if((isset($_POST['WorkDesc']) && ($_POST['WorkDesc'] != "")))
					{
						if((isset($_POST['WorkPlace']) && ($_POST['WorkPlace'] != "")))
						{
							if((isset($_POST['Wage']) && ($_POST['Wage'] != "")) && is_numeric($_POST["Wage"]))
							{
								
									$query="INSERT INTO Pracovne_pozicie(`datumPridania`, `datumNastupu`,`naplnPrace`,`hrubaMzda`,`miesto`, `obsadenost`,`ID_U`, `ID_F`) 
														VALUES 
														('".mysql_real_escape_string($_POST["AddDay"])."',
														'".mysql_real_escape_string($_POST["WorkDay"])."',
														'".mysql_real_escape_string($_POST["WorkDesc"])."',
														'".mysql_real_escape_string($_POST["Wage"])."',
														'".mysql_real_escape_string($_POST["WorkPlace"])."',
														'0',
														'".mysql_real_escape_string($_SESSION['ID'])."',
														'".mysql_real_escape_string($_SESSION['selectCompID'])."'
														)
														";
									$result = @mysql_query($query,$db);
									if((isset($_POST['desc']) && ($_POST['desc'] != "")))
									{
										$queryTMP = "SELECT * FROM Specializacia WHERE pozadovanyTitul='".$_POST["title"]."' AND nazov='".$_POST["desc"]."'";
										$resultTMP = @mysql_query($queryTMP,$db);
										if(!(is_resource($resultTMP) and mysql_num_rows($resultTMP)>0))
										{
											$query = "INSERT INTO Specializacia(`pozadovanyTitul`, `nazov`)
															VALUES 
															('".mysql_real_escape_string($_POST["title"])."','".mysql_real_escape_string($_POST["desc"])."')";
											$result = @mysql_query($query,$db);
										}
										
									}
									$queryA ="SELECT * FROM Pracovne_pozicie WHERE naplnPrace = '".$_POST['WorkDesc']."' AND datumPridania='".$_POST['AddDay']."'
												AND datumNastupu='".$_POST['WorkDay']."' 
												AND hrubaMzda='".$_POST['Wage']."' 
												AND miesto='".$_POST['WorkPlace']."' 
												AND obsadenost='0'
												AND ID_U='".$_SESSION['ID']."'
												AND ID_F='".$_SESSION['selectCompID']."'";
									$queryB = "SELECT * FROM Specializacia WHERE pozadovanyTitul='".$_POST["title"]."' AND nazov='".$_POST["desc"]."'";
									$resultA = @mysql_query($queryA,$db);
									$resultB = @mysql_query($queryB,$db);
									$dataA = mysql_fetch_array($resultA, MYSQL_ASSOC);
									$dataB = mysql_fetch_array($resultB, MYSQL_ASSOC);
									
									$queryQ = "INSERT INTO Pozicia_Specializacia(`ID_PP`, `ID_SP`) VALUES ('".$dataA["ID_PP"]."','".$dataB["ID_SP"]."')";
									$resultQ = @mysql_query($queryQ,$db);
									
									if(isset($_POST["Benefity1"]))
									{
										//echo "aloha1";
										$query="INSERT INTO Pozicia_Benefity(`ID_PP`, `ID_B`) VALUES ('".$dataA["ID_PP"]."','1')";
										$result = @mysql_query($query,$db);
									}
									if(isset($_POST["Benefity2"]))
									{
										
										$query="INSERT INTO Pozicia_Benefity(`ID_PP`, `ID_B`) VALUES ('".$dataA["ID_PP"]."','2')";
										$result = @mysql_query($query,$db);
									}
									if(isset($_POST["Benefity3"]))
									{
										//echo "aloha3";
										$query="INSERT INTO Pozicia_Benefity(`ID_PP`, `ID_B`) VALUES ('".$dataA["ID_PP"]."','3')";
										$result = @mysql_query($query,$db);
									}
									if(isset($_POST["Benefity4"]))
									{
										//echo "aloha4	";
										$query="INSERT INTO Pozicia_Benefity(`ID_PP`, `ID_B`) VALUES ('".$dataA["ID_PP"]."','4')";
										$result = @mysql_query($query,$db);
									}
									
									
									echo "Position was successfully added.";
									$_SESSION["addcompany"]=0;
									$_SESSION["selectCompID"]=0;
							}
						}
					}
				}
			}
		}
}
?>