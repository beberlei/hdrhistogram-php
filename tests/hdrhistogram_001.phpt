--TEST--
hdrhistogram: Basic Functionality
Author: kontakt@beberlei.de
--FILE--
<?php

$hdr = hdr_create(1, 360000, 3);
var_dump($hdr);
echo "Memory: " . hdr_get_memory_size($hdr) . "\n";

// Record Values
hdr_record_value($hdr, 1);
hdr_record_value($hdr, 2);
hdr_record_value($hdr, 3);

echo "Mean: " . hdr_mean($hdr) . "\n";
echo "Min: " . hdr_min($hdr) . "\n";
echo "Max: " . hdr_max($hdr) . "\n";

hdr_record_values($hdr, 4, 2);
echo "Max: " . hdr_max($hdr) . "\n";
--EXPECT--
resource(4) of type (hdr_histogram)
Memory: 82048
Mean: 2
Min: 1
Max: 3
Max: 4
