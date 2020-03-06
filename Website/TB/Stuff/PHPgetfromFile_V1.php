<?php

// author: thetoastiestbread
// filename: PHPgetfromFile_V1.php
// date: 01.03.2020 - 03.03.2020

$filepath = "file.txt";

$myfile = fopen($filepath, "r") or die("Unable to open file!");

fseek($myfile, 1, SEEK_SET);
$rpm = fread($myfile, 5);				// read rpms

fseek($myfile, 7, SEEK_SET);
$wg = fread($myfile, 6);				// read wirkungsgrad

fseek($myfile, 16, SEEK_SET);
$cmd = fread($myfile, 1);				// read current cmd

fseek($myfile, 20, SEEK_SET);
$amdoing = fread($myfile, 1);			// read what is happening right now

fclose($myfile);

$tosend = array($rpm, $wg, $cmd, $amdoing);		// put everything into array

echo json_encode($tosend);			// send as json
?>