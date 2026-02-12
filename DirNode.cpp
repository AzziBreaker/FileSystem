//
// Created by Denis Kiprov on 3.02.26.
//

#include "DirNode.h"

#include <iostream>

bool DirNode::isDir() const
{
    return true;
}

void DirNode::addChild(Node* node)
{
    node->setParent(this);
    this->children.push_back(node);
}

void DirNode::removeChild(Node* node)
{
    std::vector<Node*>::iterator it = children.begin();
    while (it != children.end())
    {
        if (*it == node)
        {
            children.erase(it);
            return;
        }
        it++;
    }
}

Node* DirNode::getChild(std::string& name)
{
    if (this->children.empty())
        return nullptr;

    for (Node* node : this->children)
    {
        if (node->getName() == name && node->isDir())
        {
            return node;
        }
    }

    return nullptr;
}

Node* DirNode::getChildFile(std::string & name)
{
    if (this->children.empty())
        return nullptr;

    for (Node* node : this->children)
    {
        if (node->getName() == name)
        {
            return node;
        }
    }

    return nullptr;
}

std::vector<Node*> DirNode::getChildren() const
{
    return this->children;
}

void DirNode::print() const
{
    std::cout << this->getName() << ":\n";

    for (Node* node: children)
    {
        std::cout << "  --" << node->getName();
        if (node->isDir())
        {
            std::cout << "\n";
            continue;
        }
        std::cout << " - size: "<< node->getSize() << "\n";
    }
}
