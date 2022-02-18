################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
APP_NAME=targomanMigrate
include($$QBUILD_PATH/templates/innerAppConfigs.pri)

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    appsrc/AppConfigs.h \
    appsrc/CommandManager.h \
    appsrc/Commands/cmdList.h \
    appsrc/Defs.h \
    appsrc/intfCommand.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    appsrc/AppConfigs.cpp \
    appsrc/CommandManager.cpp \
    appsrc/Commands/cmdList.cpp \
    appsrc/main.cpp

OTHER_FILES +=

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
#LIBS += -lXXX

DISTFILES += \
    conf/test.conf \
    conf/api.server.conf \
