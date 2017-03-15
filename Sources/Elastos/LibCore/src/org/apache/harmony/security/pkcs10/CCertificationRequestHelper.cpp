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

#include "CCertificationRequestHelper.h"
#include <CCertificationRequest.h>
#include <cmdef.h>

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Pkcs10 {

CAR_SINGLETON_IMPL(CCertificationRequestHelper)

CAR_INTERFACE_IMPL(CCertificationRequestHelper, Singleton, ICertificationRequestHelper)

ECode CCertificationRequestHelper::GetASN1(
    /* [out] */ IASN1Sequence** asn1)
{
    VALIDATE_NOT_NULL(asn1)
    *asn1 = CCertificationRequest::ASN1;
    REFCOUNT_ADD(*asn1)
    return NOERROR;
}

} // namespace Pkcs10
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

