<?php
function pracPoz($db,$pp)
{
if($pp===0)
	{
		if($_SESSION['TYPE']==1)
		{
			?>
			<h1>Job Offers</h1>
			<h3>My selected jobs</h3><?
			
			$id = "";
			$first_row = true;
			$query="SELECT Pracovne_pozicie.ID_PP AS `ID`, datumPridania as `Addition day`, naplnPrace as `Work description`, miesto as `Place` 
							FROM Pracovne_pozicie,Klient_Pozicia 
							WHERE 
							Klient_Pozicia.ID_PP = Pracovne_pozicie.ID_PP 
							AND Klient_Pozicia.ID_K = ".$_SESSION["ID"];
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
				
				echo "<td><a href='?menu=0&pp=".$id."' display:inline-block;><div>" . htmlspecialchars($field) . "</div></a></td>";
				
			}
			//echo "</a>";
			echo '</tr>';
			}
			echo '</table>';
		
		
		}
		?><h3>Offered jobs</h3><?
		$id = "";
		$query = "";
		$first_row = true;
		if($_SESSION['TYPE'] == 1 )
		{
		$query="SELECT Pracovne_pozicie.ID_PP AS  'ID', datumPridania AS  'Addition day', naplnPrace AS  `Work description` , miesto AS  'Place'
					FROM Pracovne_pozicie
					WHERE NOT 
					EXISTS (

					SELECT ID_PP
					FROM Klient_Pozicia
					WHERE Klient_Pozicia.ID_PP = Pracovne_pozicie.ID_PP
					AND Klient_Pozicia.ID_K =".$_SESSION["ID"]."
					)";
		}
		else
		{
			$query="SELECT Pracovne_pozicie.ID_PP AS  'ID', datumPridania AS  'Addition day', naplnPrace AS  `Work description` , miesto AS  'Place'
					FROM Pracovne_pozicie
					";
		}
					/*"WHERE Klient_Pozicia.ID_PP = Pracovne_pozicie.ID_PP 
							AND Klient_Pozicia.ID_K <> ".$_SESSION["ID"];*/
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
			
			echo "<td><a href='?menu=0&pp=".$id."' display:inline-block;><div>" . htmlspecialchars($field) . "</div></a></td>";
			
		}
		//echo "</a>";
		echo '</tr>';
		}
		echo '</table>';
	}
	else
	{
		if($_SESSION['TYPE'] === 0)
		{
			header( 'Location: index.php?menu=4' );
		}
		?><h3><a href='?menu=0' class="white">Back</a></h3><br><?
		

		// selecty do DB - vypis informacii o pozicii
				$query = "SELECT *
							FROM Pracovne_pozicie as PP, Uradnik AS U  
								WHERE 
									PP.ID_U = U.ID_U 									
									AND
									PP.ID_PP = ".$pp."";
									
				$queryA = "SELECT *
							FROM Specializacia AS S, Pozicia_Specializacia as PS
								WHERE
									".$pp." = PS.ID_PP
									AND
									S.ID_SP = PS.ID_SP";
				$queryB = "SELECT *
								FROM Benefity AS B, Pozicia_Benefity as PB
									WHERE B.ID_B = PB.ID_B AND PB.ID_PP = ".$pp."";					
				$queryC = "SELECT *
								FROM Pracovne_pozicie as PP,Firma AS F
									WHERE
										PP.ID_F = F.ID_F 
										AND
										PP.ID_PP = ".$pp."
										";

			$result = mysql_query($query,$db);
			
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 1. select - PP,uradnik, specializacia  
			while($data = mysql_fetch_array($result, MYSQL_ASSOC))
				{
					?><h1><? echo $data["naplnPrace"]; ?></h1><?
					?>ID: <? echo $data["ID_PP"];?><br><?
					?>Addition day: <? echo $data["datumPridania"];?><br><?
					?>Turn-out day: <? echo $data["datumNastupu"];?><br><?
					?>Wage: <? echo $data["hrubaMzda"];?> CZK<br><?
					?>Place: <? echo $data["ID_PP"];?><br><br><?					
					?><h3>Worker</h3><?
					//$_SESSION["u"] = $data["ID_U"];
					?>Name: <a class="white" href="?menu=55&u=<?echo $data["ID_U"];?>"><? echo $data["krstneMeno_U"]." ". $data["priezvisko_U"];?></a><br><?
				}
			
			
			$resultA = mysql_query($queryA, $db);
			
			if((is_resource($resultA) and mysql_num_rows($resultA)>0))
			{
				?><h3>Specialization</h3><?
				while($data = mysql_fetch_array($resultA, MYSQL_ASSOC))
					{	
						?>Name: <? echo $data["nazov"];?><br><?					
						?>Required title: <? echo $data["pozadovanyTitul"];?><br><?
					}
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 2. select - benefity
			$resultB = mysql_query($queryB, $db);
			if((is_resource($resultB) and mysql_num_rows($resultB)>0))
			{			
				?><h3>Benefits</h3><?
				while($data = mysql_fetch_array($resultB, MYSQL_ASSOC))
					{				
						?>-<?echo $data["popisBenefitu"];?><br><?
					}
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 3. select - firma, odboru
			$resultC = mysql_query($queryC, $db);

			?><h3>Company</h3><?
			while($data = mysql_fetch_array($resultC, MYSQL_ASSOC))
				{				
					?>Company name: <? echo $data["nazovFirmy"];?><br><?
					?>Authorized person: <? echo $data["zodpovednaOsoba"];?><br><?
					?>Company headquarters: <? echo $data["sidloFirmy"];?><br><?
					?>Address: <? echo $data["adresa"];?> CZK<br><?
					?>Worker count: <? echo $data["pocetZamestnancov"];?><br><br><?
				}
			/*	
			*	VYPIS KLIENTOV	
			*/	
			if(($_SESSION['TYPE'] == 2 )||($_SESSION['TYPE'] == 3 ))
			{
				$first_row = true;
				?><div><?
				?><h3>Clients</h3><?
				
				$queryW = "SELECT K.ID_K as 'ID', K.krstneMeno_K as 'First name', K.priezvisko_K as 'Last name', K.loginK as 'Login', K.email as 'Email'
							FROM Klient as K,Klient_Pozicia as KP, Pracovne_pozicie as PP
							WHERE PP.ID_PP = '".$pp."'
							AND KP.ID_PP = PP.ID_PP AND KP.ID_K = K.ID_K";
				$resultW = mysql_query($queryW, $db);
				echo '<table ID="tabulkaPP" cellspacing=0>';
				while ($row = mysql_fetch_assoc($resultW)) {
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
					// TODO, maybe NOT 
					echo "<td><a class='white' href='?menu=54&k=".$id."' display:inline-block;><div>" . htmlspecialchars($field) . "</div></a></td>";
					
				}
				//echo "</a>";
				echo '</tr>';
				}
				echo '</table>';
				
				?></div><?
				
			}
			
			

		if($_SESSION['TYPE'] === 1)//klient - prihlasenie sa k pracovnej pozicii
		{
			$query = "SELECT * FROM Klient_Pozicia WHERE ID_PP = '".$pp."' AND ID_K = '".$_SESSION["ID"]."'";
			$result = mysql_query($query, $db);
			if(!(is_resource($result) and mysql_num_rows($result)>0))
			{
				?>
					<form method="POST">
							<input type="submit" name="add_selected_job" value="Mark position">
						</form>
				<?
					if(isset($_POST['add_selected_job']))
					{
						
						$query = "INSERT INTO Klient_Pozicia (`ID_K`, `ID_PP`) 
									VALUES ('".$_SESSION["ID"]."', '".$pp."')";
						$result = mysql_query($query, $db);

						
						$query = "UPDATE Pracovne_pozicie SET obsadenost= obsadenost + 1 WHERE ID_PP = '".$pp."'";
						$result = mysql_query($query, $db);
						header( 'Location: index.php?menu=0' );
					}
			}
			else
			{
				?>
					<form method="POST">
							<input type="submit" name="delete_selected_job" value="Unmark position">
						</form>
				<?
					if(isset($_POST['delete_selected_job']))
					{
						
						$query = "DELETE FROM Klient_Pozicia
									WHERE ID_PP = '".$pp."' AND ID_K = '".$_SESSION["ID"]."'";
						$result = mysql_query($query, $db);

						
						$query = "UPDATE Pracovne_pozicie SET obsadenost= obsadenost - 1 WHERE ID_PP = '".$pp."'";
						$result = mysql_query($query, $db);
						header( 'Location: index.php?menu=0' );
					}
			}
			
		}
		
	}
}
?>