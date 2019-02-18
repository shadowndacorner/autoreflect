#pragma once
#include <reflection_common.h>

struct reflected_struct;
extern refl_data g_ReflectionData_reflected_struct[];
template <>
refl_array get_reflection_data<reflected_struct>()
{
	return {g_ReflectionData_reflected_struct, 6};
}
template<typename Archive>
void serialize(Archive& arch, reflected_struct& source) {
	arch(source.intfield);
	arch(source.another_field);
	arch(source.someChar);
	arch(source.someConstChar);
};
template <>
inline ReflectionType get_type<reflected_struct>()
{
	return Refl_reflected_struct;
}
