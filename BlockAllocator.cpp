//
// Created by Denis Kiprov on 11.02.26.
//

#include "BlockAllocator.h"

#include <ranges>


BlockAllocator::BlockAllocator(std::fstream &file, std::vector<bool>& usedTable, unsigned dataOffset)
    :dataFile(file),isUsed(usedTable),blockSize(BLOCK_SIZE),dataBlockOffset(dataOffset)
{

}

unsigned BlockAllocator::allocateBlock() {
    for (unsigned i = 0; i < isUsed.size(); i++)
    {
        if (!isUsed[i])
        {
            isUsed[i] = true;
            char buffer[BLOCK_SIZE] = {};
            dataFile.seekp(dataBlockOffset + i * blockSize, std::ios::beg);
            dataFile.write(buffer, blockSize);
            return i;
        }
    }
    std::cout << "There are no free blocks!\n";
    throw;
}

void BlockAllocator::freeBlockChain(unsigned firstBlock)
{
    unsigned current = firstBlock;
    while (current != 0xFFFFFFFF)
    {
        isUsed[current] = false;

        dataFile.seekg(dataBlockOffset + current * blockSize, std::ios::beg);
        unsigned next;
        dataFile.read(reinterpret_cast<char*>(&next), sizeof(unsigned));
        current = next;
    }
}

void BlockAllocator::writeBlock(unsigned block, unsigned nextBlock, const char* data, unsigned size)
{
    dataFile.seekp(dataBlockOffset + block * blockSize, std::ios::beg);
    dataFile.write(reinterpret_cast<const char*>(&nextBlock), sizeof(unsigned));
    dataFile.write(data, size);
}

unsigned BlockAllocator::createFile(const char* data, unsigned size)
{
    unsigned firstBlock = END;
    unsigned lastBlock = END;

    unsigned remaining = size;
    const char* currPos = data;

    while (remaining >0)
    {
        unsigned block = allocateBlock();
        if (firstBlock == END)
        {
            firstBlock = block;
        }

        unsigned toWrite = remaining;
        if (remaining > blockSize-sizeof(unsigned))
        {
            toWrite = blockSize-sizeof(unsigned);
        }

        writeBlock(block,END,currPos,toWrite);

        if (lastBlock!=END)
        {
            dataFile.seekp(dataBlockOffset + lastBlock * blockSize, std::ios::beg);
            dataFile.write(reinterpret_cast<const char*>(&block), sizeof(unsigned));
        }

        lastBlock = block;
        currPos += toWrite;
        remaining -= toWrite;
    }

    return firstBlock;
}

unsigned BlockAllocator::overrideFile(unsigned firstBlock, const char* data, unsigned size)
{
    freeBlockChain(firstBlock);

    unsigned newFirst = createFile(data,size);

    return newFirst;
}

void BlockAllocator::appendFile(unsigned firstBlock, const char* data, unsigned size)
{
    if (!data || size == 0) return;

    unsigned remaining = size;
    unsigned written = 0;
    unsigned currentBlock = firstBlock;


    if (currentBlock == END)
    {
        currentBlock = allocateBlock();
        firstBlock = currentBlock;
    }
    else
    {
        unsigned next;
        while (true) {
            dataFile.seekg(dataBlockOffset + currentBlock * blockSize, std::ios::beg);
            dataFile.read(reinterpret_cast<char*>(&next), sizeof(next));
            if (next == END) break;
            currentBlock = next;
        }
    }

    while (remaining > 0) {
        unsigned nextBlock = END;
        unsigned chunkSize = remaining;

        if (remaining > blockSize-sizeof(unsigned))
        {
            chunkSize = blockSize-sizeof(unsigned);
        }

        if (chunkSize < remaining) {
            nextBlock = allocateBlock();
        }

        dataFile.seekp(dataBlockOffset + currentBlock * blockSize, std::ios::beg);

        dataFile.write(reinterpret_cast<const char*>(&nextBlock), sizeof(nextBlock));

        dataFile.write(data + written, chunkSize);


        if (chunkSize < blockSize-sizeof(unsigned)) {
            std::vector<char> padding(blockSize-sizeof(unsigned) - chunkSize, 0);

            dataFile.write(padding.data(), padding.size());
        }

        written += chunkSize;
        remaining -= chunkSize;
        currentBlock = nextBlock;
    }

    dataFile.flush();
}

