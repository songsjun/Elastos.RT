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

#ifndef __ELASTOS_SQL_SQLITE_CDATABASE_H__
#define __ELASTOS_SQL_SQLITE_CDATABASE_H__

#include "_Elastos_Sql_SQLite_CDatabase.h"
#include "elastos/sql/sqlite/Database.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CDatabase) , public Database
{
public:
    CAR_OBJECT_DECL()
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __ELASTOS_SQL_SQLITE_CDATABASE_H__
