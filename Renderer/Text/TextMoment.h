#pragma once

#include "TextDocumentNode.h"
#include <vector>

class TextLine;

class TextMoment
{
public:
    TextMoment();
    TextMoment(TextDocumentNode* rootNode, unsigned int lineCount);

    ~TextMoment() { delete this->rootNode; }
    char GetChar(unsigned int index) const;
    unsigned int GetLength() const;
    TextLine* GetLine(unsigned int lineNumber) const;
    unsigned int GetLineCount() const { return this->lineCount; }
    std::vector<TextDocumentNode*> GetNodesContainingPosition(unsigned int position) const;
    TextDocumentNode* GetRootNode() const { return this->rootNode; }

private:
    TextDocumentNode* rootNode;
    unsigned int lineCount;
};