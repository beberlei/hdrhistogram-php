#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "hdr/hdr_histogram.h"
#include "hdr/hdr_histogram_log.h"
#ifdef HAVE_HDRHISTOGRAM_0_11_7
#include "hdr/hdr_histogram_version.h"
#endif
#include "php_hdrhistogram.h"

#if PHP_VERSION_ID < 80000
#include "hdrhistogram_legacy_arginfo.h"
#else
#include "hdrhistogram_arginfo.h"
#endif

#define PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME "hdr_histogram"

static int le_hdrhistogram_descriptor;
static int le_hdrhistogram_iter_descriptor;

#define strsize_t size_t

static zend_always_inline void hdr_register_hdr_resource(zval *return_value, struct hdr_histogram* hdr)
{
    ZVAL_RES(return_value, zend_register_resource(hdr, le_hdrhistogram_descriptor));
}

static zend_always_inline void hdr_register_iter_resource(zval *return_value, struct hdr_iter* iter)
{
    ZVAL_RES(return_value, zend_register_resource(iter, le_hdrhistogram_iter_descriptor));
}

static zend_always_inline struct hdr_histogram* hdr_fetch_resource(zval *res, zval *return_value)
{
    struct hdr_histogram *hdr;

    return (struct hdr_histogram*)zend_fetch_resource(Z_RES_P(res), PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);
}

static zend_always_inline struct hdr_iter* hdr_fetch_iterator(zval *res, zval *return_value)
{
    struct hdr_iter *iterator;

    return (struct hdr_iter*)zend_fetch_resource(Z_RES_P(res), "hdr_iterator", le_hdrhistogram_iter_descriptor);
}

static zend_always_inline zval* hdr_hash_find(HashTable *arr, const char *name, strsize_t len)
{
    return zend_hash_str_find(arr, name, len - 1);
}

zend_module_entry hdrhistogram_module_entry = {
    STANDARD_MODULE_HEADER,
    "hdrhistogram",
    ext_functions,                         /* List of functions exposed */
    PHP_MINIT(hdrhistogram),               /* Module init callback */
    PHP_MSHUTDOWN(hdrhistogram),           /* Module shutdown callback */
    PHP_RINIT(hdrhistogram),               /* Request init callback */
    PHP_RSHUTDOWN(hdrhistogram),           /* Request shutdown callback */
    PHP_MINFO(hdrhistogram),               /* Module info callback */
    PHP_HDR_HISTOGRAM_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HDRHISTOGRAM
ZEND_GET_MODULE(hdrhistogram)
#endif

static void php_hdrhistogram_iterator_descriptor_dtor(zend_resource *rsrc)
{
    efree(rsrc->ptr);
}

static void php_hdrhistogram_descriptor_dtor(zend_resource *rsrc)
{
    hdr_close(rsrc->ptr);
}

PHP_MINIT_FUNCTION(hdrhistogram)
{
    le_hdrhistogram_descriptor = zend_register_list_destructors_ex(
        php_hdrhistogram_descriptor_dtor,
        NULL,
        PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME,
        module_number
    );

    le_hdrhistogram_iter_descriptor = zend_register_list_destructors_ex(
        php_hdrhistogram_iterator_descriptor_dtor,
        NULL,
        "hdr_iterator",
        module_number
    );

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(hdrhistogram)
{
    return SUCCESS;
}

PHP_RINIT_FUNCTION(hdrhistogram)
{
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(hdrhistogram)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(hdrhistogram)
{
	php_info_print_table_start();

	php_info_print_table_row(2, "hdrhistogram support", "enabled");
	php_info_print_table_row(2, "Extension version", PHP_HDR_HISTOGRAM_VERSION);
#ifdef HDR_HISTOGRAM_VERSION
	php_info_print_table_row(2, "Library version", HDR_HISTOGRAM_VERSION);
#endif
}

PHP_FUNCTION(hdr_init)
{
    struct hdr_histogram *hdr;
    long lowest_discernible_value, highest_trackable_value, significant_figures;
    int res;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "lll",
                &lowest_discernible_value, &highest_trackable_value, &significant_figures) == FAILURE) {
        php_error_docref(NULL, E_WARNING, "Invalid arguments passed.");

        RETURN_FALSE;
    }

    res = hdr_init(lowest_discernible_value, highest_trackable_value, significant_figures, &hdr);

    if (res == 0) {
        hdr_register_hdr_resource(return_value, hdr);
    } else if (res == EINVAL) {
        php_error_docref(NULL, E_WARNING, "Lowest trackable value has to be >= 1.");

        RETURN_FALSE;
    } else if (res == ENOMEM) {
        php_error_docref(NULL, E_WARNING, "Memory error in hdr_init allocation.");

        RETURN_FALSE;
    }
}

PHP_FUNCTION(hdr_get_memory_size)
{
    struct hdr_histogram *hdr;
    zval *zhdr;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zhdr) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    RETURN_LONG(hdr_get_memory_size(hdr));
}

