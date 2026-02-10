//
// Created by Denis Kiprov on 3.02.26.
//

#ifndef FILESYSTEM_IDKEY_H
#define FILESYSTEM_IDKEY_H

#include <string>
#include <iostream>

#include "IDKeyOnDisk.h"

class IDKey
{
public:
    IDKey() = default;
    IDKey(unsigned id, unsigned parentId,unsigned firstBlock,unsigned size, bool dir, std::string&);
    IDKey(const IDKeyOnDisk&);
    IDKey(const IDKey&) = default;
    IDKey& operator=(const IDKey&) = default;
    ~IDKey() = default;

public:
    IDKeyOnDisk toDisk() const;

public:
    std::string name;
    unsigned ID;
    unsigned parentID;
    unsigned firstBlock;
    unsigned size;
    bool isDir;
};


#endif //FILESYSTEM_IDKEY_H