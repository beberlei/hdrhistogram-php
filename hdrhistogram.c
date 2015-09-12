#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "hdr/hdr_histogram.h"
#include "hdr/hdr_histogram_log.h"
#include "php_hdrhistogram.h"

#define PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME "hdr_histogram"

static int le_hdrhistogram_descriptor;
static int le_hdrhistogram_iter_descriptor;

#if PHP_VERSION_ID >= 70000
#define strsize_t size_t
#else
#define strsize_t uint
#define zend_ulong ulong
#endif

static zend_always_inline void hdr_register_hdr_resource(zval *return_value, struct hdr_histogram* hdr TSRMLS_DC)
{
#if PHP_VERSION_ID >= 70000
	ZVAL_RES(return_value, zend_register_resource(hdr, le_hdrhistogram_descriptor));
#else
	ZEND_REGISTER_RESOURCE(return_value, hdr, le_hdrhistogram_descriptor);
#endif
}

static zend_always_inline void hdr_register_iter_resource(zval *return_value, struct hdr_iter* iter TSRMLS_DC)
{
#if PHP_VERSION_ID >= 70000
	ZVAL_RES(return_value, zend_register_resource(iter, le_hdrhistogram_iter_descriptor));
#else
	ZEND_REGISTER_RESOURCE(return_value, iter, le_hdrhistogram_iter_descriptor);
#endif
}

static zend_always_inline struct hdr_histogram* hdr_fetch_resource(zval *res, zval *return_value TSRMLS_DC)
{
	struct hdr_histogram *hdr;

#if PHP_VERSION_ID >= 70000
	return (struct hdr_histogram*)zend_fetch_resource(Z_RES_P(res), PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);
#else
	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &res, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);
	return hdr;
#endif
}

static zend_always_inline struct hdr_iter* hdr_fetch_iterator(zval *res, zval *return_value TSRMLS_DC)
{
	struct hdr_iter *iterator;

#if PHP_VERSION_ID >= 70000
	return (struct hdr_iter*)zend_fetch_resource(Z_RES_P(res), "hdr_iterator", le_hdrhistogram_iter_descriptor);
#else
	ZEND_FETCH_RESOURCE(iterator, struct hdr_iter *, &res, -1, "hdr_iterator", le_hdrhistogram_iter_descriptor);
	return iterator;
#endif
}

static zend_always_inline zval* hdr_hash_find(HashTable *arr, const char *name, strsize_t len)
{
#if PHP_VERSION_ID >= 70000
	return zend_hash_str_find(arr, name, len - 1);
#else
	zval **value, *result;

	if (zend_hash_find(arr, name, len, (void**)&value) == SUCCESS) {
		result = *value;
		return result;
	}

	return NULL;
#endif
}

static zend_always_inline zval* hdr_hash_index_find(HashTable *arr, zend_ulong h)
{
#if PHP_VERSION_ID >= 70000
	return zend_hash_index_find(arr, h);
#else
	zval **item;

	if (zend_hash_index_find(arr, h, (void**)&item) == SUCCESS) {
		return *item;
	}

	return NULL;
#endif
}

zend_function_entry hdrhistogram_functions[] = {
	PHP_FE(hdr_init, NULL)
	PHP_FE(hdr_get_memory_size, NULL)
	PHP_FE(hdr_record_value, NULL)
	PHP_FE(hdr_record_values, NULL)
	PHP_FE(hdr_record_corrected_value, NULL)
	PHP_FE(hdr_mean, NULL)
	PHP_FE(hdr_stddev, NULL)
	PHP_FE(hdr_min, NULL)
	PHP_FE(hdr_max, NULL)
	PHP_FE(hdr_reset, NULL)
	PHP_FE(hdr_count_at_value, NULL)
	PHP_FE(hdr_value_at_percentile, NULL)
	PHP_FE(hdr_add, NULL)
	PHP_FE(hdr_merge_into, NULL)
	//PHP_FE(hdr_log_encode, NULL)
	//PHP_FE(hdr_log_decode, NULL)
	PHP_FE(hdr_iter_init, NULL)
	PHP_FE(hdr_iter_next, NULL)
	PHP_FE(hdr_percentile_iter_init, NULL)
	PHP_FE(hdr_percentile_iter_next, NULL)
	PHP_FE(hdr_export, NULL)
	PHP_FE(hdr_import, NULL)
	{ NULL, NULL, NULL }
};

