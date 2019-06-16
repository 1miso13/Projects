<?
function menu($num,$db,$u/*$pp,$k*/){
$tmp1 = 0;
$tmp2 = 10;
/// menu
if(!isset($_SESSION['username'])){$_SESSION['username'] = "";}
if(!isset($_SESSION['TYPE'])){$_SESSION['TYPE'] = 0;}
if(!isset($_SESSION['author'])){$_SESSION['author'] = 0;}
?>
	<div id=menu>
	<table id="tabulkaF" cellspacing=0>
	<tr>
		<td id="menu1">
			<ul>
				<?if($_SESSION["TYPE"] == 2){?>
				<li <?if($num==0){?>class="current"<?}?>><a href="?menu=0">Recenzie</a></li>
				<li <?if($num==55){?>class="current"<?}?>><a href=<?echo "'?menu=55&u=".$_SESSION['ID']."'"?>>Dokumenty</a></li>
				<?}?>
				<?if($_SESSION["TYPE"] == 1){?>
				<li <?if($num==0){?>class="current"<?}?>><a href="?menu=0">Domov</a></li>
				<li <?if($num==2){?>class="current"<?}?>><a href="?menu=2">Sluzby</a></li>
				<?}?>			
				<li <?if($num==1){?>class="current"<?}?>><a href="?menu=1">O nas</a></li>
			</ul>
		</td>	
	   <td class="box1">
				<div class="box_content">
					
					<?
					if($_SESSION['username'] == "")
					{
						?>
						<div><a href="?menu=4" class="reserve">Prihlasit</a></div>
						<!--<div><a href="?menu=5" class="reserve">Register</a></div>-->
						<?
					}
					else
					{
						?>
						<div><a href="?menu=20" class="reserve"><?echo $_SESSION['username'];?></a></div>
						<div><a href="logout.php" class="reserve">Odhlasit</a></div>
						<?
					}
					?>
				</div>                        
		</td>
	</tr>
	</table>
	</div>
</div>

<div id="telos">
<?
 switch($num){
	case 0:
	
	if($_SESSION["TYPE"] == 2)
	{
		?>
		<h1>Dokumenty na recenziu</h1>
		<?
		$first_row = true;
		$query="SELECT U.ID_U as 'ID', U.login_U as 'User', U.FILENAME as 'File'
				FROM users U, reviewer R
				WHERE U.ID_U = R.ID_AUTOR AND R.ID_RECENZENT = '".$_SESSION['ID']."'";
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
		//$pokus = stripslashes(mysql_result($result,$row,0));
		$xx = 1;
		echo "<tr align=center>";
		//echo "<a href='?menu=0&'".$row[0].">";
		foreach($row as $key => $field) {
			if($xx == 1)
			{$xx = 2; $id = htmlspecialchars($field);}
			
			echo "<td><a href='?menu=54&u=".$id."' display:inline-block;><div>" . htmlspecialchars($field) . "</div></a></td>";
			
		}
		//echo "</a>";
		echo '</tr>';
		}
		echo '</table>';
		
		
		
		
	}else if ($_SESSION["TYPE"] == 1)
	{
		?>
		<h1>Recenzia dokumentov</h1>
		<?
		$first_row = true;
		$query="SELECT U1.ID_U as 'ID', U1.login_U as 'User', U1.FILENAME as 'File', U2.login_U as 'Reviewer'
				FROM users as U1, users as U2, reviewer as R
				WHERE U1.ID_U = R.ID_AUTOR AND U2.ID_U = R.ID_RECENZENT";
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
		//$pokus = stripslashes(mysql_result($result,$row,0));
		$xx = 1;
		echo "<tr align=center>";
		//echo "<a href='?menu=0&'".$row[0].">";
		foreach($row as $key => $field) {
			if($xx == 1)
			{$xx = 2; $id = htmlspecialchars($field);}
			
			echo "<td><a href='?menu=55&u=".$id."' display:inline-block;><div>" . htmlspecialchars($field) . "</div></a></td>";
			
		}
		//echo "</a>";
		echo '</tr>';
		}
		echo '</table>';
		}
		else
		{
			header( 'Location: index.php?menu=4' ) ;
		}
	break;
	/*-----------------------------------------------------------------------*/
	case 1:
	?>
	<h1>O nas</h1>
	<h3>Autori:</h3>
	- Julius Kery (xkeryj00@stud.fit.vutbr.cz)<br>
	- Michal Ondrejo (xondre08@stud.fit.vutbr.cz)<br>
	- Juraj Sokol (xsokol08@stud.fit.vubr.cz)<br>
	<br>
	- Tato stranka bola vytvorena ako projekt do predmetu Tvorba uzivatelskych rozhrani [ITU]. <br>

	<?
	break;
	/*-----------------------------------------------------------------------*/
	case 2:
	/*ADMIN ADDS NEW USERS*/
	?>
	<h1>Sluzby</h1>
	<?if($_SESSION["TYPE"] == 1)
	{?> <!-- ADD USERS BUTTON-->
		<?/*<div>
			<form method="POST">
				<input type="file" id="my_file"><br><br>
					<input type="button" id="get_file" name="add_users" value="Upload">
			</form>
		</div>
		*/?>
		<form enctype="multipart/form-data" method="post" role="form">
			<div class="form-group">
				<label for="exampleInputFile"><h2>Nahravanie suboru</h2></label>
				<p class="help-block">Pozn.: Prosim pouzivajte CSV subory.</p>
				<input type="file" name="excel_file" id="file">				
			</div>
			<br><button type="submit" class="btn btn-default" name="Import" value="Import">Nahraj</button>
		</form>
		<!-- TU BUDE FUNKCIA NA ULOZENIE DO DB-->
		<?
		
		if(isset($_POST['Import']))
		{
			$target_dir = "uploads/";
			$target_file = $target_dir . basename($_FILES['excel_file']['name']);
			$imageFileType = pathinfo($target_file,PATHINFO_EXTENSION);
			if($imageFileType != "csv")  
				{
					?>
					<a class='err'> <?echo "Iba CSV subory."; ?></a>
					<?
					$uploadOk = 0;
				}
				else
				{
					$uploadOk = 1;
				}
				
			if($uploadOk == 1)
			{
				if (move_uploaded_file($_FILES['excel_file']['tmp_name'], $target_file)) {
					echo "Subor ". basename( $_FILES['excel_file']['name']). " sa nahral.";
					$uploaded = 1;
				} else {
					$uploaded = 0;
					echo "Sorry, there was an error uploading your file.";
				}
			}

			/*if($uploaded == 1)
			{
				echo $filename = $target_dir . $_FILES['excel_file']['name'];
				if(($_FILES['excel_file']['name'] != "") ||($_FILES['excel_file']['size'] != 0))
				{
					$file = fopen($filename, "r");
					
					$count = 0;  
					ini_set('auto_detect_line_endings',TRUE);
					while (($emapData = fgetcsv($file, 0, ";")))
					{
						
						//print_r($emapData);
						//exit();
						$count++;                                      // add this line

						if($count>1)
						{                                  // add this line
						  $name = $emapData[0];
						  $email = $emapData[1];
						  $sql = "INSERT into users(login_U,email_U, password_U, type_U) values ('".$name."','".$email."', '".randomPassword()."', 2)";
						  mysql_query($sql); 
						}                                              
					}
					fclose($file);
					$file = fopen($filename, "r");
					ini_set('auto_detect_line_endings',TRUE);
					while (($emapData = fgetcsv($file, 0, ";")))
					{
						$count++;                                      // add this line

						if($count>1)
						{
							$size = count($emapData);
							$posuv = 2;
							// potrebujeme ID_U autora
								$autor = $emapData[0];
							  $sqlA = "SELECT * FROM users WHERE login_U = '".$autor."'";
							  $resultA = mysql_query($sqlA);
							  $dataA = mysql_fetch_array($resultA, MYSQL_ASSOC);
							  while($posuv < $size)
							  {
								$login = $emapData[$posuv];
								$tmpsql = "SELECT * FROM users where login_U ='".$login."'";
								$result = mysql_query($tmpsql);
								$data = mysql_fetch_array($result, MYSQL_ASSOC);
								if(mysqli_num_rows($result)>0)
									{								
										$query = "INSERT INTO reviewer(ID_AUTOR, ID_RECENZENT) VALUES('".$dataA['ID_U']."','".$data['ID_U']."')";
										$result = mysql_query($query);
										$posuv = $posuv+1; // found such user, moving forward
									}
								else
									{
										// nenaslo, chyba
									}
							  }
						}
					}
					fclose($file);
				}
				else
				{
					?><a class='err'>File could not be uploaded.</a><?
				}
			}*/
		}
		?>
	<?
		/*tu su vsetci vypísany*/
		echo "<h2>Uzivatelia</h2>";
		$rozsah = 10;
		$offsetA = $tmp1;
		$offsetB = $offsetA + $rozsah;
		
		$first_row = true;
		$query="SELECT ID_U as 'ID', email_U as 'E-mail', login_U as 'User', password_U as 'Passwords'
				FROM users LIMIT ".$offsetA.",".$offsetB."";
		$id = 0;
		$result = mysql_query($query,$db);
		$num_rows = mysql_num_rows($result);
		?>Pocet uzivatelov <? echo $num_rows; ?><br><br><?

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
		//$pokus = stripslashes(mysql_result($result,$row,0));
		$xx = 1;
		echo "<tr align=center>";
		//echo "<a href='?menu=0&'".$row[0].">";
		foreach($row as $key => $field) {
			if($xx == 1)
			{$xx = 2; $id = htmlspecialchars($field);}
			
			echo "<td><a href='?menu=55&u=".$id."' display:inline-block;><div>" . htmlspecialchars($field) . "</div></a></td>";
			
		}
		//echo "</a>";
		echo '</tr>';
		}
		echo '</table>';
		?>
		<br>
		<?
		
		
		if($num_rows > 10)
		{	
		?>
		<div>
			<form method="POST">
				<?if($offsetA > 0){?><input type="button" name="back" value="Back"><?}?>
				<?if(($num_rows - $offsetB)>0){?><input type="button" name="next" value="Next"><?}?>
			</form>
		</div>
		<?
		}
		if(isset($_POST['back']))
		{
			$offsetA = $offsetA - $rozsah;
			$offsetB = $offsetB - $rozsah;
		}
		else if(isset($_POST['next']))
		{
			$offsetA = $offsetA + $rozsah;
			$offsetB = $offsetB + $rozsah;
		}
		/*KONIEC*/
	?>
		
	<?}
	else
	{
		header( 'Location: index.php' ) ;
	}?>

	<?
	break;
	
	/*-----------------------------------------------------------------------*/
	case 4:
	?>
	<h1>LOGIN</h1>
	<?
		login($db);
	
	break;

	case 15:
		?>
		<h3>Login successful.</h3>
		<?
		header( 'Location: index.php' ) ;
		
	break;
	/*-----------------------------------------------------------------------*/
	case 20:
	?> 
		<h1>Profil</h1>
	<?
	/*Pozriet si profil recenzenta, alebo autora, jednoducho ked chces si najst udaje toho co ti napisal nieco na dokument*/
		Profile($db);	
	break;
	/*-----------------------------------------------------------------------*/
	case 54:
	?>
			<?
			$myID = $_GET['u'];
			$query = "SELECT * FROM users WHERE ID_U = '".$myID."'";
			$result = mysql_query($query);
			$data = mysql_fetch_array($result, MYSQL_ASSOC);
			$fil = $data["FILENAME"];
			if($fil == "File was not uploaded yet")
			{
				?><a class='err'>Uzivatel nenahral dokument. Prosim vratte sa neskor.</a>
				<br><form method="post">
					<button type="submit" name="back" value="Back">Back</button>
					</form><?
				if(isset($_POST['back']))
				{
					header( 'Location: index.php' ) ;
				}
				
			} else
			{
				review();
			}
			
			?>
	<?		
	break;
	
	case 55:
	?> 
		<h1>Profil</h1>
	<?
		$query="SELECT * FROM users WHERE ID_U='".$u."'";
			$result = @mysql_query($query, $db);
			if(is_resource($result) and mysql_num_rows($result)>0){ 
				while($data = mysql_fetch_array($result, MYSQL_ASSOC))
				{
						?>Uzivatel: <? echo $data["login_U"];?><br><?
						?>Email: <? echo $data["email_U"];?><br><?	
				}	
			}
		if($_SESSION["TYPE"] == 1 || $_SESSION["TYPE"] == 2)
		{
			$myID = $_GET['u'];
			if($_SESSION["TYPE"] == 2)
			{
				if($myID != $_SESSION['ID'])
				{
					header( 'Location: index.php' ) ;
				}
				else
				{
				?>
					<form enctype="multipart/form-data" method="post" role="form">
						<div class="form-group">
							<label for="exampleInputFile"><h2>Nahravanie dokumentu</h2></label>
							<p class="help-block">Pozn. : Nahravajte iba pdf subory.</p>
							<input type="file" name="pdf_file" id="file">				
						</div>
						<br><button type="submit" class="btn btn-default" name="Import" value="Import">Nahraj</button>
					</form>	
					<?
					if(isset($_POST['Import']))
					{
						$target_dir = "uploads/";
						$target_file = $target_dir . basename($_FILES['pdf_file']['name']);
						$imageFileType = pathinfo($target_file,PATHINFO_EXTENSION);
						if($imageFileType != "pdf")  
						{
							?>
							<a class='err'> <?echo "Sorry, only PDF files are allowed."; ?></a>
							<?
							$uploadOk = 0;
						}
						else
						{
							$uploadOk = 1;
						}
						if($uploadOk == 1)
							{
								if (move_uploaded_file($_FILES['pdf_file']['tmp_name'], $target_file)) {
									chmod($target_file, 0755);
									$help = basename( $_FILES['pdf_file']['name']);
									echo "The file ". basename( $_FILES['pdf_file']['name']). " has been uploaded.";
									$uploaded = 1;
									$query="UPDATE users SET FILENAME ='".$help."' WHERE ID_U ='".$_SESSION['ID']."' ";
									mysql_query($query);
								} else 
									{
										$uploaded = 0;
										echo "Sorry, there was an error uploading your file.";
									}
							}
					}
				}
			}
			?><br><?
			
			$query = "SELECT * FROM users WHERE ID_U = '".$myID."'";
			$result = mysql_query($query);
			$data = mysql_fetch_array($result, MYSQL_ASSOC);
			$fil = $data["FILENAME"];
		if($fil != "File was not uploaded yet")
		{
			$queryF = "SELECT * from reviews AS R, reviewer AS RR , users as U
					WHERE R.ID_R = RR.ID_R 
					AND 
					RR.ID_AUTOR ='".$myID."' AND RR.ID_RECENZENT=U.ID_U";
			$resultF = mysql_query($queryF);
			//$dataF = mysql_fetch_array($resultF, MYSQL_ASSOC);
			
			?>
			<div id="reviewBody">
				<table id="reviewBody">
					<tr>
						<td class="reviewBodyLeft">
							<?
								echo "<iframe id=fileview src=\"uploads/".$fil."\" width=\"100%\" style=\"height:100%\" ></iframe>";
							?>
						</td>
						
						<td class="reviewBodyRight">
							<form name="form" method="post">
								<h4>Kolko variant rozhrani autor navrhuje?</h4>
									<?
										$resultTMP = mysql_query($queryF);
										while ($row = mysql_fetch_assoc($resultTMP)) {
													
													echo $row["login_U"];
													echo " - ";
													switch($row["answer1"])
													{
														case 0:
															echo "Menej ako 2";
															break;
														case 1:
															echo "2";
															break;
														case 2:
															echo "Viac ako 2";
															break;
														default:
															break;
													}
													?><br><?
										}
									?>

								<h4>Ake odporucania by ste zanechali ohladom navrhu pre autora?</h4>
									<?
									$resultTMP = mysql_query($queryF);
										while ($row = mysql_fetch_assoc($resultTMP)) {
													
													echo $row["login_U"];
													echo " - ";
													echo $row["answer2"];?><br><?
												}
									?>

								<h4>Ako by ste ohodnotili kvalitu obsahu dokumentu?</h4>
									<?
									$resultTMP = mysql_query($queryF);
										while ($row = mysql_fetch_assoc($resultTMP)) {
													
													echo $row["login_U"]; 
													echo " - ";
													switch($row["answer3"])
													{
														case 0:
															echo "100 - 75 %";
															break;
														case 1:
														echo "75 - 50 %";
															break;
														case 2:
														echo "50 - 25 %";
															break;
														case 3:
														echo "25 - 0 %";
															break;
														default:
															break;
													}?><br><?
												}
									?>

								<h4>Ako sa vam pozdavala formalna a jazykova uroven dokumentu?</h4>
									<?
									$resultTMP = mysql_query($queryF);
										while ($row = mysql_fetch_assoc($resultTMP)) {
													
													echo $row["login_U"];
													echo " - ";
													echo $row["answer4"];?><br><?
												}
									?>

								<h4>Pochopili ste zameranie riesenia?</h4>
									<?
									$resultTMP = mysql_query($queryF);
										while ($row = mysql_fetch_assoc($resultTMP)) {
													
													echo $row["login_U"];
													echo " - ";
													switch($row["answer5"])
													{
														case 0:
															echo "Ano";
															break;
														case 1:
															echo "Nie";
															break;
														default:
															break;
													}?><br><?
												}
									?>
								
								<h4>Aka bola technicka uroven?</h4>
									<?
									$resultTMP = mysql_query($queryF);
										while ($row = mysql_fetch_assoc($resultTMP)) {
													
													echo $row["login_U"];
													echo " - ";
													echo $row["answer6"];?><br><?
												}
									?>
								
								<h4>Co sa vam pozdavalo/nepozdavalo na navrhu testovania</h4>
									<?
										$resultTMP = mysql_query($queryF);
										while ($row = mysql_fetch_assoc($resultTMP)) {
													
													echo $row["login_U"];
													echo " - ";
													echo $row["answer7"];?><br><?
												}
									?>
								
								<h4>Co by ste doporucili autorovi?</h4>
									<?
									$resultTMP = mysql_query($queryF);
										while ($row = mysql_fetch_assoc($resultTMP)) {
													
													echo $row["login_U"];
													echo " - ";
													echo $row["answer8"];?><br><?
												}
									?>
							</form>
							
						</td>
					</tr>
				</table>	
			</div><?
		}
		else
		{
			?><a class='err'><h4>Informacie o subore sa zobrazia az po nahrati subore.</h4></a><?
		}
	}
	?>
		
	<?	
	break;
	
	default:
	?>
		Stranka nebola najdena
	<?
		
	break;
 }
}
?>

</div>
