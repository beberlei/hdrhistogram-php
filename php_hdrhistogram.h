#ifndef PHP_HDRHISTOGRAM_H
#define PHP_HDRHISTOGRAM_H

#include "hdr/hdr_histogram.h"

extern zend_module_entry hdrhistogram_module_entry;
#define phpext_hdrhistogram_ptr &hdrhistogram_module_entry
#define PHP_HDR_HISTOGRAM_VERSION "0.3.0"

PHP_MINIT_FUNCTION(hdrhistogram);
PHP_MSHUTDOWN_FUNCTION(hdrhistogram);
PHP_RINIT_FUNCTION(hdrhistogram);
PHP_RSHUTDOWN_FUNCTION(hdrhistogram);
PHP_MINFO_FUNCTION(hdrhistogram);

PHP_FUNCTION(hdr_init);
PHP_FUNCTION(hdr_get_memory_size);
PHP_FUNCTION(hdr_mean);
PHP_FUNCTION(hdr_stddev);
PHP_FUNCTION(hdr_min);
PHP_FUNCTION(hdr_max);
PHP_FUNCTION(hdr_total_count);
PHP_FUNCTION(hdr_record_value);
PHP_FUNCTION(hdr_record_values);
PHP_FUNCTION(hdr_record_corrected_value);
PHP_FUNCTION(hdr_reset);
PHP_FUNCTION(hdr_count_at_value);
PHP_FUNCTION(hdr_value_at_percentile);
PHP_FUNCTION(hdr_add);
PHP_FUNCTION(hdr_merge_into);

PHP_FUNCTION(hdr_iter_init);
PHP_FUNCTION(hdr_iter_next);

PHP_FUNCTION(hdr_percentile_iter_init);
PHP_FUNCTION(hdr_percentile_iter_next);

PHP_FUNCTION(hdr_export);
PHP_FUNCTION(hdr_import);

PHP_FUNCTION(hdr_base64_encode);
PHP_FUNCTION(hdr_base64_decode);
#endif
