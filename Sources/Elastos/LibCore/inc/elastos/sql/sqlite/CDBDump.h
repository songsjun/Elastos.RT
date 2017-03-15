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

#ifndef __ELASTOS_SQL_SQLITE_CDBDUMP_H__
#define __ELASTOS_SQL_SQLITE_CDBDUMP_H__

#include "_Elastos_Sql_SQLite_CDBDump.h"
#include "CShell.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CDBDump)
    , public Object
    , public IDBDump
    , public ICallback
{
public:
    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    CARAPI Columns(
        /* [in] */ ArrayOf<String> * coldata);

    CARAPI Types(
        /* [in] */ ArrayOf<String> * types);

    CARAPI Newrow(
        /* [in] */ ArrayOf<String> * args,
        /* [out] */ Boolean * value);

    CARAPI constructor(
        /* [in] */ IShell * ms,
        /* [in] */ ArrayOf<String> * tables);

private:
    AutoPtr<CShell> mShell;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __ELASTOS_SQL_SQLITE_CDBDUMP_H__
