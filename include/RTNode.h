#ifndef RTNODE_H
#define RTNODE_H

#include <Region.h>
#include <utility>
#include <memory>
#include <vector>

class RTNode
{
    using RTNode_ptr = std::unique_ptr<RTNode>;

    struct Entry
    {
        RTNode_ptr tree;
        Region region;

        Entry() : tree(std::move(std::unique_ptr<RTNode>(nullptr))), region() {}
        Entry(RTNode_ptr t, Region r) : tree(std::move(t)), region(r) {
            tree->mbr = r;
        }

        Entry(Entry &&e) : tree(std::move(e.tree)), region(e.region) {}

        Entry &operator=(Entry &&e)
        {
            if (this != &e)
            {
                tree = std::move(e.tree);
                region = e.region;
            }

            return *this;
        }

        

        ~Entry() = default;
    };

    int m, M;

    RTNode *parent;
    std::vector<Entry> entries;
    Region mbr;

    bool is_root, is_leaf;
    bool is_tree_node;

    RTNode() {}
    RTNode(std::nullptr_t) {}
    RTNode(int _m, int _M, bool root = false, RTNode *p = nullptr) : m(_m), M(_M), is_leaf(true), is_root(root), is_tree_node(true), parent(p)
    {}

    RTNode *find_leaf_node(RTNode *, Region &, int, int);

    int get_size() const { return M; }
    RTNode &get_child(int ind) const { return *entries[ind].tree; }

    void update_mbr();

    void print_node(std::ostream &, int, int) const;

    friend class RTree;
    friend struct Entry;
public:
    void insert(Region &, RTNode_ptr &, int&);
    void print_tree(std::ostream &, const RTNode &, int);
};

#endif
