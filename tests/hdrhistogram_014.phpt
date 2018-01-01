--TEST--
HdrHistogram: hdr_import version 2 with json hashmap
--FILE--
<?php

$payload = json_decode('{"b":{"331":1}}', true);
$imported = hdr_import($payload);

var_dump(hdr_export($imported));
var_dump(hdr_export(hdr_import(hdr_export($imported))));
--EXPECTF--
array(1) {
  ["b"]=>
  array(1) {
    [331]=>
    float(1)
  }
}
array(1) {
  ["b"]=>
  array(1) {
    [331]=>
    float(1)
  }
}
