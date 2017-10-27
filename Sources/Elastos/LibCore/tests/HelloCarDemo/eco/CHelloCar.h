
#ifndef __CHELLOCAR_H__
#define __CHELLOCAR_H__

#include "_Elastos_HelloCarDemo_CHelloCar.h"
#include <elastos/core/Object.h>

using Elastos::Core::IComparable;
using Elastos::Core::EIID_IComparable;

namespace Elastos {
namespace HelloCarDemo {

CarClass(CHelloCar)
    , public Object
    , public IHelloCar
{
public:
    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    CARAPI Hello(
        /* [out] */ String * output);

private:
    // TODO: Add your private member variables here.
};

}
}

#endif // __CHELLOCAR_H__
