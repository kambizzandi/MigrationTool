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

#include "AppMain.h"
#include "AppConfigs.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanDBM/clsDAC.h"

//#include "Interfaces/API/intfPureModule.h"
//#include <QCoreApplication>
//#include <QPluginLoader>
//#include "libTargomanDBM/clsDAC.h"

using namespace Targoman::Common;
using namespace Targoman::DBManager;

namespace Targoman::Migrate {

constexpr char _line_splitter[] = "------------------------------------------------------------------------";
constexpr char _regex_pattern_migration_filename[]     = "m[0-9]{8}_[0-9]{6}_[a-zA-Z0-9-_]*.(sh|sql)";
constexpr char _regex_pattern_migration_log_filename[] = "m[0-9]{8}_[0-9]{6}_[a-zA-Z0-9-_]*.(sh|sql).log";

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
//TARGOMAN_ADD_EXCEPTION_HANDLER(exModuleLoader, Targoman::Common::exTargomanBase);
//TARGOMAN_ADD_EXCEPTION_HANDLER(exModuleUnable2LoadFile, exModuleLoader);
//TARGOMAN_ADD_EXCEPTION_HANDLER(exInvalidAPIModule, exModuleLoader);
//TARGOMAN_ADD_EXCEPTION_HANDLER(exAPIModuleInitiailization, exModuleLoader);
#pragma clang diagnostic pop

QString bold(const QString &_s)
{
    return QString("\x1b[1m%1\x1b[0m").arg(_s);
}

QString reverse(const QString &_s)
{
    return QString("\x1b[7m%1\x1b[0m").arg(_s);
}

inline QTextStream& qStdout()
{
    static QTextStream rOUT{stdout};
    return rOUT;
}
inline QTextStream& qStdIn()
{
    static QTextStream rIN{stdin};
    return rIN;
}

AppMain::AppMain(QObject *parent) : QObject(parent)
{}

void AppMain::slotExecute()
{
    try
    {
        if (AppConfigs::Sources.size() == 0)
            throw exTargomanBase("Source not defined");

        for (size_t idxSource=0; idxSource<AppConfigs::Sources.size(); idxSource++)
        {
            stuMigrationSource &Source = AppConfigs::Sources[idxSource];

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

enum class enuChooseCreateMigrationScope
{
    db,
    dbdiff,
    local
};

bool ChooseCreateMigrationProperties(
        enuChooseCreateMigrationScope _chooseScope,
        QString &_fileName,
        QString &_fullFileName,
        qint32 *_sourceIndex = nullptr,
        qint32 *_dBIndex = nullptr
    )
{
    QDir BaseFolder(AppConfigs::MigrationsFolderName.value());
    ///TODO: check if dir exists
    BaseFolder.makeAbsolute();

    qInfo().noquote()
            << "    "
            << "Source"
            ;
    qInfo() << _line_splitter;

    struct stuSourceInfo {
        QString SourceName;
        qint32 SourceIndex;
        qint32 DBIndex;

        stuSourceInfo(
                QString _sourceName,
                qint32 _sourceIndex = -1,
                qint32 _dBIndex = -1
            ) :
            SourceName(_sourceName),
            SourceIndex(_sourceIndex),
            DBIndex(_dBIndex)
        {}

    };

    QList<stuSourceInfo> Sources;
    if (_chooseScope != enuChooseCreateMigrationScope::dbdiff)
    {
        Sources.append(stuSourceInfo(AppConfigs::ApplyToAllSourceName.value()));
        qInfo().noquote()
                << "   1"
                << QString("Apply To All [%1]").arg(AppConfigs::ApplyToAllSourceName.value())
                ;
    }

    if (AppConfigs::Sources.size() > 0)
    {
        for (size_t idxSource=0; idxSource<AppConfigs::Sources.size(); idxSource++)
        {
            stuMigrationSource &Source = AppConfigs::Sources[idxSource];

            if (_chooseScope == enuChooseCreateMigrationScope::local)
            {
                QString Name = Source.Name.value();

                Sources.append(stuSourceInfo(Name, idxSource));

                qInfo().noquote()
                        << QString::number(Sources.length()).rightJustified(4)
                        << Name
                        ;
            }
            else
            {
                for (size_t idxDB=0; idxDB<Source.DB.size(); idxDB++)
                {
                    stuMigrationDB &DB = Source.DB[idxDB];

                    QString Name = QString("%1.%2")
                                   .arg(Source.Name.value())
                                   .arg(DB.Schema.value())
                                   ;

                    Sources.append(stuSourceInfo(Name, idxSource, idxDB));

                    qInfo().noquote()
                            << QString::number(Sources.length()).rightJustified(4)
                            << Name
                            ;
                }
            }
        }
    }

    qInfo() << "";

    qint32 SourceID;
    while (true)
    {
        qStdout()
                << "For which source do you want to create a new "
                << (_chooseScope == enuChooseCreateMigrationScope::local ? "local" : "db")
                << " migration file?"
                << " "
                << reverse("[") << reverse(bold("c")) << reverse("ancel]")
                << " "
                << reverse("[") << reverse(bold("1")) << reverse("]")
                ;

        if (Sources.length() > 1)
        {
            qStdout()
                    << reverse(" to ")
                    << reverse("[") << reverse(bold(QString::number(Sources.length()))) << reverse("]")
                    ;
        }
        qStdout() << " ";
        qStdout().flush();

        QString value = qStdIn().readLine().trimmed();

        if (value.isEmpty())
            continue;

        if (value == "c")
            return false;

        bool ok = false;
        SourceID = value.toInt(&ok);

        if (ok)
        {
            if ((SourceID <= 0) || (SourceID > Sources.length()))
                qStdout() << "Input must be between 1 and " << Sources.length() << endl;
            else
            {
                qInfo().noquote().nospace()
                        << "Your choose: ["
                        << SourceID
                        << "- "
                        << Sources[SourceID - 1].SourceName
                        << "]"
                ;
                break;
            }
        }
        else
            qStdout() << "Invalid input " << value << endl;
    }
    qInfo() << "";

    QString SourceLabel;
    while (true)
    {
        qStdout()
                << "Enter label of new migration file"
                << " "
                << reverse("[only ") << reverse(bold("a-z A-Z 0-9")) << reverse(" and ") << reverse(bold("_")) << reverse(" allowed, maximum of 64 characters]")
                << " or "
                << reverse("[") << reverse(bold("empty")) << reverse(" for cancel]")
                << " : "
                ;
        qStdout().flush();

        SourceLabel = qStdIn().readLine().trimmed();

        if (SourceLabel.isEmpty())
            return false;

        if (QRegExp("[a-zA-Z0-9_]+").exactMatch(SourceLabel) == false)
        {
            SourceLabel = "";
            qStdout() << "Invalid characters in source label." << endl;
            continue;
        }

        if (SourceLabel.length() > 64)
        {
            SourceLabel = "";
            qStdout() << "Source label length is > 64" << endl;
            continue;
        }

        //ok:
        break;
    }

//    qDebug() << "***************"
//             << SourceID
//             << Sources[SourceID - 1].SourceName
//             << Sources[SourceID - 1].SourceIndex
//             << Sources[SourceID - 1].DBIndex
//             ;

    if (_sourceIndex != nullptr)
        *_sourceIndex = Sources[SourceID - 1].SourceIndex;
    if (_dBIndex != nullptr)
        *_dBIndex = Sources[SourceID - 1].DBIndex;

    _fileName = QString("m%1_%2.%3")
                .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"))
                .arg(SourceLabel)
                .arg(_chooseScope == enuChooseCreateMigrationScope::local ? "sh" : "sql")
                ;

    if ((_chooseScope != enuChooseCreateMigrationScope::dbdiff) && (SourceID == 1))
    {
        _fullFileName = QString("%1/%2/%3/%4")
                        .arg(BaseFolder.path())
                        .arg(Sources[SourceID - 1].SourceName)
                        .arg(_chooseScope == enuChooseCreateMigrationScope::local ? "local" : "db")
                        .arg(_fileName)
                        ;
    }
    else if (_chooseScope == enuChooseCreateMigrationScope::local)
    {
        _fullFileName = QString("%1/%2/local/%3")
                        .arg(BaseFolder.path())
                        .arg(Sources[SourceID - 1].SourceName)
                        .arg(_fileName)
                        ;
    }
    else
    {
        _fullFileName = QString("%1/%2/%3")
                        .arg(BaseFolder.path())
                        .arg(Sources[SourceID - 1].SourceName.replace(".", "/db/"))
                        .arg(_fileName)
                        ;
    }

    return true;
}

void AppMain::ActionCreateDB(bool _showHelp)
{
    if (_showHelp)
    {
        return;
    }

    QString FileName;
    QString FullFileName;

    if (ChooseCreateMigrationProperties(
                enuChooseCreateMigrationScope::db,
                FileName,
                FullFileName
                ) == false)
        return;

    qInfo().noquote().nospace() << "Creating new migration file: " << FullFileName;

    QFile File(FullFileName);
    if (File.open(QFile::WriteOnly | QFile::Text) == false)
    {
        qInfo() << "Could not create new migration file.";
        return;
    }

    QTextStream writer(&File);
    writer
        << "/* Migration File: "
        << FileName
        << " */"
        << endl
        << endl
        ;
    File.close();

    qInfo().noquote() << "Empty migration file created successfully.";
}

void AppMain::ActionCreateDBDiff(bool _showHelp)
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

    stuMigrationSource &Source = AppConfigs::Sources[SourceIndex];
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

void AppMain::ActionCreateLocal(bool _showHelp)
{
    if (_showHelp)
    {
        return;
    }

    QString FileName;
    QString FullFileName;

    if (ChooseCreateMigrationProperties(
                enuChooseCreateMigrationScope::local,
                FileName,
                FullFileName
                ) == false)
        return;

    qInfo().noquote().nospace() << "Creating new migration file: " << FullFileName;

    QFile File(FullFileName);
    if (File.open(QFile::WriteOnly | QFile::Text) == false)
    {
        qInfo() << "Could not create new migration file.";
        return;
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
}

struct stuSourceMigrationFileInfo {
    QString MigrationName;
    QString FileName;
    QString Scope; //db, local
    QString Source;
    QString FullFileName;

    stuSourceMigrationFileInfo(const stuSourceMigrationFileInfo &_other) :
        MigrationName(_other.MigrationName),
        FileName(_other.FileName),
        Scope(_other.Scope),
        Source(_other.Source),
        FullFileName(_other.FullFileName)
    {}

    stuSourceMigrationFileInfo(
            const QString &_migrationName,
            const QString &_fileName,
            const QString &_scope,
            const QString &_source,
            const QString &_fullFileName
        ) :
        MigrationName(_migrationName),
        FileName(_fileName),
        Scope(_scope),
        Source(_source),
        FullFileName(_fullFileName)
    {}
};
//key: MigrationName
typedef QMap<QString, stuSourceMigrationFileInfo> SourceMigrationFileInfoMap;

void dump(SourceMigrationFileInfoMap &_var, bool _renderForAppliedHistoryItem = false)
{
    qDebug().noquote() //.nospace()
//                << QString("key").leftJustified(60)
            << "    "
            << QString("FileName").leftJustified(50)
            << QString("Scope").leftJustified(8)
            << QString("Source").leftJustified(20)
            << (_renderForAppliedHistoryItem ? "Applied At" : "FullFileName")
            ;
    qDebug().noquote()
            << QString(150, '-');

    int idx = 1;
    for (SourceMigrationFileInfoMap::const_iterator it = _var.constBegin();
         it != _var.constEnd();
         it++)
    {
        QString key = it.key();
        const stuSourceMigrationFileInfo &val = it.value();

        qDebug().noquote() //.nospace()
                << QString("%1").arg(idx).rightJustified(4)
//                << key.leftJustified(60)
                << val.FileName.leftJustified(50)
                << val.Scope.leftJustified(8)
                << val.Source.leftJustified(20)
                << val.FullFileName
                ;

        ++idx;
    }
}

struct stuHistoryAppliedItem {
    QString MigrationName;
//    QString FullFileName;
    QDateTime AppliedAt;

    stuHistoryAppliedItem(const stuHistoryAppliedItem &_other) :
        MigrationName(_other.MigrationName),
//        FullFileName(_other.FullFileName),
        AppliedAt(_other.AppliedAt)
    {}

    stuHistoryAppliedItem(
            const QString &_migrationName,
//            const QString &_fullFileName,
            const QDateTime &_appliedAt
        ) :
        MigrationName(_migrationName),
//        FullFileName(_fullFileName),
        AppliedAt(_appliedAt)
    {}
};

struct stuMigrationHistory {
    QString Source; //App, App.CommonFuncs
    QString Scope; //db, local
    QString HistoryFileOrTableName;
    QMap<QString, stuHistoryAppliedItem> AppliedItems;
    bool Exists;

    stuMigrationHistory(const stuMigrationHistory &_other) :
        Source(_other.Source),
        Scope(_other.Scope),
        HistoryFileOrTableName(_other.HistoryFileOrTableName),
        AppliedItems(_other.AppliedItems),
        Exists(_other.Exists)
    {}

    stuMigrationHistory(
            const QString &_source,
            const QString &_scope,
            const QString &_historyFileOrTableName,
            const QMap<QString, stuHistoryAppliedItem> &_appliedItems
        ) :
        Source(_source),
        Scope(_scope),
        HistoryFileOrTableName(_historyFileOrTableName),
        AppliedItems(_appliedItems),
        Exists(false)
    {}
};
//key: Source + Scope
typedef QMap<QString, stuMigrationHistory> MigrationHistoryMap;

void dump(const QMap<QString, stuHistoryAppliedItem> &_var)
{
    qDebug().noquote() //.nospace()
            << "    "
            << QString("MigrationName").leftJustified(50)
            << QString("AppliedAt").leftJustified(22)
//            << QString("FullFileName")
            ;
    qDebug().noquote()
            << "    "
            << QString(100, '-');

    for (QMap<QString, stuHistoryAppliedItem>::const_iterator it = _var.constBegin();
         it != _var.constEnd();
         it++)
    {
        QString key = it.key();
        const stuHistoryAppliedItem &val = it.value();

        qDebug().noquote() //.nospace()
                << "    "
                << val.MigrationName.leftJustified(50)
                << val.AppliedAt.toString("yyyy-MM-dd hh:mm:ss a").leftJustified(22)
//                << val.FullFileName
                ;
    }
}

void dump(MigrationHistoryMap &_var)
{
    qDebug().noquote() //.nospace()
            << QString("Source").leftJustified(20)
            << QString("Scope").leftJustified(5)
            << QString("Count").leftJustified(6)
            << QString("Exists").leftJustified(6)
            << QString("HistoryFileOrTableName")
            ;
    qDebug().noquote()
            << QString(150, '-');

    for (MigrationHistoryMap::const_iterator it = _var.constBegin();
         it != _var.constEnd();
         it++)
    {
        QString key = it.key();
        const stuMigrationHistory &val = it.value();

        qDebug().noquote() //.nospace()
                << val.Source.leftJustified(20)
                << val.Scope.leftJustified(5)
                << QString("%1").arg(val.AppliedItems.count()).leftJustified(6)
                << QString(val.Exists ? "Yes" : "No").leftJustified(6)
                << val.HistoryFileOrTableName
                ;

        if (val.AppliedItems.isEmpty() == false)
        {
//            qDebug() << "    applied items:";
            dump(val.AppliedItems);
        }
    }
}

void dump(QMap<QString, QString> &_var)
{
    for (QMap<QString, QString>::const_iterator it = _var.constBegin();
         it != _var.constEnd();
         it++)
    {
        QString key = it.key();
        QString val = it.value();

        qDebug().noquote().nospace()
                << key.leftJustified(65)
                << val;
    }
}

void ExtractMigrationFiles(SourceMigrationFileInfoMap &_migrationFiles)
{
    QDir BaseFolder(AppConfigs::MigrationsFolderName.value());
    ///TODO: check if dir exists

    BaseFolder.makeAbsolute();
//    qDebug() << BaseFolder;

    QMap<QString, QString> ApplyToAllMigrationFiles;

    //get file list of ApplyToAllSourceName
    //  db    -> ApplyToAllMigrationFiles
    //  local -> MigrationFiles
    if (AppConfigs::ApplyToAllSourceName.value().isEmpty() == false)
    {
        if (BaseFolder.cd(AppConfigs::ApplyToAllSourceName.value()))
        {
            //db
            if (BaseFolder.cd("db"))
            {
                QDirIterator itdb(BaseFolder.path(), QDir::Files);
                while (itdb.hasNext())
                {
                    QString FullFileName = itdb.next();
                    QString FileName = BaseFolder.relativeFilePath(FullFileName);
                    if (QRegExp(_regex_pattern_migration_filename).exactMatch(FileName) == false)
                    {
                        if (QRegExp(_regex_pattern_migration_log_filename).exactMatch(FileName) == false)
                            qDebug() << "invalid file name:" << FileName << "in" << BaseFolder.path();
                        continue;
                    }

//                    QString MigrationName = QString("%1:db@*").arg(FileName);

//                    qDebug() << "[*]" << FullFileName << "\t\t" << FileName;

                    ApplyToAllMigrationFiles.insert(FileName, FullFileName);
                }
                BaseFolder.cdUp();
            }

            //local
            if (BaseFolder.cd("local"))
            {
                QDirIterator itlocal(BaseFolder.path(), QDir::Files | QDir::Hidden);
                while (itlocal.hasNext())
                {
                    QString FullFileName = itlocal.next();
                    QString FileName = BaseFolder.relativeFilePath(FullFileName);
                    if (QRegExp(_regex_pattern_migration_filename).exactMatch(FileName) == false)
                    {
                        if (FileName != AppConfigs::LocalHistoryFileName.value())
//                            _localHistoryFiles.insert(AppConfigs::ApplyToAllSourceName.value(), FullFileName);
//                        else
                            if (QRegExp(_regex_pattern_migration_log_filename).exactMatch(FileName) == false)
                                qDebug() << "invalid file name:" << FileName << "in" << BaseFolder.path();

                        continue;
                    }

                    QString MigrationName = QString("%1:local@%2").arg(FileName).arg(AppConfigs::ApplyToAllSourceName.value());

//                    qDebug() << "[*]" << FullFileName << "\t\t" << MigrationName;

                    _migrationFiles.insert(MigrationName,
                                          stuSourceMigrationFileInfo(
                                              MigrationName,
                                              FileName,
                                              "local",
                                              AppConfigs::ApplyToAllSourceName.value(),
                                              FullFileName
                                          ));
                }
                BaseFolder.cdUp();
            }

            //
            BaseFolder.cdUp();
        }
    }

    //get list of exists sources
    //  extract file list of every source
    if (AppConfigs::Sources.size() > 0)
    {
        for (size_t idxSource=0; idxSource<AppConfigs::Sources.size(); idxSource++)
        {
            stuMigrationSource &Source = AppConfigs::Sources[idxSource];

//            qDebug() << Source.Name.value();

            //1: fill MigrationFiles by ApplyToAllMigrationFiles (* -> source name)
            if ((Source.DB.size() > 0) && (ApplyToAllMigrationFiles.isEmpty() == false))
            {
                for (QMap<QString, QString>::const_iterator it = ApplyToAllMigrationFiles.constBegin();
                     it != ApplyToAllMigrationFiles.constEnd();
                     it++)
                {
                    QString FileName = it.key();
                    QString FullFileName = it.value();

                    for (size_t idxDB=0; idxDB<Source.DB.size(); idxDB++)
                    {
                        stuMigrationDB &DB = Source.DB[idxDB];

                        QString MigrationName = QString("%1:db@%2.%3")
                                                .arg(FileName)
                                                .arg(Source.Name.value())
                                                .arg(DB.Schema.value())
                                                ;

//                        qDebug() << "[" << Source.Name.value() << "]" << FullFileName << "\t\t" << MigrationName;

                        _migrationFiles.insert(MigrationName,
                                              stuSourceMigrationFileInfo(
                                                  MigrationName,
                                                  FileName,
                                                  "db",
                                                  QString("%1.%2").arg(Source.Name.value()).arg(DB.Schema.value()),
                                                  FullFileName
                                              ));
                    }
                }
            }

            if (BaseFolder.cd(Source.Name.value()))
            {
                //2: fill MigrationFiles by source/db
                if ((Source.DB.size() > 0) && BaseFolder.cd("db"))
                {
                    for (size_t idxDB=0; idxDB<Source.DB.size(); idxDB++)
                    {
                        stuMigrationDB &DB = Source.DB[idxDB];

                        if (BaseFolder.cd(DB.Schema.value()))
                        {
                            QDirIterator itdb(BaseFolder.path(), QDir::Files);
                            while (itdb.hasNext())
                            {
                                QString FullFileName = itdb.next();
                                QString FileName = BaseFolder.relativeFilePath(FullFileName);
                                if (QRegExp(_regex_pattern_migration_filename).exactMatch(FileName) == false)
                                {
                                    if (QRegExp(_regex_pattern_migration_log_filename).exactMatch(FileName) == false)
                                        qDebug() << "invalid file name:" << FileName << "in" << BaseFolder.path();
                                    continue;
                                }

                                QString MigrationName = QString("%1:db@%2.%3")
                                                        .arg(FileName)
                                                        .arg(Source.Name.value())
                                                        .arg(DB.Schema.value())
                                                        ;

//                                qDebug() << "[" << Source.Name.value() << "]" << FullFileName << "\t\t" << MigrationName;

                                _migrationFiles.insert(MigrationName,
                                                      stuSourceMigrationFileInfo(
                                                          MigrationName,
                                                          FileName,
                                                          "db",
                                                          QString("%1.%2").arg(Source.Name.value()).arg(DB.Schema.value()),
                                                          FullFileName
                                                      ));
                            }

                            BaseFolder.cdUp();
                        }
                    }

                    BaseFolder.cdUp();
                }

                //3: fill MigrationFiles by source/local
                if (BaseFolder.cd("local"))
                {
                    QDirIterator itlocal(BaseFolder.path(), QDir::Files | QDir::Hidden);
                    while (itlocal.hasNext())
                    {
                        QString FullFileName = itlocal.next();
                        QString FileName = BaseFolder.relativeFilePath(FullFileName);
                        if (QRegExp(_regex_pattern_migration_filename).exactMatch(FileName) == false)
                        {
                            if (FileName != AppConfigs::LocalHistoryFileName.value())
//                                _localHistoryFiles.insert(Source.Name.value(), FullFileName);
//                            else
                                if (QRegExp(_regex_pattern_migration_log_filename).exactMatch(FileName) == false)
                                    qDebug() << "invalid file name:" << FileName << "in" << BaseFolder.path();

                            continue;
                        }

                        QString MigrationName = QString("%1:local@%2").arg(FileName).arg(Source.Name.value());

//                        qDebug() << "[" << Source.Name.value() << "]" << FullFileName << "\t\t" << MigrationName;

                        _migrationFiles.insert(MigrationName,
                                              stuSourceMigrationFileInfo(
                                                  MigrationName,
                                                  FileName,
                                                  "local",
                                                  Source.Name.value(),
                                                  FullFileName
                                              ));
                    }
                    BaseFolder.cdUp();
                }

                //
                BaseFolder.cdUp();
            }
        }
    }
}

void ExtractMigrationHistories(MigrationHistoryMap &_migrationHistories)
{
    QDir BaseFolder(AppConfigs::MigrationsFolderName.value());
    ///TODO: check if dir exists

    BaseFolder.makeAbsolute();

    //extract db histories from config::db::schema
    auto fnCheckDBSource = [&](const QString &_sourceName, const stuMigrationDB &_db)
    {
        stuMigrationHistory MigrationHistory(
                    QString("%1.%2").arg(_sourceName).arg(_db.Schema.value()),
                    "db",
                    AppConfigs::GlobalHistoryTableName.value(),
                    {}
                    );

        try
        {
            clsDAC DAC(_db.Schema.value());

            QString Qry;

            Qry = R"(
                SELECT *
                  FROM information_schema.TABLES
                 WHERE TABLE_SCHEMA=?
                   AND TABLE_NAME=?
            )";
            clsDACResult ResultTable = DAC.execQuery("", Qry, {
                                                         _db.Schema.value(),
                                                         AppConfigs::GlobalHistoryTableName.value(),
                                                     });

            if (ResultTable.toJson(true).object().isEmpty())
            {
                MigrationHistory.Exists = false;

//                qDebug().noquote().nospace()
//                        << "Migration history table not exists: "
//                        << _db.Schema.value()
//                        << "."
//                        << AppConfigs::GlobalHistoryTableName.value();
            }
            else
            {
                MigrationHistory.Exists = true;

                Qry = QString("SELECT migName, migAppliedAt FROM %1").arg(AppConfigs::GlobalHistoryTableName.value());
                clsDACResult Result = DAC.execQuery("", Qry);
                QJsonDocument ResultRows = Result.toJson(false);
                QVariantList List = ResultRows.toVariant().toList();
                if (List.isEmpty() == false)
                {
                    ///TODO: complete this
                    foreach (auto Row, List)
                    {
                        QVariantMap Map = Row.toMap();
                        QString migName = Map.value("migName").toString();
                        QDateTime migAppliedAt = QDateTime::fromString(Map.value("migAppliedAt").toString(), Qt::ISODate);

//                        QString MigrationFullFileName = "";
//                        QString("%1/%2/local/%3")
//                                                        .arg(BaseFolder.path())
//                                                        .arg(_sourceName)
//                                                        .arg(migName);

                        stuHistoryAppliedItem HistoryItem(
                                    migName,
//                                    MigrationFullFileName,
                                    migAppliedAt
                                );

                        MigrationHistory.AppliedItems.insert(
                                    migName,
                                    HistoryItem);
                    }
                }
            }
        }
        catch (std::exception &exp)
        {
//            qDebug() << exp.what();
        }

        _migrationHistories.insert(
                    QString("%1.%2").arg(_sourceName).arg(_db.Schema.value()),
                    MigrationHistory
                    );
    };

    if (AppConfigs::Sources.size() > 0)
    {
        for (size_t idxSource=0; idxSource<AppConfigs::Sources.size(); idxSource++)
        {
            stuMigrationSource &Source = AppConfigs::Sources[idxSource];

//            qDebug() << Source.Name.value() << "DB" << Source.DB.size();

            if (Source.DB.size() > 0)
            {
                for (size_t idxDB=0; idxDB<Source.DB.size(); idxDB++)
                {
                    stuMigrationDB &DB = Source.DB[idxDB];
                    fnCheckDBSource(Source.Name.value(), DB);
                }
            }

        }
    }

    //extract local histories from config::sources
    auto fnCheckLocalSource = [&](const QString &_sourceName)
    {
        QString HistoryFullFileName = QString("%1/%2/local/%3")
                                      .arg(BaseFolder.path())
                                      .arg(_sourceName)
                                      .arg(AppConfigs::LocalHistoryFileName.value());

//        qDebug() << HistoryFullFileName;

        stuMigrationHistory MigrationHistory(
                    _sourceName,
                    "local",
                    HistoryFullFileName,
                    {}
                    );
        MigrationHistory.Exists = false;

        QFile File(HistoryFullFileName);
        if (File.exists() && File.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            MigrationHistory.Exists = true;

            while (File.atEnd() == false)
            {
                QByteArray Line = File.readLine().trimmed();

                //#Migration Name   Applied At
                if (Line.isEmpty() || Line.startsWith('#'))
                    continue;

                QList<QByteArray> Parts = Line.split('\t');

//                QString MigrationFullFileName = QString("%1/%2/local/%3")
//                                                .arg(BaseFolder.path())
//                                                .arg(_sourceName)
//                                                .arg(QString(Parts[0]));

                stuHistoryAppliedItem HistoryItem(
                            Parts[0],
//                            MigrationFullFileName,
                            QDateTime::fromString(Parts[1], Qt::ISODate) //"yyyy/MM/dd HH:mm:ss")
                        );

                MigrationHistory.AppliedItems.insert(
                            Parts[0],
                            HistoryItem);
            }

            File.close();
        }

        _migrationHistories.insert(
                    _sourceName,
                    MigrationHistory
                    );
    };

    //1: check ApplyToAllSourceName
    if (AppConfigs::ApplyToAllSourceName.value().isEmpty() == false)
    {
        fnCheckLocalSource(AppConfigs::ApplyToAllSourceName.value());
    }

    //2: check sources
    if (AppConfigs::Sources.size() > 0)
    {
        for (size_t idxSource=0; idxSource<AppConfigs::Sources.size(); idxSource++)
        {
            stuMigrationSource &Source = AppConfigs::Sources[idxSource];
//            qDebug() << Source.Name.value();

            fnCheckLocalSource(Source.Name.value());
        }
    }
}

void RemoveAppliedFromList(
        SourceMigrationFileInfoMap &_migrationFiles,
        const MigrationHistoryMap &_migrationHistories
    )
{
    if (_migrationFiles.isEmpty() || _migrationHistories.isEmpty())
        return;

    for (MigrationHistoryMap::const_iterator itHistoryMap = _migrationHistories.constBegin();
         itHistoryMap != _migrationHistories.constEnd();
         itHistoryMap++)
    {
        const stuMigrationHistory &MigrationHistory = itHistoryMap.value();

        if (MigrationHistory.AppliedItems.isEmpty())
            continue;

        for (QMap<QString, stuHistoryAppliedItem>::const_iterator itHistoryItem = MigrationHistory.AppliedItems.constBegin();
             itHistoryItem != MigrationHistory.AppliedItems.constEnd();
             itHistoryItem++)
        {
            QString key = itHistoryItem.key();
            const stuHistoryAppliedItem &HistoryAppliedItem = itHistoryItem.value();

            SourceMigrationFileInfoMap::iterator itMigrationFile = _migrationFiles.begin();
            while (itMigrationFile != _migrationFiles.end())
            {
                stuSourceMigrationFileInfo &SourceMigrationFileInfo = itMigrationFile.value();

                if ((SourceMigrationFileInfo.FileName == HistoryAppliedItem.MigrationName)
                        && (SourceMigrationFileInfo.Scope == MigrationHistory.Scope)
                        && (SourceMigrationFileInfo.Source == MigrationHistory.Source)
                    )
                {
//                    qDebug() << ">>>>>>>>>>>>>>>>> FOUND:"
//                             << SourceMigrationFileInfo.FileName
//                             << SourceMigrationFileInfo.Scope
//                             << SourceMigrationFileInfo.Source
//                             ;

                    itMigrationFile = _migrationFiles.erase(itMigrationFile);
                    break;
                }
                else
                    ++itMigrationFile;
            }
        }
    }
}

void GetUnappliedMigrations()
{
}

void AppMain::ActionList(bool _showHelp)
{
    if (_showHelp)
    {
        qInfo() << "List of unapplied migrations";
//        qInfo() << _line_splitter;
//        qInfo() << "./targomanMigrate" << "List     : showing the first 10 new migrations";
//        qInfo() << "./targomanMigrate" << "List 5   : showing the first 5 new migrations";
//        qInfo() << "./targomanMigrate" << "List all : showing all new migrations";
        return;
    }

    qInfo() << "Unapplied migrations:";
    qInfo() << _line_splitter;

    SourceMigrationFileInfoMap MigrationFiles;
    ExtractMigrationFiles(MigrationFiles);
//    qDebug() << "** All MigrationFiles ******************************";
//    dump(MigrationFiles);

    MigrationHistoryMap MigrationHistories;
    ExtractMigrationHistories(MigrationHistories);
//    qDebug() << "** MigrationHistories ******************************";
//    dump(MigrationHistories);

    RemoveAppliedFromList(MigrationFiles, MigrationHistories);
//    qDebug() << "** Unapplied MigrationFiles ******************************";
    dump(MigrationFiles);

    qDebug() << "";
}

void AppMain::ActionHistory(bool _showHelp)
{
    if (_showHelp)
    {
        qInfo() << "List of applied migrations";
//        qInfo() << _line_splitter;
//        qInfo() << "./targomanMigrate" << "History     : showing the first 10 applied migrations";
//        qInfo() << "./targomanMigrate" << "History 5   : showing the first 5 applied migrations";
//        qInfo() << "./targomanMigrate" << "History all : showing all applied migrations";
        return;
    }

    MigrationHistoryMap MigrationHistories;
    ExtractMigrationHistories(MigrationHistories);
//    qDebug() << "** MigrationHistories ******************************";
//    dump(MigrationHistories);

    SourceMigrationFileInfoMap MigrationFiles;
    for (MigrationHistoryMap::const_iterator it = MigrationHistories.constBegin();
         it != MigrationHistories.constEnd();
         it++)
    {
        QString key = it.key();
        const stuMigrationHistory &MigrationHistoryItem = it.value();

        if (MigrationHistoryItem.AppliedItems.isEmpty() == false)
        {
            for (QMap<QString, stuHistoryAppliedItem>::const_iterator it = MigrationHistoryItem.AppliedItems.constBegin();
                 it != MigrationHistoryItem.AppliedItems.constEnd();
                 it++)
            {
                QString key = it.key();
                const stuHistoryAppliedItem &MigrationHistoryAppliedItem = it.value();

                QString MigrationName = QString("%1:%2@%3")
                                        .arg(MigrationHistoryAppliedItem.MigrationName)
                                        .arg(MigrationHistoryItem.Scope)
                                        .arg(MigrationHistoryItem.Source);

                stuSourceMigrationFileInfo SourceMigrationFileInfo(
                    MigrationName,
                    MigrationHistoryAppliedItem.MigrationName,
                    MigrationHistoryItem.Scope,
                    MigrationHistoryItem.Source,
                    MigrationHistoryAppliedItem.AppliedAt.toString("yyyy-MM-dd hh:mm:ss a")
                );

//                qDebug() << "*" << MigrationName;
                MigrationFiles.insert(MigrationName, SourceMigrationFileInfo);
            }
        }
    }

    qInfo() << "Applied migrations:";
    qInfo() << _line_splitter;
    dump(MigrationFiles, true);
}

const stuMigrationDB& FindDBConfigByMigrationFile(const stuSourceMigrationFileInfo &_migrationFile)
{
    if (_migrationFile.Scope != "db")
        throw exTargomanBase(QString("Migration %1 is not db scope").arg(_migrationFile.Source));

    QStringList Parts = _migrationFile.Source.split('.');
    QString SourceName = Parts[0];
    QString DBSchema = Parts[1];

    if (AppConfigs::Sources.size() > 0)
    {
        for (size_t idxSource=0; idxSource<AppConfigs::Sources.size(); idxSource++)
        {
            stuMigrationSource &Source = AppConfigs::Sources[idxSource];

            if (SourceName == Source.Name.value())
            {
                if (Source.DB.size() > 0)
                {
                    for (size_t idxDB=0; idxDB<Source.DB.size(); idxDB++)
                    {
                        stuMigrationDB &DB = Source.DB[idxDB];

                        if (DBSchema == DB.Schema.value())
                            return DB;
                    }
                }
                break; //fail
            }
        }
    }

    throw exTargomanBase(QString("Definition for %1 not found in configuration").arg(_migrationFile.Source));
}
void MarkMigrationFile(const stuSourceMigrationFileInfo &_migrationFile)
{
    if (_migrationFile.Scope == "db")
    {
        stuMigrationDB DBConfig = FindDBConfigByMigrationFile(_migrationFile);

        QStringList Parts = _migrationFile.Source.split('.');

        clsDAC DAC(Parts[1]);

        QString Qry = QString(R"(
            INSERT INTO %1.%2
               SET migName=?
                 , migAppliedAt=NOW()
            )")
            .arg(Parts[1])
            .arg(AppConfigs::GlobalHistoryTableName.value())
        ;

        clsDACResult Result = DAC.execQuery("", Qry, {
                                                _migrationFile.FileName
                                            });
    } //db
    else //local
    {
        QFileInfo FileInfo(_migrationFile.FullFileName);

        if (FileInfo.exists() == false)
            throw exTargomanBase("File not found");

        QString MigrationHistoryFileName = QString("%1/%2")
                                           .arg(FileInfo.path())
                                           .arg(AppConfigs::LocalHistoryFileName.value());

        QFile File(MigrationHistoryFileName);

        if (File.open(QFile::ReadWrite | QFile::Text) == false)
            throw exTargomanBase("Could not create or open history file");

        QTextStream stream(&File);

        QString HistoryItem = "";

        if (File.size() > 0)
        {
            if (File.size() > 1)
            {
                stream.seek(File.size() - 1);
                QString b = stream.read(1);
                if (b != "\n")
                    HistoryItem = "\n";
            }
            stream.seek(File.size());
        }
        else
            stream << "#Migration Name   Applied At" << endl;

        HistoryItem += QString("%1\t%2").arg(_migrationFile.FileName).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        stream << HistoryItem << endl;

        File.close();
    }
}

void RunMigrationFile(const stuSourceMigrationFileInfo &_migrationFile, bool _run = true)
{
    if (_migrationFile.Scope == "db")
    {
        stuMigrationDB DBConfig = FindDBConfigByMigrationFile(_migrationFile);

        //1: run migration
        if (_run)
        {
            clsDAC DAC(DBConfig.Schema.value());

            QFile File(_migrationFile.FullFileName);

            if (!File.open(QFile::ReadOnly | QFile::Text))
                throw exTargomanBase("File not found");

            QTextStream stream(&File);
            QString Qry = stream.readAll();
            if (Qry.isEmpty() == false)
            {
                QStringList Parts = _migrationFile.Source.split('.');
                Qry = Qry
                    .replace("{{Schema}}", DBConfig.Schema.value())
                    .replace("{{GlobalHistoryTableName}}", AppConfigs::GlobalHistoryTableName.value())
                ;
                clsDACResult MainResult = DAC.execQuery("", Qry);
            }

            File.close();
        }

        //2: add to history
        MarkMigrationFile(_migrationFile);
    } //db
    else //local
    {
        //1: run migration
        if (_run)
        {
            QFileInfo FileInfo(_migrationFile.FullFileName);

            if (FileInfo.exists() == false)
                throw exTargomanBase("File not found");

            if (FileInfo.size() > 0)
            {
                if (FileInfo.isExecutable() == false)
                    QFile::setPermissions(
                                _migrationFile.FullFileName,
                                FileInfo.permissions() |
                                QFile::ExeUser | QFile::ExeGroup | QFile::ExeOwner | QFile::ExeOther
                                );

                QProcess MigrationProcess;
                MigrationProcess.start(_migrationFile.FullFileName);

                if (!MigrationProcess.waitForFinished())
                    throw exTargomanBase("Execution failed");

                QByteArray ExResult = MigrationProcess.readAll();
                if (ExResult.isEmpty() == false)
                {
                    QFile LogFile(QString("%1.log").arg(_migrationFile.FullFileName));
                    if (LogFile.open(QFile::WriteOnly | QFile::Text))
                    {
                        QTextStream out(&LogFile);
                        out << ExResult;
                        LogFile.close();
                    }
                }
            }
        }

        //2: add to history
        MarkMigrationFile(_migrationFile);
    }
}

void AppMain::ActionUp(bool _showHelp)
{
    if (_showHelp)
    {
        return;
    }

    SourceMigrationFileInfoMap MigrationFiles;
    ExtractMigrationFiles(MigrationFiles);
//    qDebug() << "** All MigrationFiles ******************************";
//    dump(MigrationFiles);

    MigrationHistoryMap MigrationHistories;
    ExtractMigrationHistories(MigrationHistories);
//    qDebug() << "** MigrationHistories ******************************";
//    dump(MigrationHistories);

    RemoveAppliedFromList(MigrationFiles, MigrationHistories);

    if (MigrationFiles.isEmpty())
    {
        qInfo() << "nothing to run";
        return;
    }

//    qDebug() << "** Unapplied MigrationFiles ******************************";
    dump(MigrationFiles);
    qInfo() << "";

    qint32 RemainCount = 0;

    while (true)
    {
        qStdout()
                << "Which migrations do you want to run?"
                << " "
                << reverse("[") << reverse(bold("c")) << reverse("ancel]")
                << " "
                ;

        if (MigrationFiles.count() == 1)
        {
            qStdout()
                    << reverse("[") << reverse(bold("a")) << reverse("ll]")
                    << reverse(" = ")
                    << reverse("[") << reverse(bold("1")) << reverse("]")
                    ;
        }
        else
        {
            qStdout()
                    << reverse("[") << reverse(bold("a")) << reverse("ll]")
                    << " "
                    << reverse("1 to [") << reverse(bold("1")) << reverse("]")
                    << reverse(" ... ")
                    << reverse("[") << reverse(bold(QString::number(MigrationFiles.count()))) << reverse("]")
                    ;
        }
        qStdout() << " ";
        qStdout().flush();

        QString value = qStdIn().readLine().trimmed();

        if (value.isEmpty())
            continue;

        if (value == "c")
            return;

        if (value == "a")
        {
            RemainCount = MigrationFiles.count();
            break;
        }
        else
        {
            bool ok = false;
            RemainCount = value.toInt(&ok);

            if (ok)
            {
                if ((RemainCount <= 0) || (RemainCount > MigrationFiles.count()))
                    qStdout() << "Input must be between 1 and " << MigrationFiles.count() << endl;
                else
                    break;
            }
            else
                qStdout() << "Invalid input " << value << endl;
        }
    }

    qInfo() << "Applying migrations:";
    qInfo() << _line_splitter;

    int idx = 1;
    foreach (auto MigrationFile, MigrationFiles)
    {
        qStdout()
                << "    "
                << QString::number(idx++).rightJustified(5)
                << " "
                << MigrationFile.FileName
                << " ["
                << MigrationFile.Scope
                << " @ "
                << MigrationFile.Source
                << "]"
//                << MigrationFile.FullFileName
                << " : "
                ;

        RunMigrationFile(MigrationFile);

        qStdout() << "OK" << endl;

        --RemainCount;
        if (RemainCount <= 0)
            break;
    }

    qInfo() << "";
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
//        qInfo() << _line_splitter;
//        qInfo() << "./targomanMigrate" << "Mark 20220101_010203                              : add all unapplied migrations upto 20220101_010203";
//        qInfo() << "./targomanMigrate" << "Mark m20220101_010203                             : add all unapplied migrations upto 20220101_010203";
//        qInfo() << "./targomanMigrate" << "Mark m20220101_010203_description_of_migration    : add all unapplied migrations upto 20220101_010203";
//        qInfo() << "./targomanMigrate" << "Mark m20220101_010203_description_of_migration.sh : add all unapplied migrations upto 20220101_010203";
        return;
    }

    SourceMigrationFileInfoMap MigrationFiles;
    ExtractMigrationFiles(MigrationFiles);
//    qDebug() << "** All MigrationFiles ******************************";
//    dump(MigrationFiles);

    MigrationHistoryMap MigrationHistories;
    ExtractMigrationHistories(MigrationHistories);
//    qDebug() << "** MigrationHistories ******************************";
//    dump(MigrationHistories);

    RemoveAppliedFromList(MigrationFiles, MigrationHistories);

    if (MigrationFiles.isEmpty())
    {
        qInfo() << "nothing to mark";
        return;
    }

//    qDebug() << "** Unapplied MigrationFiles ******************************";
    dump(MigrationFiles);
    qInfo() << "";

    qint32 RemainCount = 0;

    while (true)
    {
        qStdout()
                << "Which migrations do you want to mark?"
                << " "
                << reverse("[") << reverse(bold("c")) << reverse("ancel]")
                << " "
                ;

        if (MigrationFiles.count() == 1)
        {
            qStdout()
                    << reverse("[") << reverse(bold("a")) << reverse("ll]")
                    << reverse(" = ")
                    << reverse("[") << reverse(bold("1")) << reverse("]")
                    ;
        }
        else
        {
            qStdout()
                    << reverse("[") << reverse(bold("a")) << reverse("ll]")
                    << " "
                    << reverse("1 to [") << reverse(bold("1")) << reverse("]")
                    << reverse(" ... ")
                    << reverse("[") << reverse(bold(QString::number(MigrationFiles.count()))) << reverse("]")
                    ;
        }
        qStdout() << " ";
        qStdout().flush();

        QString value = qStdIn().readLine().trimmed();

        if (value.isEmpty())
            continue;

        if (value == "c")
            return;

        if (value == "a")
        {
            RemainCount = MigrationFiles.count();
            break;
        }
        else
        {
            bool ok = false;
            RemainCount = value.toInt(&ok);

            if (ok)
            {
                if ((RemainCount <= 0) || (RemainCount > MigrationFiles.count()))
                    qStdout() << "Input must be between 1 and " << MigrationFiles.count() << endl;
                else
                    break;
            }
            else
                qStdout() << "Invalid input " << value << endl;
        }
    }

    qInfo() << "Marking migrations as applied:";
    qInfo() << _line_splitter;

    int idx = 1;
    foreach (auto MigrationFile, MigrationFiles)
    {
        qStdout()
                << "    "
                << QString::number(idx++).rightJustified(5)
                << " "
                << MigrationFile.FileName
                << " ["
                << MigrationFile.Scope
                << " @ "
                << MigrationFile.Source
                << "]"
//                << MigrationFile.FullFileName
                << " : "
                ;

        RunMigrationFile(MigrationFile, false);

        qStdout() << "Marked" << endl;

        --RemainCount;
        if (RemainCount <= 0)
            break;
    }

    qInfo() << "";
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