zend_module_entry hdrhistogram_module_entry = {
	STANDARD_MODULE_HEADER,
	"hdrhistogram",
	hdrhistogram_functions,                /* List of functions exposed */
	PHP_MINIT(hdrhistogram),               /* Module init callback */
	PHP_MSHUTDOWN(hdrhistogram),           /* Module shutdown callback */
	PHP_RINIT(hdrhistogram),               /* Request init callback */
	PHP_RSHUTDOWN(hdrhistogram),           /* Request shutdown callback */
	PHP_MINFO(hdrhistogram),               /* Module info callback */
	"0.1",
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HDRHISTOGRAM
ZEND_GET_MODULE(hdrhistogram)
#endif

#if PHP_VERSION_ID >= 70000
static void php_hdrhistogram_descriptor_dtor(zend_resource *rsrc TSRMLS_DC)
#else
static void php_hdrhistogram_descriptor_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
#endif
{
	free(rsrc->ptr);
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
		php_hdrhistogram_descriptor_dtor,
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
}

PHP_FUNCTION(hdr_init)
{
	struct hdr_histogram *hdr;
	long lowest_trackable_value, highest_trackable_value, significant_figures;
	int res;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll",
				&lowest_trackable_value, &highest_trackable_value, &significant_figures) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid arguments passed.");

		RETURN_FALSE;
	}

	res = hdr_init(lowest_trackable_value, highest_trackable_value, significant_figures, &hdr);

	if (res == 0) {
		hdr_register_hdr_resource(return_value, hdr TSRMLS_CC);
	} else if (res == EINVAL) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Lowest trackable value has to be >= 1.");

		RETURN_FALSE;
	} else if (res == ENOMEM) {
		perror("Memory error in hdr_init allocation.");
	}
}

PHP_FUNCTION(hdr_get_memory_size)
{
	struct hdr_histogram *hdr;
	zval *zhdr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	hdr = hdr_fetch_resource(zhdr, return_value TSRMLS_CC);

	RETURN_LONG(hdr_get_memory_size(hdr));
}

PHP_FUNCTION(hdr_mean)
{
	struct hdr_histogram *hdr;
	zval *zhdr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	hdr = hdr_fetch_resource(zhdr, return_value TSRMLS_CC);

	RETURN_LONG(hdr_mean(hdr));
}

PHP_FUNCTION(hdr_stddev)
{
	struct hdr_histogram *hdr;
	zval *zhdr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	hdr = hdr_fetch_resource(zhdr, return_value TSRMLS_CC);

	RETURN_DOUBLE(hdr_stddev(hdr));
}


PHP_FUNCTION(hdr_min)
{
	struct hdr_histogram *hdr;
	zval *zhdr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	hdr = hdr_fetch_resource(zhdr, return_value TSRMLS_CC);

	RETURN_LONG(hdr_min(hdr));
}

PHP_FUNCTION(hdr_max)
{
	struct hdr_histogram *hdr;
	zval *zhdr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	hdr = hdr_fetch_resource(zhdr, return_value TSRMLS_CC);

	RETURN_LONG(hdr_max(hdr));
}

PHP_FUNCTION(hdr_record_value)
{
	struct hdr_histogram *hdr;
	zval *zhdr;
	long value;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &zhdr, &value) == FAILURE) {
		RETURN_FALSE;
	}

	hdr = hdr_fetch_resource(zhdr, return_value TSRMLS_CC);

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

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &zhdr, &value, &count) == FAILURE) {
		RETURN_FALSE;
	}

	hdr = hdr_fetch_resource(zhdr, return_value TSRMLS_CC);

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

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &zhdr, &value, &expected_interval) == FAILURE) {
		RETURN_FALSE;
	}

	hdr = hdr_fetch_resource(zhdr, return_value TSRMLS_CC);

	hdr_record_corrected_value(hdr, value, expected_interval);
}

PHP_FUNCTION(hdr_reset)
{
	struct hdr_histogram *hdr;
	zval *zhdr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	hdr = hdr_fetch_resource(zhdr, return_value TSRMLS_CC);

	hdr_reset(hdr);
}

PHP_FUNCTION(hdr_count_at_value)
{
	struct hdr_histogram *hdr;
	zval *zhdr;
	long value;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &zhdr, &value) == FAILURE) {
		RETURN_FALSE;
	}

	hdr = hdr_fetch_resource(zhdr, return_value TSRMLS_CC);

	RETURN_LONG(hdr_count_at_value(hdr, value));
}

PHP_FUNCTION(hdr_value_at_percentile)
{
	struct hdr_histogram *hdr;
	zval *zhdr;
	double percentile;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rd", &zhdr, &percentile) == FAILURE) {
		RETURN_FALSE;
	}

	hdr = hdr_fetch_resource(zhdr, return_value TSRMLS_CC);

	RETURN_LONG(hdr_value_at_percentile(hdr, percentile));
}

PHP_FUNCTION(hdr_add)
{
	struct hdr_histogram *hdra, *hdrb, *hdr_new;
	zval *a, *b;
	int res;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rr", &a, &b) == FAILURE) {
		RETURN_FALSE;
	}

	hdra = hdr_fetch_resource(a, return_value TSRMLS_CC);
	hdrb = hdr_fetch_resource(b, return_value TSRMLS_CC);

	res = hdr_init(hdra->lowest_trackable_value, hdra->highest_trackable_value, hdra->significant_figures, &hdr_new);

	hdr_add(hdr_new, hdra);
	hdr_add(hdr_new, hdrb);

	if (res == 0) {
		hdr_register_hdr_resource(return_value, hdr_new TSRMLS_CC);
	} else if (res == EINVAL) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Lowest trackable value has to be >= 1.");

		RETURN_FALSE;
	} else if (res == ENOMEM) {
		perror("Memory error in hdr_init allocation.");
	}
}

