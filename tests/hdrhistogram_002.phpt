--TEST--
hdrhistogram: Reset Histogram
--FILE--
<?php

$hdr = hdr_init(1, 360000, 3);
hdr_record_value($hdr, 1);
echo "Max: " . hdr_max($hdr) . "\n";
echo "Count(1): ". hdr_count_at_value($hdr, 1) . "\n";
hdr_reset($hdr);
echo "Max: " . hdr_max($hdr) . "\n";
echo "Count(1): ". hdr_count_at_value($hdr, 1) . "\n";
--EXPECTF--
Max: 1
Count(1): 1
Max: 0
Count(1): 0
