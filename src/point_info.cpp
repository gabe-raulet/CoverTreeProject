#include <iostream>
#include <vector>
#include <assert.h>
#include "ptraits.h"
#include "misc.h"
#include "timer.h"

using namespace std;

static constexpr size_t realsize = sizeof(typename select_real<FPSIZE>::Real);

int main(int argc, char *argv[])
{
    fprintf(stderr, "notice: '%s' compiled with [FP=%d]\n", argv[0], static_cast<int>(realsize*8));

    LocalTimer timer;
    timer.start_timer();

    char *fname;
    size_t fsize, rsize, n;
    int dim;
    ifstream is;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <ptsfname>\n", argv[0]);
        return 1;
    }

    fname = argv[1];

    is.open(fname, ios::binary | ios::in);
    is.seekg(0, is.end);
    fsize = is.tellg();
    is.seekg(0, is.beg);
    is.read((char*)&dim, sizeof(int));
    is.close();

    rsize = sizeof(int) + (dim * realsize);
    assert(fsize % rsize == 0);
    n = fsize / rsize;

    timer.stop_timer();

    fprintf(stderr, "[time=%.3f,msg::%s] read points file '%s' [numpts=%lu,dim=%d,size=%s]\n", timer.get_elapsed(), __func__, fname, n, dim, PrettyFileSize::str(fname).c_str());

    return 0;
}
