<?
include "init.php";

	session_start();

	$query = "DELETE FROM Pozicia_Benefity WHERE ID_PP = ".$_GET["w"];
	$queryA = "DELETE FROM Pozicia_Specializacia WHERE ID_PP = ".$_GET["w"];
	$queryB = "DELETE FROM Klient_Pozicia WHERE ID_PP = ".$_GET["w"];
	$queryC = "DELETE FROM Pracovne_pozicie WHERE ID_PP = ".$_GET["w"];
	$result = @mysql_query($query,$db);
	$resultA = @mysql_query($queryA,$db);
	$resultB = @mysql_query($queryB,$db);
	$resultC = @mysql_query($queryC,$db);
	
	header( 'Location: index.php?menu=2' ) ;
?>