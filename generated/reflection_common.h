#pragma once
#define REFL_STATIC_REFLECTION 1
#define REFL_SERIALIZERS 1
#if !defined(reflect)
#define reflect(...)
#endif

enum ReflectionType {
	Refl_Invalid = -1,
	Refl_int,
	Refl_char,
	Refl_charStar,
	Refl_some_substruct,
	Refl_double,
	Refl_some_other_struct,
	Refl_token_type,
	Refl_uint32_t,
	Refl_int64_t,
	Refl_some_struct,
	Refl_size_t,
	Refl_uint64_t,
	Refl_token,
	Refl_test_namespacedStruct,
	Refl_another,
	Refl_charStarStar,
	Refl_reflected_struct,
	Refl_Count
};

struct refl_data;
struct refl_array
{
	refl_data* address;
	size_t count;
	inline refl_data& operator[](size_t index);
};

struct refl_data
{
	ReflectionType type;
	char* type_name;
	char* fieldname;
	size_t offset;int ptr_cnt;size_t ptr_count_offset;
	inline void* get(void* base){return (void*)(((char*)base) + offset);}
	template <typename T>
	inline T& get(void* base){return *((T*)(((char*)base) + offset));}
	refl_array type_data;
};

refl_data& refl_array::operator[](size_t index){return address[index];}

template <typename T>
inline ReflectionType get_type()
{
	static_asset(sizeof(T) == -1, "Attempted to read reflected type of unregistered type; you may have forgotten to include a generated header");
	return Refl_Invalid;
}
template <typename T>
refl_array get_reflection_data()
{
	static_assert(sizeof(T) == -1, "Attempted to read reflection data of unregistered type; you may have forgotten to include a generated header");
	return {0};
}
template <typename T>
refl_array get_reflection_data(const T&)
{
	return get_reflection_data<T>();
}
