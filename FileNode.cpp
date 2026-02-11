//
// Created by Denis Kiprov on 3.02.26.
//

#include "FileNode.h"

#include "DataBlockOnDisk.h"

FileNode::FileNode()
{
    this->firstBlock = END;
}

bool FileNode::isDir() const
{
    return false;
}

void FileNode::print() const
{

}

void FileNode::setFirstBlock(unsigned block)
{
    this->firstBlock = block;
}

void FileNode::setSize(unsigned size)
{
    this->size = size;
}
