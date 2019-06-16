<?
function addCompany($db)
{
	$nameErr = "";
	$personErr = "";
	$addressErr = "";
	$hqErr = "";
	$countErr = "";
	
	if ($_SERVER["REQUEST_METHOD"] == "POST") 
		{
			if (empty($_POST["name"])) {
				$nameErr = "Company name is required";
			  }

			  if (empty($_POST["person"])) {
				$personErr = "Zodpovedna osoba is required";
			  }
			  if (empty($_POST["address"])) {
				$addressErr = "Address is required";
			  }

			if (empty($_POST["hq"])) 
			{
				$hqErr = "HQ address is required";
			}
			if (empty($_POST["count"])) 
			{
				$countErr = "Workers count is required";
		    }
			else
			{
				if(!is_numeric($_POST["count"])) 
				{
					$countErr = "The count of workers must be a number";
				} 
			}

		}
	?>
		<form method="POST">
			Company Name:<br> <input type="text" name="name" value="<?php echo isset($_POST['name']) ? $_POST['name'] : '' ?>"/><a class="err">*<? echo $nameErr; ?></a><br>
			Authorized person:<br> <input type="text" name="person" value="<?php echo isset($_POST['person']) ? $_POST['person'] : '' ?>"/><a class="err">*<? echo $personErr; ?></a><br>
			Address:<br> <input type="text" name="address" value="<?php echo isset($_POST['address']) ? $_POST['address'] : '' ?>"/><a class="err">* <? echo $addressErr; ?></a><br>
			Headquarters:<br> <input type="text" name="hq" value="<?php echo isset($_POST['hq']) ? $_POST['hq'] : '' ?>"/><a class="err">*<? echo $hqErr; ?></a><br>
			Number of workers:<br> <input type="text" name="count" value="<?php echo isset($_POST['count']) ? $_POST['count'] : '' ?>"/><a class="err">*<? echo $countErr; ?></a><br>
			<input type="submit" name="addCB" value="Add"/>
		</form>
	<?
	if(isset($_POST['addCB']))
		{	
			if((isset($_POST['name']) && ($_POST['name'] != "")))
			{
				if((isset($_POST['person']) && ($_POST['person'] != "")))
				{
					if((isset($_POST['address']) && ($_POST['address'] != "")))
					{
						if((isset($_POST['hq']) && ($_POST['hq'] != "")))
						{
							if((isset($_POST['count']) && ($_POST['count'] != "")) && is_numeric($_POST["count"]))
							{
								$query="SELECT * FROM Firma WHERE nazovFirmy='".$_POST["name"]."'";
								$result = @mysql_query($query, $db);
								if((is_resource($result) and (mysql_num_rows($result)>0)))
								{
									?><a class="err">The given company is already registered.</a><br><?
								}
								else
								{
									$query="INSERT INTO Firma(`nazovFirmy`, `zodpovednaOsoba`,`adresa`,`sidloFirmy`,`pocetZamestnancov`) 
														VALUES 
														('".mysql_real_escape_string($_POST["name"])."',
														'".mysql_real_escape_string($_POST["person"])."',
														'".mysql_real_escape_string($_POST["address"])."',
														'".mysql_real_escape_string($_POST["hq"])."',
														'".mysql_real_escape_string($_POST["count"])."'
														)
														";
									$result = @mysql_query($query,$db);
									$queryA="SELECT * FROM Firma WHERE nazovFirmy='".$_POST["name"]."'";
									$resultA = @mysql_query($queryA,$db);
									$data = mysql_fetch_array($resultA, MYSQL_ASSOC);
									$_SESSION["selectCompID"] = $data['ID_F'];
								}
							}
						}
					}
				}
			}
		}
}

?>