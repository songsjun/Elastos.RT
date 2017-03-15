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

#ifndef __CCALLBACKARGLIST_H__
#define __CCALLBACKARGLIST_H__

#include "refutil.h"

class CCallbackArgumentList
    : public ElLightRefBase
    , public ICallbackArgumentList
{
public:
    CCallbackArgumentList();

    virtual ~CCallbackArgumentList();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* object,
        /* [out] */ InterfaceID* iid);

    CARAPI Init(
        /* [in] */ ICallbackMethodInfo* callbackMethodInfo,
        /* [in] */ ParmElement* paramElem,
        /* [in] */ UInt32 paramCount,
        /* [in] */ UInt32 paramBufSize);

    CARAPI GetCallbackMethodInfo(
        /* [out] */ ICallbackMethodInfo** callbackMethodInfo);

    CARAPI GetServerPtrArgument(
        /* [out] */ PInterface* server);

    CARAPI GetParamValue(
        /* [in] */ Int32 index,
        /* [in] */ PVoid param,
        /* [in] */ CarDataType type);

    CARAPI GetInt16Argument(
        /* [in] */ Int32 index,
        /* [out] */ Int16* value);

    CARAPI GetInt32Argument(
        /* [in] */ Int32 index,
        /* [out] */ Int32* value);

    CARAPI GetInt64Argument(
        /* [in] */ Int32 index,
        /* [out] */ Int64* value);

    CARAPI GetByteArgument(
        /* [in] */ Int32 index,
        /* [out] */ Byte* value);

    CARAPI GetFloatArgument(
        /* [in] */ Int32 index,
        /* [out] */ Float* value);

    CARAPI GetDoubleArgument(
        /* [in] */ Int32 index,
        /* [out] */ Double* value);

    CARAPI GetCharArgument(
        /* [in] */ Int32 index,
        /* [out] */ Char32* value);

    CARAPI GetStringArgument(
        /* [in] */ Int32 index,
        /* [out] */ String* value);

    CARAPI GetBooleanArgument(
        /* [in] */ Int32 index,
        /* [out] */ Boolean* value);

    CARAPI GetEMuidArgument(
        /* [in] */ Int32 index,
        /* [out] */ EMuid** value);

    CARAPI GetEGuidArgument(
        /* [in] */ Int32 index,
        /* [out] */ EGuid** value);

    CARAPI GetECodeArgument(
        /* [in] */ Int32 index,
        /* [out] */ ECode* value);

    CARAPI GetLocalPtrArgument(
        /* [in] */ Int32 index,
        /* [out] */ LocalPtr* value);

    CARAPI GetEnumArgument(
        /* [in] */ Int32 index,
        /* [out] */ Int32* value);

    CARAPI GetCarArrayArgument(
        /* [in] */ Int32 index,
        /* [out] */ PCarQuintet* value);

    CARAPI GetStructPtrArgument(
        /* [in] */ Int32 index,
        /* [out] */ PVoid* value);

    CARAPI GetObjectPtrArgument(
        /* [in] */ Int32 index,
        /* [out] */ PInterface* value);

public:
    Byte*                           mParamBuf;
    UInt32                          mParamBufSize;

private:
    ParmElement*                    mParamElem;
    UInt32                          mParamCount;
    AutoPtr<ICallbackMethodInfo>    mCallbackMethodInfo;
};

#endif // __CCALLBACKARGLIST_H__
