//
// Created by Denis Kiprov on 3.02.26.
//

#include "Node.h"

Node::Node()
{

}

Node::Node(const Node &other)
{

}

Node& Node::operator=(const Node &other)
{

}

DirNode* Node::getParent() const
{
    return this->parent;
}

void Node::setId(unsigned newId)
{
    this->id = newId;
}

void Node::setName(std::string& newName)
{
    this->name = newName;
}

void Node::setCopy()
{
    this->isCopy = true;
}

void Node::setParent(DirNode* parent)
{
    this->parent = parent;
}
