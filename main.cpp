#include <iostream>
#include <fstream>

#include "FileMetadata.h"

std::string addBin(const std::string& name)
{
    if (name.size() >= 4 &&
        name.substr(name.size() - 4) == ".bin")
        return name;

    return name + ".bin";
}

int main()
{
    FileMetadata* fileMetadata = nullptr;
    FileMetadataOnDisk diskMetadata;
    std::cout << "Hello! Which file system would you like to open? :)\n";
    std::string fsFilename;

    //slagam filesystem metadata

    //TODO: dali e validno
    std::cin >> fsFilename;

    fsFilename = addBin(fsFilename);

    std::fstream fs;

    fs.open(fsFilename, std::ios::in | std::ios::out | std::ios::binary);

    if (!fs.is_open())
    {

        fs.open(fsFilename,std::ios::out | std::ios::binary | std::ios::trunc);

        if (!fs)
        {
            std::cerr << "Failed to create filesystem file\n";
            return 1;
        }

        std::cout << fsFilename << " opened successfully!\n";
        std::cout << "What is the file max size?\n";

        unsigned maxSize;
        std::cin >> maxSize;

        fileMetadata = new FileMetadata(fsFilename,maxSize);

        //initializeFileSystem(fs);
    }
    else
    {
        try
        {
            fs.read(reinterpret_cast<char*>(&diskMetadata),sizeof(FileMetadataOnDisk));
            fileMetadata = new FileMetadata(diskMetadata);
        } catch (...)
        {
            std::cout << "File reading problem\n";
        }
    }


    fileMetadata->print(std::cout);

    std::string command;

    FileMetadataOnDisk disk = fileMetadata->toDisk();
    fs.write(reinterpret_cast<const char*>(&disk),sizeof(disk));


    fs.close();
    delete fileMetadata;
}
