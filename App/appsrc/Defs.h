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

#define instanceGetterPtr(_class) static _class* instancePtr() { \
    static _class* Instance = nullptr; return (Q_LIKELY(Instance) ? Instance : (Instance = new _class)); \
}

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

constexpr char LINE_SPLITTER[] = "------------------------------------------------------------------------";
constexpr char REGEX_PATTERN_MIGRATION_FILENAME[]     = "m[0-9]{8}_[0-9]{6}_[a-zA-Z0-9-_]*.(sh|sql)";
constexpr char REGEX_PATTERN_MIGRATION_LOG_FILENAME[] = "m[0-9]{8}_[0-9]{6}_[a-zA-Z0-9-_]*.(sh|sql).log";

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

inline QString bold(const QString &_s)
{
    return QString("\x1b[1m%1\x1b[0m").arg(_s);
}

inline QString reverse(const QString &_s)
{
    return QString("\x1b[7m%1\x1b[0m").arg(_s);
}

inline QTextStream& qStdout()
{
    static QTextStream rOUT{stdout};
    return rOUT;
}
inline QTextStream& qStdIn()
{
    static QTextStream rIN{stdin};
    return rIN;
}

} // namespace Targoman::Migrate

#endif // TARGOMAN_MIGRATE_DEFS_H
