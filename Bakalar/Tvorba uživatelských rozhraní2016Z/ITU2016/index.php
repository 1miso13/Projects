<?php
include "init.php";

if(!isset($_SESSION))
	session_start();



include "login.php";
include "pass.php";
include "profile.php";
include "review.php";
include "table.php";
/*

include "addWork.php";
include "addUradnik.php";
include "updateUradnik.php";
include "addCompany.php";
include "checkdate.php";
include "register.php";

*/
// prvni nacteni stranky
if (!isset($_GET["menu"])){
  $_GET["menu"]=0;
}
/*
if (!isset($_GET["pp"])){
  $_GET["pp"]=0;
}
*/
if (!isset($_GET["u"])){
  $_GET["u"]=0;
}
/*
if (!isset($_GET["k"])){
  $_GET["k"]=0;
}
*/
    
include "header.html";
include "menu.php";

menu($_GET["menu"],$db,/*$_GET["pp"],*/ $_GET["u"]/*, $_GET["k"]*/);

include "footer.html";

?>
