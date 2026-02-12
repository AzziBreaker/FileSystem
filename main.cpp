#include <iostream>
#include <fstream>
#include <sstream>

#include "FileSystem.h"

std::vector<std::string> tokenize(const std::string& line)
{
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

std::string addBin(const std::string& name)
{
    if (name.size() >= 4 &&
        name.substr(name.size() - 4) == ".bin")
        return name;

    return name + ".bin";
}

int main()
{
    std::cout << "Hello! Which file system would you like to open? :)\n";
    std::string fsFilename;

    //TODO: dali e validno
    std::cin >> fsFilename;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    fsFilename = addBin(fsFilename);

    FileSystem fs(fsFilename);

    std::string line;
    std::vector<std::string> tokens;

    do
    {
        std::getline(std::cin, line);
        if (line.empty())
        {
            continue;
        }
        tokens = tokenize(line);

        if (tokens.empty())
        {
            std::cout << "Invalid command!";
            continue;
        }

        if(tokens[0] == "mkdir" && !tokens[1].empty())
        {
            fs.mkdir(tokens[1]);
        }
        else if (tokens[0] == "rmdir")
        {
            fs.rmdir(tokens[1]);
        }
        else if (tokens[0] == "ls")
        {
            fs.ls(tokens[1]);
        }
        else if (tokens[0] == "cd")
        {
            fs.cd(tokens[1]);
        }
        else if (tokens[0] == "cp")
        {
            fs.cp(tokens[1],tokens[2]);
        }
        else if (tokens[0] == "rm")
        {
            fs.rm(tokens[1]);
        }
        else if (tokens[0] == "cat")
        {
            fs.cat(tokens[1]);
        }
        else if (tokens[0] == "import")
        {
            fs.import(tokens[1], tokens[2], tokens[3]);
        }
        else if (tokens[0] == "save")
        {
            fs.fileMetadata->print(std::cout);
            fs.save();
        }
        else if (tokens[0] == "close")
        {
            std::cout << "Goodbye~\n";
            break;
        }
        else
        {
            std::cout << "Invalid command!\n";
        }

    } while (tokens[0] != "close");

}
