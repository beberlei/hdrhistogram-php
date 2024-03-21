--TEST--
hdrhistogram: Value At Percentile
--FILE--
<?php

$hdr = hdr_init(1, 100, 5);

for ($i = 1; $i <= 100; $i++) {
    hdr_record_value($hdr, $i);
}

echo "perc(50): " . hdr_value_at_percentile($hdr, 50) . "\n";
echo "perc(95): " . hdr_value_at_percentile($hdr, 95) . "\n";

?>
--EXPECTF--
perc(50): 50
perc(95): 95
