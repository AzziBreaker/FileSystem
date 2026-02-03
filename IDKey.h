//
// Created by Denis Kiprov on 3.02.26.
//

#ifndef FILESYSTEM_IDKEY_H
#define FILESYSTEM_IDKEY_H

#include <string>

struct IDKey
{
    std::string name;
    unsigned ID;
    unsigned parentID;
    unsigned firstBlock;
    unsigned size;
    bool isDir;

    IDKey(unsigned id, unsigned parentId,unsigned firstBlock,unsigned size, bool dir):
    ID(id),parentID(parentId),firstBlock(firstBlock),size(size), isDir(dir)
    {};
};


#endif //FILESYSTEM_IDKEY_H