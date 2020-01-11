#include "TextDocumentNode.h"
#include "TextMoment.h"
#include "TextLine.h"
#include <cmath>

TextMoment::TextMoment()
    : lineCount(0),
    rootNode(new TextDocumentNode())
{
}

TextMoment::TextMoment(TextDocumentNode* rootNode, unsigned int lineCount)
    : lineCount(lineCount),
    rootNode(rootNode)
{
}

char TextMoment::GetChar(unsigned int index) const
{
    return '\0';
}

unsigned int TextMoment::GetLength() const
{
    auto rootEntry = this->GetRootNode();

    auto& rootChildren = rootEntry->GetChildren();

    if (rootChildren.size() > 0)
    {
        // TODO: use end iterator?
        return rootChildren[rootChildren.size() - 1]->GetStart();
    }
    else
    {
        return rootEntry->GetContent().size();
    }
}

TextLine* TextMoment::GetLine(unsigned int lineNumber) const
{
    return nullptr;
}

std::vector<TextDocumentNode*> TextMoment::GetNodesContainingPosition(unsigned int position) const
{
    std::vector<TextDocumentNode*> containingNodes;
    TextDocumentNode* currentNode = this->GetRootNode();

    unsigned int l = 0;
    unsigned int r = currentNode->GetChildren().size() - 1;

    while (l <= r)
    {
        unsigned int c = static_cast<unsigned int>(std::floor(l + r / 2));
        auto& currentNodeChildren = currentNode->GetChildren();

        // This is a leaf node, return it.
        if (currentNodeChildren.size() == 0)
        {
            containingNodes.push_back(currentNode);
            return containingNodes;
        }

        TextDocumentNode* child = currentNodeChildren[c];

        if (child->GetStart() >= position)
        {
            // We may have found the containing node. Go down a level.
            if ((currentNodeChildren.size() - 1 - c) > 0 &&
                currentNodeChildren[c + 1]->GetStart() < position)
            {
                containingNodes.push_back(currentNode);
                currentNode = child;
            }
            else
            {
                r = c - 1;
            }
        }
        else if (child->GetStart() < position)
        {
            l = position;
        }
    }

    return containingNodes;
}