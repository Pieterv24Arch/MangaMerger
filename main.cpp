/**
 * Required Libraries:
 * libharu pdf library
 * cimg image processing library
 * boost c++ extention library
 * To compile use CMakeLists.txt
 */

#include <iostream>
#include "mangamerger.h"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc < 1)
    {
        return 1;
    }
    MangaMerger merger(argv[1]);
    merger.MergeStart();

    merger.Save("/home/pieter/Documents/testpdf.pdf");

    return 0;
}
