//
// Created by Denis Kiprov on 11.02.26.
//

#ifndef FILESYSTEM_DATABLOCKONDISK_H
#define FILESYSTEM_DATABLOCKONDISK_H

const unsigned BLOCK_SIZE = 1024;
const unsigned END = 0xFFFFFFFF;

struct DataBlockOnDisk
{
    unsigned nextBlock;
    char data[BLOCK_SIZE-sizeof(unsigned)];
};


#endif //FILESYSTEM_DATABLOCKONDISK_H