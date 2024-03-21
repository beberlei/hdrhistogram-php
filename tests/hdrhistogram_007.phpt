--TEST--
hdrhistogram: Basic Iterator
--FILE--
<?php

$a = hdr_init(1, 100, 1);
hdr_record_value($a, 1);
hdr_record_value($a, 2);
hdr_record_values($a, 3, 10);

$iterator = hdr_iter_init($a);
while ($data = hdr_iter_next($iterator)) {
    if ($data['count_at_index']) {
        var_dump($data);
    }
}

?>
--EXPECT--
array(4) {
  ["value"]=>
  int(1)
  ["count_at_index"]=>
  int(1)
  ["count_to_index"]=>
  int(1)
  ["highest_equivalent_value"]=>
  int(1)
}
array(4) {
  ["value"]=>
  int(2)
  ["count_at_index"]=>
  int(1)
  ["count_to_index"]=>
  int(2)
  ["highest_equivalent_value"]=>
  int(2)
}
array(4) {
  ["value"]=>
  int(3)
  ["count_at_index"]=>
  int(10)
  ["count_to_index"]=>
  int(12)
  ["highest_equivalent_value"]=>
  int(3)
}
