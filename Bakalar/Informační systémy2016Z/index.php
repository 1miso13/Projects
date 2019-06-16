<?php
include "init.php";

if(!isset($_SESSION))
	session_start();

include "checkdate.php";

include "login.php";
include "register.php";
include "profile.php";
include "pracPoz.php";
include "addWork.php";
include "addUradnik.php";
include "updateUradnik.php";
include "addCompany.php";

// prvni nacteni stranky
if (!isset($_GET["menu"])){
  $_GET["menu"]=0;
}
if (!isset($_GET["pp"])){
  $_GET["pp"]=0;
}
if (!isset($_GET["u"])){
  $_GET["u"]=0;
}
if (!isset($_GET["k"])){
  $_GET["k"]=0;
}

    
include "header.html";
include "menu.php";

menu($_GET["menu"],$db,$_GET["pp"], $_GET["u"], $_GET["k"]);

include "footer.html";

?>
