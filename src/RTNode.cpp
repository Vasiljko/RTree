#include <RTNode.h>
#include <queue>
#include <assert.h>


void RTNode::print_node(std::ostream& os, int cur_depth, int depth) const{
    if(cur_depth > depth)return;
    std::cout<<"size is "<<entries.size()<<"\n";
    for(int i=0;i<entries.size();i++){
        os<<entries[i].region<<" ";
    }
    os<<std::endl;

    for(int i=0;i<entries.size();i++){
        entries[i].tree->print_node(os, cur_depth + 1, depth);
    }
}

void RTNode::print_tree(std::ostream &os, const RTNode& tree, int depth){
    if(tree.entries.empty()){
        os<<"Tree is empty"<<std::endl;
        return;
    }

    tree.print_node(os, 1, depth);
}

void RTNode::update_mbr(){
    Region r = {};
    bool initialized_mbr = false;
    for(int i=0;i<entries.size();i++){
        if(!initialized_mbr) r = entries[i].tree->mbr;
        else r = Region::calculate_mbr(r, entries[i].tree->mbr);
        
        entries[i].region = entries[i].tree->mbr;
        initialized_mbr = true;
    }

    mbr = r;
    if(parent)parent->update_mbr();
}

RTNode* RTNode::find_leaf_node(RTNode* tree, Region& region, int cur_depth, int depth){
    if(cur_depth == depth)return tree;

    int least_increase = INT_MAX;
    int id = -1;

    for(int i=0;i<entries.size();i++){        
        int increase = entries[i].region.calculate_increase(region);

        if(increase < least_increase){
            least_increase = increase;
            id = i;
        }else if(increase == least_increase){
            id = entries[id].region.calculate_area() < entries[i].region.calculate_area() ? id : i;
        }
    }

    return find_leaf_node(entries[id].tree.get(), region, cur_depth+1, depth);
}

void RTNode::insert(Region& region, RTNode_ptr& root, int &depth){
    RTNode* leaf_node = find_leaf_node(root.get(), region, 1, depth);

    if(leaf_node->entries.size() < M){
        Entry entry(std::unique_ptr<RTNode>{new RTNode(m, M, false, leaf_node)}, region);

        assert(leaf_node == entry.tree->parent);


        leaf_node->entries.push_back(std::move(entry));
        leaf_node->update_mbr();

        for(int i=0;i<leaf_node->entries.size();i++){
            assert(leaf_node->entries[i].tree.get()  != nullptr && "leaf_node->entries[i].tree.get() is nullptr");
            assert(leaf_node->entries[i].tree.get()->parent  == leaf_node && "leaf_node->entries[i].tree.get()->parent is not equal to leaf_node");
        }
        
        return;
    }
    else{
        Entry entry{std::move(std::unique_ptr<RTNode>{new RTNode(m, M, 0, leaf_node)}), region};

        int L1, L2;
        long long area = 0;

        for(int i=0;i<M;i++){
            for(int j=i+1;j<M;j++){
                long long mbr_area = Region::calculate_intersection_area(leaf_node->entries[i].region, leaf_node->entries[j].region);
                if(mbr_area >= area){
                    area = mbr_area;
                    L1 = i;
                    L2 = j;
                }
            }
        }

        std::vector<Entry>old_entries = std::move(leaf_node->entries);
        old_entries.emplace_back(std::move(entry));

        leaf_node->entries.clear();

        RTNode_ptr new_node = RTNode_ptr(new RTNode(m, M, 0, leaf_node->parent));

        leaf_node->mbr = old_entries[L1].region;
        leaf_node->entries.emplace_back(std::move(old_entries[L1]));
        leaf_node->entries.back().tree->parent = leaf_node;
        
        new_node->mbr = old_entries[L2].region;
        new_node->entries.emplace_back(std::move(old_entries[L2]));
        new_node->entries.back().tree->parent = new_node.get();

        int remaining = old_entries.size()-2;
        for(int i=0;i<old_entries.size(); i++){
            if(i == L1 || i == L2)continue;

            if(leaf_node->entries.size() == m-remaining){
                leaf_node->mbr = Region::calculate_mbr(leaf_node->mbr, old_entries[i].region);
                leaf_node->entries.emplace_back(std::move(old_entries[i]));
                leaf_node->entries.back().tree->parent = leaf_node;
            }else if(new_node->entries.size() == m-remaining){
                new_node->mbr = Region::calculate_mbr(new_node->mbr, old_entries[i].region);
                new_node->entries.emplace_back(std::move(old_entries[i]));
                new_node->entries.back().tree->parent = new_node.get();
            }else{
                long long increase_L1 = leaf_node->mbr.calculate_increase(old_entries[i].region);
                long long increase_L2 = new_node->mbr.calculate_increase(old_entries[i].region);
                if(increase_L1 < increase_L2){
                    leaf_node->mbr = Region::calculate_mbr(leaf_node->mbr, old_entries[i].region);
                    leaf_node->entries.emplace_back(std::move(old_entries[i]));
                    leaf_node->entries.back().tree->parent = leaf_node;
                }else if(increase_L1 > increase_L2){
                    new_node->mbr = Region::calculate_mbr(new_node->mbr, old_entries[i].region);
                    new_node->entries.emplace_back(std::move(old_entries[i]));
                    new_node->entries.back().tree->parent = new_node.get();
                }else{
                    if(leaf_node->mbr.calculate_area() < new_node->mbr.calculate_area()){
                        leaf_node->mbr = Region::calculate_mbr(leaf_node->mbr, old_entries[i].region);
                        leaf_node->entries.emplace_back(std::move(old_entries[i]));
                        leaf_node->entries.back().tree->parent = leaf_node;
                    }else if(leaf_node->mbr.calculate_area() > new_node->mbr.calculate_area()){
                        new_node->mbr = Region::calculate_mbr(new_node->mbr, old_entries[i].region);
                        new_node->entries.emplace_back(std::move(old_entries[i]));
                        new_node->entries.back().tree->parent = new_node.get();
                    }else{
                        if(leaf_node->entries.size() < new_node->entries.size()){
                            leaf_node->mbr = Region::calculate_mbr(leaf_node->mbr, old_entries[i].region);
                            leaf_node->entries.emplace_back(std::move(old_entries[i]));
                            leaf_node->entries.back().tree->parent = leaf_node;
                        }else{
                            new_node->mbr = Region::calculate_mbr(new_node->mbr, old_entries[i].region);
                            new_node->entries.emplace_back(std::move(old_entries[i]));
                            new_node->entries.back().tree->parent = new_node.get();
                        }
                    }
                }
            }

            remaining--;
        }

        
        if(leaf_node->parent == nullptr){
            RTNode_ptr released_node = RTNode_ptr(root.release());

            std::unique_ptr<RTNode> parent_node = std::unique_ptr<RTNode>(new RTNode(m, M, true, nullptr)); 
            
            Region new_node_mbr = new_node->mbr;
            parent_node->entries.emplace_back(std::move(Entry(std::move(RTNode_ptr{new_node.release()}), new_node_mbr)));

            Region released_node_mbr = released_node->mbr;
            parent_node->entries.emplace_back((std::move(Entry(std::move(released_node), released_node_mbr))));
            
            for(int i=0;i<parent_node->entries.size();i++){
                parent_node->entries[i].tree->parent = parent_node.get();
                parent_node->entries[i].tree->is_leaf = false;
            }

            parent_node->update_mbr();
            root = std::move(parent_node); 
            depth++;
        }else{
            
        }    

    }
    
}