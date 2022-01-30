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

#ifndef TARGOMAN_MIGRATE_APPCONFIGS_H
#define TARGOMAN_MIGRATE_APPCONFIGS_H

#include <QHostAddress>
#include <QVariantMap>
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/tmplConfigurableArray.hpp"
using namespace Targoman::Common::Configuration;

namespace Targoman::Migrate {

TARGOMAN_DEFINE_ENHANCED_ENUM(enuAppCommand,
                              CreateDB,
                              CreateDBDiff,
                              CreateLocal,
                              List,
                              History,
                              Up,
                              UpTo,
//                              Down,
//                              Redo,
//                              Fresh,
                              Mark
                              );

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
TARGOMAN_ADD_EXCEPTION_HANDLER(exTargomanMigrate, Targoman::Common::exTargomanBase);
#pragma clang diagnostic pop

//struct stuServers
//{
//    Targoman::Common::Configuration::tmplConfigurable<QString> Address;
//    Targoman::Common::Configuration::tmplConfigurable<bool>    Active;

//    stuServers(const QString &_basePath) :
//        Address(_basePath + "Address",
//                "kjkjkldfj sl s sm klsjflkdsl jaklfjkl jdsljfldskf klsjdklfjldskfjkldsl jdsklfjskl djlfjs friostjls jlskdfjklsdjf lksklfjdskl jkldsjfkljsfkljdsklfj dlfjldskfj dsklfjklj (sdfkdsfjkdsh jkdshfjdsf hsfjksh jks) skshfhdskjfhkd hfdsf;sd shfjhds jkfhjh fhsdjfh jksdhfsd fds j; dskjfklsdj"),
//        Active(_basePath + "Active",
//               "fdsfdf",
//               true)
//    { }

//    void setFromVariant(const QVariant &_value)
//    {
//        this->Address.setFromVariant(_value.toMap().value("Address"));
//        this->Active.setFromVariant(_value.toMap().value("Active"));
//    }

//    QVariant toVariant() const
//    {
//        return QVariantMap({
//                               { "Address", this->Address.toVariant() },
//                               { "Active", this->Active.toVariant() },
//                           });
//    }
//};

/*

------------
-- dev:
------------

PWD: .../TargomanAPI

./migrations/migration.conf
0:  Name         = App
    DBHosts      = 127.0.0.1,127.0.0.1
    DBPorts      = 3306,3306
    DBUserNames  = user_commonfuncs,user_i18n
    DBPasswords  = psw1,psw2
    DBSchemas    = CommonFuncs,I18N

1:  Name         = Account
    DBHosts      = 127.0.0.1
    DBPorts      = 3306
    DBUserNames  = user_account
    DBPasswords  = psw1
    DBSchemas    = Account

./App/migrations/db/CommonFuncs/
./App/migrations/db/I18N/
./App/migrations/local/
./App/migrations/local/.migrations (omitted by .gitignore)
SYMLINK: ./migrations/App -> ./App/migrations

./Modules/Account/migrations/db/Account/
./Modules/Account/migrations/local/
./Modules/Account/migrations/local/.migrations (omitted by .gitignore)
SYMLINK: ./migrations/Account -> ./Modules/Account/migrations

------------
-- prod:
------------

PWD: /targoman

./bin/
    fasttext
    targomanAPI
./build/
    ...
./conf/
    TODO
    TextProcessor
    api.conf
    api.server.conf
    invalidPass.tbl
    test-run.conf
    test.conf
./include/
    ...
./lib/
    ...
./lib64/
    ...
./modules/
    ...
./run-targoman.sh
./submodules/
    ...
./unitTest/
    ...

-- migrations:

./migrations/migration.conf

./migrations/App/db/CommonFuncs/
./migrations/App/db/I18N/
./migrations/App/local/

./migrations/Account/db/Account/
./migrations/Account/local/

*/

struct stuMigrationDB
{
    tmplConfigurable<QStringList>  Hosts;
    tmplConfigurable<QStringList>  Ports;
    tmplConfigurable<QStringList>  UserNames;
    tmplConfigurable<QStringList>  Passwords;
    tmplConfigurable<QStringList>  Schemas;

