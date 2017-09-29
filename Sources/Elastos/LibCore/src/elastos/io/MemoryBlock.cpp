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

#include "MemoryBlock.h"
// #include "Memory.h"
#include "CByteOrderHelper.h"
#include "OsConstants.h"
#include "CLibcore.h"
#include "ElaMath.h"
#include "CByteOrderHelper.h"
#include "OsConstants.h"

using Elastos::Droid::System::OsConstants;
using Libcore::IO::CLibcore;
using Libcore::IO::IOs;
// using Libcore::IO::Memory;
using Elastos::IO::IByteOrderHelper;
using Elastos::IO::CByteOrderHelper;
using Elastos::IO::Channels::FileChannelMapMode_PRIVATE;
using Elastos::IO::Channels::FileChannelMapMode_READ_ONLY;

namespace Elastos {
namespace IO {

ECode MemoryBlock::Mmap(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 size,
    /* [in] */ FileChannelMapMode mapMode,
    /* [out] */ MemoryBlock** mb)
{
    VALIDATE_NOT_NULL(mb)
    if (size == 0) {
        // You can't mmap(2) a zero-length region, but Java allows it.
        *mb = new MemoryBlock(0, 0);
        REFCOUNT_ADD(*mb);
        return NOERROR;
    }
    // Check just those errors mmap(2) won't detect.
    if (offset < 0 || size < 0 || offset > Elastos::Core::Math::INT32_MAX_VALUE || size > Elastos::Core::Math::INT32_MAX_VALUE) {
        // throw new IllegalArgumentException("offset=" + offset + " size=" + size);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    int prot;
    int flags;
    if (mapMode == FileChannelMapMode_PRIVATE) {
        prot = OsConstants::_PROT_READ | OsConstants::_PROT_WRITE;
        flags = OsConstants::_MAP_PRIVATE;
    }
    else if (mapMode == FileChannelMapMode_READ_ONLY) {
        prot = OsConstants::_PROT_READ;
        flags = OsConstants::_MAP_SHARED;
    }
    else { // mapMode == FileChannelMapMode_READ_WRITE
        prot = OsConstants::_PROT_READ | OsConstants::_PROT_WRITE;
        flags = OsConstants::_MAP_SHARED;
    }
    // try {
    Int64 result;
    CLibcore::sOs->Mmap(0LL, size, prot, flags, fd, offset, &result);
    Int32 address = (Int32)result;
    *mb = new MemoryMappedBlock(address, size);
    REFCOUNT_ADD(*mb);
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     throw errnoException.rethrowAsIOException();
    // }
}

AutoPtr<MemoryBlock> MemoryBlock::Allocate(
    /* [in] */ Int32 byteCount)
{
    AutoPtr< ArrayOf<Byte> > array = ArrayOf<Byte>::Alloc(byteCount);
    return new NonMovableHeapBlock(array, (Int32)array->GetPayload(), byteCount);
}

AutoPtr<MemoryBlock> MemoryBlock::WrapFromNative(
    /* [in] */ Int64 address,
    /* [in] */ Int64 byteCount)
{
    return new UnmanagedBlock(address, byteCount);
}

void MemoryBlock::PokeByte(
    /* [in] */ Int32 offset,
    /* [in] */ Byte value)
{
    #if 0
    Memory::PokeByte(mAddress + offset, value);
    #endif
}

void MemoryBlock::PokeByteArray(
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<Byte>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 byteCount)
{
    #if 0
    Memory::PokeByteArray(mAddress + offset, src, srcOffset, byteCount);
    #endif
}

void MemoryBlock::PokeCharArray(
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<Char32>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount,
    /* [in] */ Boolean swap)
{
    #if 0
    Memory::PokeCharArray(mAddress + offset, src, srcOffset, charCount, swap);
    #endif
}

void MemoryBlock::PokeDoubleArray(
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<Double>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 doubleCount,
    /* [in] */ Boolean swap)
{
    #if 0
    Memory::PokeDoubleArray(mAddress + offset, src, srcOffset, doubleCount, swap);
    #endif
}

void MemoryBlock::PokeFloatArray(
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<Float>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 floatCount,
    /* [in] */ Boolean swap)
{
    #if 0
    Memory::PokeFloatArray(mAddress + offset, src, srcOffset, floatCount, swap);
    #endif
}

void MemoryBlock::PokeInt32Array(
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<Int32>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int32Count,
    /* [in] */ Boolean swap)
{
    #if 0
    Memory::PokeInt32Array(mAddress + offset, src, srcOffset, int32Count, swap);
    #endif
}

void MemoryBlock::PokeInt64Array(
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<Int64>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int64Count,
    /* [in] */ Boolean swap)
{
    #if 0
    Memory::PokeInt64Array(mAddress + offset, src, srcOffset, int64Count, swap);
    #endif
}

void MemoryBlock::PokeInt16Array(
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<Int16>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int16Count,
    /* [in] */ Boolean swap)
{
    #if 0
    Memory::PokeInt16Array(mAddress + offset, src, srcOffset, int16Count, swap);
    #endif
}

Byte MemoryBlock::PeekByte(
    /* [in] */ Int32 offset)
{
    #if 0
    return Memory::PeekByte(mAddress + offset);
    #endif
}

void MemoryBlock::PeekByteArray(
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<Byte>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 byteCount)
{
    #if 0
    Memory::PeekByteArray(mAddress + offset, dst, dstOffset, byteCount);
    #endif
}

void MemoryBlock::PeekCharArray(
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount,
    /* [in] */ Boolean swap)
{
    #if 0
    Memory::PeekCharArray(mAddress + offset, dst, dstOffset, charCount, swap);
    #endif
}

void MemoryBlock::PeekDoubleArray(
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<Double>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 doubleCount,
    /* [in] */ Boolean swap)
{
    #if 0
    Memory::PeekDoubleArray(mAddress + offset, dst, dstOffset, doubleCount, swap);
    #endif
}

void MemoryBlock::PeekFloatArray(
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 floatCount,
    /* [in] */ Boolean swap)
{
    #if 0
    Memory::PeekFloatArray(mAddress + offset, dst, dstOffset, floatCount, swap);
    #endif
}

void MemoryBlock::PeekInt32Array(
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<Int32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int32Count,
    /* [in] */ Boolean swap)
{
    #if 0
    Memory::PeekInt32Array(mAddress + offset, dst, dstOffset, int32Count, swap);
    #endif
}

void MemoryBlock::PeekInt64Array(
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<Int64>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int64Count,
    /* [in] */ Boolean swap)
{
    #if 0
    Memory::PeekInt64Array(mAddress + offset, dst, dstOffset, int64Count, swap);
    #endif
}

void MemoryBlock::PeekInt16Array(
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<Int16>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int16Count,
    /* [in] */ Boolean swap)
{
    #if 0
    Memory::PeekInt16Array(mAddress + offset, dst, dstOffset, int16Count, swap);
    #endif
}

void MemoryBlock::PokeInt16(
    /* [in] */ Int32 offset,
    /* [in] */ Int16 value,
    /* [in] */ ByteOrder order)
{
    Boolean isflag = FALSE;
    AutoPtr<CByteOrderHelper> obj;
    CByteOrderHelper::AcquireSingletonByFriend((CByteOrderHelper**)&obj);
    AutoPtr<IByteOrderHelper> boh = (IByteOrderHelper*)obj.Get();
    boh->IsNeedsSwap(order , &isflag);
    #if 0
    Memory::PokeInt16(mAddress + offset, value, isflag);
    #endif
}

Int16 MemoryBlock::PeekInt16(
    /* [in] */ Int32 offset,
    /* [in] */ ByteOrder order)
{
    Boolean isflag = FALSE;
    AutoPtr<CByteOrderHelper> obj;
    CByteOrderHelper::AcquireSingletonByFriend((CByteOrderHelper**)&obj);
    AutoPtr<IByteOrderHelper> boh = (IByteOrderHelper*)obj.Get();
    boh->IsNeedsSwap(order , &isflag);
    #if 0
    return Memory::PeekInt16(mAddress + offset, isflag);
    #endif
    return 0;
}

void MemoryBlock::PokeInt32(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 value,
    /* [in] */ ByteOrder order)
{
    Boolean isflag = FALSE;
    AutoPtr<CByteOrderHelper> obj;
    CByteOrderHelper::AcquireSingletonByFriend((CByteOrderHelper**)&obj);
    AutoPtr<IByteOrderHelper> boh = (IByteOrderHelper*)obj.Get();
    boh->IsNeedsSwap(order , &isflag);
    #if 0
    Memory::PokeInt32(mAddress + offset, value, isflag);
    #endif
}

Int32 MemoryBlock::PeekInt32(
    /* [in] */ Int32 offset,
    /* [in] */ ByteOrder order)
{
    Boolean isflag = FALSE;
    AutoPtr<CByteOrderHelper> obj;
    CByteOrderHelper::AcquireSingletonByFriend((CByteOrderHelper**)&obj);
    AutoPtr<IByteOrderHelper> boh = (IByteOrderHelper*)obj.Get();
    boh->IsNeedsSwap(order , &isflag);
    #if 0
    return Memory::PeekInt32(mAddress + offset, isflag);
    #endif
    return 0;
}

void MemoryBlock::PokeInt64(
    /* [in] */ Int32 offset,
    /* [in] */ Int64 value,
    /* [in] */ ByteOrder order)
{
    Boolean isflag = FALSE;
    AutoPtr<CByteOrderHelper> obj;
    CByteOrderHelper::AcquireSingletonByFriend((CByteOrderHelper**)&obj);
    AutoPtr<IByteOrderHelper> boh = (IByteOrderHelper*)obj.Get();
    boh->IsNeedsSwap(order , &isflag);
    #if 0
    Memory::PokeInt64(mAddress + offset, value, isflag);
    #endif
}

Int64 MemoryBlock::PeekInt64(
    /* [in] */ Int32 offset,
    /* [in] */ ByteOrder order)
{
    Boolean isflag = FALSE;
    AutoPtr<CByteOrderHelper> obj;
    CByteOrderHelper::AcquireSingletonByFriend((CByteOrderHelper**)&obj);
    AutoPtr<IByteOrderHelper> boh = (IByteOrderHelper*)obj.Get();
    boh->IsNeedsSwap(order , &isflag);
    #if 0
    return Memory::PeekInt64(mAddress + offset, isflag);
    #endif
    return 0;
}

Int64 MemoryBlock::ToInt64()
{
    return mAddress;
}

String MemoryBlock::ToString()
{
    // return getClass().getName() + "[" + address + "]";
    return String("MemoryBlock");
}

Int64 MemoryBlock::GetSize()
{
    return mSize;
}

Boolean MemoryBlock::IsFreed()
{
    return mFreed;
}

Boolean MemoryBlock::IsAccessible()
{
    return !IsFreed() && mAccessible;
}

void MemoryBlock::SetAccessible(
    /* [in] */ Boolean accessible)
{
    mAccessible = accessible;
}

ECode MemoryMappedBlock::Free()
{
    if (mAddress != 0) {
//        try {
        CLibcore::sOs->Munmap(mAddress, mSize);
//        } catch (ErrnoException errnoException) {
//            // The RI doesn't throw, presumably on the assumption that you can't get into
//            // a state where munmap(2) could return an error.
//            throw new AssertionError(errnoException);
//        }
        mAddress = 0;
    }
    return NOERROR;
}


AutoPtr< ArrayOf<Byte> > NonMovableHeapBlock::GetArray()
{
    return mArray;
}

ECode NonMovableHeapBlock::Free()
{
    mArray = NULL;
    mAddress = 0;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
