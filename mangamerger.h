//
// Created by pieter on 24-3-17.
//

#ifndef MANGAMERGER_MANGAMERGER_H
#define MANGAMERGER_MANGAMERGER_H

#include <hpdf.h>
#include <CImg.h>
#include <string>
#include <array>
#include "threadpool.h"

using namespace std;

class MangaMerger
{
public:
    MangaMerger(string path);
private:
#ifdef __WIN32__
    const string FILE_SEPARATOR = "\\";
#else
    const string FILE_SEPARATOR = "/";
#endif
    int noPages;
    HPDF_Doc pdf;
    ThreadPool pool;
    HPDF_Page pages [];
protected:
};


#endif //MANGAMERGER_MANGAMERGER_H
