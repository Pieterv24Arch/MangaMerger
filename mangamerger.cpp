#include <iostream>
#include <boost/filesystem.hpp>
#include <hpdf.h>
#include <vector>
#include <functional>
#include <Magick++.h>
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
        //Pass function to threadpool to be processed concurrently
        function<void()> merge = std::bind(&MangaMerger::Merge, this, imagePaths[i], pages[i], i);
        pool.enqueue(merge);
    }
}

void MangaMerger::Merge(string path, HPDF_Page &page, int pageNr)
{
    /*pageMtx.lock();
    cout << pageNr << " is processing" << endl;
    cout << "Path is: " << path << endl;
    pageMtx.unlock();*/

    HPDF_Font def_font = HPDF_GetFont (pdf, "Helvetica", NULL);

    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize (page, def_font, 24);
    HPDF_Page_MoveTextPos (page, 60, HPDF_Page_GetHeight(page) - 105);
    HPDF_Page_ShowText (page, processImage(path).c_str());
    HPDF_Page_EndText(page);
}

string MangaMerger::processImage(string path)
{
    boost::regex pathRegex(pathRegexString);
    boost::cmatch char_matches;
    if(boost::regex_match(path.c_str(), char_matches, pathRegex))
    {
        string outputPath = char_matches[1] + "output" + dirSeperator + char_matches[2] + ".jpg";
        boost::filesystem::path outputDir(char_matches[1]+"output" + dirSeperator);

        Magick::Image image;
        image.read(path);

        if(!boost::filesystem::exists(outputDir))
        {
            boost::filesystem::create_directory(outputDir);
        }
        //If jpg is found
        if(char_matches[3] == "jpg" || char_matches[3] == "jpeg")
        {
            image.magick("JPEG");
            image.type(Magick::GrayscaleType);
            image.compressType(Magick::JPEGCompression);
            image.quality(20);
        }
        else if(char_matches[3] == "png")
        {
            image.magick("JPEG");
            image.type(Magick::GrayscaleType);
            image.compressType(Magick::JPEGCompression);
            image.quality(20);
        }

        image.write(outputPath);
        return outputPath;
    }
    return "";
}

void MangaMerger::Save(string path)
{
    HPDF_SaveToFile(pdf, path.c_str());
}

MangaMerger::~MangaMerger() {
    HPDF_Free(pdf);
}


