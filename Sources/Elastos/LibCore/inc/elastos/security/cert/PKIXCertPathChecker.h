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

#ifndef __ELASTOS_SECURITY_CERT_CERTIFICATEFACTORYSPI_H__
#define __ELASTOS_SECURITY_CERT_CERTIFICATEFACTORYSPI_H__

#include "Elastos.CoreLibrary.Security.h"
#include "Object.h"

using Elastos::Core::Object;
using Elastos::Core::ICloneable;

namespace Elastos {
namespace Security {
namespace Cert {

/**
 * This class defines the <i>Service Provider Interface</i> (<b>SPI</b>) for the
 * {@code CertificateFactory} class. This SPI must be implemented for each
 * certificate type a security provider wishes to support.
 */
class PKIXCertPathChecker
    : public Object
    , public IPKIXCertPathChecker
    , public ICloneable
{
public:
    CAR_INTERFACE_DECL()

    /**
     * Constructs a new instance of this class.
     */
    PKIXCertPathChecker();
};

} // end Cert
} // end Security
} // end Elastos

#endif // __ELASTOS_SECURITY_CERT_CERTIFICATEFACTORYSPI_H__
