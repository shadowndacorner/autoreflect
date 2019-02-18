#pragma once
#define reflect(...)

struct someOtherStruct
{
    int thisIsMe;
};

reflect() struct reflected_struct
{
    int intfield;
    char another_field;
    const char* someStringPtr;
    const char** someStringPtrPtr;
    char someChar;
    const char someConstChar;
};