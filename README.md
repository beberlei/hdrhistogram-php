# hdrhistogram-php

[![Build Status](https://travis-ci.org/beberlei/hdrhistogram-php.svg?branch=master)](https://travis-ci.org/beberlei/hdrhistogram-php)

A PHP extension wrapping the [HdrHistogram C API](https://github.com/HdrHistogram/HdrHistogram_c).

License: New-BSD 2 Clause

## Description

HdrHistogram: A High Dynamic Range Histogram.

A Histogram that supports recording and analyzing sampled data value counts across a configurable integer value range with configurable value precision within the range. Value precision is expressed as the number of significant digits in the value recording, and provides control over value quantization behavior across the value range and the subsequent value resolution at any given level.

[Website/Documentation](http://hdrhistogram.org)

## Example

The API is very similar to the C code:

```php
<?php

$hdr = hdr_init(1, 3600000, 3);
hdr_record_value($hdr, 1337);
hdr_record_values($hdr, 42, 100);

printf(
    'Mean: %d Min: %d Max: %d Count(42): %d Perc(95): %d',
    hdr_mean($hdr),
    hdr_min($hdr),
    hdr_max($hdr),
    hdr_count_at_value($hdr, 42),
    hdr_value_at_percentile($hdr, 95)
);

$iter = hdr_iter_create($hdr);

while ($data = hdr_iter_next($iter)) {
    printf(
        'Value: %d-%d Count: %d, CountTo: %d',
        $data['value'],
        $data['highest_equivalent_value'],
        $data['count_at_index'],
        $data['count_to_index']
    );
}

```

## API

``hdr_init(int $lowestTrackableValue, int $highestTrackableValue, int $significantValues): resource``

Create a new histogram resource within the given bounds of trackable values and the precision
defined by significant values.

## TODO

* Missing Iterators
    * LinearIterator
    * LogIterator
    * RecordedIterator
