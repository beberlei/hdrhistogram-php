--TEST--
HdrHistogram: Error handling of Histogram::__construct()
--FILE--
<?php
try {
    $h = new HdrHistogram\Histogram(0, 100, 1);
    var_dump($h);
} catch (Throwable $e) {
    echo $e->getMessage(), PHP_EOL;
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
HdrHistogram\Histogram::__construct(): Argument #1 ($lowest_discernible_value) must be greater than or equal to 1
