--TEST--
HdrHistogram: Cloning a HdrHistogram\Histogram object works.
--FILE--
<?php
$h = new HdrHistogram\Histogram(1, 100, 1);
hdr_record_value($h, 1);

var_dump(hdr_total_count($h), hdr_max($h));

$h2 = clone $h;

hdr_record_value($h, 2);

var_dump(hdr_total_count($h), hdr_max($h));
var_dump(hdr_total_count($h2), hdr_max($h2));
hdr_record_value($h2, 3);

var_dump(hdr_total_count($h), hdr_max($h));
var_dump(hdr_total_count($h2), hdr_max($h2));
?>
--EXPECTF--
int(1)
int(1)
int(2)
int(2)
int(1)
int(1)
int(2)
int(2)
int(2)
int(3)
