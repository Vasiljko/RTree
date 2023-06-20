#include <RTree.h>
#include <RTreeFactory.h>

#include <iostream>
using namespace std;

int main(){
    auto tree = *RTreeFactory::createRtree(2, 5);

    cout<<tree<<endl;
    return 0;
}