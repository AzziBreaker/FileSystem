//
// Created by Denis Kiprov on 3.02.26.
//

#ifndef FILESYSTEM_NODE_H
#define FILESYSTEM_NODE_H

#include <string>

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
    virtual bool isDir() const = 0;
    void setCopy();

private:
    std::string name;
    unsigned id;
    bool isCopy;
};

#endif //FILESYSTEM_NODE_H