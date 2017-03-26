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
    return 0;
}
