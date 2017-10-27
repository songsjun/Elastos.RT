
#include "CHelloCar.h"

namespace Elastos {
namespace HelloCarDemo {


CAR_OBJECT_IMPL(CHelloCar)

CAR_INTERFACE_IMPL(CHelloCar, Object, IHelloCar);

ECode CHelloCar::Hello(
    /* [out] */ String * output)
{
    VALIDATE_NOT_NULL(output)
    *output = "Elastos";
    return NOERROR;
}

}
}