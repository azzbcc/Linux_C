cmake_minimum_required(VERSION 3.8)
project(LinuxC)

set(CMAKE_C_STANDARD 11)

INCLUDE_DIRECTORIES(include)

LINK_DIRECTORIES(.libs)

ADD_SUBDIRECTORY(src)