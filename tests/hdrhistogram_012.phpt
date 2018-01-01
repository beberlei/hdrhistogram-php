--TEST--
HdrHistogram: hdr_import Error Handling
--FILE--
<?php

$imported = hdr_import(["ltv" => 0, "htv" => 0, "sf" => 0, "sk" => 0, "c" => []]);
$imported = hdr_import();
$imported = hdr_import([]);
$imported = hdr_import(["ltv" => -1]);
$imported = hdr_import(["ltv" => 1, "htv" => -10]);
$imported = hdr_import(["ltv" => 1, "htv" => 10, "sf" => -2]);
$imported = hdr_import(["ltv" => 1, "htv" => 10, "sf" => 1, "c" => null]);
$imported = hdr_import(["ltv" => 1, "htv" => 10, "sf" => 1, "b" => null]);
$imported = hdr_import(["ltv" => 1, "htv" => 10, "sf" => 1, "b" => null, "c" => null]);
--EXPECTF--
Warning: hdr_import(): lowest_trackable_value (ltv) must be >= 1. in %s on line 3

Warning: hdr_import() expects exactly 1 parameter, 0 given in %s on line 4

Warning: hdr_import(): Missing counts (c) or bucket (b) key or not arrays. in %s on line 5

Warning: hdr_import(): lowest_trackable_value (ltv) must be >= 1. in %s on line 6

Warning: hdr_import(): highest_trackable_value (htv) must be >= 1. in %s on line 7

Warning: hdr_import(): significant_figures (sf) must be 1, 2, or 3. in %s on line 8

Warning: hdr_import(): Missing counts (c) or bucket (b) key or not arrays. in %s on line 9

Warning: hdr_import(): Missing counts (c) or bucket (b) key or not arrays. in %s on line 10

Warning: hdr_import(): Missing counts (c) or bucket (b) key or not arrays. in %s on line 11
