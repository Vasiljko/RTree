#include <RTNode.h>
#include <RTreeFactory.h>

#include <iostream>
#include <assert.h>
using namespace std;

int main(int argc, char* argv[]){
    auto tree = std::move(RTreeFactory::createRtree(2, 5));
    int n = atoi(argv[1]);

    std::vector<Region>r(n);
    for(int i=0;i<n;i++){
        r[i] = {{2*i, 2*i}, {2*i+1, 2*i+1}};
    }

    for(int i=0;i<n;i++){
        tree->insert(r[i]);
    }

    cout<<*tree<<endl;

    return 0;
}