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
array(2) {
  ["htv"]=>
  int(1000)
  ["b"]=>
  array(0) {
  }
}
array(2) {
  ["htv"]=>
  int(1000)
  ["b"]=>
  array(4) {
    [1]=>
    float(10)
    [2]=>
    float(20)
    [3]=>
    float(25)
    [10]=>
    float(10)
  }
}
SUCCESS
1: 10
2: 20
3: 25
10: 10
