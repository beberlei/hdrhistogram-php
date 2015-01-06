#ifndef PHP_HDRHISTOGRAM_H
#define PHP_HDRHISTOGRAM_H

#include "hdr/hdr_histogram.h"

extern zend_module_entry hdrhistogram_module_entry;
#define phpext_hdrhistogram_ptr &hdrhistogram_module_entry

PHP_MINIT_FUNCTION(hdrhistogram);
PHP_MSHUTDOWN_FUNCTION(hdrhistogram);
PHP_RINIT_FUNCTION(hdrhistogram);
PHP_RSHUTDOWN_FUNCTION(hdrhistogram);
PHP_MINFO_FUNCTION(hdrhistogram);

PHP_FUNCTION(hdr_init);
PHP_FUNCTION(hdr_get_memory_size);
PHP_FUNCTION(hdr_mean);
PHP_FUNCTION(hdr_min);
PHP_FUNCTION(hdr_max);
PHP_FUNCTION(hdr_record_value);
PHP_FUNCTION(hdr_record_values);
PHP_FUNCTION(hdr_record_corrected_value);
PHP_FUNCTION(hdr_reset);
PHP_FUNCTION(hdr_count_at_value);
PHP_FUNCTION(hdr_value_at_percentile);
PHP_FUNCTION(hdr_add);

#endif
