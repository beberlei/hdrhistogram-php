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

$iter = hdr_iter_init($imported);
while ($data = hdr_iter_next($iter)) {
    if ($data['count_at_index']) {
        echo $data['value'] . ': ' . $data['count_at_index'] . "\n";
    }
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
array(5) {
  ["ltv"]=>
  int(1)
  ["htv"]=>
  int(1000)
  ["sf"]=>
  int(2)
  ["c"]=>
  array(10) {
    [0]=>
    float(10)
    [1]=>
    float(20)
    [2]=>
    float(25)
    [3]=>
    float(0)
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
    float(10)
  }
  ["sk"]=>
  int(1)
}
SUCCESS
1: 10
2: 20
3: 25
10: 10
