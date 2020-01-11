#pragma once

#include <string>
#include <vector>

class TextDocumentNodeChildEntry;

class TextDocumentNode
{
public:
    TextDocumentNode() : start(0) { }
    std::vector<TextDocumentNode*>& GetChildren() { return this->children; }
    std::string& GetContent() { return this->content; }
    unsigned int GetStart() { return this->start; }

private:
    unsigned int start;
    std::vector<TextDocumentNode*> children;
    std::string content;
};
