//
// Created by Denis Kiprov on 6.02.26.
//

#include "FileMetadata.h"

#include "IDKey.h"

FileMetadata::FileMetadata()
{
    this->name = "null";
    this->maxSize = 0;
    this->blockSize = 0;
    this->dataBlocks = 0;
    this->idKeyOffset = 0;
    this->dataBlockOffset = 0;
}

FileMetadata::FileMetadata(std::string& name, unsigned maxSize)
{
    this->name = name;
    this->maxSize = maxSize;
    //hardcoded for now
    this->blockSize = 512;
    this->idKeyOffset = sizeof(FileMetadata);
    this->idKeys = 0;
    this->dataBlockOffset = idKeyOffset + idKeys * sizeof(IDKey);
    this->dataBlocks = 0;
}

FileMetadata::FileMetadata(const FileMetadataOnDisk& d)
{
    this->name = d.name;
    this->maxSize = d.maxSize;
    this->blockSize = d.blockSize;
    this->idKeys = d.idKeys;
    this->idKeyOffset = d.idKeyOffset;
    this->dataBlocks = d.dataBlocks;
    this->dataBlockOffset = d.dataBlockOffset;

}

void FileMetadata::print(std::ostream& out) const
{
    out << this->name << ":\n MaxSize: "<< maxSize << "\n BlockSize: " << blockSize<<"\n DataBlocks: " << dataBlocks << "\n";
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
        d.dataBlocks = dataBlocks;
        d.dataBlockOffset = dataBlockOffset;

        return d;
}
