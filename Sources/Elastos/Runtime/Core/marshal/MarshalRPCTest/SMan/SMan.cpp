
#include <stdio.h>
#include <unistd.h>

#include "SManS.h"



int main(int argc, char *argv[])
{
    ECode ec;
    ISManS *isms;

    ec = CSManS::New(3355, &isms);
    if (FAILED(ec))
        goto out1;

    sleep(1000);

    if (isms != NULL)
        isms->Release();

out1:
    printf("SMan leaved\n");
    return 0;
}

