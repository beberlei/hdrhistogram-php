--TEST--
hdrhistogram: Total Count
--FILE--
<?php

$a = hdr_init(1, 100, 1);

echo "Count with 0 entries: " . hdr_total_count($a) . "\n";

for ($i = 0; $i < 100; $i++) {
    hdr_record_values($a, $i, 1);
}

echo "Count with 100 entries: " . hdr_total_count($a) . "\n";

for ($i = 0; $i < 100; $i++) {
    hdr_record_values($a, $i, 1);
}

echo "Count with 200 entries: " . hdr_total_count($a) . "\n";

?>
--EXPECT--
Count with 0 entries: 0
Count with 100 entries: 100
Count with 200 entries: 200
