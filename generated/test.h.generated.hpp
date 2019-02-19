#pragma once
#include <reflection_common.h>

struct some_substruct;
extern refl_data g_ReflectionData_some_substruct[];
struct some_other_struct;
extern refl_data g_ReflectionData_some_other_struct[];
struct some_struct;
extern refl_data g_ReflectionData_some_struct[];
struct token;
extern refl_data g_ReflectionData_token[];
struct test::namespacedStruct;
extern refl_data g_ReflectionData_test_namespacedStruct[];
struct another;
extern refl_data g_ReflectionData_another[];
template <>
refl_array get_reflection_data<some_substruct>()
{
	return {g_ReflectionData_some_substruct, 2};
}
template <>
refl_array get_reflection_data<some_other_struct>()
{
	return {g_ReflectionData_some_other_struct, 1};
}
template <>
refl_array get_reflection_data<some_struct>()
{
	return {g_ReflectionData_some_struct, 7};
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
inline void serialize(Archive& arch, some_substruct& source) {
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("sub", source.sub));
	} else {
	try{
		arch(cereal::make_nvp("sub", source.sub));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.sub);
	#endif
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("subchar", source.subchar));
	} else {
	try{
		arch(cereal::make_nvp("subchar", source.subchar));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.subchar);
	#endif
}
template<typename Archive>
inline void serialize(Archive& arch, some_other_struct& source) {
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("test", source.test));
	} else {
	try{
		arch(cereal::make_nvp("test", source.test));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.test);
	#endif
}
template<typename Archive>
inline void serialize(Archive& arch, some_struct& source) {
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("int_field", source.int_field));
	} else {
	try{
		arch(cereal::make_nvp("int_field", source.int_field));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.int_field);
	#endif
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("uint_field", source.uint_field));
	} else {
	try{
		arch(cereal::make_nvp("uint_field", source.uint_field));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.uint_field);
	#endif
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("somechar", source.somechar));
	} else {
	try{
		arch(cereal::make_nvp("somechar", source.somechar));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.somechar);
	#endif
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("substruct", source.substruct));
	} else {
	try{
		arch(cereal::make_nvp("substruct", source.substruct));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.substruct);
	#endif
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("so_substr", source.so_substr));
	} else {
	try{
		arch(cereal::make_nvp("so_substr", source.so_substr));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.so_substr);
	#endif
}
template<typename Archive>
inline void serialize(Archive& arch, token& source) {
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("type", source.type));
	} else {
	try{
		arch(cereal::make_nvp("type", source.type));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.type);
	#endif
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("text_length", source.text_length));
	} else {
	try{
		arch(cereal::make_nvp("text_length", source.text_length));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.text_length);
	#endif
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("value", source.value));
	} else {
	try{
		arch(cereal::make_nvp("value", source.value));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.value);
	#endif
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("floatval", source.floatval));
	} else {
	try{
		arch(cereal::make_nvp("floatval", source.floatval));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.floatval);
	#endif
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("intval", source.intval));
	} else {
	try{
		arch(cereal::make_nvp("intval", source.intval));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.intval);
	#endif
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("uintval", source.uintval));
	} else {
	try{
		arch(cereal::make_nvp("uintval", source.uintval));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.uintval);
	#endif
}
template<typename Archive>
inline void serialize(Archive& arch, test::namespacedStruct& source) {
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("value", source.value));
	} else {
	try{
		arch(cereal::make_nvp("value", source.value));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.value);
	#endif
}
template<typename Archive>
inline void serialize(Archive& arch, another& source) {
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("nss", source.nss));
	} else {
	try{
		arch(cereal::make_nvp("nss", source.nss));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.nss);
	#endif
}
template <>
inline ReflectionType get_type<some_substruct>()
{
	return Refl_some_substruct;
}
template <>
inline ReflectionType get_type<some_other_struct>()
{
	return Refl_some_other_struct;
}
template <>
inline ReflectionType get_type<some_struct>()
{
	return Refl_some_struct;
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
