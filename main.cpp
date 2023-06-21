#include <Region.h>
#include <RTreeFactory.h>

#include <iostream>
#include <assert.h>
using namespace std;

int main(int argc, char* argv[]){
    auto tree = std::unique_ptr<RTree>();

    while(true){
        std::cout<<"0. Exit\n";
        std::cout<<"1. Input from file\n";
        std::cout<<"2. Input from console\n";
        std::cout<<"3. Print tree\n";
        std::cout<<"4. Insert point\n";
        std::cout<<"5. Query\n";
        
        int choice;
        std::cin>>choice;

        if(choice == 0){
            break;
        }else if(choice == 1){
            std::string filepath;
            int m, M;
            
            std::cout<<"Input m and M\n";
            std::cin>>m>>M;

            tree = std::move(RTreeFactory::createRtree(m, M));
            if(!tree){
                std::cout<<"Failed creation of R Tree, check m and M\n";
                continue;
            }

            std::cout<<"Input relative filepath\n";
            std::cin>>filepath;

            tree->construct_tree_from_file(filepath);
        }else if(choice == 2){
            int m, M;
            
            std::cout<<"Input m and M\n";
            std::cin>>m>>M;

            tree = std::move(RTreeFactory::createRtree(m, M)); 
            if(!tree){
                std::cout<<"Failed creation of R Tree, check m and M\n";
                continue;
            }
            int x, y;
            std::cout<<"Input point coordinates, -1 to stop insertion\n";
            while(true){
                std::cin>>x;
                if(x == -1)break;
                std::cin>>y;

                tree->insert({x, y});
            }
        }else if(choice == 3){
            if(!tree){
                std::cout<<"R Tree is not formed\n";
                continue;
            }
            std::cout<<*tree<<"\n";
        }else if(choice == 4){
            if(!tree){
                std::cout<<"R Tree is not formed\n";
                continue;
            }
            int x,y;
            std::cout<<"Insert x and y\n";
            std::cin>>x>>y;

            tree->insert({x, y});
        }else if(choice == 5){
            if(!tree){
                std::cout<<"R Tree is not formed\n";
                continue;
            }

            int x1, y1, x2, y2;

            std::cout<<"Insert upper-left point (x1, y1) and bottom-right point (x2, y2)\n";
            std::cin>>x1>>y1>>x2>>y2;

            std::cout<<"Points in given rectangle:\n";
            for(auto e : tree->search({{x1, y1}, {x2,y2}})){
                std::cout<<e.get_upper_left().first<<" "<<e.get_upper_left().second<<"\n";
            }
            std::cout<<"\n";
        }
    }

    return 0;
}