<?

//
// form.php
//
// html for file upload
//

?>

<html>
<body>


<!-- // HTML FORM -->

<h2>Choose the file to upload</h2>
<form action="index.php" method="post" enctype="multipart/form-data">
<table>
<tr><td><input type="hidden" name="MyParam" value="MyValue"/></td><td></td></tr>
<tr><td><input type="file" name="MyFile"/></td><td></td></tr>
<tr><td></td><td><input type="submit" value="go!"/></td></tr>
</table>
</form>


<!-- // print errors from previous request, if any -->

<? if (isset($_GET["error"])) { ?>
  <div style="width:320px; border:1px solid red; font-size: 9pt; ">
  <? echo $_GET["error"]; ?>
  </div>
<? } ?>


<!-- // print results from previous request, if any -->

<? if (isset($_GET["info"]) && ($_GET["info"]=="true")) { ?>
  <div style="width:320px; border:1px solid green; font-size: 9pt; ">
  <? 
  echo "file: ".$_GET["name"]."<br/>"; 
  echo "type: ".$_GET["type"]."<br/>"; 
  printf("size: %.2fMo<br/>", $_GET["size"]); 
  ?>
  </div>
<? } ?>

</body>
</html>

