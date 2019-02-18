#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>

static thread_local FILE* g_OutFile	= stdout;
static bool generate_static_reflection = true;
static bool generate_serializers = true;

// __cpp_user_defined_literals is a check for MSVC, __cplusplus should work
// everywhere else
#if __cplusplus < 201103L && !defined(__cpp_user_defined_literals)
#include <set>
typedef std::set<std::string> string_set;
static string_set g_Enums;
#if !defined(NO_THREADING)
#define NO_THREADING
#endif
#else
#if !defined(NO_THREADING)
#include <mutex>
#endif
#include <unordered_set>
typedef std::unordered_set<std::string> string_set;
static string_set g_Enums;
#endif

struct enum_class_pair
{
	std::string struct_or_class;
	std::string type_name;
	std::string enum_name;
	std::string struct_address;
	size_t num_fields;
};

static std::vector<enum_class_pair> g_EnumStructMapping;
static const char* g_OutDir = NULL;
#if !defined(NO_THREADING)
static std::mutex g_EnumMutex;
#endif

static char* GetFileText(char* path)
{
    FILE* file;
    if ((file = fopen(path, "rb")) != NULL)
    {
        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        fseek(file, 0, SEEK_SET);
        char* data = (char*)malloc(size * sizeof(char) + 1);
        fread(data, sizeof(char), size, file);
		data[size] = 0;
        fclose(file);
        return data;
    }
    return NULL;
}

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
	TT_LAngle,
	TT_RAngle,
	TT_Colon,
	TT_Semicolon,
    TT_EOF
};

struct token
{
    token_type type;
    size_t text_length;
    char* text;

	union
	{
		double floatval;
		int64_t intval;
		uint64_t uintval;
	};

	bool is_number()
	{
		return type == TT_Float || type == TT_Int || type == TT_Uint;
	}
};

struct tokenizer
{
	char* file;
    char* at;
    int line;
    int scan;
    char get()
    {
        return *at;
    }

    char peek()
    {
        return *(at+1);
    }

    void advance(int cnt = 1)
    {
        at += cnt;
    }
};

bool IsNewline(char c)
{
    return c == '\n';
}

bool IsWhitespace(char c)
{
    return  (c == ' ')  ||
            (c == '\t') ||
            (c == '\n') ||
            (c == '\v') ||
            (c == '\f') ||
            (c == '\r');
}

bool IsAlpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool IsNumber(char c)
{
	return (c >= '0' && c <= '9');
}

bool SkipComment(tokenizer* src)
{
	char c;
	if (src->get() == '#')
	{
		src->advance();
		while ((c = src->get()) && !IsNewline(c))
		{
			src->advance();
		}
		return true;
	}
	if (src->get() == '/')
	{
		char peek = src->peek();
		if (peek == '/')
		{
			src->advance(2);
			while ((c = src->get()) && !IsNewline(c))
			{
				src->advance();
			}
			return true;
		}
		else if (peek == '*')
		{
			src->advance(2);
			while ((c = src->get()) != 0)
			{
				if (c == '*')
				{
					if (src->peek() == '/')
					{
						src->advance(2);
						break;
					}
				}
				src->advance();
			}
			return true;
		}
	}
	return false;
}

void SkipWhitespace(tokenizer* src)
{
    char c;
	bool parsing = true;
	while (parsing)
	{
		parsing = false;
		while ((c = src->get()) && IsWhitespace(c))
		{
			if (IsNewline(c))
				++src->line;

			src->advance();
			parsing = true;
		}

		parsing = SkipComment(src);
	}
}

token GetToken(tokenizer* src);
bool TokenSays(token* tok, const char* target);
bool RequireToken(tokenizer* src, token_type type);
void ProcessStruct(tokenizer* src, token* type, std::vector<enum_class_pair>& t_EnumStructMapping, bool shouldWrite = true);
void ProcessReflect(tokenizer* src, std::vector<enum_class_pair>& t_EnumStructMapping);
void ProcessFile(char* path, char* enum_path);

