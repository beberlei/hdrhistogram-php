#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"
#include "hdr/hdr_histogram.h"
#include "hdr/hdr_histogram_log.h"
#ifdef HAVE_HDR_HDR_HISTOGRAM_VERSION_H
#include "hdr/hdr_histogram_version.h"
#endif
#include "php_hdrhistogram.h"

#if PHP_VERSION_ID < 80000
#include "hdrhistogram_legacy_arginfo.h"
#else
#include "hdrhistogram_arginfo.h"
#endif

#define PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME "hdr_histogram"

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
#if PHP_VERSION_ID >= 70300
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

#if PHP_VERSION_ID < 80000
static zend_object *php_hdrhistogram_histogram_clone(zval *zval)
#else
static zend_object *php_hdrhistogram_histogram_clone(zend_object *object)
#endif
{
#if PHP_VERSION_ID < 80000
    zend_object *object = Z_OBJ_P(zval);
#endif
    struct php_hdrhistogram_histogram *old = php_hdrhistogram_histogram_from_object(object);
    struct php_hdrhistogram_histogram *new = php_hdrhistogram_histogram_from_object(old->std.ce->create_object(old->std.ce));

    struct hdr_histogram *hdr_new;
    int res;

#ifdef HAVE_HDR_HISTOGRAM_LOWEST_DISCERNIBLE_VALUE
    res = hdr_init(old->histogram->lowest_discernible_value, old->histogram->highest_trackable_value, old->histogram->significant_figures, &hdr_new);
#else
    res = hdr_init(old->histogram->lowest_trackable_value, old->histogram->highest_trackable_value, old->histogram->significant_figures, &hdr_new);
#endif

    if (res != 0) {
        zend_throw_error(zend_ce_error, "Unable to initialize HdrHistogram.");
        return NULL;
    }

    new->histogram = hdr_new;

    int64_t dropped = hdr_add(new->histogram, old->histogram);
    ZEND_ASSERT(dropped == 0);
    if (dropped != 0) {
        zend_throw_error(zend_ce_error, "Unable to transfer values from old histogram.");
        return NULL;
    }

    return &new->std;
}

static zend_class_entry *php_HdrHistogram_Iterator_ce;
static zend_object_handlers php_hdrhistogram_iterator_object_handlers;

struct php_hdrhistogram_iterator {
    struct hdr_iter *iterator;
    zend_object std;
};

static inline struct php_hdrhistogram_iterator *php_hdrhistogram_iterator_from_object(zend_object *object) {
    return (struct php_hdrhistogram_iterator *)((char *)(object) - XtOffsetOf(struct php_hdrhistogram_iterator, std));
}

static zend_object *php_hdrhistogram_iterator_new(zend_class_entry *ce)
{
    struct php_hdrhistogram_iterator *iterator;
#if PHP_VERSION_ID >= 70300
    iterator = zend_object_alloc(sizeof(struct php_hdrhistogram_iterator), ce);
#else
    iterator = ecalloc(1, sizeof(struct php_hdrhistogram_iterator) + sizeof(zval) * (ce->default_properties_count - 1));
#endif

    zend_object_std_init(&iterator->std, ce);
    object_properties_init(&iterator->std, ce);
    iterator->std.handlers = &php_hdrhistogram_iterator_object_handlers;

    iterator->iterator = NULL;

    return &iterator->std;
}

static void php_hdrhistogram_iterator_free(zend_object *object)
{
    struct php_hdrhistogram_iterator *iterator = php_hdrhistogram_iterator_from_object(object);

    efree(iterator->iterator);

    zend_object_std_dtor(&iterator->std);
}

static zend_class_entry *php_HdrHistogram_Iterator_Percentile_ce;

