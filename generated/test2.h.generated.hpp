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
inline void serialize(Archive& arch, reflected_struct& source) {
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("intfield", source.intfield));
	} else {
	try{
		arch(cereal::make_nvp("intfield", source.intfield));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.intfield);
	#endif
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("another_field", source.another_field));
	} else {
	try{
		arch(cereal::make_nvp("another_field", source.another_field));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.another_field);
	#endif
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("someChar", source.someChar));
	} else {
	try{
		arch(cereal::make_nvp("someChar", source.someChar));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.someChar);
	#endif
	#if defined(CEREAL_CEREAL_HPP_)
	if (!(cereal::traits::is_text_archive<Archive>::value && Archive::is_loading::value)) {
		arch(cereal::make_nvp("someConstChar", source.someConstChar));
	} else {
	try{
		arch(cereal::make_nvp("someConstChar", source.someConstChar));
	}catch(cereal::Exception&){/*this is necessary because cereal has no support for optional named fields*/}
	}
	#else
	arch(source.someConstChar);
	#endif
}
template <>
inline ReflectionType get_type<reflected_struct>()
{
	return Refl_reflected_struct;
}
