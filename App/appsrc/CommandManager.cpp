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

#include <iostream>

#include "Defs.h"
#include "Configs.h"
#include "CommandManager.h"
#include "Commands/intfCommand.h"
#include "Commands/cmdShowConf.h"
#include "Commands/cmdCreateDB.h"
#include "Commands/cmdCreateDBDiff.h"
#include "Commands/cmdCreateLocal.h"
#include "Commands/cmdHistory.h"
#include "Commands/cmdList.h"
#include "Commands/cmdMark.h"
#include "Commands/cmdCommit.h"

//#include "libTargomanCommon/Configuration/ConfigManager.h"
//#include "libTargomanDBM/clsDAC.h"

//#include "Interfaces/API/intfPureModule.h"
//#include <QCoreApplication>
//#include <QPluginLoader>
//#include "libTargomanDBM/clsDAC.h"

using namespace Targoman::Common;
//using namespace Targoman::DBManager;

namespace Targoman::Migrate {

using namespace Commands;

CommandManager::CommandManager(QObject *parent) : QObject(parent)
{}

void CommandManager::slotExecute()
{
    try
    {
        intfCommand *Command = nullptr;

        switch (Configs::Command.value())
        {
            case enuAppCommand::showconf:
                Command = cmdShowConf::instancePtr();
                break;

            case enuAppCommand::createdb:
                Command = cmdCreateDB::instancePtr();
                break;

            case enuAppCommand::createdbdiff:
                Command = cmdCreateDBDiff::instancePtr();
                break;

            case enuAppCommand::createlocal:
                Command = cmdCreateLocal::instancePtr();
                break;

            case enuAppCommand::history:
                Command = cmdHistory::instancePtr();
                break;

            case enuAppCommand::list:
                Command = cmdList::instancePtr();
                break;

            case enuAppCommand::mark:
                Command = cmdMark::instancePtr();
                break;

            case enuAppCommand::commit:
                Command = cmdCommit::instancePtr();
                break;

//            case enuAppCommand::rollback:
//                Command = cmdRollback::instancePtr();
//                break;

//            case enuAppCommand::redo:
//                Command = cmdRedo::instancePtr();
//                break;

//            case enuAppCommand::fresh:
//                Command = cmdFresh::instancePtr();
//                break;

            default:
                break;
        }

        if (Command == nullptr)
            throw exTargomanMigrate("Invalid command");

        if (false) //SHOW_HELP
        {
            Command->help();
            QCoreApplication::exit(0);
            return;
        }

        //---------------------------------
        if (Command->needDB())
        {
            if (Configs::Sources.size() == 0)
                throw exTargomanBase("Source not defined");

            for (size_t idxSource=0; idxSource<Configs::Sources.size(); idxSource++)
            {
                stuMigrationSource &Source = Configs::Sources[idxSource];

                if (Source.DB.size() > 0)
                {
                    for (size_t idxDB=0; idxDB<Source.DB.size(); idxDB++)
                    {
                        stuMigrationDB &DB = Source.DB[idxDB];

                        clsDAC::addDBEngine(enuDBEngines::MySQL, DB.Schema.value());
                        clsDAC::setConnectionString(QString("HOST=%1;PORT=%2;USER=%3;PASSWORD=%4;SCHEMA=%5;")
                                                    .arg(DB.Host.value())
                                                    .arg(DB.Port.value())
                                                    .arg(DB.UserName.value())
                                                    .arg(DB.Password.value())
                                                    .arg(DB.Schema.value())
                                                    , DB.Schema.value());
                    }
                }
            }
        }

        //---------------------------------
        if (Command->run())
            QCoreApplication::exit(0);
        //else: async
    }
    catch(exTargomanBase &exp)
    {
        TargomanLogError(exp.what());
        QCoreApplication::exit(-1);
        return;
    }
}

} //namespace Targoman::Migrate

/*
rm `find . -name .migrations`

DROP TABLE if EXISTS AAA.tblMigrations;
DROP TABLE if EXISTS Advert.tblMigrations;
DROP TABLE if EXISTS Common.tblMigrations;
DROP TABLE if EXISTS CommonFuncs.tblMigrations;
DROP TABLE if EXISTS I18N.tblMigrations;
DROP TABLE if EXISTS Ticketing.tblMigrations;
*/
