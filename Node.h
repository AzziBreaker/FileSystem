//
// Created by Denis Kiprov on 3.02.26.
//

#ifndef FILESYSTEM_NODE_H
#define FILESYSTEM_NODE_H

#include <string>

class DirNode;

class Node
{
public:
    Node();
    Node(const Node& other);
    Node& operator=(const Node& other);
    virtual ~Node() = default;

public:
    std::string getName() const {return this->name;}
    unsigned getId() const {return this->id;}
    DirNode* getParent() const;

public:
    virtual bool isDir() const = 0;
    virtual void print() const = 0;
    virtual unsigned getSize() const = 0;

public:
    void setId(unsigned);
    void setName(std::string&);
    void setParent(DirNode*);

private:
    std::string name;
    DirNode* parent;
    unsigned id;
};

#endif //FILESYSTEM_NODE_H