--TEST--
hdrhistogram: Basic Functionality
--FILE--
<?php

$hdr = hdr_init(1, 360000, 3);
var_dump($hdr);
echo "Memory: " . hdr_get_memory_size($hdr) . "\n";

// Record Values
if (hdr_record_value($hdr, 1)) {
    echo "Recorded\n";
}
echo "StdDev: " . number_format(hdr_stddev($hdr), 2) . "\n";
hdr_record_value($hdr, 2);
hdr_record_value($hdr, 3);

echo "Mean: " . hdr_mean($hdr) . "\n";
echo "StdDev: " . number_format(hdr_stddev($hdr), 2) . "\n";
echo "Min: " . hdr_min($hdr) . "\n";
echo "Max: " . hdr_max($hdr) . "\n";

if (hdr_record_values($hdr, 4, 2)) {
    echo "Recorded\n";
}
echo "StdDev: " . number_format(hdr_stddev($hdr), 2) . "\n";
echo "Max: " . hdr_max($hdr) . "\n";

?>
--EXPECTF--
resource(4) of type (hdr_histogram)
Memory: 820%d
Recorded
StdDev: 0.00
Mean: 2
StdDev: 0.82
Min: 1
Max: 3
Recorded
StdDev: 1.17
Max: 4
