//
// Created by Denis Kiprov on 9.02.26.
//

#ifndef FILESYSTEM_FILEMETADATAONDISK_H
#define FILESYSTEM_FILEMETADATAONDISK_H

const unsigned maxNameSize = 32;

struct FileMetadataOnDisk
{
    char name[maxNameSize];
    unsigned maxSize;
    unsigned blockSize;
    unsigned idKeys;
    unsigned idKeyOffset;
    unsigned isUsedOffset;
    unsigned dataBlocks;
    unsigned dataBlockOffset;
};


#endif //FILESYSTEM_FILEMETADATAONDISK_H