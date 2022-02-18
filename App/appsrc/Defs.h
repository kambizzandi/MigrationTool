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
        Q_UNUSED(_value);
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
        Q_UNUSED(_value);
    }

    QVariant toVariant() const
    {
        return ""; //"stuMigrationSource";
    }
};


//////////////////////////////////////////////////////////////////////

struct stuDBServers
{
    tmplConfigurable<QString>  Name; //devdb1|devdb2|db1|db2|db3
    tmplConfigurable<QString>  Host;
    tmplConfigurable<QString>  Port;
    tmplConfigurable<QString>  UserName;
    tmplConfigurable<QString>  Password;

    stuDBServers(const QString &_basePath) :
        Name(
            _basePath + "Name",
            "",
            ""
        ),
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
        )
    {}

    void setFromVariant(const QVariant &_value)
    {
        Q_UNUSED(_value);
    }

    QVariant toVariant() const
    {
        return ""; //"stuDBServers";
    }
};

struct stuRunningModes
{
    tmplConfigurable<QString>       Name; //dev|prod
    tmplConfigurable<QStringList>   DBServers; //devdb1,devdb2 | db1,db2,db3

    stuRunningModes(const QString &_basePath) :
        Name(
            _basePath + "Name",
            "Name of the running mode"
        ),
        DBServers(
            _basePath + "DBServers",
            "Database servers"
        )
    {}

    void setFromVariant(const QVariant &_value)
    {
        Q_UNUSED(_value);
    }

    QVariant toVariant() const
    {
        return ""; //"stuRunningModes";
    }
};

struct stuProjects
{
    // ./migrations/{Name}/db/mYYYYMMDD_HHMMSS.sh
    // ./migrations/{Name}/local/mYYYYMMDD_HHMMSS.sh
    // ./migrations/{Name}/local/.migrations
    tmplConfigurable<QString>       Name;
    tmplConfigurable<bool>          AllowDB;
    tmplConfigurable<bool>          AllowLocal;
    tmplConfigurable<QStringList>   DBDestinations; //devdb1,db2

    stuProjects(const QString &_basePath) :
        Name(
            _basePath + "Name",
            "Name of the project. will be used in migrations path : ./migrations/{Name}/...",
            ""
        ),
        AllowDB(
            _basePath + "AllowDB",
            "",
            false
        ),
        AllowLocal(
            _basePath + "AllowLocal",
            "",
            false
        ),
        DBDestinations(
            _basePath + "DBDestinations",
            "Database destinations"
        )
    {}

    void setFromVariant(const QVariant &_value)
    {
        Q_UNUSED(_value);
    }

    QVariant toVariant() const
    {
        return ""; //"stuProjects";
    }
};

//////////////////////////////////////////////////////////////////////

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

template <> inline void Targoman::Common::Configuration::tmplConfigurableArray<Targoman::Migrate::stuMigrationDB>::setFromVariant(const QVariant& _value)
{
    Q_UNUSED(_value);
}
template <> inline QVariant Targoman::Common::Configuration::tmplConfigurableArray<Targoman::Migrate::stuMigrationDB>::toVariant() const
{
    return ""; //"tmplConfigurableArray<Targoman::Migrate::stuMigrationDB>";
}

template <> inline void Targoman::Common::Configuration::tmplConfigurableArray<Targoman::Migrate::stuMigrationSource>::setFromVariant(const QVariant& _value)
{
    Q_UNUSED(_value);

//    QJsonDocument doc;
//    doc.fromJson(_value.toString().toLatin1());

//    this->Items.clear();

//    if (doc.isEmpty())
//        return;

//    QJsonArray arr = doc.array();
//    this->Items.reserve(arr.count());

//    int idx = 0;
//    foreach (auto Value, arr)
//    {
//        this->Items[idx++].setFromVariant(Value);
//    }
}
template <> inline QVariant Targoman::Common::Configuration::tmplConfigurableArray<Targoman::Migrate::stuMigrationSource>::toVariant() const
{
    return ""; //"tmplConfigurableArray<Targoman::Migrate::stuMigrationSource>";

//    QVariantList Values;

//    foreach(auto Item, this->Items)
//    {
//        Values.append(Item.toVariant());
//    }

//    QJsonArray arr;
//    arr.fromVariantList(Values);

//    QJsonDocument doc;
//    doc.setArray(arr);

//    QVariant v = doc.toJson();
//    auto s = v.toString();
//    auto s1 = s.toUtf8();
//    auto s2 = s1.constData();

//    return doc.toJson();
}

template <> inline void Targoman::Common::Configuration::tmplConfigurableArray<Targoman::Migrate::stuDBServers>::setFromVariant(const QVariant& _value)
{
    Q_UNUSED(_value);
}
template <> inline QVariant Targoman::Common::Configuration::tmplConfigurableArray<Targoman::Migrate::stuDBServers>::toVariant() const
{
    return ""; //"tmplConfigurableArray<Targoman::Migrate::stuDBServers>";
}
template <> inline void Targoman::Common::Configuration::tmplConfigurableArray<Targoman::Migrate::stuRunningModes>::setFromVariant(const QVariant& _value)
{
    Q_UNUSED(_value);
}
template <> inline QVariant Targoman::Common::Configuration::tmplConfigurableArray<Targoman::Migrate::stuRunningModes>::toVariant() const
{
    return ""; //"tmplConfigurableArray<Targoman::Migrate::stuRunningModes>";
}
template <> inline void Targoman::Common::Configuration::tmplConfigurableArray<Targoman::Migrate::stuProjects>::setFromVariant(const QVariant& _value)
{
    Q_UNUSED(_value);
}
template <> inline QVariant Targoman::Common::Configuration::tmplConfigurableArray<Targoman::Migrate::stuProjects>::toVariant() const
{
    return ""; //"tmplConfigurableArray<Targoman::Migrate::stuProjects>";
}

#endif // TARGOMAN_MIGRATE_DEFS_H
