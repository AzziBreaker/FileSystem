//
// Created by Denis Kiprov on 9.02.26.
//

#ifndef FILESYSTEM_IDKEYONDISK_H
#define FILESYSTEM_IDKEYONDISK_H

const unsigned maxNameSizeToo = 32;

struct IDKeyOnDisk
{
    char name[maxNameSizeToo];
    unsigned ID;
    unsigned parentID;
    unsigned firstBlock;
    unsigned size;
    bool isDir;
};


#endif //FILESYSTEM_IDKEYONDISK_H