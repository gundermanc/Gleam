#pragma once

#include "TextMoment.h"
#include "TextSpan.h"
#include "../Utilities/Property.h"
#include <string>
#include <tuple>
#include <vector>

// Primitive Stand-in for a real document class (TODO).
class TextDocument
{
public:
    TextDocument();
  //  ~TextDocument();

    void AppendLine(const std::string& line) { this->textLines.push_back(line); this->length += line.length(); }

    //std::tuple<unsigned int, unsigned int> GetPositionAndLineNumberOfLineContainingPosition(unsigned int position);
    Property<std::string>& GetPreferredLineEnding() { return this->preferredLineEnding; }
    const TextMoment* GetCurrentMoment() { return this->currentMoment; } // TODO: to reference.
    void Insert(const std::string& text, unsigned int position);
    void Remove(unsigned int position, unsigned int length);
    std::string ToString();

private:
    Property<std::string> preferredLineEnding;
    unsigned int length;
    std::vector<std::string> textLines;
    const TextMoment* currentMoment;
};
