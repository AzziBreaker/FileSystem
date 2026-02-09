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

    std::cout << "BOOMED BEFORE COMMANDS!\n";
    do
    {
        std::getline(std::cin, line);
        tokens = tokenize(line);

        if(tokens[0] == "mkdir")
        {
            fs.mkdir(tokens[1]);
        }
        else if (tokens[0] == "rmdir")
        {
            //fs.rmdir(tokens[1]);
        }
        else
        {
            std::cout << "Invalid command!\n";
            continue;
        }

    } while (tokens[0] != "close");


    fs.save();
}
