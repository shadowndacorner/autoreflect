#include <reflection_common.h>
#include "test.h"
#include <test.h.generated.hpp>
#include <stdio.h>

int main(int argc, char** argv)
{
	some_struct test_struct = {0};
	test_struct.int_field = -32;
	test_struct.uint_field = 30;
	test_struct.somechar = 'C';
	test_struct.someString = "This is a test";

	auto fieldarray = get_reflection_data(test_struct);
	for (int i = 0; i < fieldarray.count; ++i)
	{
		auto& field = fieldarray[i];
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
			int& val = field.get<int>(&test_struct);
			printf(" (value: %d)", val);
			break;
		}
		case Refl_uint32_t:
		{
			uint32_t& val = field.get<uint32_t>(&test_struct);
			printf(" (value: %u)", val);
		}	break;
		case Refl_char:
		{
			char& val = field.get<char>(&test_struct);
			printf(" (value: %c)", val);
		}	break;
		case Refl_charStar:
		{
			char* val = field.get<char*>(&test_struct);
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
			char** val = field.get<char**>(&test_struct);
			printf(" (value: 0x%p)", val);
		}	break;
		}
		printf("\n");
	}

	return 1;
}