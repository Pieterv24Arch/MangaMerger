//
// Created by pieter on 24-3-17.
//

#ifndef MANGAMERGER_MANGAMERGER_H
#define MANGAMERGER_MANGAMERGER_H

#include <hpdf.h>
#include <CImg.h>
#include <string>
#include <array>
#include <boost/regex.hpp>
#include "threadpool.h"

using namespace std;

class MangaMerger
{
public:
    MangaMerger(string path);
    ~MangaMerger();
private:
    string path;
    int noPages;
    HPDF_Doc pdf;
    ThreadPool pool;
    HPDF_Page pages [34];
    static void error_handler (HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data)
    {
        printf ("ERROR: error_no=%04X, detail_no=%d\n",
                (unsigned int) error_no, (int) detail_no);
        throw std::exception (); /* throw exception on error */
    }
protected:
};


#endif //MANGAMERGER_MANGAMERGER_H
