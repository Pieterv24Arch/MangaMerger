#include <iostream>
#include <boost/filesystem.hpp>
#include <hpdf.h>
#include "mangamerger.h"

using namespace boost::filesystem;

MangaMerger::MangaMerger(string path) :
        pool(thread::hardware_concurrency() > 0 ? thread::hardware_concurrency() : 4)
{
    this->path = path;
#ifdef __WIN32__
    boost::regex pathRegex("^\\\\(?:.+\\\\)*((?:.+)\\.(?:png|jpg|jpeg))$");
#else
    boost::regex pathRegex("^\\/(?:.+\\/)*((?:.+)\\.(?:png|jpg|jpeg))$");
#endif
    int count = 0;
    pdf = HPDF_New(error_handler, NULL);

    directory_iterator end_itr;

    //Count image files in directory
    for(directory_iterator itr(path); itr != end_itr; itr++)
    {
        if(is_regular_file(itr->path()))
        {
            string current_file = itr->path().string();
            boost::cmatch char_matches;

            if(boost::regex_match(current_file.c_str(), char_matches, pathRegex)
               && !is_directory(itr->path()))
            {
                count++;
            }
        }
    }
    cout << count << " Items counted";
    noPages = count;
    
}

MangaMerger::~MangaMerger() {
    HPDF_Free(pdf);
}


