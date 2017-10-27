
#ifndef __CSMANC_H__
#define __CSMANC_H__

#include "_CSManC.h"

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include <elastos/HashMap.h>
#include <elautoptr.h>



using namespace Elastos;
using Elastos::Core::HashMap;
//using Elastos::Utility::IObjectStringMap;

CarClass(CSManC)
{
public:
    CSManC(void);

    ~CSManC();

    CARAPI constructor(void);

    CARAPI AddService(
        /* [in] */ const String& name,
        /* [in] */ IInterface * pService);

    CARAPI AddServiceEx(
        /* [in] */ const String& name,
        /* [in] */ IInterface * pService,
        /* [in] */ Boolean allowIsolated);

    CARAPI GetService(
        /* [in] */ const String& name,
        /* [out] */ IInterface ** ppService);

    /**
     * This is only intended to be called when the process is first being brought
     * up and bound by the activity manager. There is only one thread in the process
     * at that time, so no locking is done.
     *
     * @param cache the cache of service references
     * @hide
     */
#if 0
    ECode InitServiceCache(
        /* [in] */ IObjectStringMap* services);
#endif

private:
    Int32                                       m_sockfd;

    HashMap<String, AutoPtr<IInterface> > mServiceCache;
};


#endif // __CSMANC_H__
