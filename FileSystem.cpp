//
// Created by Denis Kiprov on 9.02.26.
//

#include "FileSystem.h"

FileSystem::FileSystem(const std::string& fileName)
{
    dataFile.open(fileName, std::ios::binary | std::ios::in | std::ios::out);
    if (!dataFile) {
        createEmptyFileSystem(fileName);
        return;
    }

    FileMetadataOnDisk diskMeta{};
    dataFile.read(reinterpret_cast<char*>(&diskMeta), sizeof(diskMeta));
    this->fileMetadata = new FileMetadata(diskMeta);

    for (unsigned i = 0; i < fileMetadata->idKeys; ++i)
    {
        IDKeyOnDisk diskKey{};
        dataFile.read(reinterpret_cast<char*>(&diskKey), sizeof(diskKey));

        IDKey key(diskKey);
        this->idKeys.insert({key.ID,key});
    }

    std::cout << "File system loaded successfully!\n";

    //loadIsUsedTable();

    //buildTree();
}

FileSystem::~FileSystem()
{
    delete fileMetadata;
    dataFile.close();
}

void FileSystem::save()
{
    fileMetadata->print(std::cout);

    FileMetadataOnDisk meta = fileMetadata->toDisk();
    dataFile.seekp(0, std::ios::beg);
    dataFile.write(reinterpret_cast<const char*>(&meta), sizeof(meta));

    dataFile.seekp(fileMetadata->idKeyOffset, std::ios::beg);
    for (unsigned id = 0; id < fileMetadata->idKeys; ++id)
    {
        IDKeyOnDisk diskKey{};
        if (idKeys.contains(id))
        {
            diskKey = idKeys[id].toDisk();
        }
        dataFile.write(reinterpret_cast<const char*>(&diskKey), sizeof(diskKey));
    }

    dataFile.seekp(fileMetadata->isUsedOffset, std::ios::beg);
    for (bool used : isUsed)
    {
        bool v = used;
        dataFile.write(reinterpret_cast<const char*>(&v), sizeof(v));
    }

    //blocks



    dataFile.flush();
}

void FileSystem::buildTree()
{
}

Node* FileSystem::getRoot()
{
    return this->root;
}

void FileSystem::addKey(const IDKey& key, const IDKey& parent)
{
    this->idKeys.insert({key.ID,key});

}

void FileSystem::createEmptyFileSystem(const std::string& fileName)
{
    dataFile.open(fileName,std::ios::out | std::ios::binary | std::ios::trunc);

    if (!dataFile)
    {
        std::cerr << "Failed to create filesystem file\n";
        throw;
    }

    std::cout << fileName << " opened successfully!\n";
    std::cout << "What is the file max size?\n";

    unsigned maxSize;
    std::cin >> maxSize;

    this->fileMetadata = new FileMetadata(fileName,maxSize);

    std::string name = "root";
    IDKey rootKey(0, 0, 0, 0, true,name);

    idKeys[rootKey.ID] = rootKey;

    this->root = new DirNode();
    root->setId(rootKey.ID);
    root->setName(rootKey.name);
    root->setParent(nullptr);
    this->currDir = root;

    unsigned numBlocks = maxSize / fileMetadata->blockSize;
    isUsed.resize(numBlocks, false);

    nextID++;

    FileMetadataOnDisk disk = fileMetadata->toDisk();
    dataFile.seekp(0, std::ios::beg);
    dataFile.write(reinterpret_cast<const char*>(&disk), sizeof(disk));

    IDKeyOnDisk rootDisk = rootKey.toDisk();
    dataFile.write(reinterpret_cast<const char*>(&rootDisk), sizeof(rootDisk));

    dataFile.seekp(fileMetadata->isUsedOffset, std::ios::beg);
    for (unsigned i = 0; i < numBlocks; ++i)
    {
        bool val = false;
        dataFile.write(reinterpret_cast<const char*>(&val), sizeof(bool));
    }


    std::cout << "File system created successfully!\n";
}

void FileSystem::mkdir(std::string &path)
{
    std::string parentPath, dirName;
    if (!splitPath(path,parentPath,dirName))
    {
        std::cout << "Invalid path name!\n";
        throw;
    }

    DirNode* parent;
    if (parentPath.empty())
    {
        parent = currDir;
    }
    else
    {
        parent = reachPath(parentPath);
    }

    if (!parent)
    {
        std::cout << "Parent directory doesnt exist!\n";
        throw;
    }

    if (parent->getChild(dirName)) {
        std::cout << "Directory already exists!\n";
        throw;
    }

    IDKey key(nextID++, parent->getId(), 0, 0, true, dirName);
    addKey(key, idKeys[parent->getId()]);

    fileMetadata->idKeys++;

    //????????????????????????????????????
    DirNode* dir = new DirNode();
    dir->setId(key.ID);
    dir->setName(dirName);
    dir->setParent(parent);

    parent->addChild(dir);
}

bool FileSystem::splitPath(const std::string& path, std::string& parentPath, std::string& name)
{
    auto pos = path.find_last_of('/');
    if (pos == std::string::npos)
    {
        parentPath = "";
        name = path;
        return true;
    }

    if (pos == 0)
    {
        parentPath = "/";
    }
    else
    {
        parentPath = path.substr(0,pos);
    }

    name = path.substr(pos + 1);
    return !name.empty();
}

DirNode* FileSystem::reachPath(const std::string &path)
{
    DirNode* curr;

    if (path[0] == '/')
    {
        curr = root;
    }
    else
    {
        curr = currDir;
    }

    std::istringstream iss(path);
    std::string part;

    while (std::getline(iss,part,'/'))
    {
        if (part.empty() || part == ".")
        {
            continue;
        }

        if (part == "..")
        {
            if (curr->getParent())
            {
                curr = curr->getParent();
            }
            continue;
        }

        Node* node = curr->getChild(part);
        if (!node)
        {
            return nullptr;
        }

        if (!node->isDir())
        {
            return nullptr;
        }

        curr = dynamic_cast<DirNode*>(node);
    }

    return curr;
}

