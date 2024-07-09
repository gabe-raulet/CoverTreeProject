#include <iostream>
#include <string>
#include <stdio.h>
#include <assert.h>
#include "itree.h"

using namespace std;

using T = string;
using Index = int64_t;
using Tree = InsertTree<T, Index>;

struct MyOp
{
    using Record = tuple<T, Index, Index>; // string, level, num_children

    Record operator()(Index id, const Tree& tree)
    {
        return {tree.get_item(id), tree.get_level(id), tree.get_num_children(id)};
    }
};

using Record = typename MyOp::Record;

int main(int argc, char *argv[])
{
    Tree tree1, tree2;

    tree1.add_vertex("foo", -1);
    tree1.add_vertex("bar", 0);
    tree1.add_vertex("ya", 1);
    tree1.add_vertex("row", 0);
    tree1.add_vertex("quatre", 1);

    tree2.add_vertex("foo", -1);
    tree2.add_vertex("row", 0);
    tree2.add_vertex("bar", 0);
    tree2.add_vertex("quatre", 2);
    tree2.add_vertex("ya", 2);

    assert(tree1 == tree2);
    return 0;
}
