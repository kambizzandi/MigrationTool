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
#include <iostream>

//#include "Interfaces/API/intfPureModule.h"
//#include <QCoreApplication>
//#include <QPluginLoader>
//#include "libTargomanDBM/clsDAC.h"

using namespace Targoman::Common;

namespace Targoman::Migrate {

constexpr char _line_splitter[] = "------------------------------------------------------------------------";

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
        switch (AppConfigs::Command.value())
        {
            case enuAppCommand::CreateDB:
                ActionCreateDB();
                break;

            case enuAppCommand::CreateDBDiff:
                ActionCreateDBDiff();
                break;

            case enuAppCommand::CreateLocal:
                ActionCreateLocal();
                break;

            case enuAppCommand::List:
                ActionList();
                break;

            case enuAppCommand::History:
                ActionHistory();
                break;

            case enuAppCommand::Up:
                ActionUp();
                break;

            case enuAppCommand::UpTo:
                ActionUpTo();
                break;

//            case enuAppCommand::Down:
//                ActionDown();
//                break;

//            case enuAppCommand::Redo:
//                ActionRedo();
//                break;

//            case enuAppCommand::Fresh:
//                ActionFresh();
//                break;

            case enuAppCommand::Mark:
                ActionMark();
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

void AppMain::ActionCreateDB(bool _showHelp)
{
    if (_showHelp)
    {
        return;
    }

    qInfo() << "New global database migration:";
    qInfo() << _line_splitter;
}

void AppMain::ActionCreateDBDiff(bool _showHelp)
{
    if (_showHelp)
    {
        return;
    }

    qInfo() << "New global database migration by db diff:";
    qInfo() << _line_splitter;
}

void AppMain::ActionCreateLocal(bool _showHelp)
{
    if (_showHelp)
    {
        return;
    }

    qInfo() << "New local migration:";
    qInfo() << _line_splitter;
}

void AppMain::ActionList(bool _showHelp)
{
    if (_showHelp)
    {
        qInfo() << "List of unapplied migrations";
        qInfo() << _line_splitter;
        qInfo() << "./targomanMigrate" << "List     : showing the first 10 new migrations";
        qInfo() << "./targomanMigrate" << "List 5   : showing the first 5 new migrations";
        qInfo() << "./targomanMigrate" << "List all : showing all new migrations";
        return;
    }

    qInfo() << "Unapplied migrations:";
    qInfo() << _line_splitter;
}

void AppMain::ActionHistory(bool _showHelp)
{
    if (_showHelp)
    {
        qInfo() << "List of applied migrations";
        qInfo() << _line_splitter;
        qInfo() << "./targomanMigrate" << "History     : showing the first 10 applied migrations";
        qInfo() << "./targomanMigrate" << "History 5   : showing the first 5 applied migrations";
        qInfo() << "./targomanMigrate" << "History all : showing all applied migrations";
        return;
    }

    qInfo() << "Applied migrations:";
    qInfo() << _line_splitter;
}

void AppMain::ActionUp(bool _showHelp)
{
    if (_showHelp)
    {
        return;
    }

    qInfo() << "Applied migrations:";
    qInfo() << _line_splitter;
}

void AppMain::ActionUpTo(bool _showHelp)
{
    if (_showHelp)
    {
        return;
    }

    qInfo() << "Applied migrations:";
    qInfo() << _line_splitter;
}

//void AppMain::ActionDown(bool _showHelp)
//{
//    if (_showHelp)
//    {
//        return;
//    }

//    qInfo() << "Rolled back migrations:";
//    qInfo() << _line_splitter;
//}

//void AppMain::ActionRedo()
//{
//}

//void AppMain::ActionFresh()
//{
//}

void AppMain::ActionMark(bool _showHelp)
{
    if (_showHelp)
    {
        qInfo() << "Modifying migration history without actually run migrations";
        qInfo() << _line_splitter;
        qInfo() << "./targomanMigrate" << "Mark 20220101_010203                              : add all unapplied migrations upto 20220101_010203";
        qInfo() << "./targomanMigrate" << "Mark m20220101_010203                             : add all unapplied migrations upto 20220101_010203";
        qInfo() << "./targomanMigrate" << "Mark m20220101_010203_description_of_migration    : add all unapplied migrations upto 20220101_010203";
        qInfo() << "./targomanMigrate" << "Mark m20220101_010203_description_of_migration.sh : add all unapplied migrations upto 20220101_010203";
        return;
    }

    qInfo() << "Marking migrations as applied:";
    qInfo() << _line_splitter;

    std::cout << "are you sure? (y/n):";
    std::string s;
    std::cin >> s;
    std::cout << "your answer:" << s;
    std::cout << std::endl;
}

} //namespace Targoman::Migrate
