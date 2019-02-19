#if defined(WITH_CEREAL)
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <fstream>
#endif

#include <reflection_common.h>
#include "test.h"
#include <test.h.generated.hpp>
#include <stdio.h>

void print_refl_data(void* object, refl_array fieldarray, int indent = 0)
{
	for (int i = 0; i < fieldarray.count; ++i)
	{
		auto& field = fieldarray[i];
		for (int _ = 0; _ < indent; ++_)
		{
			printf("\t");
		}
		printf("Field");
		if (field.ptr_cnt > 0)
		{
			printf(" (pointer)");
		}
		printf(": %s %s", field.type_name, field.fieldname);
		switch (field.type)
		{
		case Refl_int:
		{
			int& val = field.get<int>(object);
			printf(" (value: %d)", val);
			break;
		}
		case Refl_uint32_t:
		{
			uint32_t& val = field.get<uint32_t>(object);
			printf(" (value: %u)", val);
		}	break;
		case Refl_char:
		{
			char& val = field.get<char>(object);
			printf(" (value: %c)", val);
		}	break;
		case Refl_charStar:
		{
			char* val = field.get<char*>(object);
			if (val)
			{
				printf(" (value: %s)", val);
			}
			else
			{
				printf(" (value: NULL)");
			}
		}	break;
		case Refl_charStarStar:
		{
			char** val = field.get<char**>(object);
			printf(" (value: 0x%p)", val);
		}	break;
		case Refl_some_substruct:
		{
			printf("\n");
			print_refl_data(&field.get<some_substruct>(object), get_reflection_data<some_substruct>(), indent + 1);
		}	break;
		case Refl_some_other_struct:
		{
			printf("\n");
			print_refl_data(&field.get<some_other_struct>(object), get_reflection_data<some_other_struct>(), indent + 1);
			break;
		}
		}
		printf("\n");
	}
}

int main(int argc, char** argv)
{
	some_struct test_struct = {0};
	test_struct.int_field = -32;
	test_struct.uint_field = 30;
	test_struct.somechar = 'C';
	test_struct.substruct = { 100, 'f' };

#if defined(REFL_SERIALIZERS) && defined(WITH_CEREAL)
	{
		std::cout << "Writing JSON to stdout" << std::endl;
		cereal::JSONOutputArchive arch(std::cout);
		arch(test_struct);
		std::cout << std::endl;
	}

	{
		std::cout << "Writing binary to out.bin" << std::endl;
		
		auto file = std::ofstream("out.bin", std::ios::binary);
		cereal::BinaryOutputArchive arch(file);
		arch(test_struct);
	}

	{
		memset(&test_struct, 0, sizeof(some_struct));
		std::cout << "Reading binary from out.bin" << std::endl;

		auto file = std::ifstream("out.bin", std::ios::binary);
		cereal::BinaryInputArchive arch(file);
		arch(test_struct);
	}
#endif
	test_struct.someString = "This is a test";

	auto fieldarray = get_reflection_data(test_struct);
	print_refl_data(&test_struct, fieldarray);
	return 1;
}