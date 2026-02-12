//
// Created by Denis Kiprov on 9.02.26.
//

#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include <sstream>

#include "FileMetadata.h"
#include "IDKey.h"
#include "DirNode.h"
#include "FileNode.h"
#include "BlockAllocator.h"


class FileSystem
{
public:
    FileSystem(const std::string& fileName);
    ~FileSystem();

public:
    void addKey(const IDKey&, const IDKey&);
    void save();

private:
    void createEmptyFileSystem(const std::string&);


public:
    void mkdir(std::string& path);
    void rmdir(std::string& path);
    void ls(std::string& path);
    void cd(std::string& path);
    void cp(std::string& source, std::string& destination);
    void rm(std::string& source);
    void cat(std::string& source);
    void import(std::string& source, std::string& destination, std::string& param);



    //change to private and add getters setters
public:
    FileMetadata* fileMetadata;
    BlockAllocator* allocator;
    std::unordered_map<unsigned, IDKey> idKeys;
    std::vector<bool> isUsed;
    std::fstream dataFile;
    DirNode* root;
    DirNode* currDir;
    unsigned nextID;
    unsigned idKeysCount;

private:
    void loadIsUsedTable();
    void buildTree();

private:
    bool splitPath(const std::string& path, std::string& parentPath, std::string& name);
    bool matchPattern(const std::string& str, const std::string& pattern);
    DirNode* reachPath(const std::string& path);
    DirNode* getRoot();
};


#endif //FILESYSTEM_FILESYSTEM_H