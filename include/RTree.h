#ifndef RTREE_H
#define RTREE_H

#include "RTNode.h"

#include <memory>
#include <fstream>
#include <cstring>

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

    void construct_tree_from_file(std::string filepath){
        std::ifstream ifs(filepath);

        if(!ifs)
            throw std::runtime_error(filepath + ": " + std::strerror(errno));

        int f, s;
        while(ifs >> f >> s){
            insert(std::make_pair(f, s));
        }
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