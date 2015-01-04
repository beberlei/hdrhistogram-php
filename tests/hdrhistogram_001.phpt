--TEST--
hdrhistogram: Basic Functionality
Author: kontakt@beberlei.de
--FILE--
<?php

$hdr = new HdrHistogram(0, 360000, 3);
$hdr->recordValue(1);
$hdr->recordValue(2);
$hdr->recordValue(3);

echo "Mean: " . $hdr->mean() . "\n";
echo "Min: " . $hdr->min() . "\n";
echo "Max: " . $hdr->max();
--EXPECT--
Mean: 2
Min: 1
Max: 3
