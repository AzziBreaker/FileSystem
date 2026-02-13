//
// Created by Denis Kiprov on 6.02.26.
//

#include "FileMetadata.h"

#include "DataBlockOnDisk.h"
#include "IDKey.h"

FileMetadata::FileMetadata()
{
    this->name = "null";
    this->maxSize = 0;
    this->blockSize = 0;
    this->dataBlocks = 0;
    this->idKeyOffset = 0;
    this->isUsedOffset = 0;
    this->dataBlockOffset = 0;
}

FileMetadata::FileMetadata(const std::string& name, unsigned maxSize)
{

    this->name = name;
    this->maxSize = maxSize;
    //hardcoded for now
    this->blockSize = BLOCK_SIZE;
    this->idKeyOffset = sizeof(FileMetadataOnDisk);
    this->idKeys = 1;

    this->maxIDKeys = maxSize / blockSize;

    //std::cout << dataBlocks << "\n";

    this->isUsedOffset = idKeyOffset + maxIDKeys*sizeof(IDKeyOnDisk);
    this->dataBlockOffset = isUsedOffset + (maxSize / blockSize);
    this->dataBlocks = (maxSize - dataBlockOffset) / blockSize;

    if (maxSize <= dataBlockOffset+BLOCK_SIZE)
    {
        std::cout << "Max size too small for header + tables + at least one block\n";
        throw std::invalid_argument(":c");
    }
}

FileMetadata::FileMetadata(const FileMetadataOnDisk& d)
{
    //std::cout << d.name;
    this->name = d.name;
    this->maxSize = d.maxSize;
    this->blockSize = d.blockSize;
    this->idKeys = d.idKeys;
    this->idKeyOffset = d.idKeyOffset;
    this->isUsedOffset = d.isUsedOffset;
    this->dataBlocks = d.dataBlocks;
    this->dataBlockOffset = d.dataBlockOffset;
    this->maxIDKeys = maxSize / 4096;

}

void FileMetadata::print(std::ostream& out) const
{
    out << this->name << ":\n"
    << "  MaxSize: " << maxSize << "\n"
    << "  BlockSize: " << blockSize << "\n"
    << "  DataBlocks: " << dataBlocks << "\n"
    << "  IdKeys: " << idKeys << "\n"
    << "  MaxIDKeys: " << maxIDKeys << "\n"
    << "  IdKeyOffset: " << idKeyOffset << "\n"
    << "  IsUsedOffset: " << isUsedOffset << "\n"
    << "  DataBlockOffset: " << dataBlockOffset << "\n";
}

FileMetadataOnDisk FileMetadata::toDisk() const
{
        FileMetadataOnDisk d{};

        std::strncpy(d.name, name.c_str(), maxNameSize - 1);
        d.name[maxNameSize - 1] = '\0';

        d.maxSize = maxSize;
        d.blockSize = blockSize;
        d.idKeys = idKeys;
        d.idKeyOffset = idKeyOffset;
        d.isUsedOffset = isUsedOffset;
        d.dataBlocks = dataBlocks;
        d.dataBlockOffset = dataBlockOffset;

        return d;
}
