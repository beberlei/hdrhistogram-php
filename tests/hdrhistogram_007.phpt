--TEST--
hdrhistogram: Basic Iterator
Author: kontakt@beberlei.de
--FILE--
<?php

$a = hdr_init(1, 100, 1);
hdr_record_value($a, 1);
hdr_record_value($a, 2);
hdr_record_values($a, 3, 10);

$iterator = hdr_iter_init($a);
var_dump(hdr_iter_next($iterator));
var_dump(hdr_iter_next($iterator));
var_dump(hdr_iter_next($iterator));
var_dump(hdr_iter_next($iterator));
var_dump(hdr_iter_next($iterator));
--EXPECT--
array(4) {
  ["value"]=>
  int(0)
  ["count_at_index"]=>
  int(0)
  ["count_to_index"]=>
  int(0)
  ["highest_equivalent_value"]=>
  int(0)
}
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
bool(false)
