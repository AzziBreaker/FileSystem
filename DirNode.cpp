//
// Created by Denis Kiprov on 3.02.26.
//

#include "DirNode.h"

bool DirNode::isDir() const
{
    return true;
}

void DirNode::addChild(Node* node)
{
    node->setParent(this);
    this->children.push_back(node);
}

Node* DirNode::getChild(std::string& name)
{
    for (Node* node : this->children)
    {
        if (node->getName() == name && node->isDir())
        {
            return node;
        }
    }

    return nullptr;
}
