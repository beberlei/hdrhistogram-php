--TEST--
HdrHistogram: Directly constructing a HdrHistogram\Histogram object works.
--FILE--
<?php
$h = new HdrHistogram\Histogram(1, 100, 1);
var_dump($h);
?>
--EXPECTF--
object(HdrHistogram\Histogram)#%d (0) {
}
