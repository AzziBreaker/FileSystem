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
    DirNode() = default;
    DirNode& operator=(const DirNode&);
    DirNode(const DirNode&);
    virtual ~DirNode() = default;

public:
    bool isDir() const override;
    void addChild(Node* node);
    void removeChild(Node* node);

public:
    Node* getChild(std::string&);
    Node* getChildFile(std::string&);
    std::vector<Node*> getChildren() const;
    inline unsigned getSize() const override {return 0;}

public:
    void print() const override;

private:
    std::vector<Node*> children;

};


#endif //FILESYSTEM_DIRNODE_H