token GetToken(tokenizer* src)
{
    SkipWhitespace(src);
    char cur = *src->at;

    token result;
    result.type = TT_Unknown;
    result.text = src->at;
    result.text_length = 1;
    ++src->at;

    switch(cur)
    {
        case '\0':	result.type = TT_EOF;		break;
		case '*':	result.type = TT_Asterisk;	break;
		case '(':	result.type = TT_LParen;	break;
		case ')':	result.type = TT_RParen;	break;
		case '{':	result.type = TT_LCurl;		break;
		case '}':	result.type = TT_RCurl;		break;
		case '<':	result.type = TT_LAngle;	break;
		case '>':	result.type = TT_RAngle;	break;
		case ':':	result.type = TT_Colon;		break;
		case ';':	result.type = TT_Semicolon;	break;
		case '"':
		{
			result.type = TT_String;
			++result.text;

			size_t length = 0;
			while (src->get() && src->get() != '"')
			{
				if (src->get() == '\\')
				{
					src->advance();
					++length;
				}
				src->advance();
				++length;
			}
			result.text_length = length;
			src->advance();
			break;
		}
		default:
		{
			if (IsAlpha(cur))
			{
				result.type = TT_Identifier;
				while ((cur = src->get()) && (IsAlpha(cur) || IsNumber(cur) || cur == '_' || (cur == ':' && src->peek() == ':')))
				{
					if (cur == ':')
					{
						++result.text_length;
						src->advance();
					}
					++result.text_length;
					src->advance();
				}
			}
			else if (IsNumber(cur) || (cur == '-' && IsNumber(src->peek())))
			{
				bool is_float = false;
				bool is_signed = (cur == '-');
				while ((cur = src->get()) && (IsAlpha(cur) || IsNumber(cur) || cur == '.' || cur == '-'))
				{
					if (cur == '.')
					{
						is_float = true;
					}
					++result.text_length;
					src->advance();
				}
				
				if (cur == 'f')
					src->advance();

				if (is_float)
				{
					result.type = TT_Float;
					sscanf(result.text, "%Lf", &result.floatval);
				}
				else if (is_signed)
				{
					result.type = TT_Int;
					sscanf(result.text, "%lld", &result.intval);
				}
				else
				{
					result.type = TT_Uint;
					sscanf(result.text, "%llud", &result.uintval);
				}
			}
			break;
		}
    };
    return result;
}

bool TokenSays(token* tok, const char* target)
{
	size_t i;
	for (i = 0; i < tok->text_length; ++i)
	{
		if (tok->text[i] != target[i])
		{
			return false;
		}
	}
	return target[tok->text_length] == 0;
}

bool RequireToken(tokenizer* src, token_type type)
{
	token tok = GetToken(src);
	return tok.type == type;
}

