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

#ifndef __ORG_APACHE_HTTP_PROTOCOL_CHTTPSERVICE_H_
#define __ORG_APACHE_HTTP_PROTOCOL_CHTTPSERVICE_H_

#include "_Org_Apache_Http_Protocol_CHttpService.h"
#include "org/apache/http/protocol/HttpService.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Protocol {

/**
 * Minimalistic server-side implementation of an HTTP processor.
 *
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 *
 * @version $Revision: 610763 $
 */
CarClass(CHttpService) , public HttpService
{
public:
    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ IHttpProcessor* processor,
        /* [in] */ IConnectionReuseStrategy* connStrategy,
        /* [in] */ IHttpResponseFactory* responseFactory);
};

} // namespace Protocol
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_PROTOCOL_CHTTPSERVICE_H_
