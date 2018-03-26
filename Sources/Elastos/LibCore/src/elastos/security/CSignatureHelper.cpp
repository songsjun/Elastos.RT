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

#include "elastos/security/CSignatureHelper.h"
#include "elastos/security/Signature.h"

namespace Elastos {
namespace Security {

CAR_SINGLETON_IMPL(CSignatureHelper);
CAR_INTERFACE_IMPL(CSignatureHelper, Singleton, ISignatureHelper)

ECode CSignatureHelper::GetInstance(
    /* [in] */ const String& algorithm,
    /* [out] */ ISignature **instance)
{
    return Signature::GetInstance(algorithm, instance);
}

ECode CSignatureHelper::GetInstance(
    /* [in] */ const String& algorithm,
    /* [in] */ const String& provider,
    /* [out] */ ISignature **instance)
{
    return Signature::GetInstance(algorithm, provider, instance);
}

ECode CSignatureHelper::GetInstance(
    /* [in] */ const String& algorithm,
    /* [in] */ IProvider *provider,
    /* [out] */ ISignature **instance)
{
    return Signature::GetInstance(algorithm, provider, instance);
}

} // namespace Security
} // namespace Elastos