PHP_FUNCTION(hdr_mean)
{
    struct hdr_histogram *hdr;
    zval *zhdr;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zhdr) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    RETURN_LONG(hdr_mean(hdr));
}

PHP_FUNCTION(hdr_stddev)
{
    struct hdr_histogram *hdr;
    zval *zhdr;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zhdr) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    RETURN_DOUBLE(hdr_stddev(hdr));
}


PHP_FUNCTION(hdr_min)
{
    struct hdr_histogram *hdr;
    zval *zhdr;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zhdr) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    RETURN_LONG(hdr_min(hdr));
}

PHP_FUNCTION(hdr_max)
{
    struct hdr_histogram *hdr;
    zval *zhdr;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zhdr) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    RETURN_LONG(hdr_max(hdr));
}

PHP_FUNCTION(hdr_total_count)
{
    struct hdr_histogram *hdr;
    zval *zhdr;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zhdr) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    RETURN_LONG(hdr->total_count);
}

PHP_FUNCTION(hdr_record_value)
{
    struct hdr_histogram *hdr;
    zval *zhdr;
    long value;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "rl", &zhdr, &value) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    if (hdr_record_value(hdr, value) == 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}

PHP_FUNCTION(hdr_record_values)
{
    struct hdr_histogram *hdr;
    zval *zhdr;
    long value;
    long count;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "rll", &zhdr, &value, &count) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    if (hdr_record_values(hdr, value, count) == 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}

PHP_FUNCTION(hdr_record_corrected_value)
{
    struct hdr_histogram *hdr;
    zval *zhdr;
    long value;
    long expected_interval;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "rll", &zhdr, &value, &expected_interval) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    if (hdr_record_corrected_value(hdr, value, expected_interval)) {
        RETURN_TRUE;
    }
    RETURN_FALSE;
}

PHP_FUNCTION(hdr_reset)
{
    struct hdr_histogram *hdr;
    zval *zhdr;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zhdr) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    hdr_reset(hdr);
}

PHP_FUNCTION(hdr_count_at_value)
{
    struct hdr_histogram *hdr;
    zval *zhdr;
    long value;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "rl", &zhdr, &value) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    RETURN_LONG(hdr_count_at_value(hdr, value));
}

PHP_FUNCTION(hdr_value_at_percentile)
{
    struct hdr_histogram *hdr;
    zval *zhdr;
    double percentile;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "rd", &zhdr, &percentile) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    RETURN_LONG(hdr_value_at_percentile(hdr, percentile));
}

PHP_FUNCTION(hdr_add)
{
    struct hdr_histogram *hdra, *hdrb, *hdr_new;
    zval *a, *b;
    int res;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "rr", &a, &b) == FAILURE) {
        RETURN_FALSE;
    }

    hdra = hdr_fetch_resource(a, return_value);
    hdrb = hdr_fetch_resource(b, return_value);

#ifdef HAVE_HDRHISTOGRAM_0_11_4
    res = hdr_init(hdra->lowest_discernible_value, hdra->highest_trackable_value, hdra->significant_figures, &hdr_new);
#else
    res = hdr_init(hdra->lowest_trackable_value, hdra->highest_trackable_value, hdra->significant_figures, &hdr_new);
