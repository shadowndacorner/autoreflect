#pragma once
#include <reflection_common.h>

struct some_struct;
extern refl_data g_ReflectionData_some_struct[];
struct some_other_struct;
extern refl_data g_ReflectionData_some_other_struct[];
struct token;
extern refl_data g_ReflectionData_token[];
struct test::namespacedStruct;
extern refl_data g_ReflectionData_test_namespacedStruct[];
struct another;
extern refl_data g_ReflectionData_another[];
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
template <>
refl_array get_reflection_data<test::namespacedStruct>()
{
	return {g_ReflectionData_test_namespacedStruct, 1};
}
template <>
refl_array get_reflection_data<another>()
{
	return {g_ReflectionData_another, 1};
}
template<typename Archive>
void serialize(Archive& arch, some_struct& source) {
	arch(source.int_field);
	arch(source.uint_field);
	arch(source.somechar);
};
template<typename Archive>
void serialize(Archive& arch, some_other_struct& source) {
	arch(source.test);
};
template<typename Archive>
void serialize(Archive& arch, token& source) {
	arch(source.type);
	arch(source.text_length);
	arch(source.value);
	arch(source.floatval);
	arch(source.intval);
	arch(source.uintval);
};
template<typename Archive>
void serialize(Archive& arch, test::namespacedStruct& source) {
	arch(source.value);
};
template<typename Archive>
void serialize(Archive& arch, another& source) {
	arch(source.nss);
};
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
template <>
inline ReflectionType get_type<test::namespacedStruct>()
{
	return Refl_test_namespacedStruct;
}
template <>
inline ReflectionType get_type<another>()
{
	return Refl_another;
}