    stuMigrationDB(const QString &_basePath) :
        Hosts(
            _basePath + "Hosts",
            "",
            ""
        ),
        Ports(
            _basePath + "Ports",
            "",
            ""
        ),
        UserNames(
            _basePath + "UserNames",
            "",
            ""
        ),
        Passwords(
            _basePath + "Passwords",
            "",
            ""
        ),
        Schemas(
            _basePath + "Schemas",
            "",
            ""
        )
    {}

    void setFromVariant(const QVariant &_value)
    {
    }

    QVariant toVariant() const
    {
        return "stuMigrationDB";
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
        return "stuMigrationSource";
    }
};

struct AppConfigs
{
    static inline QString makeConfig(const QString& _name) { return "/Migrate/" + _name; }

    static tmplConfigurable<enuAppCommand::Type>        Command;
    static tmplConfigurable<QString>                    MigrationsFolderName;
    static tmplConfigurable<QString>                    GlobalHistoryTableName;
    static tmplConfigurable<QString>                    LocalHistoryFileName;
    static tmplConfigurableArray<stuMigrationSource>    Sources;
    static tmplConfigurable<QString>                    ApplyToAllSourceName;




//    static tmplConfigurable<QString>       BasePath;
//    static tmplConfigurable<QString>       Version;
//    static tmplRangedConfigurable<quint16> ListenPort;
//    static tmplConfigurable<bool>          IndentedJson;
//    static tmplRangedConfigurable<qint64>  MaxUploadSize;
//    static tmplRangedConfigurable<qint64>  MaxUploadedFileSize;
//    static tmplConfigurable<FilePath_t>    BaseOpenAPIObjectFile;
//    static tmplRangedConfigurable<quint8>  StatisticsInterval;
//    static tmplRangedConfigurable<quint32> MaxCachedItems;
//    static tmplConfigurable<QString>       CacheConnector;
//    static tmplConfigurable<QString>       AccessControl;
//    static tmplConfigurable<bool>          MultiThreaded;
//    static tmplRangedConfigurable<qint32>  APICallTimeout;
//    static tmplConfigurable<FilePath_t>    PublicPath;
//    static tmplConfigurable<FilePath_t>    SwaggerUI;
//    static tmplRangedConfigurable<quint32> FileMaxChunk;

//    static QString BasePathWithVersion;

//    struct MasterDB {
//        static inline QString makeConfig(const QString& _name){return "/MasterDB/" + _name;}
//        static tmplConfigurable<QString>      Host;
//        static tmplRangedConfigurable<quint16>Port;
//        static tmplConfigurable<QString>      User;
//        static tmplConfigurable<QString>      Pass;
//        static tmplConfigurable<QString>      Schema;
//    };
};

//constexpr  quint16 TAPI_BASE_USER_DEFINED_TYPEID = QMetaType::User + 3;

//extern TAPI::stuStatistics gServerStats;

} // namespace Targoman::Migrate

ENUM_CONFIGURABLE(Targoman::Migrate::enuAppCommand);
//S P ECIAL_CONFIGURABLE(Targoman::Migrate::stuServers);

template <> inline void Targoman::Common::Configuration::tmplConfigurableArray<Targoman::Migrate::stuMigrationDB>::setFromVariant(const QVariant& _value)
{
}
template <> inline QVariant Targoman::Common::Configuration::tmplConfigurableArray<Targoman::Migrate::stuMigrationDB>::toVariant() const
{
    return "tmplConfigurableArray<Targoman::Migrate::stuMigrationDB>";
}

template <> inline void Targoman::Common::Configuration::tmplConfigurableArray<Targoman::Migrate::stuMigrationSource>::setFromVariant(const QVariant& _value)
{
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
    return "tmplConfigurableArray<Targoman::Migrate::stuMigrationSource>";

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

#endif // TARGOMAN_MIGRATE_APPCONFIGS_H
