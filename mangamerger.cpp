#include <iostream>
#include <boost/filesystem.hpp>
#include <hpdf.h>
#include <vector>
#include "mangamerger.h"

using namespace boost::filesystem;

MangaMerger::MangaMerger(string path) :
        pool(thread::hardware_concurrency() > 0 ? thread::hardware_concurrency() : 4)
{
    this->path = path;
    boost::regex pathRegex(pathRegexString);
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
                imagePaths.push_back(itr->path().string());
            }
        }
    }

    cout << count << " Items counted" << endl;

    pageCount = count;
    //Sort array on name.
    std::sort(imagePaths.begin(), imagePaths.end());
}

void MangaMerger::MergeStart()
{
    HPDF_Page pages [pageCount];
    //Initalize all pages
    for(int i = 0; i < pageCount; i++)
    {
        pages[i] = HPDF_AddPage(pdf);
    }

    for(int i = 0; i < pageCount; i++)
    {
        Merge(imagePaths[i], pages[i], i);
    }
}

void MangaMerger::Merge(string path, HPDF_Page &page, int pageNr)
{
    pageMtx.lock();
    cout << pageNr << " is processing" << endl;
    cout << "Path is: " << path << endl;
    pageMtx.unlock();

    HPDF_Font def_font = HPDF_GetFont (pdf, "Helvetica", NULL);

    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize (page, def_font, 24);
    HPDF_Page_MoveTextPos (page, 60, HPDF_Page_GetHeight(page) - 105);
    HPDF_Page_ShowText (page, path.c_str());
    HPDF_Page_EndText(page);
}

void MangaMerger::Save(string path)
{
    HPDF_SaveToFile(pdf, path.c_str());
}

MangaMerger::~MangaMerger() {
    HPDF_Free(pdf);
}


