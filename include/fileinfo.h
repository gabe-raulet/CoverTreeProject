#ifndef FILE_INFO_H_
#define FILE_INFO_H_

#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct FileInfo
{
    uintmax_t fsize;
    string fsize_str;

    FileInfo(const char *fname)
    {
        ifstream is;
        is.open(fname, ios::binary | ios::in);
        is.seekg(0, is.end);
        fsize = is.tellg();
        is.seekg(0, is.beg);
        is.close();
    }

    FileInfo(const string& fname) : FileInfo(fname.c_str()) {}

    uintmax_t get_file_size() const { return fsize; }

    const char* get_file_size_str()
    {
        if (fsize_str.size() == 0)
        {
            /*
             * The following code was copied/adapted from here: 'https://en.cppreference.com/w/cpp/filesystem/file_size'
             */

            int i{};
            stringstream ss;
            double mantissa = fsize;
            for (; mantissa >= 1024.0; mantissa /= 1024.0, ++i);
            ss << ceil(mantissa * 10.) / 10. << i["BKMGTPE"];
            if (i) ss << "B";
            fsize_str = ss.str();
        }

        return fsize_str.c_str();
    }
};

#endif
