#ifndef RTREE_H
#define RTREE_H

#include <Region.h>
#include <utility>
#include <memory>
#include <vector>

class RTree
{
    using RTree_ptr = std::unique_ptr<RTree>;

    struct Entry{
        RTree_ptr tree;
        Region region;

        Entry():tree(std::move(std::unique_ptr<RTree>(nullptr))), region(){}
        Entry(RTree_ptr t, Region r):tree(std::move(t)), region(r){}

        Entry(Entry&& e) : tree(std::move(e.tree)), region(e.region){}

        Entry& operator=(Entry&& e){
            if(this != &e){
                tree = std::move(e.tree);
                region = e.region;
            }

            return *this;
        }

        Entry(const Entry&) = delete;
        Entry& operator=(const Entry&) = delete;
    };

    int m, M;
    int entries_cnt = 0; // number of entries with actual data
    std::vector<Entry> entries;
    Region mbr;
    static int h;

    bool is_root, is_leaf;
    bool is_tree_node;

    RTree(std::nullptr_t) {}
    RTree(int _m, int _M, bool root = false) : m(_m), M(_M), entries(std::move(std::vector<Entry>(M))), is_leaf(true), is_root(root), is_tree_node(true){
        h = 1;
    }

    /*Entry create_new_entry(Region& region){
        std::unique_ptr<RTree> new_Rtree_node(new RTree(m, M));
        return Entry(std::move(new_Rtree_node), region);
    }*/

    RTree& find_leaf_node(RTree&, Region&, int);

    int get_size() const { return M; }
    RTree &get_child(int ind) const { return *entries[ind].tree; }

    static void print_tree(std::ostream &, const RTree &);
    void print_node(std::ostream &) const;

    friend class RTreeFactory;
public:
    void insert(Region&);

    friend std::ostream &operator<<(std::ostream &os, const RTree &tree)
    {
        print_tree(os, tree);
        return os;
    }
};

#endif
