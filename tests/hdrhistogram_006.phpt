--TEST--
hdrhistogram: Merge Into
--FILE--
<?php

$a = hdr_init(1, 100, 5);
hdr_record_values($a, 1, 10);

$b = hdr_init(1, 100, 5);
hdr_record_values($b, 1, 20);

$dropped = hdr_merge_into($a, $b);
echo "dropped: " . $dropped . "\n";
echo "count(a+b): " . hdr_count_at_value($a, 1) . "\n";

?>
--EXPECT--
dropped: 0
count(a+b): 30
