#pragma once

#include <vector>

template <typename TValue>
class ImmutableBTreeNode
{
public:
    ImmutableBTreeNode() { }

private:
    std::vector<ImmutableBTreeNode> branches;
    std::vector<TValue> leaves;
};