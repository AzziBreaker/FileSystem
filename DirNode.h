//
// Created by Denis Kiprov on 3.02.26.
//

#ifndef FILESYSTEM_DIRNODE_H
#define FILESYSTEM_DIRNODE_H

#include <vector>

#include "Node.h"


class DirNode : public Node
{
public:
    DirNode();
    DirNode& operator=(const DirNode&);
    DirNode(const DirNode&);
    virtual ~DirNode() override;


    //TODO::izmisli kakvo kude se dobavq (addchild i getchild)
public:


private:
    std::vector<unsigned> children;
};


#endif //FILESYSTEM_DIRNODE_H