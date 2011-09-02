#include <iostream>
#include <stdexcept>
#include <sqlitepp/sqlitepp.hpp>
#include <fitsio.h>
#include <tclap/CmdLine.h>
#include "timer.h"



using namespace std;

struct Fits
{
    fitsfile *fptr;
    int status;
    string filename;
    Fits(const string &filename)
        : status(0), filename(filename)
    {
        fits_open_file(&fptr, filename.c_str(), READWRITE, &status);
        check();
    }


    virtual ~Fits()
    {
        fits_close_file(fptr, &status);
        check();
    }

    void check()
    {
        if (status)
        {
            throw runtime_error("Error with fitsio");
        }
    }

    void moveHDU(const string &hduname)
    {
        fits_movnam_hdu(fptr, ANY_HDU, const_cast<char*>(hduname.c_str()), 0, &status);
        check();
    }
};

template <typename T>
void CopyImageData(Fits &infile, Fits &outfile, float MemLimit)
{
}



int main(int argc, char *argv[])
{
    Timer ts;
    ts.start("all");
    ts.stop("all");
    return 0;
}
