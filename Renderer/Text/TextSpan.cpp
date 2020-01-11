#include "TextSpan.h"

#include <stdexcept>

TextSpan::TextSpan(const TextMoment* textMoment, unsigned int start, unsigned int length)
    : textMoment(textMoment),
    start(start),
    length(length)
{
    auto documentLength = textMoment->GetLength();
    if (start < 0 || start >= documentLength || (start + length) >= documentLength)
    {
        throw std::out_of_range("Span bounds out of range");
    }
}

char TextSpan::GetChar(unsigned int index)
{
    if (index < this->GetStart() || index >= this->GetLength())
    {
        throw std::out_of_range("Index out of range");
    }

    return this->GetTextMoment()->GetChar(this->GetStart() + index);
}