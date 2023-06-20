#ifndef RTREEFACTORY_H
#define RTREEFACTORY_H

#include <RTree.h>
#include <memory>

class RTreeFactory{
    RTreeFactory() = default;
public:
    static std::unique_ptr<RTree> createRtree(int m, int M){
        if(m > M/2){
            return std::unique_ptr<RTree>(nullptr);
        }

        return std::unique_ptr<RTree>(new RTree(m, M, true));
    }

};

#endif