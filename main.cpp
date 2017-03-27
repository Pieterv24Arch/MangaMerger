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

void usage()
{
    cout << "Usage: MangaMerger <Path to source> <Path to output>/<filename>.pdf" << endl;
}

int main(int argc, char* argv[])
{
    if(argc > 2 || argc > 3)
    {
        Magick::InitializeMagick(*argv);

        MangaMerger merger(argv[1]);
        merger.MergeStart();

        merger.Save(argv[2]);
    } else {
        usage();
    }
    return 0;
}
