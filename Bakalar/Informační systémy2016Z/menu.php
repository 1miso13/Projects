<?
function menu($num,$db,$pp,$u,$k){
/// menu
if(!isset($_SESSION['username'])){$_SESSION['username'] = "";}
if(!isset($_SESSION['TYPE'])){$_SESSION['TYPE'] = 0;}
if(!isset($_SESSION['addcompany'])){$_SESSION['addcompany'] = 0;}
if(!isset($_SESSION['selectCompID'])){$_SESSION['selectCompID'] = 0;}
?>
	<div id=menu>
	<table cellspacing=0>
	<tr>
		<td id="menu1">
			<ul>                                              
				<li <?if($num==0){?>class="current"<?}?>><a href="?menu=0">Job Offers</a></li>
				<li <?if($num==1){?>class="current"<?}?>><a href="?menu=1">About us</a></li>
				<?if($_SESSION["TYPE"] == 2 || $_SESSION["TYPE"] == 3){?>
				<li <?if($num==2){?>class="current"<?}?>><a href="?menu=2">Services</a></li>
				<?}?>
			</ul>
		</td>	
	   <td class="box1">
				<div class="box_content">
					
					<?
					if($_SESSION['username'] == "")
					{
						?>
						<div><a href="?menu=4" class="reserve">Login</a></div>
						<div><a href="?menu=5" class="reserve">Register</a></div>
						<?
					}
					else
					{
						?>
						<div><a href="?menu=20" class="reserve"><?echo $_SESSION['username'];?></a></div>
						<div><a href="logout.php" class="reserve">Logout</a></div>
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
		pracPoz($db,$pp);
	break;
	case 1:
	?>
	<h1>About Us</h1>
	<h3>Authors:</h3>
	- Julius Kery (xkeryj00@stud.fit.vutbr.cz)<br>
	- Michal Ondrejo (xondre08@stud.fit.vutbr.cz)<br>
	<br>
	- This site was created as a project for the course: Information Systems. <br>
	
	<h3> Documentation </h3>
	<a class ='white' href="doc.html">Link to Documentation</a>
	
	<?
	break;
	case 2:
	// Uradnikovi - pridat prac. poziciu, adminovi - pridat prac.poziciu+pridat uradnika
	?>
	<h1>Services</h1>
	<?if($_SESSION["TYPE"] == 3)
	{?>
		<div>
			<?addWork($db,$u);?>
		</div>
	<?
		echo "<h2>Workers</h2>";
		
		
		$first_row = true;
		$query="SELECT ID_U as 'ID', krstneMeno_U as 'First name', priezvisko_U as 'Last name', titul as 'Title' ,denNastupu as 'Turn-out day', datumNarodenia_U as 'Birthday', email as 'E-mail', mzda as 'Wage', typ as 'Type', loginU as 'Username'  
				FROM Uradnik";
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
		
		
	?>
		<div><h3>Add Worker</h3>
			<?addUradnik($db);?>
		</div>
		
	<?}else if($_SESSION["TYPE"] == 2){?>
		<div>
			<?addWork($db,$u);?>
		</div>
	<?}
	else
	{
		header( 'Location: index.php' ) ;
	}?>

	<?
	break;
	case 4:
	?>
	<h1>LOGIN</h1>
	<?
		login($db);
	
	break;
	
	case 5:
	?>
	<h1>REGISTER</h1>
	<?
		register($db);
	break;
	
	case 15:
		?>
		<h3>Login successful.</h3>
		<?
		header( 'Location: index.php' ) ;
		
	break;
	
	// Zobrazenie profilu a moznost zmeny udajov 
	case 20:
	?> 
		<h1>Profile</h1>
	<?
		Profile($db);	
	break;
	
	case 54:
	?>
		<h3>Profile</h3>
		
		<?
		if($_SESSION["TYPE"] == 2 || $_SESSION["TYPE"] == 3 )
		{
		$query="SELECT * FROM Klient WHERE ID_K='".$k."'";
			$result = @mysql_query($query, $db);
			if(is_resource($result) and mysql_num_rows($result)>0)
			{ 
				$data = mysql_fetch_array($result, MYSQL_ASSOC)
				
						?>Username: <? echo $data["loginK"];?><br><?	
						?>First name: <? echo $data["krstneMeno_K"];?><br><?
						?>Last name: <?echo $data["priezvisko_K"];?><br><?
						?>Title: <?echo $data["titul"];?><br><?
						?>Birthday: <?echo $data["datumNarodenia_K"];?><br><?
						?>Email: <? echo $data["email"];?><br><?
						
						
				
				$queryX = "SELECT * FROM Klient_Preukaz WHERE ID_K ='".$data["ID_K"]."'";
				$queryY = "SELECT * FROM Klient_Certifikat WHERE ID_K ='".$data["ID_K"]."'";
				$queryZ = "SELECT * FROM Klient_Skolenie WHERE ID_K ='".$data["ID_K"]."'";
				
				$resultX = @mysql_query($queryX, $db);
				$resultY = @mysql_query($queryY, $db);
				$resultZ = @mysql_query($queryZ, $db);
				
					if(is_resource($resultX) and mysql_num_rows($resultX)>0)
					{
						?>
						<h3>Licenses:</h3><br>
						<? 
						$query="SELECT * FROM Preukaz P,Klient_Preukaz PK WHERE PK.ID_K ='".$k."' AND P.ID_PR = PK.ID_PR";
							$result = @mysql_query($query,$db);
						while($data = mysql_fetch_array($result, MYSQL_ASSOC)){
							echo $data["ID_PR"];?>: <? echo $data["nazovPreukazu"]; ?> (<?echo $data["informaciaOPreukaze"]?>)<br>
							
						<?}
					}
					if(is_resource($resultY) and mysql_num_rows($resultY)>0)
					{?>
						<h3>Certificates:</h3><br>
						<? 
						$query="SELECT * FROM Certifikaty C,Klient_Certifikat CS WHERE CS.ID_K ='".$k."' AND C.ID_C = CS.ID_C";
							$result = @mysql_query($query,$db);
						while($data = mysql_fetch_array($result, MYSQL_ASSOC)){
							echo $data["ID_C"];?>: <? echo $data["nazovCertifikatu"]; ?> (<?echo $data["informaciaOCertifikatu"]?>)<br>
							
						<?}
					}
					if(is_resource($resultZ) and mysql_num_rows($resultZ)>0)
					{?>
						<h3>Courses:</h3><br>
						<? 
							$query="SELECT * FROM Skolenie S,Klient_Skolenie KS WHERE KS.ID_K ='".$k."' AND S.ID_SK = KS.ID_SK";
							$result = @mysql_query($query,$db);
						while($data = mysql_fetch_array($result, MYSQL_ASSOC)){
							echo $data["ID_SK"];?>: <? echo $data["nazovSkolenia"]; ?> (<?echo $data["informaciaOSkoleni"]?>)<br>
						<?}
					}
			
			
			}
		}
	else
	{
		
	}		
	break;
	// vypis informacii uradnika
	case 55:
	?> 
		<h1>Profile</h1>
	<?
	// vypis info a moznost zmeny udajov 
	if($_SESSION["TYPE"] != 3)
	{
		$query="SELECT * FROM Uradnik WHERE ID_U='".$u."'";
			$result = @mysql_query($query, $db);
			if(is_resource($result) and mysql_num_rows($result)>0){ 
				while($data = mysql_fetch_array($result, MYSQL_ASSOC))
				{
					if($_SESSION["TYPE"] != 1)
					{
						?>Username: <? echo $data["loginU"];?><br><?
					}
						?>First name: <? echo $data["krstneMeno_U"];?><br><?
						?>Last name: <?echo $data["priezvisko_U"];?><br><?
						?>Title: <?echo $data["titul"];?><br><?
						?>Birthday: <?echo $data["datumNarodenia_U"];?><br><?
						?>Email: <? echo $data["email"];?><br><?
						
						
				}	
			}
	}
	else
	{
		updateUradnik($db,$u);
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
