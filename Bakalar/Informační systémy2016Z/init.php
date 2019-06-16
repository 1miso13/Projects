<?
$DB_HOST = "localhost:/var/run/mysql/mysql.sock";
$DB_USERNAME = "xkeryj00";
$DB_PASSWORD = "orurku6o";
$DB_NAME = "xkeryj00";

$db=mysql_connect($DB_HOST,$DB_USERNAME,$DB_PASSWORD);
if (!$db) 
	die('Neda sa pripojit. '.mysql_error());

if (!mysql_select_db($DB_NAME, $db)) 
	die('Databaza nie je dostupna. '.mysql_error());
?>

