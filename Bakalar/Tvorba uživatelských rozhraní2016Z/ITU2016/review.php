<?
function review(){

	
	$myID = $_GET['u'];
	if(!isset($_SESSION['ID']))
	{
		header( 'Location: index.php?menu=4' ) ;
	}
	
	$queryX = "SELECT * FROM reviewer WHERE ID_AUTOR = '".$myID."' AND ID_RECENZENT='".$_SESSION['ID']."'";
	$resultX = mysql_query($queryX);
	$num_rows = mysql_num_rows($resultX);
	
	
	if($num_rows == 0)
	{
		header( 'Location: index.php?menu=0' ) ;
	}

	$query = "SELECT * FROM users WHERE ID_U = '".$myID."'";
	$result = mysql_query($query);
	$data = mysql_fetch_array($result, MYSQL_ASSOC);
	$fil = $data["FILENAME"];
	
	if(isset($_POST['Save']))
	{
		$answer1= $_POST['quest1'];
		$answer2= $_POST['quest2'];
		$answer3= $_POST['quest3'];
		$answer4= $_POST['quest4'];
		$answer5= $_POST['quest5'];
		$answer6= $_POST['quest6'];
		$answer7= $_POST['quest7'];
		$answer8= $_POST['quest8'];
		
		$query = "SELECT * FROM reviewer WHERE ID_AUTOR='".$myID."' AND ID_RECENZENT='".$_SESSION['ID']."'";
		$resultA = mysql_query($query);
		$dataA = mysql_fetch_array($resultA, MYSQL_ASSOC);
		
		if($dataA['ID_R'] == NULL)
		{
			$queryB= "INSERT INTO reviews (answer1,answer2,answer3,answer4,answer5,answer6,answer7,answer8) VALUES ('".$_POST['quest1']."','".$_POST['quest2']."',
													'".$_POST['quest3']."','".$_POST['quest4']."','".$_POST['quest5']."',
														'".$_POST['quest6']."','".$_POST['quest7']."','".$_POST['quest8']."')";
			mysql_query($queryB);
			$lastID = mysql_insert_id();
			$queryA = "UPDATE reviewer SET ID_R='".$lastID."' WHERE ID_AUTOR='".$myID."' AND ID_RECENZENT='".$_SESSION['ID']."'";
			mysql_query($queryA);
		}
		else
		{
			$queryC = "UPDATE reviews SET answer1='".$_POST['quest1']."', answer2='".$_POST['quest2']."', answer3='".$_POST['quest3']."',
					answer4='".$_POST['quest4']."', answer5='".$_POST['quest5']."', answer6='".$_POST['quest6']."',
					answer7='".$_POST['quest7']."', answer8='".$_POST['quest8']."' WHERE ID_R='".$dataA['ID_R']."'";
			mysql_query($queryC);
		}
	}
	
	$queryF = "SELECT * from reviews AS R, reviewer AS RR 
					WHERE R.ID_R = RR.ID_R 
					AND 
					RR.ID_AUTOR ='".$myID."' AND RR.ID_RECENZENT='".$_SESSION['ID']."'";
	$resultF = mysql_query($queryF);
	$dataF = mysql_fetch_array($resultF, MYSQL_ASSOC);
	
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
							<input type="radio" name="quest1" value="0" <?if($dataF['answer1'] == 0){?> checked<?}?>> Menej ako 2<br>
							<input type="radio" name="quest1" value="1" <?if($dataF['answer1'] == 1){?> checked<?}?>> 2<br>
							<input type="radio" name="quest1" value="2" <?if($dataF['answer1'] == 2){?> checked<?}?>> Viac ako 2<br>

						<h4>Ake odporucania by ste zanechali ohladom navrhu pre autora?</h4>
							<input type="text" name="quest2" placeholder="Type something..." value="<?echo $dataF['answer2'];?>"><br>

						<h4>Ako by ste ohodnotili kvalitu obsahu dokumentu?</h4>
							<input type="radio" name="quest3" value="0" <?if($dataF['answer3'] == 0){?> checked<?}?>> 100 - 75 %<br>
							<input type="radio" name="quest3" value="1" <?if($dataF['answer3'] == 1){?> checked<?}?>> 75 - 50 %<br>
							<input type="radio" name="quest3" value="2" <?if($dataF['answer3'] == 2){?> checked<?}?>> 50 - 25 %<br>
							<input type="radio" name="quest3" value="3" <?if($dataF['answer3'] == 3){?> checked<?}?>> 25 - 0 %<br>

						<h4>Ako sa vam pozdavala formalna a jazykova uroven dokumentu?</h4>
							<input type="text" name="quest4" placeholder="Type something..." value="<?echo $dataF['answer4'];?>"><br>

						<h4>Pochopili ste zameranie riesenia?</h4>
							<input type="radio" name="quest5" value="0" <?if($dataF['answer5'] == 0){?> checked<?}?>> Ano<br>
							<input type="radio" name="quest5" value="1" <?if($dataF['answer5'] == 1){?> checked<?}?>> Nie<br>
						
						<h4>Aka bola technicka uroven?</h4>
							<input type="text" name="quest6" placeholder="Type something..." value="<?echo $dataF['answer6'];?>"><br>
						
						<h4>Co sa vam pozdavalo/nepozdavalo na navrhu testovania</h4>
							<input type="text" name="quest7" placeholder="Type something..." value="<?echo $dataF['answer7'];?>"><br>
						
						<h4>Co by ste doporucili autorovi?</h4>
							<input type="text" name="quest8" placeholder="Type something..." value="<?echo $dataF['answer8'];?>"><br>
						<h3><input type="submit" name="Save" value="Save"></h3>
					</form>
					
				</td>
			</tr>
		</table>	
	</div>
	<?
}

?>