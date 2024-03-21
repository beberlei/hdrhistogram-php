--TEST--
hdrhistogram: Corrected Values
--FILE--
<?php

$hdr = hdr_init(1, 360000, 3);
hdr_record_corrected_value($hdr, 1, 10);
hdr_record_corrected_value($hdr, 10, 10);
hdr_record_corrected_value($hdr, 20, 10);

echo "Mean: " . hdr_mean($hdr) . "\n";
echo "Max: " . hdr_max($hdr) . "\n";

?>
--EXPECTF--
Mean: 10
Max: 20
