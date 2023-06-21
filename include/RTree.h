#ifndef RTREE_H
#define RTREE_H

#include <RTNode.h>

#include <memory>

class RTree{
    using RTNode_ptr = std::unique_ptr<RTNode>;

    int m, M, h = 1;
    RTNode_ptr root;

    RTree(){}
    RTree(std::nullptr_t) {}
    RTree(int _m, int _M) : m(_m), M(_M), root(std::move(RTNode_ptr(new RTNode(m, M, true, nullptr)))){}

    friend class RTreeFactory;
public:
    void insert(std::pair<int, int> point){
        root->insert(Region{{point.first, point.second}, {point.first, point.second}}, root, h);
    }

    std::vector<Region> search(Region r){
        return root->search(r, root, h);
    }

    friend std::ostream &operator<<(std::ostream &os, const RTree& tree)
    {
        tree.root->print_tree(os, *tree.root.get(), tree.h);
        return os;
    }
};

#endif