/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 195b1ceef170a9dd44acc536ff6ae157b79648be */

ZEND_BEGIN_ARG_INFO_EX(arginfo_hdr_init, 0, 0, 3)
	ZEND_ARG_INFO(0, lowest_trackable_value)
	ZEND_ARG_INFO(0, highest_trackable_value)
	ZEND_ARG_INFO(0, significant_figures)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_hdr_get_memory_size, 0, 0, 1)
	ZEND_ARG_INFO(0, hdr)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_hdr_record_value, 0, 0, 2)
	ZEND_ARG_INFO(0, hdr)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_hdr_record_values, 0, 0, 3)
	ZEND_ARG_INFO(0, hdr)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, count)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_hdr_record_corrected_value, 0, 0, 3)
	ZEND_ARG_INFO(0, hdr)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, expected_interval)
ZEND_END_ARG_INFO()

#define arginfo_hdr_mean arginfo_hdr_get_memory_size

#define arginfo_hdr_stddev arginfo_hdr_get_memory_size

#define arginfo_hdr_min arginfo_hdr_get_memory_size

#define arginfo_hdr_max arginfo_hdr_get_memory_size

#define arginfo_hdr_total_count arginfo_hdr_get_memory_size

#define arginfo_hdr_reset arginfo_hdr_get_memory_size

#define arginfo_hdr_count_at_value arginfo_hdr_record_value

ZEND_BEGIN_ARG_INFO_EX(arginfo_hdr_value_at_percentile, 0, 0, 2)
	ZEND_ARG_INFO(0, hdr)
	ZEND_ARG_INFO(0, percentile)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_hdr_add, 0, 0, 2)
	ZEND_ARG_INFO(0, hdr1)
	ZEND_ARG_INFO(0, hdr2)
ZEND_END_ARG_INFO()

#define arginfo_hdr_merge_into arginfo_hdr_add

#define arginfo_hdr_iter_init arginfo_hdr_get_memory_size

#define arginfo_hdr_iter_next arginfo_hdr_get_memory_size

ZEND_BEGIN_ARG_INFO_EX(arginfo_hdr_percentile_iter_init, 0, 0, 2)
	ZEND_ARG_INFO(0, hdr)
	ZEND_ARG_INFO(0, ticks_per_half_distance)
ZEND_END_ARG_INFO()

#define arginfo_hdr_percentile_iter_next arginfo_hdr_get_memory_size

#define arginfo_hdr_export arginfo_hdr_get_memory_size

ZEND_BEGIN_ARG_INFO_EX(arginfo_hdr_import, 0, 0, 1)
	ZEND_ARG_INFO(0, import)
ZEND_END_ARG_INFO()

#define arginfo_hdr_base64_encode arginfo_hdr_get_memory_size

ZEND_BEGIN_ARG_INFO_EX(arginfo_hdr_base64_decode, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()


ZEND_FUNCTION(hdr_init);
ZEND_FUNCTION(hdr_get_memory_size);
ZEND_FUNCTION(hdr_record_value);
ZEND_FUNCTION(hdr_record_values);
ZEND_FUNCTION(hdr_record_corrected_value);
ZEND_FUNCTION(hdr_mean);
ZEND_FUNCTION(hdr_stddev);
ZEND_FUNCTION(hdr_min);
ZEND_FUNCTION(hdr_max);
ZEND_FUNCTION(hdr_total_count);
ZEND_FUNCTION(hdr_reset);
ZEND_FUNCTION(hdr_count_at_value);
ZEND_FUNCTION(hdr_value_at_percentile);
ZEND_FUNCTION(hdr_add);
ZEND_FUNCTION(hdr_merge_into);
ZEND_FUNCTION(hdr_iter_init);
ZEND_FUNCTION(hdr_iter_next);
ZEND_FUNCTION(hdr_percentile_iter_init);
ZEND_FUNCTION(hdr_percentile_iter_next);
ZEND_FUNCTION(hdr_export);
ZEND_FUNCTION(hdr_import);
ZEND_FUNCTION(hdr_base64_encode);
ZEND_FUNCTION(hdr_base64_decode);


static const zend_function_entry ext_functions[] = {
	ZEND_FE(hdr_init, arginfo_hdr_init)
	ZEND_FE(hdr_get_memory_size, arginfo_hdr_get_memory_size)
	ZEND_FE(hdr_record_value, arginfo_hdr_record_value)
	ZEND_FE(hdr_record_values, arginfo_hdr_record_values)
	ZEND_FE(hdr_record_corrected_value, arginfo_hdr_record_corrected_value)
	ZEND_FE(hdr_mean, arginfo_hdr_mean)
	ZEND_FE(hdr_stddev, arginfo_hdr_stddev)
	ZEND_FE(hdr_min, arginfo_hdr_min)
	ZEND_FE(hdr_max, arginfo_hdr_max)
	ZEND_FE(hdr_total_count, arginfo_hdr_total_count)
	ZEND_FE(hdr_reset, arginfo_hdr_reset)
	ZEND_FE(hdr_count_at_value, arginfo_hdr_count_at_value)
	ZEND_FE(hdr_value_at_percentile, arginfo_hdr_value_at_percentile)
	ZEND_FE(hdr_add, arginfo_hdr_add)
	ZEND_FE(hdr_merge_into, arginfo_hdr_merge_into)
	ZEND_FE(hdr_iter_init, arginfo_hdr_iter_init)
	ZEND_FE(hdr_iter_next, arginfo_hdr_iter_next)
	ZEND_FE(hdr_percentile_iter_init, arginfo_hdr_percentile_iter_init)
	ZEND_FE(hdr_percentile_iter_next, arginfo_hdr_percentile_iter_next)
	ZEND_FE(hdr_export, arginfo_hdr_export)
	ZEND_FE(hdr_import, arginfo_hdr_import)
	ZEND_FE(hdr_base64_encode, arginfo_hdr_base64_encode)
	ZEND_FE(hdr_base64_decode, arginfo_hdr_base64_decode)
	ZEND_FE_END
};
