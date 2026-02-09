//
// Created by Denis Kiprov on 3.02.26.
//

#include "IDKey.h"

IDKey::IDKey(unsigned id, unsigned parentId, unsigned firstBlock, unsigned size, bool dir, std::string& name):
ID(id),parentID(parentId),firstBlock(firstBlock),size(size), isDir(dir), name(name)
{

}

IDKey::IDKey(const IDKeyOnDisk& d)
{
    this->ID = d.ID;
    this->parentID = d.parentID;
    this->firstBlock = d.firstBlock;
    this->size = d.size;
    this->isDir = d.isDir;

    //fix this?
    this->name = std::string(d.name,strnlen(d.name,maxNameSizeToo));
}

IDKeyOnDisk IDKey::toDisk() const
{
    IDKeyOnDisk d{};

    std::strncpy(d.name, name.c_str(), maxNameSizeToo - 1);
    d.name[maxNameSizeToo - 1] = '\0';

    d.ID = ID;
    d.parentID = parentID;
    d.firstBlock = firstBlock;
    d.size = size;
    d.isDir = isDir;

    return d;
}
