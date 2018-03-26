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

#ifndef __LIBCORE_ICU_CCOLLATIONELEMENTITERATORICUHELPER_H__
#define __LIBCORE_ICU_CCOLLATIONELEMENTITERATORICUHELPER_H__

#include "_Libcore_ICU_CCollationElementIteratorICUHelper.h"
#include "elastos/core/Singleton.h"

using Elastos::Core::Singleton;

namespace Libcore {
namespace ICU {

CarClass(CCollationElementIteratorICUHelper)
    , public Singleton
    , public ICollationElementIteratorICUHelper
{
public:
    CAR_SINGLETON_DECL()

    CAR_INTERFACE_DECL()

    CARAPI PrimaryOrder(
        /* [in] */ Int32 order,
        /* [out] */ Int32 * value);

    CARAPI SecondaryOrder(
        /* [in] */ Int32 order,
        /* [out] */ Int32 * value);

    CARAPI TertiaryOrder(
        /* [in] */ Int32 order,
        /* [out] */ Int32 * value);

    CARAPI GetInstance(
        /* [in] */ Int64 collatorAddress,
        /* [in] */ const String& source,
        /* [out] */ ICollationElementIteratorICU ** outiterICU);
};

} // namespace ICU
} // namespace Libcore

#endif //__LIBCORE_ICU_CCOLLATIONELEMENTITERATORICUHELPER_H__
