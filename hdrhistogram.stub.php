<?php

/**
 * @generate-class-entries
 * @generate-function-entries
 * @generate-legacy-arginfo
 */
namespace {
    function hdr_init(int $lowest_trackable_value, int $highest_trackable_value, int $significant_figures): \HdrHistogram\Histogram|false {}

    function hdr_get_memory_size(\HdrHistogram\Histogram $hdr): int {};

    function hdr_record_value(\HdrHistogram\Histogram $hdr, int $value): bool {};

    function hdr_record_values(\HdrHistogram\Histogram $hdr, int $value, int $count): bool {};

    function hdr_record_corrected_value(\HdrHistogram\Histogram $hdr, int $value, int $expected_interval): bool {};

    function hdr_mean(\HdrHistogram\Histogram $hdr): int {};

    function hdr_stddev(\HdrHistogram\Histogram $hdr): float {};

    function hdr_min(\HdrHistogram\Histogram $hdr): int {};

    function hdr_max(\HdrHistogram\Histogram $hdr): int {};

    function hdr_total_count(\HdrHistogram\Histogram $hdr): int {};

    function hdr_reset(\HdrHistogram\Histogram $hdr): void {};

    function hdr_count_at_value(\HdrHistogram\Histogram $hdr, int $value): int {};

    function hdr_value_at_percentile(\HdrHistogram\Histogram $hdr, float $percentile): int {};

    function hdr_add(\HdrHistogram\Histogram $hdr1, \HdrHistogram\Histogram $hdr2): \HdrHistogram\Histogram|false {};

    function hdr_merge_into(\HdrHistogram\Histogram $hdr1, \HdrHistogram\Histogram $hdr2): int {};

    /**
     * @return resource|false
     */
    function hdr_iter_init(\HdrHistogram\Histogram $hdr) {};

    /**
     * @param resource $hdr
     */
    function hdr_iter_next($hdr): false|array {};

    /**
     * @return resource|false
     */
    function hdr_percentile_iter_init(\HdrHistogram\Histogram $hdr, int $ticks_per_half_distance) {};

    /**
     * @param resource $hdr
     */
    function hdr_percentile_iter_next($hdr): false|array {};

    function hdr_export(\HdrHistogram\Histogram $hdr): array {};

    function hdr_import(array $import): \HdrHistogram\Histogram|false {};

    function hdr_base64_encode(\HdrHistogram\Histogram $hdr): false|string {};

    function hdr_base64_decode(string $data): \HdrHistogram\Histogram|false {};
}

namespace HdrHistogram {
    /**
     * @strict-properties
     * @not-serializable
     */
    final class Histogram {
        public function __construct(int $lowest_trackable_value, int $highest_trackable_value, int $significant_figures) { }
    }
}