void ProcessStruct(tokenizer* src, token* type, std::vector<enum_class_pair>& t_EnumStructMapping, bool shouldWrite)
{
	string_set t_Enums;
	token nametok = GetToken(src);
	//fprintf(g_OutFile, "%.*s %.*s {\n", int(type->text_length), type->text, int(nametok.text_length), nametok.text);

	if (!RequireToken(src, TT_LCurl))
	{
		fprintf(stderr, "Parser error (%s:%d): Invalid struct/class definition: expected curly brace\n", src->file, src->line);
		exit(-1);
	}

	struct field_data
	{
		std::string enumName;
		token fieldname;
		token type;
		int ptr_cnt;
	};

	std::vector<field_data> fields;

	token curtok;
	while ((curtok = GetToken(src)).type != TT_EOF)
	{
		if (curtok.type == TT_RCurl)
		{
			break;
		}

		bool is_template = false;
		bool is_const = false;
		int ptr_cnt = 0;
		
		token fieldtype = curtok;
		if (TokenSays(&curtok, "template"))
		{
			curtok = GetToken(src);
			is_template = true;

			// Ignore template definitions
			if (curtok.type == TT_LAngle)
			{
				while ((curtok = GetToken(src)).type != TT_RAngle) {}
			}
			else
			{
				fprintf(stderr, "Parser error (%s:%d): Invalid template definition - expected '<', got %c\n", src->file, src->line, curtok.text[0]);
				exit(-1);
			}
			fieldtype = GetToken(src);
		}
		else if (TokenSays(&curtok, "reflect"))
		{
			//ProcessReflect(src, t_EnumStructMapping);
			fprintf(stderr, "Parser error (%s:%d): Reflection of nested structures not currently supported\n", src->file, src->line);
			exit(-1);
		}
		else if (TokenSays(&curtok, "struct") || TokenSays(&curtok, "struct"))
		{
			ProcessStruct(src, &curtok, std::vector<enum_class_pair>(), false);
			continue;
		}
		else
		{
			while (TokenSays(&fieldtype, "inline") || (TokenSays(&fieldtype, "const")))
			{
				is_const = is_const || TokenSays(&fieldtype, "const");
				fieldtype = GetToken(src);
				if (fieldtype.type == TT_EOF)
				{
					fprintf(stderr, "Parser error (%s:%d): Unexpected EOF in struct definition\n", src->file, src->line);
					exit(-1);
				}
			}
		}

		token fieldname = GetToken(src);
		while(fieldname.type == TT_Asterisk)
		{
			++ptr_cnt;
			fieldname = GetToken(src);
		}
		token delim = GetToken(src);

		if (delim.type == TT_LParen)
		{
			while ((delim = GetToken(src)).type != TT_EOF)
			{
				if (delim.type == TT_RParen)
					break;
				
				// TODO: log things
			}
			delim = GetToken(src);
			if (delim.type == TT_Semicolon)
			{
				continue;
			}

			if (delim.type == TT_LCurl)
			{
				int ogLine = src->line;
				int depth = 1;
				char* oldAt = src->at;
				while ((delim = GetToken(src)).type != TT_EOF && depth > 0)
				{
					switch (delim.type)
					{
					case TT_LCurl:
						++depth;
						break;
					case TT_RCurl:
						--depth;
						break;
					}
					if (depth < 1)
					{
						break;
					}
				}
				if (delim.type == TT_EOF)
				{
					fprintf(stderr, "Parser error (%s:%d): Method %.*s in %.*s does not terminate\n", src->file, ogLine, int(fieldname.text_length), fieldname.text, int(nametok.text_length), nametok.text);
					exit(-1);
				}
				continue;
			}

			fprintf(stderr, "Parser error (%s:%d): Unsupported method definition\n", src->file, src->line);
			exit(-1);
		}
		else if (delim.type == TT_Semicolon)
		{
			if (shouldWrite)
			{
				std::string enumName = "Refl_";
				if (
					is_template)
				{
					fprintf(stderr, "Parser error (%s:%d): Invalid member template for field %.*s\n", src->file, src->line, int(fieldname.text_length), fieldname.text);
					exit(-1);
				}
				enumName.reserve(enumName.length() + fieldtype.text_length);
				enumName.append(fieldtype.text, fieldtype.text_length);
				for (int i = 0; i < ptr_cnt; ++i)
				{
					enumName.append("Star");
				}
				if (t_Enums.count(enumName) == 0)
				{
					t_Enums.insert(enumName);
				}
				fields.push_back({ enumName, fieldname, fieldtype, ptr_cnt });
			}
		}
		else
		{
			fprintf(stderr, "Parser error (%s:%d): Invalid field definition: expected member function or variable\n", src->file, src->line);
			exit(-1);
		}

		// TODO: catch errors
	}

	if (curtok.type == TT_EOF)
	{
		fprintf(stderr, "Parser error (%s:%d): Invalid struct/class definition: expected ending curly brace\n", src->file, src->line);
		exit(-1);
	}

	if (!RequireToken(src, TT_Semicolon))
	{
		fprintf(stderr, "Parser error (%s:%d): Invalid struct/class definition: expected semicolon\n", src->file, src->line);
		exit(-1);
	}

	if (fields.size() > 0)
	{
		std::string enumName = "Refl_";
		enumName.reserve(enumName.length() + nametok.text_length);
		enumName.append(nametok.text, nametok.text_length);

		std::string structName = "g_ReflectionData_";
		structName.reserve(structName.length() + nametok.text_length);
		structName.append(nametok.text, nametok.text_length);
		if (t_Enums.count(enumName) == 0)
		{
			t_Enums.insert(enumName);
		}

		t_EnumStructMapping.push_back({
			std::string(type->text, type->text_length),
			std::string(nametok.text, nametok.text_length),
			enumName,
			structName,
			fields.size()
			});

		if (generate_static_reflection)
		{
			fprintf(g_OutFile, "refl_data g_ReflectionData_%.*s[] = {\n", int(nametok.text_length), nametok.text);
			for (size_t i = 0; i < fields.size(); ++i)
			{
				auto& enumName = fields[i].enumName;
				auto& fieldtype = fields[i].type;
				auto& fieldname = fields[i].fieldname;
				fprintf(g_OutFile, "\t{%s, \"%.*s\", \"%.*s\", (size_t)&((%.*s*)0)->%.*s, %d}", enumName.c_str(), int(fieldtype.text_length), fieldtype.text, int(fieldname.text_length), fieldname.text, int(nametok.text_length), nametok.text, int(fieldname.text_length), fieldname.text, fields[i].ptr_cnt);
				// just continue on, scotty
				if (i < fields.size() - 1)
					fprintf(g_OutFile, ",");

				fprintf(g_OutFile, "\n");
			}
			fprintf(g_OutFile, "};\n\n");
		}

		if (generate_serializers)
		{
			fprintf(g_OutFile, "#if defined(CEREAL_ARCHIVES_BINARY_HPP_)\n");
			fprintf(g_OutFile, "void serialize(cereal::BinaryInputArchive& arch, %.*s& source) {\n", int(nametok.text_length), nametok.text);
			for (size_t i = 0; i < fields.size(); ++i)
			{
				if (fields[i].ptr_cnt > 0)
					continue;

				auto& enumName = fields[i].enumName;
				auto& fieldtype = fields[i].type;
				auto& fieldname = fields[i].fieldname;
				fprintf(g_OutFile, "\tarch(source.%.*s);", int(fieldname.text_length), fieldname.text);

				fprintf(g_OutFile, "\n");
			}
			fprintf(g_OutFile, "};\n");

			fprintf(g_OutFile, "void serialize(cereal::BinaryOutputArchive& arch, %.*s& source) {\n", int(nametok.text_length), nametok.text);
			for (size_t i = 0; i < fields.size(); ++i)
			{
				if (fields[i].ptr_cnt > 0)
					continue;

				auto& enumName = fields[i].enumName;
				auto& fieldtype = fields[i].type;
				auto& fieldname = fields[i].fieldname;
				fprintf(g_OutFile, "\tarch(source.%.*s);", int(fieldname.text_length), fieldname.text);

				fprintf(g_OutFile, "\n");
			}
			fprintf(g_OutFile, "};\n");
			fprintf(g_OutFile, "#endif\n\n");
		}
	}

#if !defined(NO_THREADING)
	std::lock_guard<std::mutex> lock(g_EnumMutex);
#endif
	for (auto& it : t_Enums)
	{
		g_Enums.insert(it);
	}
	for (auto& it : t_EnumStructMapping)
	{
		g_EnumStructMapping.emplace_back(it);
	}
}

