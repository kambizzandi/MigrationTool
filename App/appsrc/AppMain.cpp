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

#include "AppMain.h"
#include "AppConfigs.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"

//#include "Interfaces/API/intfPureModule.h"
//#include <QCoreApplication>
//#include <QPluginLoader>
//#include "libTargomanDBM/clsDAC.h"

using namespace Targoman::Common;

namespace Targoman::Migrate {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
//TARGOMAN_ADD_EXCEPTION_HANDLER(exModuleLoader, Targoman::Common::exTargomanBase);
//TARGOMAN_ADD_EXCEPTION_HANDLER(exModuleUnable2LoadFile, exModuleLoader);
//TARGOMAN_ADD_EXCEPTION_HANDLER(exInvalidAPIModule, exModuleLoader);
//TARGOMAN_ADD_EXCEPTION_HANDLER(exAPIModuleInitiailization, exModuleLoader);
#pragma clang diagnostic pop

AppMain::AppMain(QObject *parent) : QObject(parent)
{}

void AppMain::slotExecute()
{
    try
    {
        switch (AppConfigs::Action.value())
        {
            case enuAppAction::CreateDB:
                doActionCreateDB();
                break;

            case enuAppAction::CreateLocal:
                doActionCreateLocal();
                break;

            case enuAppAction::List:
                doActionList();
                break;

            case enuAppAction::History:
                doActionHistory();
                break;

            case enuAppAction::Up:
                doActionUp();
                break;

            case enuAppAction::UpTo:
                doActionUpTo();
                break;

            case enuAppAction::Down:
                doActionDown();
                break;

//            case enuAppAction::Redo:
//                doActionRedo();
//                break;

//            case enuAppAction::Fresh:
//                doActionFresh();
//                break;

            case enuAppAction::Mark:
                doActionMark();
                break;

            default:
                break;
        }

//        RESTServer::instance().start();
//        OpenAPIGenerator::retrieveJson();
//        TargomanInfo(5, "\n" + RESTAPIRegistry::registeredAPIs("", true, true).join("\n"));
    }
    catch(exTargomanBase &exp)
    {
        TargomanLogError(exp.what());
        QCoreApplication::exit(-1);
        return;
    }

    QCoreApplication::exit(0);
}

void AppMain::doActionCreateDB()
{
}

void AppMain::doActionCreateLocal()
{
}

void AppMain::doActionList()
{
    qInfo() << "List of not applied migrations:";
}

void AppMain::doActionHistory()
{
    qInfo() << "List of applied migrations:";
}

void AppMain::doActionUp()
{
}

void AppMain::doActionUpTo()
{
}

void AppMain::doActionDown()
{
}

void AppMain::doActionRedo()
{
}

void AppMain::doActionFresh()
{
}

void AppMain::doActionMark()
{
}

} //namespace Targoman::Migrate
