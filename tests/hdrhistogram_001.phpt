--TEST--
hdrhistogram: Basic Functionality
Author: kontakt@beberlei.de
--FILE--
<?php

$hdr = hdr_init(1, 360000, 3);
var_dump($hdr);
echo "Memory: " . hdr_get_memory_size($hdr) . "\n";

// Record Values
if (hdr_record_value($hdr, 1)) {
    echo "Recorded\n";
}
hdr_record_value($hdr, 2);
hdr_record_value($hdr, 3);

echo "Mean: " . hdr_mean($hdr) . "\n";
echo "Min: " . hdr_min($hdr) . "\n";
echo "Max: " . hdr_max($hdr) . "\n";

if (hdr_record_values($hdr, 4, 2)) {
    echo "Recorded\n";
}
echo "Max: " . hdr_max($hdr) . "\n";
--EXPECT--
resource(4) of type (hdr_histogram)
Memory: 82024
Recorded
Mean: 2
Min: 1
Max: 3
Recorded
Max: 4
