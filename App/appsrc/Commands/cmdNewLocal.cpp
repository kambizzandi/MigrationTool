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

#include "cmdNewLocal.h"
#include "../Configs.h"
#include <signal.h>
#include <unistd.h>

namespace Targoman::Migrate::Commands {

cmdNewLocal::cmdNewLocal()
{
}

void cmdNewLocal::help()
{
}

bool cmdNewLocal::run()
{
    QString FileName;
    QString FullFileName;
    quint32 ProjectIndex;

    if (ChooseCreateMigrationProperties(
                enuChooseCreateMigrationScope::local,
                FileName,
                FullFileName,
                ProjectIndex
                ) == false)
        return true;

    qInfo().noquote().nospace() << "Creating new migration file: " << FullFileName;

    QFile File(FullFileName);
    if (File.open(QFile::WriteOnly | QFile::Text) == false)
    {
        qInfo() << "Could not create new migration file.";
        return true;
    }

    QTextStream writer(&File);
    writer
        << "#!/bin/bash"
        << endl
        << "# Migration File: "
        << FileName
        << endl
        << endl
        ;
    File.close();

    qInfo().noquote() << "Empty migration file created successfully.";

    qint64 PID;
    if (QProcess::startDetached(Configs::DefaultEditor.value(),
                                QStringList() << FullFileName,
                                {},
                                &PID) == false)
        throw exTargomanBase("Execution of default editor failed");
    while (kill(PID, 0) == 0) { usleep(1); }

    return true;
}

} // namespace Targoman::Migrate::Commands
