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

#ifndef __ELASTOS_SECURITY_ALGORITHMPARAMETERGENERATORSPI_H__
#define __ELASTOS_SECURITY_ALGORITHMPARAMETERGENERATORSPI_H__

#include "Elastos.CoreLibrary.Security.h"
#include "elastos/core/Object.h"

using Elastos::Core::Object;
using Elastos::Security::Spec::IAlgorithmParameterSpec;

namespace Elastos {
namespace Security {

class CAlgorithmParameterGenerator;

/**
 * {@code AlgorithmParameterGeneratorSpi} is the Service Provider Interface
 * (SPI) definition for {@code AlgorithmParameterGenerator}.
 *
 * @see AlgorithmParameterGenerator
 */
class AlgorithmParameterGeneratorSpi
    : public Object
    , public IAlgorithmParameterGeneratorSpi
{
public:
    CAR_INTERFACE_DECL()

    /**
     * Constructs a new instance of {@code AlgorithmParameterGeneratorSpi} .
     */
    AlgorithmParameterGeneratorSpi();

    virtual ~AlgorithmParameterGeneratorSpi();

protected:
    /**
     * Initializes this {@code AlgorithmParameterGeneratorSpi} with the given
     * size and the given {@code SecureRandom}. The default parameter set
     * will be used.
     *
     * @param size
     *            the size (in number of bits).
     * @param random
     *            the source of randomness.
     */
    virtual CARAPI EngineInit(
        /* [in] */ Int32 size,
        /* [in] */ ISecureRandom* random) = 0;

    /**
     * Initializes this {@code AlgorithmParameterGeneratorSpi} with the given
     * {@code AlgorithmParameterSpec} and the given {@code SecureRandom}.
     *
     * @param genParamSpec
     *            the parameters to use.
     * @param random
     *            the source of randomness.
     * @throws InvalidAlgorithmParameterException
     *             if the specified parameters are not supported.
     */
    virtual CARAPI EngineInit(
        /* [in] */ IAlgorithmParameterSpec* genParamSpec,
        /* [in] */ ISecureRandom* random) /*throws InvalidAlgorithmParameterException*/ = 0;

    /**
     * Computes and returns {@code AlgorithmParameters} for this generator's
     * algorithm.
     *
     * @return {@code AlgorithmParameters} for this generator's algorithm.
     */
    virtual AutoPtr<IAlgorithmParameters> EngineGenerateParameters() = 0;

private:
    friend class CAlgorithmParameterGenerator;
};

} // namespace Security
} // namespace Elastos

#endif // __ELASTOS_SECURITY_ALGORITHMPARAMETERGENERATORSPI_H__
