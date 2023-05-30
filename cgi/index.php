<?php
$cookie_name = "user";
$cookie_value = "John Doe";
setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/"); // 86400 = 1 day
// Start the session
session_start();
?>
<html>
<body>

<?php
if(!isset($_COOKIE[$cookie_name])) {
  echo "Cookie named '" . $cookie_name . "' is not set!";
} else {
  echo "Cookie '" . $cookie_name . "' is set!<br>";
  echo "Value is: " . $_COOKIE[$cookie_name];
}
echo '<br>';
echo $_GET["lname"];

echo '<br>';

if(isset($_SESSION["favcolor"] )) {
  echo "Session named '" . $_SESSION["favcolor"] . "' is not set!";
}
// Set session variables
$_SESSION["favcolor"] = "green";
$_SESSION["favanimal"] = "cat";
// echo "Session variables are set.";


?>

</body>
</html>