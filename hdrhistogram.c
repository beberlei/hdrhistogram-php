#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"
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

static int le_hdrhistogram_iter_descriptor;

static zend_class_entry *php_HdrHistogram_Histogram_ce;
static zend_object_handlers php_hdrhistogram_histogram_object_handlers;

struct php_hdrhistogram_histogram {
    struct hdr_histogram *histogram;
    zend_object std;
};

static inline struct php_hdrhistogram_histogram *php_hdrhistogram_histogram_from_object(zend_object *object) {
    return (struct php_hdrhistogram_histogram *)((char *)(object) - XtOffsetOf(struct php_hdrhistogram_histogram, std));
}

static zend_object *php_hdrhistogram_histogram_new(zend_class_entry *ce)
{
    struct php_hdrhistogram_histogram *histogram;
#if PHP_VERSION_ID >= 80000
    histogram = zend_object_alloc(sizeof(struct php_hdrhistogram_histogram), ce);
#else
    histogram = ecalloc(1, sizeof(struct php_hdrhistogram_histogram) + sizeof(zval) * (ce->default_properties_count - 1));
#endif

    zend_object_std_init(&histogram->std, ce);
    object_properties_init(&histogram->std, ce);
    histogram->std.handlers = &php_hdrhistogram_histogram_object_handlers;

    histogram->histogram = NULL;

    return &histogram->std;
}

static void php_hdrhistogram_histogram_free(zend_object *object)
{
    struct php_hdrhistogram_histogram *histogram = php_hdrhistogram_histogram_from_object(object);

    hdr_close(histogram->histogram);

    zend_object_std_dtor(&histogram->std);
}

static zend_always_inline void hdr_register_iter_resource(zval *return_value, struct hdr_iter* iter)
{
    ZVAL_RES(return_value, zend_register_resource(iter, le_hdrhistogram_iter_descriptor));
}

