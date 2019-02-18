# Autoreflect

This is a tiny application that parses C/C++ source code and reads struct/class definitions, emitting static reflection data and serialization methods compatible with [cereal](https://github.com/USCiLab/cereal).

Autoreflect implements a very simple, sort-of recursive descent C/C++ parser that seeks only to understand structs/classes.  Despite its simplicity, it is able to understand and reflect most non-templated C/C++ data structures.

## Building
Autoreflect uses the CMake build system, but it is a single C++ source file whose only dependency is the C++ STL, so it should compile on most compliant C++ compilers.

## Usage
Simply tag data structures you would like to be reflectable with the `reflect()` macro, as so:

```c
reflect() struct someStruct
{
    int someField;
    uint32_t someOtherField;
    char* someStringField;
};
```

Then, run the command line utility with reflectable files as arguments.

See the sample application for an example of using the C-style reflection data.  To use the generated serialization methods, simply include `headername.generated.hpp` and serialize the data structures.

## Motivation
The primary purpose of autoreflect, aside from being an educational exercise, was to automatically generate static reflection data for only fields of C-like structs.  As I was working, I realized that it would be trivial to add support for automatically generating cereal-style serialization methods, so I added that.