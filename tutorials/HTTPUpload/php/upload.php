<?

//
// upload.php
//
// server entry point
//

//exit();

?>

<!-- // print errors from previous request, if any -->

<?
// check filename
if ($_FILES["MyFile"]["name"] == "")
{
  ?>
  <div style="width:320px; border:1px solid red; font-size: 9pt; ">
  <? echo "Location: no file selected!"; ?>
  </div>
  <?
  exit();
}

// check file data upload
if (!is_uploaded_file($_FILES["MyFile"]["tmp_name"]))
{
  ?>
  <div style="width:320px; border:1px solid red; font-size: 9pt; ">
  <? echo $_FILES["MyFile"]["name"]." :<br/>file upload error!"; ?>
  </div>
  <?
  exit();
}

// try and store the file from the temp. path to a proper path
$name = $_FILES["MyFile"]["name"];
$type = $_FILES["MyFile"]["type"];
$size = $_FILES["MyFile"]["size"] / (1024 * 1024);
$tmppath = $_FILES["MyFile"]["tmp_name"];
$destinationPath = "store/".$name;
//$data = @file_get_contents($tmppath);  
//if ($data)
//{
//    echo "<br/>\n<br/>\n<br/>\n<br/>\n".$data;
//}

if (!rename($tmppath, $destinationPath))
{
  ?>
    <div style="width:320px; border:1px solid red; font-size: 9pt; ">
    <? echo "Location: index.php?error=".$destinationPath." :<br/>could not store this file!"; ?>
    </div>
  <?
  exit();
}

?> <div>copied file <? echo $tmppath; ?> to <? echo $destinationPath; ?></div><br/> <?

// everything's ok

// debug
// print_r($_FILES["MyFile"]);

// go back to the form, displaying the info about this current file

?>

<!-- // print results from previous request, if any -->

<div style="width:320px; border:1px solid green; font-size: 9pt; ">
<? 
echo "file: ".$name."<br/>"; 
echo "type: ".$type."<br/>"; 
printf("size: %.2fMb<br/>", $size); 
?>
</div>