#endif
    hdr_add(hdr_new, hdra);
    hdr_add(hdr_new, hdrb);

    if (res == 0) {
        hdr_register_hdr_resource(return_value, hdr_new);
    } else if (res == EINVAL) {
        php_error_docref(NULL, E_WARNING, "Lowest trackable value has to be >= 1.");

        RETURN_FALSE;
    } else if (res == ENOMEM) {
        php_error_docref(NULL, E_WARNING, "Memory error in hdr_init allocation.");

        RETURN_FALSE;
    }
}

PHP_FUNCTION(hdr_merge_into)
{
    struct hdr_histogram *hdra, *hdrb;
    zval *a, *b;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "rr", &a, &b) == FAILURE) {
        RETURN_FALSE;
    }

    hdra = hdr_fetch_resource(a, return_value);
    hdrb = hdr_fetch_resource(b, return_value);

    RETURN_LONG(hdr_add(hdra, hdrb));
}

PHP_FUNCTION(hdr_iter_init)
{
    struct hdr_iter *iterator;
    struct hdr_histogram *hdr;
    zval *zhdr;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zhdr) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    iterator = emalloc(sizeof(struct hdr_iter));
    hdr_iter_init(iterator, hdr);

    hdr_register_iter_resource(return_value, iterator);
}

PHP_FUNCTION(hdr_percentile_iter_init)
{
    struct hdr_iter *iterator;
    struct hdr_histogram *hdr;
    zval *zhdr;
    long ticks_per_half_distance;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "rl", &zhdr, &ticks_per_half_distance) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    iterator = emalloc(sizeof(struct hdr_iter));
    hdr_iter_percentile_init(iterator, hdr, ticks_per_half_distance);

    hdr_register_iter_resource(return_value, iterator);
}

PHP_FUNCTION(hdr_iter_next)
{
    struct hdr_iter *iterator;
    zval *zhdr;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zhdr) == FAILURE) {
        RETURN_FALSE;
    }

    iterator = hdr_fetch_iterator(zhdr, return_value);

    if (hdr_iter_next(iterator)) {
        array_init(return_value);
        add_assoc_long(return_value, "value", iterator->value);
        add_assoc_long(return_value, "count_at_index", iterator->count);
        add_assoc_long(return_value, "count_to_index", iterator->cumulative_count);
        add_assoc_long(return_value, "highest_equivalent_value", iterator->highest_equivalent_value);
    } else {
        RETURN_FALSE;
    }
}

PHP_FUNCTION(hdr_percentile_iter_next)
{
    struct hdr_iter *iterator;
    zval *zhdr;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zhdr) == FAILURE) {
        RETURN_FALSE;
    }

    iterator = hdr_fetch_iterator(zhdr, return_value);

    if (hdr_iter_next(iterator)) {
        array_init(return_value);
        add_assoc_long(return_value, "value", iterator->value);
        add_assoc_long(return_value, "count_at_index", iterator->count);
        add_assoc_long(return_value, "count_to_index", iterator->cumulative_count);
        add_assoc_long(return_value, "highest_equivalent_value", iterator->highest_equivalent_value);
        add_assoc_long(return_value, "seen_last_value", iterator->specifics.percentiles.seen_last_value);
        add_assoc_long(return_value, "ticks_per_half_distance", iterator->specifics.percentiles.ticks_per_half_distance);
        add_assoc_double(return_value, "percentile_to_iterate_to", iterator->specifics.percentiles.percentile_to_iterate_to);
        add_assoc_double(return_value, "percentile", iterator->specifics.percentiles.percentile);
    } else {
        RETURN_FALSE;
    }
}

PHP_FUNCTION(hdr_export)
{
    zval *zhdr;
    zval values;
    int32_t i;
    struct hdr_histogram *hdr;
    int found = 0;
    long skipped = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zhdr) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    array_init(return_value);


#ifdef HAVE_HDRHISTOGRAM_0_11_4
    if (hdr->lowest_discernible_value > 1) {
        add_assoc_long(return_value, "ltv", hdr->lowest_discernible_value);
    }
#else
    if (hdr->lowest_trackable_value > 1) {
        add_assoc_long(return_value, "ltv", hdr->lowest_trackable_value);
    }
