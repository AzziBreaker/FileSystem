//
// Created by Denis Kiprov on 3.02.26.
//

#ifndef FILESYSTEM_FILENODE_H
#define FILESYSTEM_FILENODE_H
#include "Node.h"

class FileNode : public Node
{
public:
    FileNode() = default;
    FileNode& operator=(const FileNode&) = default;
    FileNode(const FileNode&) = default;
    virtual ~FileNode() = default;

public:
    unsigned getFirstBlock() const {return this->firstBlock;}
    unsigned getSize() const {return this->size;}
    bool isDir() const override;

public:
    void setFirstBlock(unsigned);
    void setSize(unsigned);

private:
    unsigned firstBlock;
    unsigned size;
};


#endif //FILESYSTEM_FILENODE_H