zend_module_entry hdrhistogram_module_entry = {
    STANDARD_MODULE_HEADER,
    "hdrhistogram",
    ext_functions,                         /* List of functions exposed */
    PHP_MINIT(hdrhistogram),               /* Module init callback */
    NULL,                                  /* Module shutdown callback */
    NULL,                                  /* Request init callback */
    NULL,                                  /* Request shutdown callback */
    PHP_MINFO(hdrhistogram),               /* Module info callback */
    PHP_HDR_HISTOGRAM_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HDRHISTOGRAM
ZEND_GET_MODULE(hdrhistogram)
#endif

PHP_MINIT_FUNCTION(hdrhistogram)
{
    php_HdrHistogram_Histogram_ce = register_class_HdrHistogram_Histogram();
    php_HdrHistogram_Histogram_ce->create_object = php_hdrhistogram_histogram_new;
    memcpy(&php_hdrhistogram_histogram_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    php_hdrhistogram_histogram_object_handlers.offset = XtOffsetOf(struct php_hdrhistogram_histogram, std);
    php_hdrhistogram_histogram_object_handlers.free_obj = php_hdrhistogram_histogram_free;
    php_hdrhistogram_histogram_object_handlers.clone_obj = php_hdrhistogram_histogram_clone;
#if PHP_VERSION_ID < 80100
    php_HdrHistogram_Histogram_ce->serialize    = zend_class_serialize_deny;
    php_HdrHistogram_Histogram_ce->unserialize  = zend_class_unserialize_deny;
#endif

    php_HdrHistogram_Iterator_ce = register_class_HdrHistogram_Iterator();
    php_HdrHistogram_Iterator_ce->create_object = php_hdrhistogram_iterator_new;
    memcpy(&php_hdrhistogram_iterator_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    php_hdrhistogram_iterator_object_handlers.offset = XtOffsetOf(struct php_hdrhistogram_iterator, std);
    php_hdrhistogram_iterator_object_handlers.free_obj = php_hdrhistogram_iterator_free;
    php_hdrhistogram_iterator_object_handlers.clone_obj = NULL;
#if PHP_VERSION_ID < 80100
    php_HdrHistogram_Iterator_ce->serialize    = zend_class_serialize_deny;
    php_HdrHistogram_Iterator_ce->unserialize  = zend_class_unserialize_deny;
#endif

    php_HdrHistogram_Iterator_Percentile_ce = register_class_HdrHistogram_Iterator_Percentile();
    php_HdrHistogram_Iterator_Percentile_ce->create_object = php_hdrhistogram_iterator_new;
#if PHP_VERSION_ID < 80100
    php_HdrHistogram_Iterator_Percentile_ce->serialize    = zend_class_serialize_deny;
    php_HdrHistogram_Iterator_Percentile_ce->unserialize  = zend_class_unserialize_deny;
#endif

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
        zend_throw_exception_ex(NULL, 0, "%s(): Argument #%d ($%s) must be greater than or equal to 1", "HdrHistogram\\Histogram::__construct", 1, "lowestDiscernibleValue");
#endif
        return;
    }

    if (highest_trackable_value < lowest_discernible_value * 2) {
#if PHP_VERSION_ID >= 80000
        zend_argument_value_error(2, "must be greater than or equal to twice the value of argument #%d ($%s)", 1, "lowestDiscernibleValue");
#else
        zend_throw_exception_ex(NULL, 0, "%s(): Argument #%d ($%s) must be greater than or equal to twice the value of argument #%d ($%s)", "HdrHistogram\\Histogram::__construct", 2, "highestTrackableValue", 1, "lowestDiscernibleValue");
#endif
        return;
    }

    if (significant_figures < 1 || significant_figures > 5) {
#if PHP_VERSION_ID >= 80000
        zend_argument_value_error(3, "must be between 1 and 5");
#else
        zend_throw_exception_ex(NULL, 0, "%s(): Argument #%d ($%s) must be between 1 and 5", "HdrHistogram\\Histogram::__construct", 3, "significantFigures");
#endif
        return;
    }

    struct php_hdrhistogram_histogram *h = php_hdrhistogram_histogram_from_object(Z_OBJ_P(getThis()));

    struct hdr_histogram *hdr;
    int res = hdr_init(lowest_discernible_value, highest_trackable_value, significant_figures, &hdr);

    if (res != 0) {
        zend_throw_error(zend_ce_error, "Unable to initialize HdrHistogram.");
        return;
    }

    h->histogram = hdr;
}

ZEND_METHOD(HdrHistogram_Iterator, __construct)
{
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    struct hdr_iter *iterator = emalloc(sizeof(*iterator));
    hdr_iter_init(iterator, hdr);

    object_init_ex(return_value, php_HdrHistogram_Iterator_ce);
    struct php_hdrhistogram_iterator *i = php_hdrhistogram_iterator_from_object(Z_OBJ_P(getThis()));
    i->iterator = iterator;
}

ZEND_METHOD(HdrHistogram_Iterator_Percentile, __construct)
{
    zval *zhdr;
    zend_long ticks_per_half_distance;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
        Z_PARAM_LONG(ticks_per_half_distance);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    struct hdr_iter *iterator = emalloc(sizeof(*iterator));
    hdr_iter_percentile_init(iterator, hdr, ticks_per_half_distance);

    object_init_ex(return_value, php_HdrHistogram_Iterator_ce);
    struct php_hdrhistogram_iterator *i = php_hdrhistogram_iterator_from_object(Z_OBJ_P(getThis()));
    i->iterator = iterator;
}

PHP_FUNCTION(hdr_init)
{
    zend_long lowest_discernible_value, highest_trackable_value, significant_figures;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "lll",
                &lowest_discernible_value, &highest_trackable_value, &significant_figures) == FAILURE) {
        php_error_docref(NULL, E_WARNING, "Invalid arguments passed.");

        RETURN_FALSE;
    }

    struct hdr_histogram *hdr;
    int res = hdr_init(lowest_discernible_value, highest_trackable_value, significant_figures, &hdr);

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
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_LONG(hdr_get_memory_size(hdr));
}

