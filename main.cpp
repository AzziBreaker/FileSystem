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


    //on creation of filstystem doesnt save properly AND on load doesnt create data tree
    do
    {

        //std::cout << "Going inside!\n";
        std::getline(std::cin, line);
        //std::cout << "In between!\n";
        tokens = tokenize(line);
        //std::cout << "Reached end of tokenize\n";
        if (tokens.empty())
        {
            std::cout << "Invalid command!";
            continue;
        }

        if(tokens[0] == "mkdir" && !tokens[1].empty())
        {
           // std::cout << "STARTING TO MAKE DIRECTORY!\n";
            fs.mkdir(tokens[1]);
        }
        else if (tokens[0] == "rmdir")
        {
            //fs.rmdir(tokens[1]);
        }
        else if (tokens[0] == "save")
        {
            fs.save();
        }
        else
        {
            std::cout << "Invalid command!\n";
        }

    } while (tokens[0] != "close");


    fs.save();
}
