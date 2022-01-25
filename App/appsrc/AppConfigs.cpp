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

#include "AppConfigs.h"
#include "libTargomanCommon/Configuration/Validators.hpp"

namespace Targoman::Migrate {

using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;

//QString AppConfigs::BasePathWithVersion;
//TAPI::stuStatistics gServerStats;

tmplConfigurable<enuAppAction::Type> AppConfigs::Action(
    AppConfigs::makeConfig("Action"),
    "Application main action",
    enuAppAction::List,
    ReturnTrueCrossValidator(),
    "",
    "ACTION",
    "action",
    enuConfigSource::Arg
);

//tmplConfigurable<QString> AppConfigs::ActionCreateDB(
//    AppConfigs::makeConfig("ActionCreateDB"),
//    "Create new database migration file",
//    "",
//    ReturnTrueCrossValidator(),
//    "",
//    "",
//    "create-db",
//    enuConfigSource::Arg
//);

//tmplConfigurable<QString> AppConfigs::ActionCreateLocal(
//    AppConfigs::makeConfig("ActionCreateLocal"),
//    "Create new local migration file",
//    "",
//    ReturnTrueCrossValidator(),
//    "",
//    "",
//    "create-local",
//    enuConfigSource::Arg
//);

//tmplConfigurable<QString> AppConfigs::ActionList(
//    AppConfigs::makeConfig("ActionList"),
//    "Shows the first (n|all) new migrations",
//    "10",
//    ReturnTrueCrossValidator(),
//    "",
//    "",
//    "list",
//    enuConfigSource::Arg
//);

//tmplConfigurable<QString> AppConfigs::ActionHistory(
//    AppConfigs::makeConfig("ActionHistory"),
//    "Shows the last (n|all) applied migrations",
//    "10",
//    ReturnTrueCrossValidator(),
//    "",
//    "",
//    "history",
//    enuConfigSource::Arg
//);

//tmplConfigurable<bool> AppConfigs::ActionUp(
//    AppConfigs::makeConfig("ActionUp"),
//    "Upgrade by running migrate files",
//    false,
//    ReturnTrueCrossValidator(),
//    "",
//    "",
//    "up",
//    enuConfigSource::Arg
//);

//tmplConfigurable<QString> AppConfigs::ActionUpTo(
//    AppConfigs::makeConfig("ActionUpTo"),
//    "Upgrade by running migrate files up to migration file name",
//    "",
//    ReturnTrueCrossValidator(),
//    "",
//    "",
//    "up-to",
//    enuConfigSource::Arg
//);

/*
tmplConfigurable<QString> AppConfigs::BasePath(
        AppConfigs::makeConfig("BasePath"),
        "Base path to access the REST API",
        "rest",
        ReturnTrueCrossValidator(),
        "",
        "PATH",
        "base-path",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> AppConfigs::Version(
        AppConfigs::makeConfig("Version"),
        "REST API version to be appended to base path",
        "v1",
        ReturnTrueCrossValidator(),
        "",
        "VERSION",
        "version",
        enuConfigSource::Arg | enuConfigSource::File);


tmplRangedConfigurable<quint16> AppConfigs::ListenPort(
        AppConfigs::makeConfig("ListenPort"),
        "Listen port for main REST server",
        1000,65000,
        10000,
        ReturnTrueCrossValidator(),
        "p",
        "PORT",
        "listen-port",
        enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<quint8> AppConfigs::StatisticsInterval(
        AppConfigs::makeConfig("StatisticsInterval"),
        "Listen port for main REST server",
        1,60,
        10,
        ReturnTrueCrossValidator(),
        "",
        "INTERVAL",
        "stats-interval",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<bool> AppConfigs::IndentedJson(
        AppConfigs::makeConfig("IndentedJson"),
        "If set to true Json outputs will be indented",
        false,
        ReturnTrueCrossValidator(),
        "",
        "",
        "indented-json",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<bool> AppConfigs::MultiThreaded(
        AppConfigs::makeConfig("MultiThreaded"),
        "If set to false then application will run in single-threaded mode and will be blocked on time-consuming API calls",
        true,
        ReturnTrueCrossValidator(),
        "",
        "",
        "",
        enuConfigSource::File);

tmplRangedConfigurable<qint32> AppConfigs::APICallTimeout(
        AppConfigs::makeConfig("APICallTimeout"),
        "default timeout for API Calls. It can be reconfigured on each API Call. set to -1 for infinite",
        -1, 60 * 1000,
        -1,
        ReturnTrueCrossValidator(),
        "",
        "MILISECONDS",
        "default-api-timeout",
        enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<qint64> AppConfigs::MaxUploadSize(
        AppConfigs::makeConfig("MaxUploadSize"),
        "Max Upload size for any type of request",
        10*1024, 10*1024*1024,
        10*1024*1024,
        ReturnTrueCrossValidator(),
        "",
        "SIZE",
        "max-upload-size",
        enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<quint32> AppConfigs::FileMaxChunk(
        AppConfigs::makeConfig("FileMaxChunk"),
        "Max file size to send on each chunk.",
        10*1024, 10*1024*1024,
        10*1024,
        ReturnTrueCrossValidator(),
        "",
        "SIZE",
        "chunk-size",
        enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<qint64> AppConfigs::MaxUploadedFileSize(
        AppConfigs::makeConfig("MaxUploadedFileSize"),
        "Max Upload size for files",
        1*1024, 100*1024*1024,
        100*1024*1024,
        ReturnTrueCrossValidator(),
        "",
        "SIZE",
        "max-file-upload-size",
        enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<quint32> AppConfigs::MaxCachedItems(
        AppConfigs::makeConfig("MaxCachedItems"),
        "Maximum API calls to be cached",
        0,65000,
        5000,
        ReturnTrueCrossValidator(),
        "",
        "SIZE",
        "max-cache-size",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> AppConfigs::CacheConnector(
        AppConfigs::makeConfig("CacheConnector"),
        "Connection string to connect to Redis cache server",
        "",
        ReturnTrueCrossValidator(),
        "",
        "CONNECTION_STRING",
        "cache-connector",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> AppConfigs::AccessControl(
        AppConfigs::makeConfig("AccessControl"),
        "Default access control",
        "*",
        ReturnTrueCrossValidator(),
        "",
        "ACCESS-CONTROL-STRING",
        "",
        enuConfigSource::File);

tmplConfigurable<FilePath_t>     AppConfigs::BaseOpenAPIObjectFile(
        AppConfigs::makeConfig("BaseOpenAPIObjectFile"),
        "Input file path where default OpenAPI object file resides",
        "",
        Validators::tmplPathAccessValidator<
        TARGOMAN_PATH_ACCESS(enuPathAccess::File | enuPathAccess::Readable),
        false>,
        "",
        "FILEPATH",
        "openapi-file",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<FilePath_t>     AppConfigs::SwaggerUI(
        AppConfigs::makeConfig("SwaggerUI"),
        "Directory where swaggerUI files reside",
        "",
        Validators::tmplPathAccessValidator<
        TARGOMAN_PATH_ACCESS(enuPathAccess::Dir | enuPathAccess::Readable),
        false>,
        "",
        "DIRECTORY",
        "swaggerui-path",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<FilePath_t>     AppConfigs::PublicPath(
        AppConfigs::makeConfig("PublicFolder"),
        "Directory static files are stored",
        "",
        Validators::tmplPathAccessValidator<
        TARGOMAN_PATH_ACCESS(enuPathAccess::Dir | enuPathAccess::Readable),
        false>,
        "",
        "DIRECTORY",
        "public-path",
        enuConfigSource::Arg | enuConfigSource::File);


tmplConfigurable<QString> AppConfigs::MasterDB::Host(
        AppConfigs::MasterDB::makeConfig("Host"),
        "Database Host address",
        "127.0.0.1",
        ReturnTrueCrossValidator(),
        "",
        "HOST",
        "db-host",
        enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<quint16> AppConfigs::MasterDB::Port(
        AppConfigs::MasterDB::makeConfig("Port"),
        "Database port",
        10, 65000,
        3306,
        ReturnTrueCrossValidator(),
        "",
        "PORT",
        "db-port",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> AppConfigs::MasterDB::User(
        AppConfigs::MasterDB::makeConfig("User"),
        "Database username for connection",
        "root",
        ReturnTrueCrossValidator(),
        "",
        "USER",
        "db-user",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> AppConfigs::MasterDB::Pass(
        AppConfigs::MasterDB::makeConfig("Pass"),
        "Database password",
        "",
        ReturnTrueCrossValidator(),
        "",
        "PASS",
        "db-pass",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> AppConfigs::MasterDB::Schema(
        AppConfigs::MasterDB::makeConfig("Schema"),
        "Database schema",
        "MT",
        ReturnTrueCrossValidator(),
        "",
        "SCHEMA",
        "db-schema",
        enuConfigSource::Arg | enuConfigSource::File);
*/

} //namespace Targoman::Migrate

ENUM_CONFIGURABLE_IMPL(Targoman::Migrate::enuAppAction);