static zend_always_inline struct hdr_iter* hdr_fetch_iterator(zval *res, zval *return_value)
{
    struct hdr_iter *iterator;

    return (struct hdr_iter*)zend_fetch_resource(Z_RES_P(res), "hdr_iterator", le_hdrhistogram_iter_descriptor);
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

PHP_MINIT_FUNCTION(hdrhistogram)
{
    php_HdrHistogram_Histogram_ce = register_class_HdrHistogram_Histogram();
    php_HdrHistogram_Histogram_ce->create_object = php_hdrhistogram_histogram_new;
    memcpy(&php_hdrhistogram_histogram_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    php_hdrhistogram_histogram_object_handlers.offset = XtOffsetOf(struct php_hdrhistogram_histogram, std);
    php_hdrhistogram_histogram_object_handlers.free_obj = php_hdrhistogram_histogram_free;
    php_hdrhistogram_histogram_object_handlers.clone_obj = NULL;
#if PHP_VERSION_ID < 80100
    php_HdrHistogram_Histogram_ce->serialize    = zend_class_serialize_deny;
    php_HdrHistogram_Histogram_ce->unserialize  = zend_class_unserialize_deny;
#endif

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

ZEND_METHOD(HdrHistogram_Histogram, __construct)
{
    zend_long lowest_discernible_value, highest_trackable_value, significant_figures;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_LONG(lowest_discernible_value);
        Z_PARAM_LONG(highest_trackable_value);
        Z_PARAM_LONG(significant_figures);
    ZEND_PARSE_PARAMETERS_END();

    if (lowest_discernible_value < 1) {
#if PHP_VERSION_ID >= 80000
        zend_argument_value_error(1, "must be greater than or equal to 1");
#else
        zend_throw_exception_ex(NULL, 0, "%s(): Argument #%d ($%s) must be greater than or equal to 1", "HdrHistogram\\Histogram::__construct", 1, "lowest_trackable_value");
#endif
        return;
    }

    struct php_hdrhistogram_histogram *h = php_hdrhistogram_histogram_from_object(Z_OBJ_P(getThis()));

    struct hdr_histogram *hdr;
    int res;

    res = hdr_init(lowest_discernible_value, highest_trackable_value, significant_figures, &hdr);

    if (res != 0) {
        zend_throw_error(zend_ce_error, "Unable to initialize HdrHistogram.");
        return;
    }

    h->histogram = hdr;
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
        object_init_ex(return_value, php_HdrHistogram_Histogram_ce);
        struct php_hdrhistogram_histogram *h = php_hdrhistogram_histogram_from_object(Z_OBJ_P(return_value));
        h->histogram = hdr;
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

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_LONG(hdr_get_memory_size(hdr));
}

PHP_FUNCTION(hdr_mean)
{
    struct hdr_histogram *hdr;
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_LONG(hdr_mean(hdr));
}

PHP_FUNCTION(hdr_stddev)
{
    struct hdr_histogram *hdr;
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_DOUBLE(hdr_stddev(hdr));
}


PHP_FUNCTION(hdr_min)
{
    struct hdr_histogram *hdr;
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_LONG(hdr_min(hdr));
}

PHP_FUNCTION(hdr_max)
{
    struct hdr_histogram *hdr;
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_LONG(hdr_max(hdr));
}

PHP_FUNCTION(hdr_total_count)
{
    struct hdr_histogram *hdr;
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_LONG(hdr->total_count);
}

PHP_FUNCTION(hdr_record_value)
{
    struct hdr_histogram *hdr;
    zval *zhdr;
    zend_long value;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
        Z_PARAM_LONG(value);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    if (hdr_record_value(hdr, value) == 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}

PHP_FUNCTION(hdr_record_values)
{
    struct hdr_histogram *hdr;
    zval *zhdr;
    zend_long value;
    zend_long count;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
        Z_PARAM_LONG(value);
        Z_PARAM_LONG(count);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    if (hdr_record_values(hdr, value, count) == 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}

PHP_FUNCTION(hdr_record_corrected_value)
{
    struct hdr_histogram *hdr;
    zval *zhdr;
    zend_long value;
    zend_long expected_interval;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
        Z_PARAM_LONG(value);
        Z_PARAM_LONG(expected_interval);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    if (hdr_record_corrected_value(hdr, value, expected_interval)) {
        RETURN_TRUE;
    }

    RETURN_FALSE;
}

PHP_FUNCTION(hdr_reset)
{
    struct hdr_histogram *hdr;
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    hdr_reset(hdr);
}

PHP_FUNCTION(hdr_count_at_value)
{
    struct hdr_histogram *hdr;
    zval *zhdr;
    zend_long value;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
        Z_PARAM_LONG(value);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_LONG(hdr_count_at_value(hdr, value));
}

PHP_FUNCTION(hdr_value_at_percentile)
{
    struct hdr_histogram *hdr;
    zval *zhdr;
    double percentile;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
        Z_PARAM_DOUBLE(percentile);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_LONG(hdr_value_at_percentile(hdr, percentile));
}

PHP_FUNCTION(hdr_add)
{
    struct hdr_histogram *hdra, *hdrb, *hdr_new;
    zval *a, *b;
    int res;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(a, php_HdrHistogram_Histogram_ce);
        Z_PARAM_OBJECT_OF_CLASS(b, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    hdra = php_hdrhistogram_histogram_from_object(Z_OBJ_P(a))->histogram;
    hdrb = php_hdrhistogram_histogram_from_object(Z_OBJ_P(b))->histogram;

#ifdef HAVE_HDRHISTOGRAM_0_11_4
    res = hdr_init(hdra->lowest_discernible_value, hdra->highest_trackable_value, hdra->significant_figures, &hdr_new);
#else
    res = hdr_init(hdra->lowest_trackable_value, hdra->highest_trackable_value, hdra->significant_figures, &hdr_new);
#endif
    hdr_add(hdr_new, hdra);
    hdr_add(hdr_new, hdrb);

    if (res == 0) {
        object_init_ex(return_value, php_HdrHistogram_Histogram_ce);
        struct php_hdrhistogram_histogram *h = php_hdrhistogram_histogram_from_object(Z_OBJ_P(return_value));
        h->histogram = hdr_new;
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

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(a, php_HdrHistogram_Histogram_ce);
        Z_PARAM_OBJECT_OF_CLASS(b, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    hdra = php_hdrhistogram_histogram_from_object(Z_OBJ_P(a))->histogram;
    hdrb = php_hdrhistogram_histogram_from_object(Z_OBJ_P(b))->histogram;

    RETURN_LONG(hdr_add(hdra, hdrb));
}

PHP_FUNCTION(hdr_iter_init)
{
    struct hdr_iter *iterator;
    struct hdr_histogram *hdr;
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    iterator = emalloc(sizeof(struct hdr_iter));
    hdr_iter_init(iterator, hdr);

    hdr_register_iter_resource(return_value, iterator);
}

PHP_FUNCTION(hdr_percentile_iter_init)
{
    struct hdr_iter *iterator;
    struct hdr_histogram *hdr;
    zval *zhdr;
    zend_long ticks_per_half_distance;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
        Z_PARAM_LONG(ticks_per_half_distance);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

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

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

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

    if ((value = zend_hash_str_find(Z_ARRVAL_P(import), "ltv", strlen("ltv"))) != NULL) {
        lowest_discernible_value = Z_LVAL_P(value);
    } else {
        lowest_discernible_value = 1;
    }

    if (lowest_discernible_value <= 0) {
        php_error_docref(NULL, E_WARNING, "lowest_trackable_value (ltv) must be >= 1.");
        RETURN_FALSE;
    }

    if ((value = zend_hash_str_find(Z_ARRVAL_P(import), "htv", strlen("htv"))) != NULL) {
        highest_trackable_value = Z_LVAL_P(value);
    } else {
        highest_trackable_value = 60000;
    }

    if (highest_trackable_value <= 0) {
        php_error_docref(NULL, E_WARNING, "highest_trackable_value (htv) must be >= 1.");
        RETURN_FALSE;
    }

    if ((value = zend_hash_str_find(Z_ARRVAL_P(import), "sf", strlen("sf"))) != NULL) {
        significant_figures = Z_LVAL_P(value);
    } else {
        significant_figures = 2;
    }

    if (significant_figures <= 0 || significant_figures > 3) {
        php_error_docref(NULL, E_WARNING, "significant_figures (sf) must be 1, 2, or 3.");
        RETURN_FALSE;
    }

    if ((value = zend_hash_str_find(Z_ARRVAL_P(import), "sk", strlen("sk"))) != NULL) {
        skipped = Z_LVAL_P(value);
    } else {
        skipped = 0;
    }

    if (skipped < 0 || lowest_discernible_value < 1 || highest_trackable_value < lowest_discernible_value || significant_figures < 1) {
        php_error_docref(NULL, E_WARNING, "Invalid values for ltv, htv, sf or sk keys given.");
        RETURN_FALSE;
    }

    value = zend_hash_str_find(Z_ARRVAL_P(import), "v", strlen("v"));

    // version 3 format
    if (value != NULL && Z_TYPE_P(value) == IS_ARRAY) {
        count = zend_hash_num_elements(Z_ARRVAL_P(value));
        res = hdr_init(lowest_discernible_value, highest_trackable_value, significant_figures, &hdr);

        if (res == 0) {
            object_init_ex(return_value, php_HdrHistogram_Histogram_ce);
            struct php_hdrhistogram_histogram *h = php_hdrhistogram_histogram_from_object(Z_OBJ_P(return_value));
            h->histogram = hdr;
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

    value = zend_hash_str_find(Z_ARRVAL_P(import), "c", strlen("c"));

    // version 1 format
    if (value != NULL && Z_TYPE_P(value) == IS_ARRAY) {
        count = zend_hash_num_elements(Z_ARRVAL_P(value));

        res = hdr_init(lowest_discernible_value, highest_trackable_value, significant_figures, &hdr);

        if (res == 0) {
            object_init_ex(return_value, php_HdrHistogram_Histogram_ce);
            struct php_hdrhistogram_histogram *h = php_hdrhistogram_histogram_from_object(Z_OBJ_P(return_value));
            h->histogram = hdr;
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

    value = zend_hash_str_find(Z_ARRVAL_P(import), "b", strlen("b"));

    // version 2 format
    if (value != NULL && Z_TYPE_P(value) == IS_ARRAY) {
        res = hdr_init(lowest_discernible_value, highest_trackable_value, significant_figures, &hdr);

        if (res == 0) {
            object_init_ex(return_value, php_HdrHistogram_Histogram_ce);
            struct php_hdrhistogram_histogram *h = php_hdrhistogram_histogram_from_object(Z_OBJ_P(return_value));
            h->histogram = hdr;
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

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

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

    object_init_ex(return_value, php_HdrHistogram_Histogram_ce);
    struct php_hdrhistogram_histogram *h = php_hdrhistogram_histogram_from_object(Z_OBJ_P(return_value));
    h->histogram = hdr;
}
