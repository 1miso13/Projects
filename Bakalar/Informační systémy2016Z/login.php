<?
function login($db){	
		$name = "";		// username
		$nameErr = "";	// chybajuci username
		$passErr = "";	// chybajuci password
		if ($_SERVER["REQUEST_METHOD"] == "POST") 
		{
			if (empty($_POST["user"])) {
				$nameErr = "Username is required";
			  } 
			  
			if (empty($_POST["pass"])) 
			{
				$passErr = "Password is required";
			  } 
		}
		
		?>
		
		<form method="POST">
			Username:<br> <input type="text" name="user" value="<?php echo isset($_POST['user']) ? $_POST['user'] : '' ?>"/> <a class="err">*<? echo $nameErr; ?></a><br>
			Password:<br> <input type="PASSWORD" name="pass"/><a class="err">*<? echo $passErr; ?></a><br>
			<input type="submit">
		</form>
		<?
			if((isset($_POST['user']) && ($_POST['user'] != "")) && (isset($_POST['pass']) && ($_POST['pass'] != "")))
			{
					$query="SELECT * FROM Klient WHERE loginK='".$_POST["user"]."'";
					$result = @mysql_query($query, $db);

					if(is_resource($result) and mysql_num_rows($result)>0)
					{
						while($data = mysql_fetch_array($result, MYSQL_ASSOC))
						{
							if($data["hesloK"] == $_POST["pass"])
							{
								$_SESSION['username'] = $_POST['user'];
								$_SESSION['menoU'] = $data["krstneMeno_K"];
								$_SESSION['ID'] = $data["ID_K"];
								$_SESSION['TYPE'] = 1;
								header('Location: ?menu=15');
							}
						}	
					}
					else
					{
						$query="SELECT * FROM Uradnik WHERE loginU='".$_POST["user"]."'";
							$result = @mysql_query($query, $db);
							if(is_resource($result) and mysql_num_rows($result)>0){ 
								while($data = mysql_fetch_array($result, MYSQL_ASSOC))
								{
									if($data["hesloU"] == $_POST["pass"])
									{
										$_SESSION['username'] = $_POST['user'];
										$_SESSION['menoU'] = $data["krstneMeno_U"];
										
										$_SESSION['ID'] = $data["ID_U"];
										if($data["typ"] == 1) // 1 == uradnik
										{
										$_SESSION['TYPE'] = 2;
										} // 2 == admin
										else
										{
											$_SESSION['TYPE'] = 3;
										}
										header('Location: ?menu=15');
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
			}					
}
?>
