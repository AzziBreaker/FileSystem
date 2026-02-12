//
// Created by Denis Kiprov on 9.02.26.
//

#include "FileSystem.h"

#include <ranges>

FileSystem::FileSystem(const std::string& fileName)
{
    unsigned maxID = 0;
    dataFile.open(fileName, std::ios::binary | std::ios::in | std::ios::out);
    if (!dataFile) {
        createEmptyFileSystem(fileName);
        return;
    }

    FileMetadataOnDisk diskMeta{};
    dataFile.read(reinterpret_cast<char*>(&diskMeta), sizeof(diskMeta));
    this->fileMetadata = new FileMetadata(diskMeta);
    this->idKeysCount = fileMetadata->idKeys;

    for (unsigned i = 0; i < fileMetadata->idKeys; ++i)
    {
        IDKeyOnDisk diskKey{};
        dataFile.read(reinterpret_cast<char*>(&diskKey), sizeof(diskKey));

        /*std::cout << "LOADED ID: " << diskKey.ID
        << " size: " << diskKey.size
        << " firstBlock: " << diskKey.firstBlock
        << "\n";*/

        if (maxID < diskKey.ID)
        {
            maxID = diskKey.ID;
        }
        IDKey key(diskKey);
        this->idKeys.insert({key.ID,key});

    }

    this->nextID = maxID;
    this->nextID++;
    std::cout << "File system loaded successfully!\n";

    this->loadIsUsedTable();

    this->buildTree();

    this->allocator = new BlockAllocator(dataFile,isUsed,this->fileMetadata->dataBlockOffset);

    save();
}

FileSystem::~FileSystem()
{
    delete fileMetadata;
    delete allocator;
    dataFile.close();
}

void FileSystem::save()
{
    fileMetadata->idKeys = idKeys.size();

    FileMetadataOnDisk meta = fileMetadata->toDisk();

    dataFile.seekp(0, std::ios::beg);
    dataFile.write(reinterpret_cast<const char*>(&meta), sizeof(meta));

    dataFile.seekp(fileMetadata->idKeyOffset, std::ios::beg);

    IDKeyOnDisk empty{};
    for (unsigned i = 0; i < fileMetadata->maxIDKeys; ++i)
    {
        dataFile.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
    }

    dataFile.seekp(fileMetadata->idKeyOffset, std::ios::beg);

    for (const auto& pair : idKeys)
    {
        const IDKey& key = pair.second;
        //std::cout << "Saving: " << key.name << " ID=" << key.ID << " firstBlock=" << key.firstBlock << " size=" << key.size << "\n";

        IDKeyOnDisk diskKey = key.toDisk();

        //std::cout << "Saving: " << diskKey.name << " ID=" << diskKey.ID << " firstBlock=" << diskKey.firstBlock << " size=" << diskKey.size << "\n";

        std::streampos pos = fileMetadata->idKeyOffset + key.ID * sizeof(IDKeyOnDisk);

        dataFile.seekp(pos, std::ios::beg);
        dataFile.write(reinterpret_cast<const char*>(&diskKey), sizeof(diskKey));


    }

    dataFile.seekp(fileMetadata->isUsedOffset, std::ios::beg);

    for (bool used : isUsed)
    {
        dataFile.write(reinterpret_cast<const char*>(&used), sizeof(bool));
    }

    dataFile.flush();
}

void FileSystem::buildTree()
{
    std::unordered_map<unsigned,Node*> nodes;

    std::unordered_map<unsigned, IDKey>::iterator it = idKeys.begin();
    while (it != idKeys.end())
    {
        unsigned id = it->first;
        IDKey& key = it->second;

        Node* node;



        if (key.isDir)
        {
            node = new DirNode();
        }
        else
        {
            FileNode* file = new FileNode();
            file->setSize(key.size);
            file->setFirstBlock(key.firstBlock);
            node = file;
        }

        node->setName(key.name);
        node->setId(key.ID);

        nodes[key.ID] = node;

        it++;
    }

    it = idKeys.begin();
    while (it!=idKeys.end())
    {
        unsigned id = it->first;
        IDKey& key = it->second;

        Node* child = nodes[key.ID];

        if (key.ID == 0)
        {
            root = dynamic_cast<DirNode*>(nodes[id]);
            root->setParent(nullptr);
            it++;
            continue;
        }

        DirNode* parent = dynamic_cast<DirNode*>(nodes[key.parentID]);

        if (!parent)
        {
            std::cout << "Error: parentID " << key.parentID << " not found for node " << key.name << "\n";
            throw;
        }

        child->setParent(parent);
        parent->addChild(child);

        it++;
    }

    currDir = root;
}

DirNode* FileSystem::getRoot()
{
    return this->root;
}