void ProcessReflect(tokenizer* src, std::vector<enum_class_pair>& t_EnumStructMapping)
{
	RequireToken(src, TT_LParen);
	
	token tok;
	while ((tok = GetToken(src)).type != TT_EOF)
	{
		if (tok.type == TT_RParen)
			break;
	}

	tok = GetToken(src);
	if (tok.type != TT_Identifier)
	{
		fprintf(stderr, "Parser error (%s:%d): Reflection expected struct or class\n", src->file, src->line);
		exit(-1);
	}
	
	if (!TokenSays(&tok, "struct") && !TokenSays(&tok, "class"))
	{
		fprintf(stderr, "Parser error (%s:%d): Reflection expected struct or class\n", src->file, src->line);
		exit(-1);
	}

	ProcessStruct(src, &tok, t_EnumStructMapping);
}

void ProcessFile(char* path, char* enum_path)
{
    auto text = GetFileText(path);
	if (!text)
	{
		return;
	}
    tokenizer src;
	src.file = path;
    src.at = text;
	src.line = 1;

	char* localpath = path;
	size_t length = strlen(path);
	for (size_t i = length - 1; i >= 0; --i)
	{
		if (path[i] == '/' || path[i] == '\\')
		{
			localpath = path + i + 1;
			break;
		}
	}

	std::vector<enum_class_pair> t_EnumStructMapping;
	// TODO: fix this, it's terrible
	FILE* temp = fopen((std::string(g_OutDir ? g_OutDir : "") + std::string(localpath) + ".generated.cpp").c_str(), "w");
	
	fprintf(stdout, "Processing file %s, writing output to %s%s...\n", localpath, localpath, ".generated.cpp");
	if (temp)
	{
		g_OutFile = temp;
	}

	fprintf(g_OutFile, "#pragma once\n");
	if (generate_serializers)
	{
		fprintf(g_OutFile, "#if defined(REFL_USE_CEREAL)\n");
		fprintf(g_OutFile, "#include <cereal/archives/binary.hpp>\n");
		fprintf(g_OutFile, "#endif\n\n");
	}

	fprintf(g_OutFile, "#include <%s>\n", enum_path);
	fprintf(g_OutFile, "#include <%s>\n\n", localpath); 


    token tok;
    while((tok = GetToken(&src)).type != TT_EOF)
    {
		switch (tok.type)
		{
		case TT_Float:
			//printf("%lf", tok.floatval);
			break;
		case TT_Uint:
			//printf("%llu", tok.uintval);
			break;
		case TT_Int:
			//printf("%lld", tok.intval);
			break;
		case TT_Unknown:
			//printf("%c", *tok.text);
			break;
		case TT_String:
			//printf("\"%.*s\"", int(tok.text_length), tok.text);
			break;
		case TT_Identifier:
			if (TokenSays(&tok, "reflect"))
			{
				ProcessReflect(&src, t_EnumStructMapping);
				break;
			}
			//printf("%.*s ", int(tok.text_length), tok.text);
			break;
		default:
			//printf("%.*s", int(tok.text_length), tok.text);
			break;
		}
    }

	if (temp)
	{
		fclose(temp);
		g_OutFile = NULL;
	}

	FILE* enumf = fopen((std::string(g_OutDir ? g_OutDir : "") + std::string(localpath) + ".generated.hpp").c_str(), "w");;

	// TODO: Move this section to a generated header file
	fprintf(enumf, "#pragma once\n");
	fprintf(enumf, "#include <%s>\n\n", enum_path);

	if (generate_static_reflection)
	{
		for (auto& it : t_EnumStructMapping)
		{
			fprintf(enumf, "%s %s;\n", it.struct_or_class.c_str(), it.type_name.c_str());
			fprintf(enumf, "extern refl_data %s[];\n", it.struct_address.c_str());
		}

		/*fprintf(enumf, "static inline refl_array get_array(ReflectionType type){\n\tswitch(type) {\n");

		fprintf(enumf, "\t}\n\treturn {0};\n}\n");*/
		for (auto& it : t_EnumStructMapping)
		{
			fprintf(enumf, "template <>\nrefl_array get_reflection_data<%s>()\n{\n\treturn {%s, %llu};\n}\n", it.type_name.c_str(), it.struct_address.c_str(), it.num_fields);
			//fprintf(enumf, "\tcase %s: return {%s, %llu};\n", it.enum_name.c_str(), it.struct_address.c_str(), it.num_fields);
		}
	}

	if (generate_serializers)
	{
		fprintf(enumf, "#if defined(CEREAL_ARCHIVES_BINARY_HPP_)\n");
		for (auto& it : t_EnumStructMapping)
		{
			fprintf(enumf, "void serialize(cereal::BinaryInputArchive& arch, %s& source);\n", it.type_name.c_str());
			fprintf(enumf, "void serialize(cereal::BinaryOutputArchive& arch, %s& source);\n", it.type_name.c_str());
		}
		fprintf(enumf, "#endif\n");
	}

	if (generate_static_reflection)
	{
		for (auto& it : t_EnumStructMapping)
		{
			fprintf(enumf, "template <>\ninline ReflectionType get_type<%s>()\n{\n\treturn %s;\n}\n", it.type_name.c_str(), it.enum_name.c_str());
		}
	}
	// ENDTODO: Move this section to a generated header file
}

