#ifndef PHP_HDRHISTOGRAM_H
#define PHP_HDRHISTOGRAM_H

extern zend_module_entry hdrhistogram_module_entry;
#define phpext_hdrhistogram_ptr &hdrhistogram_module_entry

PHP_MINIT_FUNCTION(hdrhistogram);
PHP_MSHUTDOWN_FUNCTION(hdrhistogram);
PHP_RINIT_FUNCTION(hdrhistogram);
PHP_RSHUTDOWN_FUNCTION(hdrhistogram);
PHP_MINFO_FUNCTION(hdrhistogram);

PHP_METHOD(HdrHistogram, __construct);
PHP_METHOD(HdrHistogram, recordValue);
PHP_METHOD(HdrHistogram, mean);
PHP_METHOD(HdrHistogram, min);
PHP_METHOD(HdrHistogram, max);

#endif
