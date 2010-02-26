<?

//
// upload.php
//
// server entry point
//


// check filename
if ($_FILES["MyFile"]["name"] == "")
{
  header("Location: index.php?error=no file selected!");
  exit();
}

// check file data upload
if (!is_uploaded_file($_FILES["MyFile"]["tmp_name"]))
{
  header("Location: index.php?error=".$_FILES["MyFile"]["name"]." :<br/>file upload error!");
  exit();
}

// try and store the file from the temp. path to a proper path
$destinationPath = "store/".$_FILES["MyFile"]["name"];
if (!rename($_FILES["MyFile"]["tmp_name"], $destinationPath))
{
  header("Location: index.php?error=".$destinationPath." :<br/>could not store this file!");
  exit();
}

// everything's ok

// debug
// print_r($_FILES["MyFile"]);

// go back to the form, displaying the info about this current file
$name = $_FILES["MyFile"]["name"];
$type = $_FILES["MyFile"]["type"];
$size = $_FILES["MyFile"]["size"] / (1024 * 1024);
header("Location: index.php?info=true&name=".$name."&type=".$type."&size=".$size);
exit();

// note: I ask form.php (via index.php which is called here) to process the results of the request, instead of doing it here
// This way, if the user reloads the current page of his browser, it doesn't duplicate the request process...

?>

