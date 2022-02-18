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

#ifndef TARGOMAN_MIGRATE_CMDCOMMIT_H
#define TARGOMAN_MIGRATE_CMDCOMMIT_H

#include "../Defs.h"
#include "intfCommand.h"

namespace Targoman::Migrate::Commands {

class cmdCommit : public intfCommand
{
public:
    instanceGetterPtr(cmdCommit);
    cmdCommit();

    virtual bool needDB() { return true; }
    virtual void run(bool _showHelp = false);
};

} // namespace Targoman::Migrate::Commands

#endif // TARGOMAN_MIGRATE_CMDCOMMIT_H
