--TEST--
HdrHistogram: hdr_export/hdr_import
--FILE--
<?php

$hdr = hdr_init(1, 1000, 2);

$data = hdr_export($hdr);
var_dump($data);

hdr_record_values($hdr, 1, 10);
hdr_record_values($hdr, 2, 20);
hdr_record_values($hdr, 3, 25);
hdr_record_values($hdr, 10, 10);

$data = hdr_export($hdr);
var_dump($data);

$imported = hdr_import($data);

if (hdr_mean($imported) === hdr_mean($hdr)) {
    echo "SUCCESS\n";
} else {
    echo "FAILURE\n";
}

--EXPECTF--
array(4) {
  ["ltv"]=>
  int(1)
  ["htv"]=>
  int(1000)
  ["sf"]=>
  int(2)
  ["c"]=>
  array(0) {
  }
}
array(4) {
  ["ltv"]=>
  int(1)
  ["htv"]=>
  int(1000)
  ["sf"]=>
  int(2)
  ["c"]=>
  array(11) {
    [0]=>
    float(0)
    [1]=>
    float(10)
    [2]=>
    float(20)
    [3]=>
    float(25)
    [4]=>
    float(0)
    [5]=>
    float(0)
    [6]=>
    float(0)
    [7]=>
    float(0)
    [8]=>
    float(0)
    [9]=>
    float(0)
    [10]=>
    float(10)
  }
}
SUCCESS
