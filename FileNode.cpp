//
// Created by Denis Kiprov on 3.02.26.
//

#include "FileNode.h"

bool FileNode::isDir() const
{
    return false;
}

void FileNode::setFirstBlock(unsigned block)
{
    this->firstBlock = block;
}

void FileNode::setSize(unsigned size)
{
    this->size = size;
}
