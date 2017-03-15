//=========================================================================
// Copyright (C) 2012 The Elastos Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

#ifndef __LIBCORE_NET_URICODECHELPER_H__
#define __LIBCORE_NET_URICODECHELPER_H__

#include "_Libcore_Net_CUriCodecHelper.h"
#include "Singleton.h"

using Elastos::Core::Singleton;
using Elastos::IO::Charset::ICharset;

namespace Libcore {
namespace Net {

CarClass(CUriCodecHelper)
    , public Singleton
    , public IUriCodecHelper
{
public :
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI ValidateSimple(
        /* [in] */ const String& s,
        /* [in] */ const String& legal);

    CARAPI Decode(
        /* [in] */ const String& s,
        /* [out] */ String* result);

    CARAPI Decode(
        /* [in] */ const String& s,
        /* [in] */ Boolean convertPlus,
        /* [in] */ ICharset* charset,
        /* [in] */ Boolean throwOnFailure,
        /* [out] */ String* result);
};

} // namespace Net
} // namespace Libcore

#endif // __LIBCORE_NET_URICODECHELPER_H__
