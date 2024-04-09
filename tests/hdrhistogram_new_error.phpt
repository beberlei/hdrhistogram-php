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

try {
    $h = new HdrHistogram\Histogram(2, 3, 1);
    var_dump($h);
} catch (Throwable $e) {
    echo $e->getMessage(), PHP_EOL;
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

try {
    $h = new HdrHistogram\Histogram(1, 100, 50);
    var_dump($h);
} catch (Throwable $e) {
    echo $e->getMessage(), PHP_EOL;
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

try {
    $h = new HdrHistogram\Histogram(1, 100, 0);
    var_dump($h);
} catch (Throwable $e) {
    echo $e->getMessage(), PHP_EOL;
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
HdrHistogram\Histogram::__construct(): Argument #1 ($lowestDiscernibleValue) must be greater than or equal to 1
HdrHistogram\Histogram::__construct(): Argument #2 ($highestTrackableValue) must be greater than or equal to twice the value of argument #1 ($lowestDiscernibleValue)
HdrHistogram\Histogram::__construct(): Argument #3 ($significantFigures) must be between 1 and 5
HdrHistogram\Histogram::__construct(): Argument #3 ($significantFigures) must be between 1 and 5
