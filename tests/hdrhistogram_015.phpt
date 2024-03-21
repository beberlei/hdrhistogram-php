--TEST--
HdrHistogram: hdr_import version 3 with negative skip counters
--FILE--
<?php

$payload = json_decode('{"v":[-100,5,-20,1]}', true);
$imported = hdr_import($payload);

var_dump(hdr_export($imported));
var_dump(hdr_export(hdr_import(hdr_export($imported))));
var_dump(hdr_value_at_percentile($imported, 95));
var_dump(hdr_value_at_percentile(hdr_import(hdr_export($imported)), 95));

?>
--EXPECTF--
array(1) {
  ["v"]=>
  array(4) {
    [0]=>
    int(-100)
    [1]=>
    int(5)
    [2]=>
    int(-20)
    [3]=>
    int(1)
  }
}
array(1) {
  ["v"]=>
  array(4) {
    [0]=>
    int(-100)
    [1]=>
    int(5)
    [2]=>
    int(-20)
    [3]=>
    int(1)
  }
}
int(121)
int(121)
