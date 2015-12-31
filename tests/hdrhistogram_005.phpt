--TEST--
hdrhistogram: Value At Percentile
--FILE--
<?php

$a = hdr_init(1, 100, 5);
hdr_record_values($a, 1, 10);

$b = hdr_init(1, 100, 5);
hdr_record_values($b, 1, 20);

$c = hdr_add($a, $b);
echo "count(a): " . hdr_count_at_value($a, 1) . "\n";
echo "count(b): " . hdr_count_at_value($b, 1) . "\n";
echo "count(a+b): " . hdr_count_at_value($c, 1) . "\n";
--EXPECT--
count(a): 10
count(b): 20
count(a+b): 30