void FileSystem::addKey(const IDKey& key, const IDKey& parent)
{
    this->idKeys.insert({key.ID,key});
    this->idKeysCount++;
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
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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

    this->nextID = 1;

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

void FileSystem::mkdir(std::string& path)
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

    //????????
    if (parent->getChild(dirName)) {
        std::cout << "Directory already exists!\n";
        throw;
    }

    std::cout << "Creating directory " << dirName << " in dir: " << parent->getName() << " with ID: " << parent->getId() << "\n";

    IDKey key(nextID++, parent->getId(), 0, 0, true, dirName);
    idKeys[key.ID] = key;
    this->idKeysCount++;

    fileMetadata->idKeys++;

    DirNode* dir = new DirNode();
    dir->setId(key.ID);
    dir->setName(dirName);
    dir->setParent(parent);

    parent->addChild(dir);

    save();
}

void FileSystem::rmdir(std::string& path)
{
    DirNode* node = nullptr;

    if (path.empty())
    {
        node = currDir;
        currDir = root;
    }
    else
    {
        node = reachPath(path);

        if (!node)
        {
            std::cout << "Directory doesnt exist!\n";
            return;
        }
    }

    if (node==root)
    {
        std::cout << "Cannot delete root!\n";
        return;
    }

    if (!node->getChildren().empty())
    {
        std::cout << "Cannot remove directory! It has children!\n";
        return;
    }

    DirNode* parent = node->getParent();
    parent->removeChild(node);

    this->idKeys.erase(node->getId());

    this->idKeysCount = idKeys.size();

    delete node;
    std::cout << "Removed directory!\n";

    save();
}

void FileSystem::ls(std::string& path)
{
    DirNode* node;

    if (path.empty())
    {
        node = currDir;
    }
    else
    {
        node = reachPath(path);

        if (!node)
        {
            std::cout << "Directory doesnt exist!";
            return;
        }
    }

    node->print();
}

void FileSystem::cd(std::string &path)
{
    DirNode* node;

    if (path.empty())
    {
        std::cout << "Invalid path entered!\n";
        return;
    }
    else
    {
        node = reachPath(path);

        if (!node)
        {
            std::cout << "Directory doesnt exist!";
            return;
        }
    }

    this->currDir = node;
}

void FileSystem::cp(std::string &source, std::string &destination)
{
    std::string parentPath, name;

    if (!splitPath(source,parentPath,name))
    {
        std::cout << "Invalid source!\n";
        return;
    }


    DirNode* srcDir = reachPath(parentPath);
    if (!srcDir)
    {
        std::cout << "Source directory not found!\n";
        return;
    }

    DirNode* destDir = reachPath(destination);
    if (!destDir)
    {
        std::cout << "Destination directory not found!\n";
        return;
    }

    std::vector<Node*> matches;

    std::vector<Node*> children = srcDir->getChildren();

    for (unsigned i = 0;i<children.size();i++)
    {
        Node* node = children[i];
        if (!node->isDir() && matchPattern(node->getName(),name))
        {
            matches.push_back(node);
        }
    }

    for (unsigned i = 0;i<matches.size();i++)
    {
        FileNode* srcFile = reinterpret_cast<FileNode*>(matches[i]);
        std::vector<char> buff = allocator->readFile(srcFile->getFirstBlock());

        unsigned newFirst = allocator->createFile(buff.data(),srcFile->getSize());

        FileNode* newFile = new FileNode();
        newFile->setId(nextID++);
        std::string newName(srcFile->getName());
        newName.append("_cpy");
        newFile->setName(newName);
        newFile->setFirstBlock(newFirst);
        newFile->setSize(srcFile->getSize());
        newFile->setParent(destDir);

        destDir->addChild(newFile);

        IDKey key(newFile->getId(),destDir->getId(),newFirst,srcFile->getSize(),false,newName);
        idKeys[key.ID] = key;
        fileMetadata->idKeys++;
        idKeysCount++;

        std::cout << "Copied item!\n";
    }

    save();

}

void FileSystem::rm(std::string &path)
{
    std::string parentPath, pattern;

    if (!splitPath(path,parentPath,pattern))
    {
        std::cout << "Invalid path!\n";
        return;
    }

    DirNode* parent = reachPath(parentPath);
    if (!parent)
    {
        std::cout << "Parent directory does not exist!\n";
        return;
    }

    std::vector<Node*> children = parent->getChildren();

    std::vector<FileNode*> toRemove;

    for (Node* node: children)
    {
        if (!node->isDir() && matchPattern(node->getName(),pattern))
        {
            toRemove.push_back(dynamic_cast<FileNode*>(node));
        }
    }

    if (toRemove.empty())
    {
        std::cout << "No matched items to remove!\n";
        return;
    }

    for (FileNode* node: toRemove)
    {
        if (node->getFirstBlock() != END)
        {
            allocator->freeBlockChain(node->getFirstBlock());
        }

        parent->removeChild(node);

        idKeys.erase(node->getId());
        fileMetadata->idKeys--;
        idKeysCount--;

        std::cout << "Removed file " << node->getName() << "\n";

        delete node;
    }

    save();
}

