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

#ifndef __ELASTOS_UTILITY_CONCURRENT_ELASTOS_UTILITY_RECURSIVEACTION_H__
#define __ELASTOS_UTILITY_CONCURRENT_ELASTOS_UTILITY_RECURSIVEACTION_H__

#include "elastos/utility/concurrent/ForkJoinTask.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

class RecursiveAction
    : public ForkJoinTask
    , public IRecursiveAction
{
public:
    CAR_INTERFACE_DECL()

    virtual CARAPI Compute() = 0;

    virtual CARAPI GetRawResult(
        /* [out] */ IInterface** outface);

    virtual CARAPI SetRawResult(
        /* [in] */ IInterface* value);

    virtual CARAPI Exec(
        /* [out] */ Boolean* res);

private:
    static Int64 mSerialVersionUID;
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CONCURRENT_ELASTOS_UTILITY_RECURSIVEACTION_H__
