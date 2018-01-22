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
  ["v"]=>
  array(2) {
    [0]=>
    int(-331)
    [1]=>
    int(1)
  }
}
array(1) {
  ["v"]=>
  array(2) {
    [0]=>
    int(-331)
    [1]=>
    int(1)
  }
}
