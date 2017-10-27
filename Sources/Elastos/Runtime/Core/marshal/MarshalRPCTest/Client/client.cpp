
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#include "SManC.h"

#include "TC.h"



int main(int argc, char *argv[])
{
    ECode ec;
    ISManC *ismc;

    printf("Hello Client\n");

    ec = CSManC::New(&ismc);
    if (FAILED(ec))
        goto out1;

    printf("Client 1\n");
    IInterface *ifc;
    ITC *itc;

    ec = ismc->GetService(String("TC"), &ifc);
    if (FAILED(ec))
        goto out2;
    printf("Client 2\n");
    itc = ITC::Probe(ifc);
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    if (itc == NULL)
        goto out2;
    itc->AddRef();
    itc->Release();

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    Int32 i;

    printf("itc point %x\n", itc);
    ec = itc->Get(&i);
    printf("itc.Get return ec %x\n", ec);
    if (FAILED(ec))
        goto out2;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    printf("%d\n", i);

    itc->Increment();

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    ec = itc->Get(&i);
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    if (FAILED(ec))
        goto out2;

    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    printf("%d\n", i);

    sleep(10);

out2:
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    if (ismc != NULL)
        ismc->Release();

out1:
    printf("TRACE %s:%d \n", __FILE__, __LINE__);
    printf("Byebye client\n");
    return 0;
}

