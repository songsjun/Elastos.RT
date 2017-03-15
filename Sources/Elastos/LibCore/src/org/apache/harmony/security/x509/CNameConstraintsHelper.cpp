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

#include "org/apache/harmony/security/x509/CNameConstraintsHelper.h"
#include "org/apache/harmony/security/x509/CNameConstraints.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CAR_SINGLETON_IMPL(CNameConstraintsHelper)

CAR_INTERFACE_IMPL(CNameConstraintsHelper, Singleton, INameConstraintsHelper)

ECode CNameConstraintsHelper::Decode(
    /* [in] */ ArrayOf<Byte>* pEncoding,
    /* [out] */ INameConstraints** ppObject)
{
    VALIDATE_NOT_NULL(ppObject);

    return CNameConstraints::Decode(pEncoding, ppObject);
}

ECode CNameConstraintsHelper::GetASN1(
    /* [out] */ IASN1Sequence** ppAsn1)
{
    VALIDATE_NOT_NULL(ppAsn1);

    return CNameConstraints::GetASN1(ppAsn1);
}

ECode CNameConstraintsHelper::SetASN1(
    /* [in] */ IASN1Sequence* pAsn1)
{
    return CNameConstraints::SetASN1(pAsn1);
}

} // namespace X509
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org