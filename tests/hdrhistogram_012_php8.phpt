--TEST--
HdrHistogram: hdr_import Error Handling
--SKIPIF--
<?php
if (PHP_VERSION_ID < 80000) die("skip PHP 8 only");
?>
--FILE--
<?php

$imported = hdr_import(["ltv" => 0, "htv" => 0, "sf" => 0, "sk" => 0, "c" => []]);
try {
	$imported = hdr_import();
} catch (ArgumentCountError $e) {
	echo $e->getMessage()."\n";
}

$imported = hdr_import([]);
$imported = hdr_import(["ltv" => -1]);
$imported = hdr_import(["ltv" => 1, "htv" => -10]);
$imported = hdr_import(["ltv" => 1, "htv" => 10, "sf" => -2]);
$imported = hdr_import(["ltv" => 1, "htv" => 10, "sf" => 1, "c" => null]);
$imported = hdr_import(["ltv" => 1, "htv" => 10, "sf" => 1, "b" => null]);
$imported = hdr_import(["ltv" => 1, "htv" => 10, "sf" => 1, "b" => null, "c" => null]);
--EXPECTF--
Warning: hdr_import(): lowest_trackable_value (ltv) must be >= 1. in %s
hdr_import() expects exactly 1 argument, 0 given

Warning: hdr_import(): Missing counts (c) or bucket (b) key or not arrays. in %s

Warning: hdr_import(): lowest_trackable_value (ltv) must be >= 1. in %s

Warning: hdr_import(): highest_trackable_value (htv) must be >= 1. in %s

Warning: hdr_import(): significant_figures (sf) must be 1, 2, or 3. in %s

Warning: hdr_import(): Missing counts (c) or bucket (b) key or not arrays. in %s

Warning: hdr_import(): Missing counts (c) or bucket (b) key or not arrays. in %s

Warning: hdr_import(): Missing counts (c) or bucket (b) key or not arrays. in %s
