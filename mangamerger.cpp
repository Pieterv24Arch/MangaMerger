#include <iostream>
#include <boost/filesystem.hpp>
#include <hpdf.h>
#include <vector>
#include <functional>
#include "mangamerger.h"

using namespace boost::filesystem;

MangaMerger::MangaMerger(string path) :
        pool(thread::hardware_concurrency() > 0 ? thread::hardware_concurrency() : 2)
{
    this->path = path;
    boost::regex pathRegex(pathRegexString);
    int count = 0;
    //Initiate new pdf file
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

    //Set pagecount
    pageCount = count;
    //Sort array on name.
    std::sort(imagePaths.begin(), imagePaths.end());
}

void MangaMerger::MergeStart()
{
    HPDF_Page pages [pageCount];
    tasksRemaining = pageCount;
    //Initalize all pages
    for(int i = 0; i < pageCount; i++)
    {
        pages[i] = HPDF_AddPage(pdf);
    }
    for(int i = 0; i < pageCount; i++)
    {
        //Pass function to threadpool to be processed concurrently
        function<void()> merge = std::bind(&MangaMerger::Merge, this, imagePaths[i], pages[i]);
        pool.enqueue(merge);
    }

    while (true)
    {
        unique_lock<mutex> locker(monitorMtx);
        cond.wait(locker, [this](){ return tasksRemaining <= 0;});
        if(tasksRemaining <= 0)
        {
            return;
        }
    }
}

void MangaMerger::Merge(string path, HPDF_Page &page)
{
    string imageLocation = processImage(path, HPDF_Page_GetHeight(page), HPDF_Page_GetWidth(page));

    HPDF_Image image = HPDF_LoadJpegImageFromFile(pdf, imageLocation.c_str());
    int heightDiff = (HPDF_Page_GetHeight(page) - HPDF_Image_GetHeight(image))/2;
    int widthDiff = (HPDF_Page_GetWidth(page) - HPDF_Image_GetWidth(image))/2;

    HPDF_Page_DrawImage(page, image, widthDiff, heightDiff, HPDF_Image_GetWidth(image), HPDF_Image_GetHeight(image));

    tasksRemaining--;
    cond.notify_one();
}

string MangaMerger::processImage(string path, int pageHeight, int pageWidth)
{
    boost::regex pathRegex(pathRegexString);
    boost::cmatch char_matches;
    if(boost::regex_match(path.c_str(), char_matches, pathRegex))
    {
        string outputPath = char_matches[1] + "output" + dirSeperator + char_matches[2] + ".jpg";
        boost::filesystem::path outputDir(char_matches[1]+"output" + dirSeperator);

        Magick::Image image;
        image.read(path);
        Magick::Geometry imageSize = image.size();

        if(!boost::filesystem::exists(outputDir))
        {
            boost::filesystem::create_directory(outputDir);
        }
        image.magick("JPEG");
        image.type(Magick::GrayscaleType);
        image.compressType(Magick::JPEGCompression);
        image.quality(40);
        if(imageSize.width() > imageSize.height())
            image.rotate(90);
        image.resize(calculateImageSize(pageHeight, pageWidth, imageSize.height(), imageSize.width()));

        image.write(outputPath);
        return outputPath;
    }
    return "";
}

Magick::Geometry MangaMerger::calculateImageSize(int pageHeight, int pageWidth, int imageHeight, int imageWidth)
{
    int heightDiff = abs(imageHeight-pageHeight);
    int widthDiff = abs(imageWidth-pageWidth);
    if(heightDiff*imageHeight < widthDiff*imageWidth)
    {
        int newHeight = imageHeight - heightDiff;
        int newWidth = newHeight * imageWidth / imageHeight;
        return Magick::Geometry(newWidth, newHeight);
    } else
    {
        int newWidth = imageWidth - widthDiff;
        int newHeight = newWidth * imageHeight / imageWidth;
        return Magick::Geometry(newWidth, newHeight);
    }
}

void MangaMerger::Save(string path)
{
    HPDF_SaveToFile(pdf, path.c_str());
}

MangaMerger::~MangaMerger() {
    cout << "Destruciont Initiated" << endl;
    if(path != "")
    {
        boost::filesystem::path outputDir(path + dirSeperator + "output" + dirSeperator);
        if (boost::filesystem::exists(outputDir))
        {
            boost::filesystem::remove_all(outputDir);
        }
    }
    HPDF_Free(pdf);
}


