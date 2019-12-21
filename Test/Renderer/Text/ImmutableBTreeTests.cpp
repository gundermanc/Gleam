#include "gtest/gtest.h"

#include "../../../Renderer/Text/ImmutableBTree.h"

TEST(ImmutableBTreeTests, EmptyTree) {
    ImmutableBTree<char> immutableBTree;
}