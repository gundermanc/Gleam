#include "TextDocument.h"
#include "TextDocumentNode.h"
#include "TextUtils.h"
#include <cassert>
#include <sstream>

TextDocument::TextDocument() : length(0), currentMoment(new TextMoment()), preferredLineEnding("\r\n")
{
}

//TextDocument::~TextDocument()
//{
//    delete this->rootNode.Node;
//}

// HACK: just iterate through for now.
// Eventually replace with a smarter data structure (like a tree) or a binary search
// and replace this method with multiple constant-time lookup methods for this info.
//std::tuple<unsigned int, unsigned int> TextDocument::GetPositionAndLineNumberOfLineContainingPosition(unsigned int position)
//{
    //return std::tuple<unsigned int, unsigned int>(0, 0);
    //if (position >= this->GetLength())
    //{
    //    return std::tuple<unsigned int, unsigned int>(-1, -1);
    //}

    //unsigned int startPosition = 0;
    //unsigned int lineNumber = 0;

    //for (auto& line : this->GetLines())
    //{
    //    if (position >= startPosition && position < (startPosition + line.size()))
    //    {
    //        return std::tuple<unsigned int, unsigned int>(startPosition, lineNumber);
    //    }

    //    // NOTE: this doesn't yet account for line endings.
    //    startPosition += line.size();
    //    lineNumber++;
    //}
//}

void TextDocument::Insert(const std::string& text, unsigned int position)
{
    auto nodes = this->currentMoment->GetNodesContainingPosition(position);

    auto currentMoment = this->GetCurrentMoment();

    // TODO: atomic synchronize.
    TextMoment* newMoment = new TextMoment(
        currentMoment->GetRootNode(),
        currentMoment->GetLineCount());

    auto containingNodes = newMoment->GetNodesContainingPosition(position);

    // Get the current text of the line.
    std::string currentLineText;
    unsigned int insertionStart = 0;
    if (containingNodes.size() > 0)
    {
        currentLineText = containingNodes[containingNodes.size() - 1]->GetContent();
        insertionStart = containingNodes[containingNodes.size() - 1]->GetStart();
    }

    // Create the new text of the line.
    currentLineText = currentLineText.insert(position - insertionStart, text);

    // Remove the last node, we're going to replace it.
    containingNodes.pop_back();

    // Add in the lines one by one as sub-nodes of the current node.
    for (size_t currentLineStart = 0; currentLineStart < text.size();)
    {
        size_t endOfLine = text.find_first_of(LineBreakCharacters, currentLineStart);

        // TODO: is this right?
        if (endOfLine != -1)
        {
            // Insert line text to existing node.
            auto nodeStart = node->GetStart();
            node->GetContent().insert(position - nodeStart, text);

            // TODO: insert linebreak?

            currentLineStart = endOfLine + 2; // TODO: should be length of line-break.
            node = new TextDocumentNode();
        }
        else
        {
            currentLineStart = text.size();
        }
    }

    // TODO: atomic insertion.
    // TODO: cap insertion to max node length.
    // TODO: sift up to containing nodes.
    auto nodeStart = node->GetStart();
    node->GetContent().insert(position - nodeStart, text);
}

void TextDocument::Remove(unsigned int position, unsigned int length)
{
    TextDocumentNode* node = this->currentMoment->GetNodeContainingPosition(position);
    if (node == nullptr)
    {
        // TODO: log.
        assert(false);
        return;
    }
    else
    {
        // TODO: atomic insertion.
        // TODO: delete across nodes.
        // TODO: sift up to containing nodes.
        auto nodeStart = node->GetStart();
        auto changeStart = position - nodeStart;
        node->GetContent().replace(changeStart, changeStart + length, "");
    }
}

std::string TextDocument::ToString()
{
    std::stringstream stream;
    std::vector<TextDocumentNode*> nodes;

    // Push the root.
    nodes.push_back(this->currentMoment->GetRootNode());

    // Walk all the nodes.
    while (!nodes.empty())
    {
        // TODO: via iterator?
        TextDocumentNode* node = nodes[nodes.size()-1];
        nodes.pop_back();

        // Add current node's children.
        for (auto childNode : node->GetChildren())
        {
            nodes.push_back(childNode);
        }

        // Print current node's text.
        for (char c : node->GetContent())
        {
            stream << c;
        }
    }

    return stream.str();
}
