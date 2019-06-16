<?


function profile($db)
{
	$nameErr = "";
	$mailErr = "";
	$Lerr = "";
	$Ferr = "";
	$passErr = "";
	
	$username = "";
	$password = "";
	$Meno = "";
	$Priezvisko = "";
	$titul = "";
	$date = "";
	$email = "";
	
	
	$result = NULL;
	
	
	$query="SELECT * FROM users WHERE login_U='".$_SESSION['username']."'";
		$result = @mysql_query($query, $db);
		if(is_resource($result) and mysql_num_rows($result)>0){ 
			while($data = mysql_fetch_array($result, MYSQL_ASSOC))
			{
				if($data["ID_U"] == $_SESSION["ID"])
				{
					$username = $data["login_U"];
					$password = $data["password_U"];
					$email = $data["email_U"];
				}
			}	
		}
		else
		{
			?>
				<a class="err">Takyto uzivatel sa nenasiel.</a>
			<?
		}						

					
	if ($_SERVER["REQUEST_METHOD"] == "POST") 
		{
			if (empty($_POST["user"])) {
				$nameErr = "Username is required";
			  } 

			if (empty($_POST["mail"])) 
			{
				$mailErr = "E-mail is required";
			}
			else{
				if(!filter_var($_POST['mail'], FILTER_VALIDATE_EMAIL))
				{
					$mailErr = "E-mail is incorrect";
				}
			}			  
			if (empty($_POST["pass"])) 
			{
				$passErr = "Password is required";
			} 

		}
	?>
	<?
				if((isset($_POST['pass']) && ($_POST['pass'] != "")))
				{
							if((isset($_POST['mail']) && ($_POST['mail'] != "")) && (filter_var($_POST['mail'], FILTER_VALIDATE_EMAIL)))
							{
								
								$result = "";
								if($_POST['mail'] != $email)
								{
										$query="SELECT * FROM users WHERE email_U='".$_POST["mail"]."'";
										$result = @mysql_query($query, $db);
								}
								if((is_resource($result) and mysql_num_rows($result)>0))
								{
									?><a class="err">The given E-mail already exists.<?
								}
								else
								{
									$query = "";

										$password = $_POST["pass"];
										$email = $_POST["mail"];
										$query = "UPDATE users SET email_U='".$_POST["mail"]."' , password_U='".$_POST["pass"]."'
													WHERE ID_U='".$_SESSION["ID"]."'";
									$result = @mysql_query($query, $db);
									?>
									<a class="err">Changes were successful</a>
									<?
									
								}
							}
				}
					
				
				?>
	<form method="POST">
			Uzivatel: <? echo $username; ?><br>
			Heslo:<br> <input type="PASSWORD" name="pass" value="<?echo $password;?>"/><a class="err" >*<? echo $passErr; ?></a><br>
			E-mail:<br> <input type="text" name="mail" value="<?php echo $email ?>"/><a class="err">*<? echo $mailErr; ?></a><br>
			<?// 1 ==klient , 2 == uradnik
			?>
				<input type="submit">
	</form>	
<?			
}
?>	
	