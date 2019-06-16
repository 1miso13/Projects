<?php     
  session_start();
  unset($_SESSION["username"]);
  unset($_SESSION["prava"]);
  unset($_SESSION["lastaction"]);
  unset($_SESSION["TYPE"]);
  header( 'Location: index.php' ) ;
?>