PHP_FUNCTION(hdr_merge_into)
{
	struct hdr_histogram *hdra, *hdrb;
	zval *a, *b;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rr", &a, &b) == FAILURE) {
		RETURN_FALSE;
	}

	hdra = hdr_fetch_resource(a, return_value TSRMLS_CC);
	hdrb = hdr_fetch_resource(b, return_value TSRMLS_CC);

	RETURN_LONG(hdr_add(hdra, hdrb));
}

PHP_FUNCTION(hdr_iter_init)
{
	struct hdr_iter *iterator;
	struct hdr_histogram *hdr;
	zval *zhdr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	hdr = hdr_fetch_resource(zhdr, return_value TSRMLS_CC);

	iterator = malloc(sizeof(struct hdr_iter));
	hdr_iter_init(iterator, hdr);

	hdr_register_iter_resource(return_value, iterator TSRMLS_CC);
}

PHP_FUNCTION(hdr_percentile_iter_init)
{
	struct hdr_iter *iterator;
	struct hdr_histogram *hdr;
	zval *zhdr;
	long ticks_per_half_distance;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &zhdr, &ticks_per_half_distance) == FAILURE) {
		RETURN_FALSE;
	}

	hdr = hdr_fetch_resource(zhdr, return_value TSRMLS_CC);

	iterator = malloc(sizeof(struct hdr_iter));
	hdr_iter_percentile_init(iterator, hdr, ticks_per_half_distance);

	hdr_register_iter_resource(return_value, iterator TSRMLS_CC);
}

PHP_FUNCTION(hdr_iter_next)
{
	struct hdr_iter *iterator;
	zval *zhdr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	iterator = hdr_fetch_iterator(zhdr, return_value TSRMLS_CC);

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

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	iterator = hdr_fetch_iterator(zhdr, return_value TSRMLS_CC);

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
#if PHP_VERSION_ID >= 70000
	zval counts_v;
	zval *counts = &counts_v;
#else
	zval *counts;
#endif
	int32_t i;
	struct hdr_histogram *hdr;
	int found = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	hdr = hdr_fetch_resource(zhdr, return_value TSRMLS_CC);

	array_init(return_value);
	add_assoc_long(return_value, "ltv", hdr->lowest_trackable_value);
	add_assoc_long(return_value, "htv", hdr->highest_trackable_value);
	add_assoc_long(return_value, "sf", hdr->significant_figures);

#if PHP_VERSION_ID < 70000
	MAKE_STD_ZVAL(counts);
#endif
	array_init(counts);

	for (i = 0; i < hdr->counts_len; i++) {
		if (found >= hdr->total_count) {
			break;
		}

		add_index_double(counts, i, hdr->counts[i]);
		found += hdr->counts[i];
	}

	add_assoc_zval(return_value, "c", counts);
}

PHP_FUNCTION(hdr_import)
{
	struct hdr_histogram *hdr;
	zval *import, *value, *item;
	long lowest_trackable_value, highest_trackable_value, significant_figures;
	int res, count;
	zend_ulong i;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &import) == FAILURE) {
		RETURN_FALSE;
	}

	if (value = hdr_hash_find(Z_ARRVAL_P(import), "ltv", 4)) {
		lowest_trackable_value = Z_LVAL_P(value);
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Missing lowest_trackable_value (ltv) key.");
		RETURN_FALSE;
	}

	if (value = hdr_hash_find(Z_ARRVAL_P(import), "htv", 4)) {
		highest_trackable_value = Z_LVAL_P(value);
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Missing highest_trackable_value (htv) key.");
		RETURN_FALSE;
	}

	if (value = hdr_hash_find(Z_ARRVAL_P(import), "sf", 3)) {
		significant_figures = Z_LVAL_P(value);
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Missing significant_figures (sf) key.");
		RETURN_FALSE;
	}

	value = hdr_hash_find(Z_ARRVAL_P(import), "c", 2);

	if (value == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Missing counts (c) key.");
		RETURN_FALSE;
	}

	if (Z_TYPE_P(value) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Count is required to be an array.");
		RETURN_FALSE;
	}

	count = zend_hash_num_elements(Z_ARRVAL_P(value));

	res = hdr_init(lowest_trackable_value, highest_trackable_value, significant_figures, &hdr);

	if (res == 0) {
		hdr_register_hdr_resource(return_value, hdr TSRMLS_CC);
	} else if (res == EINVAL) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Lowest trackable value has to be >= 1.");

		RETURN_FALSE;
	} else if (res == ENOMEM) {
		perror("Memory error in hdr_init allocation.");
	}

	for (i = 0; i < count; i++) {
		if (item = hdr_hash_index_find(Z_ARRVAL_P(value), i)) {
			if (i < hdr->counts_len) {
#if PHP_VERSION_ID >= 70000
				convert_to_long_ex(item);
#else
				convert_to_long_ex(&item);
#endif
			  	hdr->counts[i] = Z_LVAL_P(item);
			}
		}
	}

	hdr_reset_internal_counters(hdr);
	hdr->normalizing_index_offset = 0;
	hdr->conversion_ratio = 1.0;
}
