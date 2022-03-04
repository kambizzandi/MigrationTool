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

#ifndef TARGOMAN_MIGRATE_CONFIGS_H
#define TARGOMAN_MIGRATE_CONFIGS_H

#include "Defs.h"

namespace Targoman::Migrate {

struct Configs
{
    static inline QString makeConfig(const QString& _name) { return "/App/" + _name; }

    static tmplConfigurable<enuAppCommand::Type>        Command;
    static tmplConfigurable<QString>                    MigrationsFolderName;
    static tmplConfigurable<QString>                    GlobalHistoryTableName;
    static tmplConfigurable<QString>                    LocalHistoryFileName;

    static tmplConfigurableArray<stuDBServer>           DBServers;
    static tmplConfigurableArray<stuRunningMode>        RunningModes;
    static tmplConfigurable<QString>                    ActiveRunningMode;
    static tmplConfigurableArray<stuProject>            Projects;

    static tmplConfigurable<bool>                       DBOnly;
    static tmplConfigurable<bool>                       LocalOnly;

    static tmplConfigurable<QString>                    DefaultEditor;

    //--------------------------
    struct stuRunningParameters
    {
//        int RunningModeIndex;

//        QString       RunningModeName; //dev|prod
        QStringList   RunningModeDBServers; //devdb1,devdb2 | db1,db2,db3

        //key: Project.Name
        QMap<QString, QStringList> ProjectAllowedDBServers;

        //key: Project.Name @ DBServer.Name
        QMap<QString, QString> ProjectDBConnectionStrings;

        stuRunningParameters() :
//            RunningModeIndex(-1),
//            RunningModeName(),
            RunningModeDBServers()
        {}
    };
    static stuRunningParameters RunningParameters;
    static void FillRunningParameters();
};

} // namespace Targoman::Migrate

ENUM_CONFIGURABLE(Targoman::Migrate::enuAppCommand);
//S P ECIAL_CONFIGURABLE(Targoman::Migrate::stuServers);

#endif // TARGOMAN_MIGRATE_CONFIGS_H
