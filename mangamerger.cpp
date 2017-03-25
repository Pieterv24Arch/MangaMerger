#include <iostream>
#include "mangamerger.h"

MangaMerger::MangaMerger(string path) :
        pool(thread::hardware_concurrency() > 0 ? thread::hardware_concurrency() : 4)
{
    
}


