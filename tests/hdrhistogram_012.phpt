--TEST--
HdrHistogram: hdr_import Error Handling
--FILE--
<?php

$imported = hdr_import(["ltv" => 0, "htv" => 0, "sf" => 0, "sk" => 0, "c" => []]);
$imported = hdr_import();
$imported = hdr_import([]);
$imported = hdr_import(["ltv" => 1]);
$imported = hdr_import(["ltv" => 1, "htv" => 10]);
$imported = hdr_import(["ltv" => 1, "htv" => 10, "sf" => 1]);
$imported = hdr_import(["ltv" => 1, "htv" => 10, "sf" => 1, "c" => []]);
--EXPECTF--
Warning: hdr_import(): lowest_trackable_value (ltv) must be >= 1. in %s on line 3

Warning: hdr_import() expects exactly 1 parameter, 0 given in %s on line 4

Warning: hdr_import(): Missing counts (c) or bucket (b) key. in %s on line 5

Warning: hdr_import(): Missing counts (c) or bucket (b) key. in %s on line 6

Warning: hdr_import(): Missing counts (c) or bucket (b) key. in %s on line 7

Warning: hdr_import(): Missing counts (c) or bucket (b) key. in %s on line 8
