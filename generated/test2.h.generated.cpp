#pragma once
#if defined(REFL_USE_CEREAL)
#include <cereal/archives/binary.hpp>
#endif

#include <reflection_common.h>
#include <test2.h>

refl_data g_ReflectionData_reflected_struct[] = {
	{Refl_int, "int", "intfield", (size_t)&((reflected_struct*)0)->intfield, 0},
	{Refl_char, "char", "another_field", (size_t)&((reflected_struct*)0)->another_field, 0},
	{Refl_charStar, "char", "someStringPtr", (size_t)&((reflected_struct*)0)->someStringPtr, 1},
	{Refl_charStarStar, "char", "someStringPtrPtr", (size_t)&((reflected_struct*)0)->someStringPtrPtr, 2},
	{Refl_char, "char", "someChar", (size_t)&((reflected_struct*)0)->someChar, 0},
	{Refl_char, "char", "someConstChar", (size_t)&((reflected_struct*)0)->someConstChar, 0}
};

#if defined(CEREAL_ARCHIVES_BINARY_HPP_)
void serialize(cereal::BinaryInputArchive& arch, reflected_struct& source) {
	arch(source.intfield);
	arch(source.another_field);
	arch(source.someChar);
	arch(source.someConstChar);
};
void serialize(cereal::BinaryOutputArchive& arch, reflected_struct& source) {
	arch(source.intfield);
	arch(source.another_field);
	arch(source.someChar);
	arch(source.someConstChar);
};
#endif

