#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "hdr/hdr_histogram.h"
#include "php_hdrhistogram.h"

#define PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME "hdr_histogram"

static int le_hdrhistogram_descriptor;

zend_function_entry hdrhistogram_functions[] = {
	PHP_FE(hdr_create, NULL)
	PHP_FE(hdr_get_memory_size, NULL)
	PHP_FE(hdr_record_value, NULL)
	PHP_FE(hdr_record_values, NULL)
	PHP_FE(hdr_mean, NULL)
	PHP_FE(hdr_min, NULL)
	PHP_FE(hdr_max, NULL)
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

PHP_FUNCTION(hdr_create)
{
	struct hdr_histogram *hdr;
	int lowest_trackable_value, highest_trackable_value, significant_figures, res;

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
	int value;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &zhdr, &value) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &zhdr, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

	hdr_record_value(hdr, value);
}

PHP_FUNCTION(hdr_record_values)
{
	struct hdr_histogram *hdr;
	zval *zhdr;
	int value;
	int count;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &zhdr, &value, &count) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE(hdr, struct hdr_histogram *, &zhdr, -1, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, le_hdrhistogram_descriptor);

	hdr_record_values(hdr, value, count);
}
