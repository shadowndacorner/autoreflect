#pragma once
#include <reflection_common.h>
#include <test2.h>

refl_data g_ReflectionData_reflected_struct[] = {
	{Refl_int, "int", "intfield", (size_t)&((reflected_struct*)0)->intfield, 0, 0, {0, 0}},
	{Refl_char, "char", "another_field", (size_t)&((reflected_struct*)0)->another_field, 0, 0, {0, 0}},
	{Refl_charStar, "char", "someStringPtr", (size_t)&((reflected_struct*)0)->someStringPtr, 1, 0, {0, 0}},
	{Refl_charStarStar, "char", "someStringPtrPtr", (size_t)&((reflected_struct*)0)->someStringPtrPtr, 2, 0, {0, 0}},
	{Refl_char, "char", "someChar", (size_t)&((reflected_struct*)0)->someChar, 0, 0, {0, 0}},
	{Refl_char, "char", "someConstChar", (size_t)&((reflected_struct*)0)->someConstChar, 0, 0, {0, 0}}
};