#if !defined(NO_THREADING)
struct thread_data_t
{
	char** farray;
	char* enum_path;
	int minIndex;
	int maxIndex;
};

void thread_func(thread_data_t* data)
{
	for (int i = data->minIndex; i < data->maxIndex; ++i)
	{
		ProcessFile(data->farray[i], data->enum_path);
	}
}

int min(int a, int b)
{
	return a < b ? a : b;
}

void process_with_threads(char** fileArray, int fCount, char* enum_outfile, int jobCount)
{
	int jobs = jobCount;
	if (jobs < 1)
	{
		jobs = min(std::thread::hardware_concurrency(), fCount);
	}

	std::vector<std::thread> threads;
	std::vector<thread_data_t> thread_data;
	threads.reserve(jobs);
	thread_data.reserve(jobs);
	
	int current = 0;
	int delta = fCount / jobs;
	for (int i = 0; i < jobs; ++i)
	{
		thread_data_t data;
		data.farray = fileArray;
		data.enum_path = enum_outfile;
		data.minIndex = current;
		if (i == jobs - 1)
		{
			data.maxIndex = fCount;
		}
		else
		{
			data.maxIndex = current + delta;
		}
		current = data.maxIndex;
		thread_data.push_back(data);
		threads.push_back(std::thread(&thread_func, &thread_data[i]));
	}

	for (auto& it : threads)
	{
		it.join();
	}
}

