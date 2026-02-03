cmake_minimum_required(VERSION 4.1)
project(FileSystem)

set(CMAKE_CXX_STANDARD 20)

add_executable(FileSystem main.cpp
        Node.cpp
        Node.h
        IDKey.cpp
        IDKey.h
        FileNode.cpp
        FileNode.h
        DirNode.cpp
        DirNode.h
)
