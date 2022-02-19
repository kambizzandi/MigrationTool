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

#ifndef TARGOMAN_MIGRATE_CMDCREATEDB_H
#define TARGOMAN_MIGRATE_CMDCREATEDB_H

#include "../Defs.h"
#include "intfCommand.h"
#include <QObject>

namespace Targoman::Migrate::Commands {

class cmdCreateDB : public QObject, public intfCommand
{
    Q_OBJECT

public:
    instanceGetterPtr(cmdCreateDB);
    cmdCreateDB(QObject *parent = nullptr);

    virtual bool needDB() { return true; }
    virtual void help();
    virtual bool run();

//private Q_SLOTS:
//    void error(QProcess::ProcessError error);
//    void finished(int exitCode, QProcess::ExitStatus exitStatus);

//private:
//    QProcess *EditorProcess;
};

} // namespace Targoman::Migrate::Commands

#endif // TARGOMAN_MIGRATE_CMDCREATEDB_H
