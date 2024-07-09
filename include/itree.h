#ifndef INSERT_TREE_H_
#define INSERT_TREE_H_

#include <vector>

using namespace std;

template <class T, class Index>
class InsertTree
{
    public:

        using ItemVector = vector<T>;
        using IndexVector = vector<Index>;
        using IndexVectorVector = vector<IndexVector>;

        InsertTree();
        InsertTree(const InsertTree& rhs);

        Index add_vertex(T item, Index parent);
        Index get_children(Index parent, IndexVector& ids) const;
        Index get_num_children(Index parent) const { return children[parent].size(); }

        Index num_levels() const { return nlevels; }
        Index num_vertices() const { return vertices.size(); }
        Index get_level(Index id) const { return levels[id]; }
        T get_item(Index id) const { return vertices[id]; }

        bool operator==(const InsertTree& rhs) const;
        bool operator!=(const InsertTree& rhs) const { return !(*this == rhs); }

        template <class Record, class Op>
        void dfs(Index start, vector<Record>& records, Op op) const;

        template <class Iter>
        void add_vertices(Iter first, Iter last, Index parent)
        {
            for (; first != last; ++first)
                add_vertex(*first, parent);
        }

    private:

        ItemVector vertices;
        IndexVector levels;
        IndexVectorVector children;
        Index nlevels;
};

#include "itree.hpp"

#endif
