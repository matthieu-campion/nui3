<?

//
// form.php
//
// html for file upload
//

?>

<!-- // HTML FORM -->

<h2>Choose the file to upload</h2>
<form action="index.php" method="post" enctype="multipart/form-data">
<table>
<tr><td><input type="hidden" name="MyParam" value="MyValue"/></td><td></td></tr>
<tr><td><input type="file" name="MyFile"/></td><td></td></tr>
<tr><td></td><td><input type="submit" value="go!"/></td></tr>
</table>
</form>