PHP_FUNCTION(hdr_mean)
{
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_DOUBLE(hdr_mean(hdr));
}

PHP_FUNCTION(hdr_stddev)
{
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_DOUBLE(hdr_stddev(hdr));
}


PHP_FUNCTION(hdr_min)
{
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_LONG(hdr_min(hdr));
}

PHP_FUNCTION(hdr_max)
{
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_LONG(hdr_max(hdr));
}

PHP_FUNCTION(hdr_total_count)
{
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_LONG(hdr->total_count);
}

PHP_FUNCTION(hdr_record_value)
{
    zval *zhdr;
    zend_long value;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
        Z_PARAM_LONG(value);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    if (hdr_record_value(hdr, value) == 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}

PHP_FUNCTION(hdr_record_values)
{
    zval *zhdr;
    zend_long value;
    zend_long count;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
        Z_PARAM_LONG(value);
        Z_PARAM_LONG(count);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    if (hdr_record_values(hdr, value, count) == 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}

PHP_FUNCTION(hdr_record_corrected_value)
{
    zval *zhdr;
    zend_long value;
    zend_long expected_interval;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
        Z_PARAM_LONG(value);
        Z_PARAM_LONG(expected_interval);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    if (hdr_record_corrected_value(hdr, value, expected_interval)) {
        RETURN_TRUE;
    }

    RETURN_FALSE;
}

PHP_FUNCTION(hdr_reset)
{
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    hdr_reset(hdr);
}

PHP_FUNCTION(hdr_count_at_value)
{
    zval *zhdr;
    zend_long value;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
        Z_PARAM_LONG(value);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_LONG(hdr_count_at_value(hdr, value));
}

PHP_FUNCTION(hdr_value_at_percentile)
{
    zval *zhdr;
    double percentile;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
        Z_PARAM_DOUBLE(percentile);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    RETURN_LONG(hdr_value_at_percentile(hdr, percentile));
}

PHP_FUNCTION(hdr_add)
{
    zval *a, *b;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(a, php_HdrHistogram_Histogram_ce);
        Z_PARAM_OBJECT_OF_CLASS(b, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdra = php_hdrhistogram_histogram_from_object(Z_OBJ_P(a))->histogram;
    struct hdr_histogram *hdrb = php_hdrhistogram_histogram_from_object(Z_OBJ_P(b))->histogram;

    struct hdr_histogram *hdr_new;

    int res;
#ifdef HAVE_HDR_HISTOGRAM_LOWEST_DISCERNIBLE_VALUE
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
    zval *a, *b;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(a, php_HdrHistogram_Histogram_ce);
        Z_PARAM_OBJECT_OF_CLASS(b, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdra = php_hdrhistogram_histogram_from_object(Z_OBJ_P(a))->histogram;
    struct hdr_histogram *hdrb = php_hdrhistogram_histogram_from_object(Z_OBJ_P(b))->histogram;

    RETURN_LONG(hdr_add(hdra, hdrb));
}

PHP_FUNCTION(hdr_iter_init)
{
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    struct hdr_iter *iterator = emalloc(sizeof(*iterator));
    hdr_iter_init(iterator, hdr);

    object_init_ex(return_value, php_HdrHistogram_Iterator_ce);
    struct php_hdrhistogram_iterator *i = php_hdrhistogram_iterator_from_object(Z_OBJ_P(return_value));
    i->iterator = iterator;
}

PHP_FUNCTION(hdr_percentile_iter_init)
{
    zval *zhdr;
    zend_long ticks_per_half_distance;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
        Z_PARAM_LONG(ticks_per_half_distance);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    struct hdr_iter *iterator = emalloc(sizeof(*iterator));
    hdr_iter_percentile_init(iterator, hdr, ticks_per_half_distance);

    object_init_ex(return_value, php_HdrHistogram_Iterator_Percentile_ce);
    struct php_hdrhistogram_iterator *i = php_hdrhistogram_iterator_from_object(Z_OBJ_P(return_value));
    i->iterator = iterator;
}

PHP_FUNCTION(hdr_iter_next)
{
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Iterator_ce);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_iter *iterator = php_hdrhistogram_iterator_from_object(Z_OBJ_P(zhdr))->iterator;

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
    zval *zhdr;

   ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Iterator_Percentile_ce);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_iter *iterator = php_hdrhistogram_iterator_from_object(Z_OBJ_P(zhdr))->iterator;

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

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    array_init(return_value);

#ifdef HAVE_HDR_HISTOGRAM_LOWEST_DISCERNIBLE_VALUE
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

    zval values;
    array_init(&values);

    int64_t found = 0;
    zend_long skipped = 0;
    for (int32_t i = 0; i < hdr->counts_len; i++) {
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
            add_next_index_long(&values, (zend_long)hdr->counts[i]);
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
    zval *zhdr;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(zhdr, php_HdrHistogram_Histogram_ce);
    ZEND_PARSE_PARAMETERS_END();

    struct hdr_histogram *hdr = php_hdrhistogram_histogram_from_object(Z_OBJ_P(zhdr))->histogram;

    char *result = NULL;

    if (hdr_log_encode(hdr, &result) != 0) {
        php_error_docref(NULL, E_WARNING, "Cannot encode histogram");

        RETURN_FALSE;
    }

    RETVAL_STRING(result);
    free(result);
}

PHP_FUNCTION(hdr_base64_decode)
{
    zend_string *data;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &data) == FAILURE) {
        RETURN_FALSE;
    }

    struct hdr_histogram *hdr = NULL;

    if (hdr_log_decode(&hdr, ZSTR_VAL(data), ZSTR_LEN(data)) != 0) {
        php_error_docref(NULL, E_WARNING, "Cannot decode histogram");

        RETURN_FALSE;
    }

    object_init_ex(return_value, php_HdrHistogram_Histogram_ce);
    struct php_hdrhistogram_histogram *h = php_hdrhistogram_histogram_from_object(Z_OBJ_P(return_value));
    h->histogram = hdr;
}
