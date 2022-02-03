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
constexpr char _regex_pattern_migration_filename[] = "m[0-9]{8}_[0-9]{6}_[a-zA-Z0-9-_]*.(sh|sql)";

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

struct stuHistoryAppliedItem {
    QString MigrationName;
//    QString FullFileName;
    QString AppliedAt;

    stuHistoryAppliedItem(const stuHistoryAppliedItem &_other) :
        MigrationName(_other.MigrationName),
//        FullFileName(_other.FullFileName),
        AppliedAt(_other.AppliedAt)
    {}

    stuHistoryAppliedItem(
            const QString &_migrationName,
//            const QString &_fullFileName,
            const QString &_appliedAt
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

//key: MigrationName
typedef QMap<QString, stuSourceMigrationFileInfo> SourceMigrationFileInfoMap;

//key: Source + Scope
typedef QMap<QString, stuMigrationHistory> MigrationHistoryMap;

void dump(SourceMigrationFileInfoMap &_var)
{
    qDebug().noquote() //.nospace()
//                << QString("key").leftJustified(60)
            << "    "
            << QString("FileName").leftJustified(50)
            << QString("Scope").leftJustified(8)
            << QString("Source").leftJustified(20)
            << QString("FullFileName")
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

void dump(const QMap<QString, stuHistoryAppliedItem> &_var)
{
    qDebug().noquote() //.nospace()
            << "    "
            << QString("MigrationName").leftJustified(50)
            << QString("AppliedAt").leftJustified(20)
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
                << val.AppliedAt.leftJustified(20)
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
                        QString migAppliedAt = Map.value("migAppliedAt").toString();

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
                            Parts[1]
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

    SourceMigrationFileInfoMap MigrationFiles;
    ExtractMigrationFiles(MigrationFiles);
    qDebug() << "** All MigrationFiles ******************************";
    dump(MigrationFiles);

    MigrationHistoryMap MigrationHistories;
    ExtractMigrationHistories(MigrationHistories);
    qDebug() << "** MigrationHistories ******************************";
    dump(MigrationHistories);

    RemoveAppliedFromList(MigrationFiles, MigrationHistories);
    qDebug() << "** Unapplied MigrationFiles ******************************";
    dump(MigrationFiles);

    qDebug() << "";
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
