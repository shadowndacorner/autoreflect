#pragma once
#include <reflection_common.h>

struct some_struct;
extern refl_data g_ReflectionData_some_struct[];
struct some_other_struct;
extern refl_data g_ReflectionData_some_other_struct[];
struct token;
extern refl_data g_ReflectionData_token[];
template <>
refl_array get_reflection_data<some_struct>()
{
	return {g_ReflectionData_some_struct, 5};
}
template <>
refl_array get_reflection_data<some_other_struct>()
{
	return {g_ReflectionData_some_other_struct, 1};
}
template <>
refl_array get_reflection_data<token>()
{
	return {g_ReflectionData_token, 7};
}
#if defined(CEREAL_ARCHIVES_BINARY_HPP_)
void serialize(cereal::BinaryInputArchive& arch, some_struct& source);
void serialize(cereal::BinaryOutputArchive& arch, some_struct& source);
void serialize(cereal::BinaryInputArchive& arch, some_other_struct& source);
void serialize(cereal::BinaryOutputArchive& arch, some_other_struct& source);
void serialize(cereal::BinaryInputArchive& arch, token& source);
void serialize(cereal::BinaryOutputArchive& arch, token& source);
#endif
template <>
inline ReflectionType get_type<some_struct>()
{
	return Refl_some_struct;
}
template <>
inline ReflectionType get_type<some_other_struct>()
{
	return Refl_some_other_struct;
}
template <>
inline ReflectionType get_type<token>()
{
	return Refl_token;
}
