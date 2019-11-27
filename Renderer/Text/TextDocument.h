#pragma once

#include <string>
#include <vector>

// Primitive Stand-in for a real document class (TODO).
class TextDocument
{
public:
    TextDocument() { }

    void AppendLine(const std::string& line) { this->textLines.push_back(line); }
    const std::vector<std::string> GetLines() { return this->textLines;  }

private:
    std::vector<std::string> textLines;
};
