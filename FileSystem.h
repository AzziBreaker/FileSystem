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


class FileSystem
{
public:
    FileSystem(const std::string& fileName);
    ~FileSystem();

public:
    void save();
    void buildTree();
    Node* getRoot();
    void addKey(const IDKey&, const IDKey&);

private:
    void createEmptyFileSystem(const std::string&);

public:
    void mkdir(std::string& path);

    //change to private and add getters setters
public:
    FileMetadata* fileMetadata;
    std::unordered_map<unsigned, IDKey> idKeys;
    std::vector<unsigned> isUsed;
    std::fstream dataFile;
    DirNode* root;
    DirNode* currDir;
    unsigned nextID;

private:
    bool splitPath(const std::string& path, std::string& parentPath, std::string& name);
    DirNode* reachPath(const std::string& path);
};


#endif //FILESYSTEM_FILESYSTEM_H