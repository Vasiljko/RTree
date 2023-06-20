#include <RTree.h>
#include <queue>

int RTree::h = 0;

void RTree::print_node(std::ostream& os) const{
    for(int i=0;i<M;i++){
        os<<entries[i].region<<" ";
    }
    os<<std::endl;
}

void RTree::print_tree(std::ostream &os, const RTree& tree){

    tree.print_node(os);
    
    /*for(int i=0;i<tree.get_size();i++){
        print_tree(os, tree.get_child(i));
    }*/

}

RTree& RTree::find_leaf_node(RTree& rtree, Region& region, int depth){
    if(depth == h)return rtree;

    int least_increase = INT_MAX;
    int id = -1;
    for(int i=0;i<entries_cnt;i++){
        if(!region.intersect(entries[i].region))continue;
        
        int increase = entries[i].region.calculate_increase(region);

        if(increase < least_increase){
            least_increase = increase;
            id = i;
        }else if(increase == least_increase){
            id = entries[id].region.calculate_area() < entries[i].region.calculate_area() ? id : i;
        }
    }

    return find_leaf_node(*entries[id].tree, region, depth+1);
}

void RTree::insert(Region& region){
    RTree leaf_node = find_leaf_node(*this, region, 1);

    if(leaf_node.entries_cnt < M){
        Entry entry{std::unique_ptr<RTree>{new RTree(m, M)}, region};

        return;
    }
}