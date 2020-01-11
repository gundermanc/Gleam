#include "TextLine.h"
#include "TextSpan.h"

TextLine::TextLine(const TextSpan& lineSpan)
    : lineSpan(TextSpan(lineSpan.GetTextMoment())),
    lineBreakSpan(TextSpan(lineSpan.GetTextMoment()))
{
    this->lineSpan = lineSpan;
    this->lineBreakSpan = TextSpan(lineSpan.GetTextMoment(), 0, 0);

    if (lineSpan.GetLength() >= 1)
    {
    }
}