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

#ifndef __HELLOCAR_CMYTHREAD_H__
#define __HELLOCAR_CMYTHREAD_H__

//#include "_Elastos_HelloCar_CMyThread.h"    // include 编译器生成的夹壁墙头文件，文件名格式：_命名空间_CAR类名称.h
#include "_CMyThread.h"    // include 编译器生成的夹壁墙头文件，文件名格式：_命名空间_CAR类名称.h
#include "_Elastos.CoreLibrary.IO.h"
#include <elastos/core/Thread.h>            // include 基类

using Elastos::Core::Thread;
using Elastos::IO::IFileDescriptor;

//namespace Elastos {
//namespace HelloCar {

CarClass(CMyThread)
    , public Thread
{
public:
    CAR_OBJECT_DECL()   // 非单例 CAR 类需要使用宏 CAR_OBJECT_DECL/CAR_OBJECT_IMPL

    CARAPI constructor(IFileDescriptor *ifd);

    CARAPI Run();

private:
	AutoPtr<IFileDescriptor> mOutFd;
};

//} // HelloCar
//} // Elastos

#endif //__HELLOCAR_CTHREAD_H__