#endif

#include <chrono>
int main(int argc, char** argv)
{
	using clock = std::chrono::high_resolution_clock;

	auto start = clock::now();
	int fcount = 0;
	char** files = (char**)malloc(sizeof(char*) * (argc - 1));
	memset(files, 0, sizeof(char*) * (argc - 1));

	bool has_outfile = false;
	bool has_enumoutfile = false;
	char outfile[512] = {0};
	char enum_outfile[512] = { 0 };
	int jobcount = -1;

    for(int i = 1; i < argc; ++i)
    {
		char* arg = argv[i];
		if (!strcmp(arg, "-o") || !strcmp(arg, "--output"))
		{
			if (i < argc - 1)
			{
				++i;
				arg = argv[i];
				strcpy(outfile, arg);
				has_outfile = true;
				g_OutDir = outfile;
			}
			else
			{
				fprintf(stderr, "Error: Attempted to specify output file without providing output file\n");
				exit(-1);
			}
			continue;
		}
		if (!strcmp(arg, "-j") || !strcmp(arg, "--jobs"))
		{
			if (i < argc - 1)
			{
				++i;
				arg = argv[i];
				sscanf(arg, "%d", &jobcount);
			}
			else
			{
				fprintf(stderr, "Error: Attempted to specify job count without providing count\n");
				exit(-1);
			}
			continue;
		}
		if (!strcmp(arg, "-nsr") || !strcmp(arg, "--no-static-reflection"))
		{
			generate_static_reflection = false;
			continue;
		}
		if (!strcmp(arg, "-ns") || !strcmp(arg, "--no-serialization"))
		{
			generate_serializers = false;
			continue;
		}
		if (!strcmp(arg, "-e") || !strcmp(arg, "--enumout"))
		{
			if (i < argc - 1)
			{
				++i;
				arg = argv[i];
				strcpy(enum_outfile, arg);
				has_enumoutfile = true;
			}
			else
			{
				fprintf(stderr, "Error: Attempted to specify output file without providing output file\n");
				exit(-1);
			}
			continue;
		}
        files[fcount++] = (argv[i]);
    }

	if (fcount == 0)
	{
		fprintf(stderr, "Error: No input files given");
		return -1;
	}

	if (!has_enumoutfile)
	{
		strcpy(enum_outfile, "reflection_common.h");
	}

#if defined(NO_THREADING)
	for (int i = 0; i < fcount; ++i)
	{
		ProcessFile(files[i], enum_outfile);
	}
#else
	process_with_threads(files, fcount, enum_outfile, jobcount);
#endif

	FILE* enumf = fopen((std::string(g_OutDir ? g_OutDir : "") + enum_outfile).c_str(), "w");
	if (enumf)
	{
		fprintf(stdout, "Writing to common header %s...\n", enum_outfile);
		fprintf(enumf, "#pragma once\n");
		fprintf(enumf, "#if !defined(reflect)\n");
		fprintf(enumf, "#define reflect(...)\n");
		fprintf(enumf, "#endif\n\n");

		if (generate_static_reflection)
		{
			fprintf(enumf, "enum ReflectionType {\n");
			fprintf(enumf, "\tRefl_Invalid = -1,\n");

			for (auto& it : g_Enums)
			{
				fprintf(enumf, "\t%s,\n", it.c_str());
			}

			fprintf(enumf, "\tRefl_Count\n");

			fprintf(enumf, "};\n\n");
			fprintf(enumf, "struct refl_data\n{\n\tReflectionType type;\n\tchar* type_name;\n\tchar* fieldname;\n\tsize_t offset;int ptr_cnt;size_t ptr_count_offset;\n\tinline void* get(void* base){return (void*)(((char*)base) + offset);}\n\ttemplate <typename T>\n\tinline T& get(void* base){return *((T*)(((char*)base) + offset));}\n};\n\n");
			fprintf(enumf, "struct refl_array\n{\n\trefl_data* address;\n\tsize_t count;\n\tinline refl_data& operator[](size_t index){return address[index];}\n};\n\n");
			fprintf(enumf, "template <typename T>\ninline ReflectionType get_type()\n{\n\tstatic_asset(sizeof(T) == -1, \"Attempted to read reflected type of unregistered type; you may have forgotten to include a generated header\");\n\treturn Refl_Invalid;\n}\n");
			fprintf(enumf, "template <typename T>\nrefl_array get_reflection_data()\n{\n\tstatic_assert(sizeof(T) == -1, \"Attempted to read reflection data of unregistered type; you may have forgotten to include a generated header\");\n\treturn {0};\n}\n");
			fprintf(enumf, "template <typename T>\nrefl_array get_reflection_data(const T&)\n{\n\treturn get_reflection_data<T>();\n}\n");
		}
		fclose(enumf);
	}
	else
	{
		fprintf(stderr, "Error: Failed to open header %s for writing!\n", enum_outfile);
	}

	auto end = clock::now();
	auto diff = end - start;
	auto dt = ((double)std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()) / 1000000000.0;
	fprintf(stdout, "Code generation finished in %fs\n", dt);

    return 0;
}