#endif
    if (hdr->highest_trackable_value != 60000) {
        add_assoc_long(return_value, "htv", hdr->highest_trackable_value);
    }
    if (hdr->significant_figures != 2) {
        add_assoc_long(return_value, "sf", hdr->significant_figures);
    }

    array_init(&values);

    for (i = 0; i < hdr->counts_len; i++) {
        if (found >= hdr->total_count) {
            break;
        }
        if (hdr->counts[i] == 0) {
            skipped--;
        } else {
            if (skipped < 0) {
                add_next_index_long(&values, skipped);
                skipped = 0;
            }
            add_next_index_long(&values, (long)hdr->counts[i]);
        }

        found += hdr->counts[i];
    }

    add_assoc_zval(return_value, "v", &values);
}

PHP_FUNCTION(hdr_import)
{
    struct hdr_histogram *hdr;
    zval *import, *value, *item;
    long lowest_discernible_value, highest_trackable_value, significant_figures;
    int res, count;
    zend_ulong i, bucket, skipped;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "a", &import) == FAILURE) {
        RETURN_FALSE;
    }

    if ((value = hdr_hash_find(Z_ARRVAL_P(import), "ltv", 4)) != NULL) {
        lowest_discernible_value = Z_LVAL_P(value);
    } else {
        lowest_discernible_value = 1;
    }

    if (lowest_discernible_value <= 0) {
        php_error_docref(NULL, E_WARNING, "lowest_trackable_value (ltv) must be >= 1.");
        RETURN_FALSE;
    }

    if ((value = hdr_hash_find(Z_ARRVAL_P(import), "htv", 4)) != NULL) {
        highest_trackable_value = Z_LVAL_P(value);
    } else {
        highest_trackable_value = 60000;
    }

    if (highest_trackable_value <= 0) {
        php_error_docref(NULL, E_WARNING, "highest_trackable_value (htv) must be >= 1.");
        RETURN_FALSE;
    }

    if ((value = hdr_hash_find(Z_ARRVAL_P(import), "sf", 3)) != NULL) {
        significant_figures = Z_LVAL_P(value);
    } else {
        significant_figures = 2;
    }

    if (significant_figures <= 0 || significant_figures > 3) {
        php_error_docref(NULL, E_WARNING, "significant_figures (sf) must be 1, 2, or 3.");
        RETURN_FALSE;
    }

    if ((value = hdr_hash_find(Z_ARRVAL_P(import), "sk", 3)) != NULL) {
        skipped = Z_LVAL_P(value);
    } else {
        skipped = 0;
    }

    if (skipped < 0 || lowest_discernible_value < 1 || highest_trackable_value < lowest_discernible_value || significant_figures < 1) {
        php_error_docref(NULL, E_WARNING, "Invalid values for ltv, htv, sf or sk keys given.");
        RETURN_FALSE;
    }

    value = hdr_hash_find(Z_ARRVAL_P(import), "v", 2);

    // version 3 format
    if (value != NULL && Z_TYPE_P(value) == IS_ARRAY) {
        count = zend_hash_num_elements(Z_ARRVAL_P(value));
        res = hdr_init(lowest_discernible_value, highest_trackable_value, significant_figures, &hdr);

        if (res == 0) {
            hdr_register_hdr_resource(return_value, hdr);
        } else if (res == EINVAL) {
            php_error_docref(NULL, E_WARNING, "Lowest trackable value has to be >= 1.");

            RETURN_FALSE;
        } else if (res == ENOMEM) {
            php_error_docref(NULL, E_WARNING, "Memory error in hdr_init allocation.");

            RETURN_FALSE;
        }

        zend_string *key;
        zend_ulong num_key;
        int bucket = 0;
        ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(value), num_key, key, item) {
            if (!key && bucket < hdr->counts_len) {
                convert_to_long_ex(item);
                if (Z_LVAL_P(item) > 0) {
                    hdr->counts[bucket] = Z_LVAL_P(item);
                    bucket++;
                } else {
                    bucket += (Z_LVAL_P(item) * -1);
                }
            }
        } ZEND_HASH_FOREACH_END();

        hdr_reset_internal_counters(hdr);
        hdr->normalizing_index_offset = 0;
        hdr->conversion_ratio = 1.0;

        return;
    }

    value = hdr_hash_find(Z_ARRVAL_P(import), "c", 2);

    // version 1 format
    if (value != NULL && Z_TYPE_P(value) == IS_ARRAY) {
        count = zend_hash_num_elements(Z_ARRVAL_P(value));

        res = hdr_init(lowest_discernible_value, highest_trackable_value, significant_figures, &hdr);

        if (res == 0) {
            hdr_register_hdr_resource(return_value, hdr);
        } else if (res == EINVAL) {
            php_error_docref(NULL, E_WARNING, "Lowest trackable value has to be >= 1.");

            RETURN_FALSE;
        } else if (res == ENOMEM) {
            php_error_docref(NULL, E_WARNING, "Memory error in hdr_init allocation.");

            RETURN_FALSE;
        }

        for (i = 0; i < skipped; i++) {
            if (i < hdr->counts_len) {
                hdr->counts[i] = 0;
            }
        }

        for (i = 0; i < count; i++) {
            if ((item = zend_hash_index_find(Z_ARRVAL_P(value), i)) != NULL) {
                bucket = i + skipped;
                if (bucket < hdr->counts_len) {
                    convert_to_long_ex(item);
                    if (Z_LVAL_P(item) > 0) {
                        hdr->counts[bucket] = Z_LVAL_P(item);
                    }
                }
            }
        }

        hdr_reset_internal_counters(hdr);
        hdr->normalizing_index_offset = 0;
        hdr->conversion_ratio = 1.0;

        return;
    }

    value = hdr_hash_find(Z_ARRVAL_P(import), "b", 2);

    // version 2 format
    if (value != NULL && Z_TYPE_P(value) == IS_ARRAY) {
        res = hdr_init(lowest_discernible_value, highest_trackable_value, significant_figures, &hdr);

        if (res == 0) {
            hdr_register_hdr_resource(return_value, hdr);
        } else if (res == EINVAL) {
            php_error_docref(NULL, E_WARNING, "Lowest trackable value has to be >= 1.");

            RETURN_FALSE;
        } else if (res == ENOMEM) {
            php_error_docref(NULL, E_WARNING, "Memory error in hdr_init allocation.");

            RETURN_FALSE;
        }

        zend_string *key;
        zend_ulong num_key;
        ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(value), num_key, key, item) {
            if (!key) {
                if (num_key < hdr->counts_len) {
                    convert_to_long_ex(item);
                    if (Z_LVAL_P(item) > 0) {
                        hdr->counts[num_key] = Z_LVAL_P(item);
                    }
                }
            }
        } ZEND_HASH_FOREACH_END();

        hdr_reset_internal_counters(hdr);
        hdr->normalizing_index_offset = 0;
        hdr->conversion_ratio = 1.0;
    } else {
        php_error_docref(NULL, E_WARNING, "Missing counts (c) or bucket (b) key or not arrays.");
        RETURN_FALSE;
    }
}

PHP_FUNCTION(hdr_base64_encode)
{
    struct hdr_histogram *hdr;
    zval *zhdr;
    char *result = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zhdr) == FAILURE) {
        RETURN_FALSE;
    }

    hdr = hdr_fetch_resource(zhdr, return_value);

    if (hdr_log_encode(hdr, &result) != 0) {
        php_error_docref(NULL, E_WARNING, "Cannot encode histogram");

        RETURN_FALSE;
    }

    RETVAL_STRING(result);
    free(result);
}

PHP_FUNCTION(hdr_base64_decode)
{
    struct hdr_histogram *hdr = NULL;
    zend_string *data;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &data) == FAILURE) {
        RETURN_FALSE;
    }

    if (hdr_log_decode(&hdr, ZSTR_VAL(data), ZSTR_LEN(data)) != 0) {
        php_error_docref(NULL, E_WARNING, "Cannot decode histogram");

        RETURN_FALSE;
    }

    hdr_register_hdr_resource(return_value, hdr);
}
