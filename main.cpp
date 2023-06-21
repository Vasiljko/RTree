#include <Region.h>
#include <RTreeFactory.h>

#include <iostream>
#include <assert.h>
using namespace std;

int main(int argc, char* argv[]){
    auto tree = std::move(RTreeFactory::createRtree(2, 5));
    int n = atoi(argv[1]);

    std::vector<pair<int,int> >r(n);
    for(int i=0;i<n;i++){
        r[i] = {2*i, 2*i+1};
    }

    for(int i=0;i<n;i++){
        tree->insert(r[i]);
    }

    cout<<*tree<<endl;

    while(true){
        int a, b, c, d;
        cin>>a;
        if(a == -1)break;

        cin>>b>>c>>d;
        
        for(auto e : tree->search({{a,b},{c,d}})){
            std::cout<<e<<"\n";
        }
        std::cout<<"\n\n";
    }
    return 0;
}