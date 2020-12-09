<?php

/**
 * @generate-function-entries
 * @generate-legacy-arginfo
 */

/**
 * @return resource|false
 */
function hdr_init(int $lowest_trackable_value, int $highest_trackable_value, int $significant_figures) {}

/**
 * @param resource $hdr
 */
function hdr_get_memory_size($hdr): int {};

/**
 * @param resource $hdr
 */
function hdr_record_value($hdr, int $value): bool {};

/**
 * @param resource $hdr
 */
function hdr_record_values($hdr, int $value, int $count): bool {};

/**
 * @param resource $hdr
 */
function hdr_record_corrected_value($hdr, int $value, int $expected_interval): void {};

/**
 * @param resource $hdr
 */
function hdr_mean($hdr): int {};

/**
 * @param resource $hdr
 */
function hdr_stddev($hdr): float {};

/**
 * @param resource $hdr
 */
function hdr_min($hdr): int {};

/**
 * @param resource $hdr
 */
function hdr_max($hdr): int {};

/**
 * @param resource $hdr
 */
function hdr_total_count($hdr): int {};

/**
 * @param resource $hdr
 */
function hdr_reset($hdr): void {};

/**
 * @param resource $hdr
 */
function hdr_count_at_value($hdr, int $value): int {};

/**
 * @param resource $hdr
 */
function hdr_value_at_percentile($hdr, float $percentile): int {};

/**
 * @param resource $hdr1
 * @param resource $hdr2
 * @return resource|false
 */
function hdr_add($hdr1, $hdr2) {};

/**
 * @param resource $hdr1
 * @param resource $hdr2
 */
function hdr_merge_into($hdr1, $hdr2): int {};

/**
 * @param resource $hdr
 * @return resource|false
 */
function hdr_iter_init($hdr) {};

/**
 * @param resource $hdr
 */
function hdr_iter_next($hdr): false|Array {};

/**
 * @param resource $hdr
 * @return resource|false
 */
function hdr_percentile_iter_init($hdr, int $ticks_per_half_distance) {};

/**
 * @param resource $hdr
 */
function hdr_percentile_iter_next($hdr): false|Array {};

/**
 * @param resource $hdr
 */
function hdr_export($hdr): Array {};

/**
 * @return resource|false
 */
function hdr_import(Array $import) {};

/**
 * @param resource $hdr
 */
function hdr_base64_encode($hdr): false|string {};

/**
 * @return resource|false
 */
function hdr_base64_decode(string $data) {};

