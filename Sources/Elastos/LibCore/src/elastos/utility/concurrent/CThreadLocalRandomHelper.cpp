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

#include "elastos/utility/concurrent/CThreadLocalRandomHelper.h"
#include "elastos/utility/concurrent/CThreadLocalRandom.h"

using Elastos::Utility::Concurrent::CThreadLocalRandom;

namespace Elastos {
namespace Utility {
namespace Concurrent {

//====================================================================
// CThreadLocalRandomHelper::
//====================================================================
CAR_INTERFACE_IMPL(CThreadLocalRandomHelper, Singleton, IThreadLocalRandomHelper)

CAR_SINGLETON_IMPL(CThreadLocalRandomHelper)

ECode CThreadLocalRandomHelper::GetCurrent(
    /* [out] */ IThreadLocalRandom** threadloc)
{
    VALIDATE_NOT_NULL(threadloc)
    AutoPtr<IThreadLocalRandom> p = CThreadLocalRandom::GetCurrent();
    *threadloc = p.Get();
    REFCOUNT_ADD(*threadloc)
    return NOERROR;
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
