//
// Created by pieter on 24-3-17.
//

#ifndef MANGAMERGER_MANGAMERGER_H
#define MANGAMERGER_MANGAMERGER_H

#include <hpdf.h>
#include <string>
#include <boost/regex.hpp>
#include <mutex>
#include <vector>
#include "threadpool.h"

using namespace std;

class MangaMerger
{
public:
    MangaMerger(string path);
    void MergeStart();
    void Merge(string path, HPDF_Page& page, int pageNr);
    void Save(string path);
    string processImage(string path);
    ~MangaMerger();
private:
#ifdef __WIN32__
    const string pathRegexString = "^((?:.:)?\\\\(?:.+\\\\)*)(.+)\\.(png|jpg|jpeg)$";
    const string dirSeperator = "\\";
#else
    const string pathRegexString = "^(\\/(?:.+\\/)*)(.+)\\.(png|jpg|jpeg)$";
    const string dirSeperator = "/";
#endif
    string path;
    int pageCount;
    HPDF_Doc pdf;
    ThreadPool pool;
    mutex pageMtx;
    vector<string> imagePaths;
    vector<string> processedImagePaths;

    static void error_handler (HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data)
    {
        printf ("ERROR: error_no=%04X, detail_no=%d\n",
                (unsigned int) error_no, (int) detail_no);
        throw std::exception (); /* throw exception on error */
    }
protected:
};


#endif //MANGAMERGER_MANGAMERGER_H
