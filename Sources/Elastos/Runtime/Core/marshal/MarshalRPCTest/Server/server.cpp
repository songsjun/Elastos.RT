
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#include "SManC.h"

#include "TC.h"



int main(int argc, char *argv[])
{
    ECode ec;
    ISManC *ismc;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    ec = CSManC::New(&ismc);
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    if (FAILED(ec))
        goto out1;

    ITC *itc;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    ec = CTC::New(10, &itc);
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    if (FAILED(ec))
        goto out2;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    if (ismc->AddService(String("TC"), itc))
        goto out3;

    sleep(3000);

out3:
    if (itc != NULL)
        itc->Release();

out2:
    if (ismc != NULL)
        ismc->Release();

out1:
    printf("Byebye Server\n");
    return 0;
}

