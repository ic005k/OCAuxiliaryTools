greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT       += core gui xml network

ICON = src/Icon.icns

CONFIG += c++11
TARGET= OCAuxiliaryTools

unix:!macx:{
    QMAKE_RPATHDIR=$ORIGIN
    QMAKE_LFLAGS += -no-pie
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/lib\'"
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/platforms\'"
}

TRANSLATIONS += cn.ts
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += \
               src/views/ \
               src/watcher \
               src/utils \
               src/app

RC_FILE += myapp.rc

SOURCES += \
    src/views/aboutdialog.cpp \
    src/views/autoupdatedialog.cpp \
    src/views/dlgMountESP.cpp \
    src/views/dlgOCValidate.cpp \
    src/views/dlgParameters.cpp \
    src/views/dlgPreset.cpp \
    src/views/dlgdatabase.cpp \
    src/views/dlgkernelpatch.cpp \
    src/views/dlgmisc.cpp \
    src/views/dlgnewkeyfield.cpp \
    src/views/dlgpreference.cpp \
    src/views/mainwindow.cpp \
    src/views/syncocdialog.cpp \
    src/views/BalloonTip.cpp \
    src/views/tooltip.cpp \
    src/watcher/filesystemwatcher.cpp \
    src/utils/Method.cpp \
    src/utils/Plist.cpp \
    src/utils/PlistDate.cpp \
    src/utils/commands.cpp \
    src/utils/plistparser.cpp \
    src/utils/plistserializer.cpp \
    src/utils/pugixml.cpp \
    src/utils/recentfiles.cpp \
    src/app/myapp.cpp \
    src/main.cpp

HEADERS += \
    src/views/aboutdialog.h \
    src/views/autoupdatedialog.h \
    src/views/dlgMountESP.h \
    src/views/dlgOCValidate.h \
    src/views/dlgParameters.h \
    src/views/dlgPreset.h \
    src/views/dlgdatabase.h \
    src/views/dlgkernelpatch.h \
    src/views/dlgmisc.h \
    src/views/dlgnewkeyfield.h \
    src/views/dlgpreference.h \
    src/views/mainwindow.h \
    src/views/syncocdialog.h \
    src/views/BalloonTip.h \
    src/views/tooltip.h \
    src/watcher/filesystemwatcher.h \
    src/utils/Method.h \
    src/utils/Plist.hpp \
    src/utils/PlistDate.hpp \
    src/utils/base64.hpp \
    src/utils/commands.h \
    src/utils/plistparser.h \
    src/utils/plistserializer.h \
    src/utils/pugiconfig.hpp \
    src/utils/pugixml.hpp \
    src/utils/recentfiles.h
    src/app/myapp.h \

FORMS += \
    src/views/aboutdialog.ui \
    src/views/autoupdatedialog.ui \
    src/views/dlgMountESP.ui \
    src/views/dlgOCValidate.ui \
    src/views/dlgParameters.ui \
    src/views/dlgPreset.ui \
    src/views/dlgdatabase.ui \
    src/views/dlgkernelpatch.ui \
    src/views/dlgmisc.ui \
    src/views/dlgnewkeyfield.ui \
    src/views/dlgpreference.ui \
    src/views/mainwindow.ui \
    src/views/syncocdialog.ui

# Default rules for deployment.
# qnx: target.path = /tmp/$${TARGET}/bin
# else: unix:!android: target.path = /opt/$${TARGET}/bin
# !isEmpty(target.path): INSTALLS += target

DISTFILES += \
    src/Info.plist \
    src/myapp.rc

RESOURCES += \
    qtocc.qrc

CONFIG(debug,debug|release) {
    DESTDIR = $$absolute_path($${_PRO_FILE_PWD_}/bin/debug)
} else {
    DESTDIR = $$absolute_path($${_PRO_FILE_PWD_}/bin/release)
}



