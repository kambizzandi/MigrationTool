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

#include <QCoreApplication>
#include <QTimer>

#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"

#include "CommandManager.h"

static const char* LicenseStr =
"%1 Ver: %2 Build %3\n"
"Published under the terms of GNU AFFERO GENERAL PUBLIC LICENSE version 3\n\n"
"%1 [Arguments]\n"
"Arguments: \n";

using namespace Targoman::Common;
using namespace Targoman::Migrate;

int main(int _argc, char *_argv[])
{
    try
    {
        QCoreApplication App(_argc, _argv);

        qInfo().noquote() << "Targoman Migrate. version" << TARGOMAN_M2STR(PROJ_VERSION);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdate-time"
        Configuration::ConfigManager::instance().init(
                    QString(LicenseStr).arg(_argv[0]).arg(TARGOMAN_M2STR(PROJ_VERSION)).arg(__DATE__),
                    App.arguments().mid(1)
                    );
#pragma GCC diagnostic pop

        QTimer::singleShot(10, new CommandManager, SLOT(slotExecute()));

        return App.exec();
    }
    catch(exTargomanBase &exp)
    {
        TargomanError(exp.what());
    }
    catch (std::exception &exp)
    {
        TargomanError(exp.what());
    }
    catch (...)
    {
        TargomanError("FATAL Unrecognized exception");
    }

    return -1;
}
