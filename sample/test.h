#pragma once
#include <stdint.h>
#include <reflection_common.h>

//namespace nspace
//{
	reflect() struct some_substruct
	{
		int sub;
		char subchar;
	};
//}

reflect() struct some_other_struct
{
    int test;
};


reflect() struct some_struct
{
	// this is a method
	void test(int arg1, const char& arg2);

	/* this is a block comment */
	int int_field;
	uint32_t uint_field;
	char somechar;
	char* someString;

	inline void test2()
	{
		int x = 0;
		{
			++x;
		}
	}

	char* someOtherString;
	some_substruct substruct;
	some_other_struct so_substr;
};

enum token_type
{
    TT_Unknown,

	TT_Identifier,
	TT_String,
	TT_Int,
	TT_Uint,
	TT_Float,

	TT_Asterisk,
	TT_LParen,
	TT_RParen,
	TT_LCurl,
	TT_RCurl,
	TT_Colon,
	TT_Semicolon,
    TT_EOF
};

reflect() struct token
{
    token_type type;
    size_t text_length;
    char* text;

	struct subStruct
	{
		int substr;
	};

	template <typename T>
	void something() {
		/* Here is a comment */
		type = TT_EOF;
	}

	int value;

    double floatval;
    int64_t intval;
    uint64_t uintval;
};

namespace test
{
	inline void test_method()
	{
	}

	reflect() struct namespacedStruct
	{
		int value;
	};
}

reflect() struct another
{
	test::namespacedStruct nss;
};