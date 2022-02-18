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

#include "cmdCreateDBDiff.h"
#include "../Configs.h"

namespace Targoman::Migrate::Commands {

cmdCreateDBDiff::cmdCreateDBDiff()
{
}

void cmdCreateDBDiff::run(bool _showHelp)
{
    if (_showHelp)
    {
        return;
    }

    QString FileName;
    QString FullFileName;
    qint32 SourceIndex = -1;
    qint32 DBIndex = -1;

    if (ChooseCreateMigrationProperties(
                enuChooseCreateMigrationScope::dbdiff,
                FileName,
                FullFileName,
                &SourceIndex,
                &DBIndex
                ) == false)
        return;

//    qDebug() << "===================="
//             << SourceIndex
//             << DBIndex
//             ;

    if (SourceIndex == -1)
        throw exTargomanBase("Invalid Source Index");

    if (DBIndex == -1)
        throw exTargomanBase("Invalid DB Index");

    stuMigrationSource &Source = Configs::Sources[SourceIndex];
    stuMigrationDB &DB = Source.DB[DBIndex];

    qDebug() << Source.Name.value();
    qDebug() << DB.Schema.value();




//    libTargomanCompare


    qInfo().noquote().nospace() << "Creating new migration file: " << FullFileName;





//    QFile File(FullFileName);

//    if (File.open(QFile::WriteOnly | QFile::Text) == false)
//    {
//        qInfo() << "Could not create new migration file.";
//        return;
//    }

//    QTextStream writer(&File);
//    writer
//        << "/* Migration File: "
//        << FileName
//        << " */"
//        << endl
//        << endl
//        ;
//    File.close();

//    qInfo().noquote() << "Migration file by diff created successfully.";
}

} // namespace Targoman::Migrate::Commands
