#pragma once

#include <string>
#include <tuple>
#include <vector>

// Primitive Stand-in for a real document class (TODO).
class TextDocument
{
public:
    TextDocument() : length(0) { }

    void AppendLine(const std::string& line) { this->textLines.push_back(line); this->length += line.length(); }
    const std::vector<std::string> GetLines() { return this->textLines;  }

    std::tuple<unsigned int, unsigned int> GetPositionAndLineNumberOfLineContainingPosition(unsigned int position);
    unsigned int GetLength() { return this->length; }
    void Insert(const std::string& text, unsigned int position);
    void Remove(unsigned int position, unsigned int length);

private:
    unsigned int length;
    std::vector<std::string> textLines;
};
