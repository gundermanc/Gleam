#include "TextDocument.h"

// HACK: just iterate through for now.
// Eventually replace with a smarter data structure (like a tree) or a binary search
// and replace this method with multiple constant-time lookup methods for this info.
std::tuple<unsigned int, unsigned int> TextDocument::GetPositionAndLineNumberOfLineContainingPosition(unsigned int position)
{
    if (position >= this->GetLength())
    {
        return std::tuple<unsigned int, unsigned int>(-1, -1);
    }

    unsigned int startPosition = 0;
    unsigned int lineNumber = 0;

    for (auto& line : this->GetLines())
    {
        if (position >= startPosition && position < (startPosition + line.size()))
        {
            return std::tuple<unsigned int, unsigned int>(startPosition, lineNumber);
        }

        // NOTE: this doesn't yet account for line endings.
        startPosition += line.size();
        lineNumber++;
    }
}

void TextDocument::Insert(const std::string& text, unsigned int position)
{
    auto positionAndLineNumber = this->GetPositionAndLineNumberOfLineContainingPosition(position);

    auto startPosition = std::get<0>(positionAndLineNumber);
    auto lineNumber = std::get<1>(positionAndLineNumber);

    if (lineNumber != -1)
    {
        auto& insertionLine = this->textLines.at(lineNumber);

        this->length += text.size();

        insertionLine.insert(position - startPosition, text);
    }
}

void TextDocument::Remove(unsigned int position, unsigned int length)
{
    auto positionAndLineNumber = this->GetPositionAndLineNumberOfLineContainingPosition(position);

    auto startPosition = std::get<0>(positionAndLineNumber);
    auto lineNumber = std::get<1>(positionAndLineNumber);

    if (lineNumber != -1)
    {
        auto& insertionLine = this->textLines.at(lineNumber);

        this->length -= length;

        insertionLine.erase(position - startPosition, length);
    }
}
