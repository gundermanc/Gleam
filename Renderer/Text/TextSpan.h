#pragma once

#include "TextMoment.h"

#include <stdexcept>

class TextSpan
{
public:
    TextSpan(const TextMoment* textMoment)
        : textMoment(textMoment),
        start(0),
        length(0)
    {
    }

    TextSpan(const TextMoment* textMoment, unsigned int start, unsigned int length);

    const TextMoment* GetTextMoment() const { return this->textMoment; }

    char GetChar(unsigned int index);
    unsigned int GetStart() const { return this->start; }
    unsigned int GetLength() const { return this->length; }
    unsigned int GetEnd() const { return this->start + this->length; }
    TextSpan operator =(const TextSpan& otherSpan) const { return otherSpan; }

private:
    const TextMoment* textMoment;
    unsigned int start;
    unsigned int length;
};
