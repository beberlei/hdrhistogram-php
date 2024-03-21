--TEST--
HdrHistogram: Serializing a HdrHistogram\Histogram object is not allowed.
--FILE--
<?php
$h = new HdrHistogram\Histogram(1, 100, 1);
try {
    serialize($h);
} catch (Throwable $e) {
    echo $e->getMessage(), PHP_EOL;
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
Serialization of 'HdrHistogram\Histogram' is not allowed
