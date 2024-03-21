/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: ad32d9ddee3742f596783ac0beeab9929583ae12 */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_hdr_init, 0, 3, HdrHistogram\\Histogram, MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, lowest_trackable_value, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, highest_trackable_value, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, significant_figures, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hdr_get_memory_size, 0, 1, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, hdr, HdrHistogram\\Histogram, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hdr_record_value, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, hdr, HdrHistogram\\Histogram, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hdr_record_values, 0, 3, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, hdr, HdrHistogram\\Histogram, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, count, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hdr_record_corrected_value, 0, 3, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, hdr, HdrHistogram\\Histogram, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, expected_interval, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_hdr_mean arginfo_hdr_get_memory_size

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hdr_stddev, 0, 1, IS_DOUBLE, 0)
	ZEND_ARG_OBJ_INFO(0, hdr, HdrHistogram\\Histogram, 0)
ZEND_END_ARG_INFO()

#define arginfo_hdr_min arginfo_hdr_get_memory_size

#define arginfo_hdr_max arginfo_hdr_get_memory_size

#define arginfo_hdr_total_count arginfo_hdr_get_memory_size

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hdr_reset, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, hdr, HdrHistogram\\Histogram, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hdr_count_at_value, 0, 2, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, hdr, HdrHistogram\\Histogram, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hdr_value_at_percentile, 0, 2, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, hdr, HdrHistogram\\Histogram, 0)
	ZEND_ARG_TYPE_INFO(0, percentile, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_hdr_add, 0, 2, HdrHistogram\\Histogram, MAY_BE_FALSE)
	ZEND_ARG_OBJ_INFO(0, hdr1, HdrHistogram\\Histogram, 0)
	ZEND_ARG_OBJ_INFO(0, hdr2, HdrHistogram\\Histogram, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hdr_merge_into, 0, 2, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, hdr1, HdrHistogram\\Histogram, 0)
	ZEND_ARG_OBJ_INFO(0, hdr2, HdrHistogram\\Histogram, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_hdr_iter_init, 0, 1, HdrHistogram\\Iterator, MAY_BE_FALSE)
	ZEND_ARG_OBJ_INFO(0, hdr, HdrHistogram\\Histogram, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_hdr_iter_next, 0, 1, MAY_BE_FALSE|MAY_BE_ARRAY)
	ZEND_ARG_OBJ_INFO(0, hdr, HdrHistogram\\Iterator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_hdr_percentile_iter_init, 0, 2, HdrHistogram\\Iterator\\Percentile, MAY_BE_FALSE)
	ZEND_ARG_OBJ_INFO(0, hdr, HdrHistogram\\Histogram, 0)
	ZEND_ARG_TYPE_INFO(0, ticks_per_half_distance, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_hdr_percentile_iter_next, 0, 1, MAY_BE_FALSE|MAY_BE_ARRAY)
	ZEND_ARG_OBJ_INFO(0, hdr, HdrHistogram\\Iterator\\Percentile, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_hdr_export, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_OBJ_INFO(0, hdr, HdrHistogram\\Histogram, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_hdr_import, 0, 1, HdrHistogram\\Histogram, MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, import, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_hdr_base64_encode, 0, 1, MAY_BE_FALSE|MAY_BE_STRING)
	ZEND_ARG_OBJ_INFO(0, hdr, HdrHistogram\\Histogram, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_hdr_base64_decode, 0, 1, HdrHistogram\\Histogram, MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_HdrHistogram_Histogram___construct, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, lowestDiscernibleValue, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, highestTrackableValue, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, significantFigures, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_HdrHistogram_Iterator___construct, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, histogram, HdrHistogram\\Histogram, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_HdrHistogram_Iterator_Percentile___construct, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, histogram, HdrHistogram\\Histogram, 0)
	ZEND_ARG_TYPE_INFO(0, ticksPerHalfDistance, IS_LONG, 0)
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
ZEND_METHOD(HdrHistogram_Histogram, __construct);
ZEND_METHOD(HdrHistogram_Iterator, __construct);
ZEND_METHOD(HdrHistogram_Iterator_Percentile, __construct);


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


static const zend_function_entry class_HdrHistogram_Histogram_methods[] = {
	ZEND_ME(HdrHistogram_Histogram, __construct, arginfo_class_HdrHistogram_Histogram___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_HdrHistogram_Iterator_methods[] = {
	ZEND_ME(HdrHistogram_Iterator, __construct, arginfo_class_HdrHistogram_Iterator___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_HdrHistogram_Iterator_Percentile_methods[] = {
	ZEND_ME(HdrHistogram_Iterator_Percentile, __construct, arginfo_class_HdrHistogram_Iterator_Percentile___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_HdrHistogram_Histogram(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "HdrHistogram", "Histogram", class_HdrHistogram_Histogram_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
#if (PHP_VERSION_ID >= 80100)
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE;
#elif (PHP_VERSION_ID >= 80000)
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES;
#elif (PHP_VERSION_ID >= 70000)
	class_entry->ce_flags |= ZEND_ACC_FINAL;
#endif

	return class_entry;
}

static zend_class_entry *register_class_HdrHistogram_Iterator(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "HdrHistogram", "Iterator", class_HdrHistogram_Iterator_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
#if (PHP_VERSION_ID >= 80100)
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE;
#elif (PHP_VERSION_ID >= 80000)
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES;
#elif (PHP_VERSION_ID >= 70000)
	class_entry->ce_flags |= ZEND_ACC_FINAL;
#endif

	return class_entry;
}

static zend_class_entry *register_class_HdrHistogram_Iterator_Percentile(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "HdrHistogram\\Iterator", "Percentile", class_HdrHistogram_Iterator_Percentile_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
#if (PHP_VERSION_ID >= 80100)
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE;
#elif (PHP_VERSION_ID >= 80000)
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES;
#elif (PHP_VERSION_ID >= 70000)
	class_entry->ce_flags |= ZEND_ACC_FINAL;
#endif

	return class_entry;
}
