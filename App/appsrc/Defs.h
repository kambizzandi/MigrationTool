/******************************************************************************
#   TargomanMigrate
#
#   Copyright 2014-2020 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   TargomanMigrate is free software: you can redistribute it and/or modify
#   it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   TargomanMigrate is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU AFFERO GENERAL PUBLIC LICENSE for more details.
#
#   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_MIGRATE_DEFS_H
#define TARGOMAN_MIGRATE_DEFS_H

#include <QHostAddress>
#include <QVariantMap>
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/tmplConfigurableArray.hpp"
using namespace Targoman::Common::Configuration;

namespace Targoman::Migrate {

TARGOMAN_DEFINE_ENHANCED_ENUM(enuAppCommand,
                              showconf,
                              createdb,
                              createdbdiff,
                              createlocal,
                              list,
                              history,
                              commit,
//                              rollback,
//                              redo,
//                              fresh,
                              mark
                              );

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
TARGOMAN_ADD_EXCEPTION_HANDLER(exTargomanMigrate, Targoman::Common::exTargomanBase);
#pragma clang diagnostic pop

struct stuMigrationDB
{
    tmplConfigurable<QString>  Host;
    tmplConfigurable<QString>  Port;
    tmplConfigurable<QString>  UserName;
    tmplConfigurable<QString>  Password;
    tmplConfigurable<QString>  Schema;

    stuMigrationDB(const QString &_basePath) :
        Host(
            _basePath + "Host",
            "",
            ""
        ),
        Port(
            _basePath + "Port",
            "",
            ""
        ),
        UserName(
            _basePath + "UserName",
            "",
            ""
        ),
        Password(
            _basePath + "Password",
            "",
            ""
        ),
        Schema(
            _basePath + "Schema",
            "",
            ""
        )
    {}

    void setFromVariant(const QVariant &_value)
    {
    }

    QVariant toVariant() const
    {
        return ""; //"stuMigrationDB";
    }
};

struct stuMigrationSource
{
    // ./migrations/{Name}/db/{DBSchema}/mYYYYMMDD_HHMMSS.sh
    // ./migrations/{Name}/local/mYYYYMMDD_HHMMSS.sh
    // ./migrations/{Name}/local/.migrations
    tmplConfigurable<QString>               Name;
    tmplConfigurableArray<stuMigrationDB>   DB;

    stuMigrationSource(const QString &_basePath) :
        Name(
            _basePath + "Name",
            "Name of the source. will be used in migrations path : ./migrations/{Name}/...",
            ""
        ),
        DB(
            _basePath + "DB",
            "Database information of source",
            1
        )
    {}

    void setFromVariant(const QVariant &_value)
    {
    }

    QVariant toVariant() const
    {
        return ""; //"stuMigrationSource";
    }
};

} // namespace Targoman::Migrate

#endif // TARGOMAN_MIGRATE_DEFS_H
