#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_hdrhistogram.h"
#include "hdr/hdr_histogram.h"

#define PHP_HDRHISTOGRAM_HISTOGRAM_NAME "HdrHistogram"
#define PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME "HdrHistogram Resource Descriptor"

static int le_hdrhistogram_descriptor;

zend_function_entry hdrhistogram_functions[] = {
	{ NULL, NULL, NULL }
};

zend_class_entry *php_hdrhistogram_histogram_entry;
static zend_function_entry php_hdrhistogram_histogram_functions[] = {
	PHP_ME(HdrHistogram, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(HdrHistogram, recordValue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(HdrHistogram, mean, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(HdrHistogram, min, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(HdrHistogram, max, NULL, ZEND_ACC_PUBLIC)
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
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, PHP_HDRHISTOGRAM_HISTOGRAM_NAME, php_hdrhistogram_histogram_functions);
	php_hdrhistogram_histogram_entry = zend_register_internal_class(&ce TSRMLS_CC);

	le_hdrhistogram_descriptor = zend_register_list_destructors_ex(
		NULL, NULL, PHP_HDRHISTOGRAM_DESCRIPTOR_RES_NAME, module_number
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

PHP_METHOD(HdrHistogram, __construct)
{
	static struct hdr_histogram *hdr;

	hdr_init(1, 1000, 5, &hdr);
}

PHP_METHOD(HdrHistogram, recordValue)
{
}

PHP_METHOD(HdrHistogram, mean)
{
}

PHP_METHOD(HdrHistogram, min)
{
}

PHP_METHOD(HdrHistogram, max)
{
}
