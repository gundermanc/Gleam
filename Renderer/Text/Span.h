#pragma once

#include <assert.h>

class Span
{
public:
    Span(unsigned int start, unsigned int length) : Start(start), Length(length) { }

    unsigned int Start;
    unsigned int Length;
};