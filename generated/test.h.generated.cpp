#pragma once
#include <reflection_common.h>
#include <test.h>

refl_data g_ReflectionData_some_struct[] = {
	{Refl_int, "int", "int_field", (size_t)&((some_struct*)0)->int_field, 0},
	{Refl_uint32_t, "uint32_t", "uint_field", (size_t)&((some_struct*)0)->uint_field, 0},
	{Refl_char, "char", "somechar", (size_t)&((some_struct*)0)->somechar, 0},
	{Refl_charStar, "char", "someString", (size_t)&((some_struct*)0)->someString, 1},
	{Refl_charStar, "char", "someOtherString", (size_t)&((some_struct*)0)->someOtherString, 1}
};

refl_data g_ReflectionData_some_other_struct[] = {
	{Refl_int, "int", "test", (size_t)&((some_other_struct*)0)->test, 0}
};

refl_data g_ReflectionData_token[] = {
	{Refl_token_type, "token_type", "type", (size_t)&((token*)0)->type, 0},
	{Refl_size_t, "size_t", "text_length", (size_t)&((token*)0)->text_length, 0},
	{Refl_charStar, "char", "text", (size_t)&((token*)0)->text, 1},
	{Refl_int, "int", "value", (size_t)&((token*)0)->value, 0},
	{Refl_double, "double", "floatval", (size_t)&((token*)0)->floatval, 0},
	{Refl_int64_t, "int64_t", "intval", (size_t)&((token*)0)->intval, 0},
	{Refl_uint64_t, "uint64_t", "uintval", (size_t)&((token*)0)->uintval, 0}
};

refl_data g_ReflectionData_test_namespacedStruct[] = {
	{Refl_int, "int", "value", (size_t)&((test::namespacedStruct*)0)->value, 0}
};

refl_data g_ReflectionData_another[] = {
	{Refl_test_namespacedStruct, "test::namespacedStruct", "nss", (size_t)&((another*)0)->nss, 0}
};

