--TEST--
HdrHistogram: Directly constructing a HdrHistogram\Iterator object works.
--FILE--
<?php
$h = new HdrHistogram\Histogram(1, 100, 1);
$it = new HdrHistogram\Iterator($h);
var_dump($it);
?>
--EXPECTF--
object(HdrHistogram\Iterator)#%d (0) {
}
