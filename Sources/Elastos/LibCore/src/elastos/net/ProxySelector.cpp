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

#include "elastos/net/ProxySelector.h"
#include "elastos/net/ProxySelectorImpl.h"

namespace Elastos {
namespace Net {

AutoPtr<IProxySelector> ProxySelector::sDefaultSelector;

CAR_INTERFACE_IMPL(ProxySelector, Object, IProxySelector)

ProxySelector::ProxySelector()
{}

ProxySelector::~ProxySelector()
{}

ECode ProxySelector::GetDefault(
    /* [out] */ IProxySelector** defaultSelector)
{
    VALIDATE_NOT_NULL(defaultSelector);

    if (sDefaultSelector == NULL) {
        sDefaultSelector = new ProxySelectorImpl();
    }

    *defaultSelector = sDefaultSelector;
    REFCOUNT_ADD(*defaultSelector);
    return NOERROR;
}

ECode ProxySelector::SetDefault(
    /* [in] */ IProxySelector* selector)
{
    sDefaultSelector = selector;
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
