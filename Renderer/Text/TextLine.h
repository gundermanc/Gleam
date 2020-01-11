#pragma once

#include "TextSpan.h"

class TextLine
{
public:
    TextLine(const TextSpan& lineSpan);

private:
    TextSpan lineSpan;
    TextSpan lineBreakSpan;
};