void FileSystem::cat(std::string &source)
{
    std::string parentPath, name;

    if (!splitPath(source,parentPath,name))
    {
        std::cout << "Invalid source!\n";
        return;
    }

    DirNode* srcDir = reachPath(parentPath);
    if (!srcDir)
    {
        std::cout << "Source directory not found!\n";
        return;
    }

    std::vector<Node*> children = srcDir->getChildren();

    for (unsigned i = 0;i<children.size();i++)
    {
        Node* node = children[i];
        if (!node->isDir() && matchPattern(node->getName(),name))
        {
            FileNode* fileNode = reinterpret_cast<FileNode*>(node);
            std::vector<char> text = allocator->readFile(fileNode->getFirstBlock());

            std::cout << fileNode->getName() << ": ";

            for (unsigned j = 0; j < fileNode->getSize();j++)
            {
                std::cout << text[j];
            }

            std::cout << " \n";
        }
    }
}

//TODO IMPORTING A MULTI BLOCK THINGY (RABOTI?)
void FileSystem::import(std::string& source, std::string& destination, std::string &param)
{
    std::ifstream sourceFile(source, std::ios::binary|std::ios::ate);
    if (!sourceFile) {
        std::cout << "Failed to open source file: " << source << "\n";
        return;
    }

    int fileSize = sourceFile.tellg();
    sourceFile.seekg(0, std::ios::beg);

    if (fileSize<=0)
    {
        std::cout << "File is empty! Cant be copied!\n";
        return;
    }

    std::vector<char> buffer(fileSize);
    sourceFile.read(buffer.data(),fileSize);

    sourceFile.close();

    DirNode* parent;
    std::string path;
    std::string name;

    if (!splitPath(destination,path,name))
    {
        std::cout << "Invalid path!\n";
        return;
    }

    parent = reachPath(path);

    Node* exists = parent->getChild(name);;
    FileNode* fileNode = nullptr;

    IDKey thisKey;

    if (exists)
    {
        if (exists->isDir())
        {
            std::cout << "Cant have a dir and a file with the same name!\n";
            return;
        }

        fileNode = dynamic_cast<FileNode*>(exists);
    }
    else
    {
        fileNode = new FileNode();
        fileNode->setName(name);
        fileNode->setId(nextID++);

        IDKey key(fileNode->getId(),parent->getId(),END,0,false,name);
        idKeys[key.ID] = key;
        fileMetadata->idKeys++;
        idKeysCount++;

        thisKey = key;

        fileNode->setParent(parent);
        parent->addChild(fileNode);
    }

    unsigned firstBlock = fileNode->getFirstBlock();

    if (param == "+append" && firstBlock != END)
    {
        allocator->appendFile(firstBlock, buffer.data(), fileSize, fileNode->getSize());
        fileNode->setSize(fileNode->getSize() + fileSize);
    }
    else
    {
        if (firstBlock != END)
        {
            allocator->freeBlockChain(firstBlock);
        }
        unsigned newFirst = allocator->createFile(buffer.data(), fileSize);
        fileNode->setFirstBlock(newFirst);
        fileNode->setSize(fileSize);
    }

    IDKey& mapKey = idKeys[fileNode->getId()];
    mapKey.size = fileNode->getSize();
    mapKey.firstBlock = fileNode->getFirstBlock();

    std::cout << "Imported " << source << " into " << destination << "(" << name << ": " << fileNode->getSize() << " bytes)\n";

    save();
}

void FileSystem::loadIsUsedTable()
{
    unsigned blocks = fileMetadata->dataBlocks;
    isUsed.resize(blocks);

    dataFile.seekg(fileMetadata->isUsedOffset, std::ios::beg);

    for (unsigned i = 0; i < blocks; i++)
    {
        bool used;
        dataFile.read(reinterpret_cast<char*>(&used), sizeof(bool));
        isUsed[i] = used;
    }
}

bool FileSystem::splitPath(const std::string& path, std::string& parentPath, std::string& name)
{
    //neznam npos kak se vrushta sus unsigned???????
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

bool FileSystem::matchPattern(const std::string &str, const std::string &pattern)
{
    unsigned strPos = 0;
    unsigned patternPos = 0;

    int starPos = -1;
    int matchPos = 0;

    while (strPos<str.size())
    {
        if (patternPos<pattern.size() && (str[strPos] == pattern[patternPos] || pattern[patternPos] == '?'))
        {
            strPos++;
            patternPos++;
        }
        else if (patternPos<pattern.size() && pattern[patternPos] == '*')
        {
            starPos = patternPos;
            matchPos = strPos;
            patternPos++;
        }
        else if (starPos!=-1)
        {
            patternPos = starPos+1;
            matchPos++;
            strPos = matchPos;
        }
        else
        {
            return false;
        }
    }

    while (patternPos<pattern.size() && pattern[patternPos] == '*')
    {
        patternPos++;
    }

    return patternPos==pattern.size();

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

