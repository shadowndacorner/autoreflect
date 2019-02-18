#pragma once
#include <reflection_common.h>

struct reflected_struct;
extern refl_data g_ReflectionData_reflected_struct[];
template <>
refl_array get_reflection_data<reflected_struct>()
{
	return {g_ReflectionData_reflected_struct, 6};
}
#if defined(CEREAL_ARCHIVES_BINARY_HPP_)
void serialize(cereal::BinaryInputArchive& arch, reflected_struct& source);
void serialize(cereal::BinaryOutputArchive& arch, reflected_struct& source);
#endif
template <>
inline ReflectionType get_type<reflected_struct>()
{
	return Refl_reflected_struct;
}
