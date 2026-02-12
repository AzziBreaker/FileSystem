//
// Created by Denis Kiprov on 11.02.26.
//

#ifndef FILESYSTEM_BLOCKALLOCATOR_H
#define FILESYSTEM_BLOCKALLOCATOR_H
#include <fstream>

#include "DataBlockOnDisk.h"
#include <iostream>

class BlockAllocator
{
public:
    BlockAllocator(std::fstream& file, std::vector<bool>& usedTable, unsigned dataOffset);

public:
    unsigned allocateBlock();
    void freeBlockChain(unsigned firstBlock);
    void writeBlock(unsigned block, unsigned nextBlock, const char* data, unsigned size);

public:
    unsigned createFile(const char* data, unsigned size);
    unsigned overrideFile(unsigned firstBlock, const char* data, unsigned size);
    void appendFile(unsigned firstBlock, const char* data, unsigned size, unsigned);
    std::vector<char> readFile(unsigned firstBlock);

private:
    std::fstream& dataFile;
    std::vector<bool>& isUsed;
    unsigned blockSize;
    unsigned dataBlockOffset;
};


#endif //FILESYSTEM_BLOCKALLOCATOR_H