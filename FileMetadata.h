//
// Created by Denis Kiprov on 6.02.26.
//

#ifndef FILESYSTEM_FILEMETADATA_H
#define FILESYSTEM_FILEMETADATA_H

#include <string>
#include <fstream>
#include <iostream>

#include "FileMetadataOnDisk.h"

class FileMetadata
{
public:
    FileMetadata();
    FileMetadata(std::string&, unsigned);
    explicit FileMetadata(const FileMetadataOnDisk&);
    ~FileMetadata() = default;

public:
    void print(std::ostream&) const;
    FileMetadataOnDisk toDisk() const;

public:
    std::string name;
    unsigned maxSize;
    unsigned blockSize;
    unsigned idKeys;
    unsigned idKeyOffset;
    unsigned dataBlocks;
    unsigned dataBlockOffset;

};


#endif //FILESYSTEM_FILEMETADATA_H