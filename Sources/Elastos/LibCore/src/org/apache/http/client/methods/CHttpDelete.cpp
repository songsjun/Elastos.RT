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

#include "org/apache/http/client/methods/CHttpDelete.h"
#include "elastos/net/CURI.h"

using Elastos::Net::CURI;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Methods {

CAR_INTERFACE_IMPL(CHttpDelete, HttpRequestBase, IHttpDelete)

CAR_OBJECT_IMPL(CHttpDelete)

ECode CHttpDelete::GetMethod(
    /* [out] */ String* method)
{
    VALIDATE_NOT_NULL(method)
    *method = METHOD_NAME;
    return NOERROR;
}

ECode CHttpDelete::Clone(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj)

    AutoPtr<CHttpDelete> httpDelete;
    CHttpDelete::NewByFriend((CHttpDelete**)&httpDelete);
    HttpRequestBase::CloneImpl((HttpRequestBase*)httpDelete);
    *obj = httpDelete->Probe(EIID_IInterface);
    REFCOUNT_ADD(*obj)
    return NOERROR;
}

ECode CHttpDelete::constructor()
{
    return NOERROR;
}

ECode CHttpDelete::constructor(
    /* [in] */ IURI* uri)
{
    return SetURI(uri);
}

ECode CHttpDelete::constructor(
    /* [in] */ const String& uri)
{
    AutoPtr<IURI> iuri;
    CURI::Create(uri, (IURI**)&iuri);
    return SetURI(iuri);
}

} // namespace Methods
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org