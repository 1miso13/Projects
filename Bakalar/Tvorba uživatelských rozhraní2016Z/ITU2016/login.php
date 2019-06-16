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
					$query="SELECT * FROM users WHERE login_U='".$_POST["user"]."'";
					$result = @mysql_query($query, $db);

					if(is_resource($result) and mysql_num_rows($result)>0)
					{
						while($data = mysql_fetch_array($result, MYSQL_ASSOC))
						{
							if($data["password_U"] == $_POST["pass"])
							{
								$_SESSION['username'] = $_POST['user'];
								//$_SESSION['menoU'] = $data["krstneMeno_K"];
								$_SESSION['ID'] = $data["ID_U"];
								$_SESSION['TYPE'] = $data["type_U"];
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
?>
