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
static int le_hdrhistogram_percentile_iter_descriptor;

zend_function_entry hdrhistogram_functions[] = {
	PHP_FE(hdr_init, NULL)
	PHP_FE(hdr_get_memory_size, NULL)
	PHP_FE(hdr_record_value, NULL)
	PHP_FE(hdr_record_values, NULL)
	PHP_FE(hdr_record_corrected_value, NULL)
	PHP_FE(hdr_mean, NULL)
	PHP_FE(hdr_min, NULL)
	PHP_FE(hdr_max, NULL)
	PHP_FE(hdr_reset, NULL)
	PHP_FE(hdr_count_at_value, NULL)
	PHP_FE(hdr_value_at_percentile, NULL)
	PHP_FE(hdr_add, NULL)
	PHP_FE(hdr_merge_into, NULL)
	PHP_FE(hdr_log_encode, NULL)
	PHP_FE(hdr_log_decode, NULL)
	PHP_FE(hdr_iter_init, NULL)
	PHP_FE(hdr_iter_next, NULL)
	PHP_FE(hdr_percentile_iter_init, NULL)
	PHP_FE(hdr_percentile_iter_next, NULL)
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

static void php_hdrhistogram_descriptor_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
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

	le_hdrhistogram_percentile_iter_descriptor = zend_register_list_destructors_ex(
		php_hdrhistogram_descriptor_dtor,
		NULL,
		"hdr_percentile_iterator",
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
		ZEND_REGISTER_RESOURCE(return_value, hdr, le_hdrhistogram_descriptor);
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

	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &zhdr, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

	RETURN_LONG(hdr_get_memory_size(hdr));
}

PHP_FUNCTION(hdr_mean)
{
	struct hdr_histogram *hdr;
	zval *zhdr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &zhdr, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

	RETURN_LONG(hdr_mean(hdr));
}

PHP_FUNCTION(hdr_min)
{
	struct hdr_histogram *hdr;
	zval *zhdr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &zhdr, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

	RETURN_LONG(hdr_min(hdr));
}

PHP_FUNCTION(hdr_max)
{
	struct hdr_histogram *hdr;
	zval *zhdr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &zhdr, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

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

	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &zhdr, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

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

	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &zhdr, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

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

	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &zhdr, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

	hdr_record_corrected_value(hdr, value, expected_interval);
}

PHP_FUNCTION(hdr_reset)
{
	struct hdr_histogram *hdr;
	zval *zhdr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &zhdr, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

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

	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &zhdr, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

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

	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &zhdr, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

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

	ZEND_FETCH_RESOURCE(hdra, struct hdr_histogram *, &a, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);
	ZEND_FETCH_RESOURCE(hdrb, struct hdr_histogram *, &b, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

	res = hdr_init(hdra->lowest_trackable_value, hdra->highest_trackable_value, hdra->significant_figures, &hdr_new);

	hdr_add(hdr_new, hdra);
	hdr_add(hdr_new, hdrb);

	if (res == 0) {
		ZEND_REGISTER_RESOURCE(return_value, hdr_new, le_hdrhistogram_descriptor);
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

	ZEND_FETCH_RESOURCE(hdra, struct hdr_histogram *, &a, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);
	ZEND_FETCH_RESOURCE(hdrb, struct hdr_histogram *, &b, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

	RETURN_LONG(hdr_add(hdra, hdrb));
}

PHP_FUNCTION(hdr_log_encode)
{
	struct hdr_histogram *hdr;
	zval *zhdr;
	char *data;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &zhdr, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

	hdr_log_encode(hdr, &data);

	RETURN_STRING(data, 1);
}

PHP_FUNCTION(hdr_log_decode)
{
	struct hdr_histogram *hdr;
	char *data;
	int len, result;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &len) == FAILURE) {
		RETURN_FALSE;
	}

	result = hdr_log_decode(&hdr, data, len);

	if (result == 0) {
		ZEND_REGISTER_RESOURCE(return_value, hdr, le_hdrhistogram_descriptor);
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Could not decode binary hdrhistogram.");
		RETURN_FALSE;
	}
}

PHP_FUNCTION(hdr_iter_init)
{
	struct hdr_iter *iterator;
	struct hdr_histogram *hdr;
	zval *zhdr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &zhdr, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

	iterator = malloc(sizeof(struct hdr_iter));
	hdr_iter_init(iterator, hdr);

	ZEND_REGISTER_RESOURCE(return_value, iterator, le_hdrhistogram_iter_descriptor);
}

PHP_FUNCTION(hdr_iter_next)
{
	struct hdr_iter *iterator;
	zval *zhdr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE(iterator, struct hdr_iter *, &zhdr, -1, "hdr_iterator", le_hdrhistogram_iter_descriptor);

	if (hdr_iter_next(iterator) == 1) {
		array_init(return_value);
		add_assoc_long(return_value, "value", iterator->value_from_index);
		add_assoc_long(return_value, "count_at_index", iterator->count_at_index);
		add_assoc_long(return_value, "count_to_index", iterator->count_to_index);
		add_assoc_long(return_value, "highest_equivalent_value", iterator->highest_equivalent_value);
	} else {
		RETURN_FALSE;
	}
}

PHP_FUNCTION(hdr_percentile_iter_init)
{
	struct hdr_percentile_iter *iterator;
	struct hdr_histogram *hdr;
	zval *zhdr;
	long ticks_per_half_distance;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &zhdr, &ticks_per_half_distance) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &zhdr, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

	iterator = malloc(sizeof(struct hdr_percentile_iter));
	hdr_percentile_iter_init(iterator, hdr, ticks_per_half_distance);

	ZEND_REGISTER_RESOURCE(return_value, iterator, le_hdrhistogram_percentile_iter_descriptor);
}

PHP_FUNCTION(hdr_percentile_iter_next)
{
	struct hdr_percentile_iter *iterator;
	zval *zhdr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zhdr) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE(iterator, struct hdr_percentile_iter *, &zhdr, -1, "hdr_percentile_iterator", le_hdrhistogram_percentile_iter_descriptor);

	if (hdr_percentile_iter_next(iterator) == 1) {
		array_init(return_value);
		add_assoc_long(return_value, "value", iterator->iter.value_from_index);
		add_assoc_long(return_value, "count_at_index", iterator->iter.count_at_index);
		add_assoc_long(return_value, "count_to_index", iterator->iter.count_to_index);
		add_assoc_long(return_value, "highest_equivalent_value", iterator->iter.highest_equivalent_value);
		add_assoc_long(return_value, "seen_last_value", iterator->seen_last_value);
		add_assoc_long(return_value, "ticks_per_half_distance", iterator->ticks_per_half_distance);
		add_assoc_double(return_value, "percentile_to_iterate_to", iterator->percentile_to_iterate_to);
		add_assoc_double(return_value, "percentile", iterator->percentile);
	} else {
		RETURN_FALSE;
	}
}
