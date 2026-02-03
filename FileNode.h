//
// Created by Denis Kiprov on 3.02.26.
//

#ifndef FILESYSTEM_FILENODE_H
#define FILESYSTEM_FILENODE_H
#include "Node.h"

class FileNode : public Node
{
public:
    FileNode();
    FileNode& operator=(const FileNode&);
    FileNode(const FileNode&);
    virtual ~FileNode() override;

public:
    unsigned getFirstBlock() const {return this->firstBlock;}
    unsigned getSize() const {return this->size;}

private:
    unsigned firstBlock;
    unsigned size;
};


#endif //FILESYSTEM_FILENODE_H