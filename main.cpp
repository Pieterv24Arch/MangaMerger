/**
 * Required Libraries:
 * libharu pdf library
 * magick++ image processing library
 * boost c++ extention library
 * To compile use CMakeLists.txt
 * OR
 * Compile with
 * g++ `Magick++-config --cxxflags --cppflags` -std=c++11 -pthread main.cpp mangamerger.cpp threadpool.cpp -o MangaMerger -lhpdf -lboost_regex -lboost_filesystem -lboost_system `Magick++-config --ldflags --libs`
 */

#include <Magick++.h>
#include <iostream>
#include "mangamerger.h"

using namespace std;

int main(int argc, char* argv[])
{
    Magick::InitializeMagick(*argv);
    if(argc < 1)
    {
        return 1;
    }
    MangaMerger merger(argv[1]);
    merger.MergeStart();

    cin.ignore();

    merger.Save("/home/pieter/Documents/testpdf.pdf");

    return 0;
}
