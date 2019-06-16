<?
$DB_HOST = "localhost:/var/run/mysql/mysql.sock";
$DB_USERNAME = "xondre08";
$DB_PASSWORD = "n7ovamgo";
$DB_NAME = "xondre08";

$db=mysql_connect($DB_HOST,$DB_USERNAME,$DB_PASSWORD);
if (!$db) 
	die('Neda sa pripojit. '.mysql_error());

if (!mysql_select_db($DB_NAME, $db)) 
	die('Databaza nie je dostupna. '.mysql_error());
?>

