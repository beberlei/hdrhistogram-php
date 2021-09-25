# hdrhistogram-php

A PHP extension wrapping the [HdrHistogram C API](https://github.com/HdrHistogram/HdrHistogram_c).

License: New-BSD 2 Clause

## Description

HdrHistogram: A High Dynamic Range Histogram.

A Histogram that supports recording and analyzing sampled data value counts across a configurable integer value range with configurable value precision within the range. Value precision is expressed as the number of significant digits in the value recording, and provides control over value quantization behavior across the value range and the subsequent value resolution at any given level.

[Website/Documentation](http://hdrhistogram.org)

## Installation

This extension depends on HdrHistogram C API, you can install it easily:

    $ git clone git@github.com:HdrHistogram/HdrHistogram_c
    $ cd HdrHistogram_c
    $ cmake .
    $ make
    $ sudo make install

Then you can compile this extension:

    $ phpize
    $ ./configure
    $ make
    $ sudo make install

You can then load the extension in your PHP.ini:

    extension=hdrhistogram.so

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

``int hdr_get_memory_size(resource $hdr)``

Calculates the required memory size (C) for the given histogram, excluding the PHP overhead for zval and resource representation.

``int hdr_mean(resource $hdr)``

Estimate the mean of the given histogram.

``double hdr_stddev(resource $hdr)``

Estimate the standard deviation of the given histogram.

``int hdr_min(resource $hdr)``

Retrieve the minimal value of the given histogram.

``int hdr_max(resource $hdr)``

Retrieve the maximum value of the given histogram.

``bool hdr_record_value(resource $hdr, int $value)``

Record a value (integer) for the given histogram.

Returns false if the value is larger than the `highest_trackable_value` and can't be recorded, true otherwise.

``bool hdr_record_values(resource $hdr, int $value, int $count)``

Record a value (integer) for $count number of times in the given histogram.

Returns false if the value is larger than the `highest_trackable_value` and can't be recorded, true otherwise.

``bool hdr_record_corrected_value(resource $hdr, int $value, int $expected_interval)``

Record a value in the histogram and backfill based on an expected interval.

Records a value in the histogram, will round this value of to a precision at or
better than the `significant_figure` specified at contruction time.  This is
specifically used for recording latency.  If the value is larger than the
`expected_interval` then the latency recording system has experienced
co-ordinated omission. This method fills in the values that would have occured
had the client providing the load not been blocked.

``hdr_reset(resource $hdr)``

Drop all recorded values of the given histogram.

``int hdr_count_at_value(resource $hdr, int $value)``

Get the count of recorded values at a specific value (to within the histogram resolution at the value level).

``int hdr_value_at_percentile(resource $hdr, double $percentile)``

Get the value at a specific percentile.

``resource hdr_add(resource $to, resource $from)``

Adds all of the values from 'from' to 'to' histogram and create a new histogram
as return value. `$to` will not be modified.  Values will be dropped if they around
outside of h.lowest_trackable_value and h.highest_trackable_value.

``int hdr_merge_into(resource $to, resource $from)``

Mutable version of ``hdr_add``, modifiying the ``$to`` histogram.

``resource hdr_iter_init(resource $hdr)``

Initializes the basic iterator for given histogram.

``array|false hdr_iter_next(resource $iter)``

Iterate to the next value for the iterator. If there are no more values available return false.

Returns array with keys `value`, `count_at_index`, `count_to_index`, `highest_equivalent_value`.


``resource hdr_percentile_iter_init(resource $hdr, int $ticks_per_half_distance)``

Initializes the percentile iterator for given histogram.

``array|false hdr_percentile_iter_next(resouce $iter)``

Returns array with keys `value`, `count_at_index`, `count_to_index`, `highest_equivalent_value`, `seen_last_value`, `ticks_per_half_distance`, `percentile_to_iterate_to`, `percentile`.

``array|false hdr_export(resource $hdr)``

Export array into a serializable array format.

``resource hdr_import(array $data)``

Import and create HDR histogram from serializable array data format that is created by ``hdr_export``.

## TODO

* Missing Iterators
    * LinearIterator
    * LogIterator
    * RecordedIterator
