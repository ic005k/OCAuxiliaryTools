QT       += core gui
QT       += xml
QT       += network

ICON = Icon.icns
RC_FILE += myapp.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += c++11
TARGET= OCAuxiliaryTools

unix:!macx:{

    QMAKE_RPATHDIR=$ORIGIN
    QMAKE_LFLAGS += -no-pie
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/lib\'"
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/platforms\'"
}

TRANSLATIONS += $$PWD/translations/cn.ts

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH +=  \
                src/widgets/ \
                src/watcher \
                src/utils \
                src/app

SOURCES += \
    src/widgets/BalloonTip.cpp \
    src/utils/Method.cpp \
    src/utils/Plist.cpp \
    src/utils/PlistDate.cpp \
    src/widgets/aboutdialog.cpp \
    src/widgets/autoupdatedialog.cpp \
    src/utils/commands.cpp \
    src/widgets/dlgMountESP.cpp \
    src/widgets/dlgOCValidate.cpp \
    src/widgets/dlgParameters.cpp \
    src/widgets/dlgPreset.cpp \
    src/widgets/dlgdatabase.cpp \
    src/widgets/dlgkernelpatch.cpp \
    src/widgets/dlgmisc.cpp \
    src/widgets/dlgnewkeyfield.cpp \
    src/widgets/dlgpreference.cpp \
    src/watcher/filesystemwatcher.cpp \
    src/main.cpp \
    src/widgets/mainwindow.cpp \
    src/app/myapp.cpp \
    src/utils/plistparser.cpp \
    src/utils/plistserializer.cpp \
    src/utils/pugixml.cpp \
    src/widgets/recentfiles.cpp \
    src/widgets/syncocdialog.cpp \
    src/widgets/tooltip.cpp \
    src/utils/fileoperation.cpp

HEADERS += \
    src/widgets/BalloonTip.h \
    src/utils/Method.h \
    src/utils/Plist.hpp \
    src/utils/PlistDate.hpp \
    src/widgets/aboutdialog.h \
    src/widgets/autoupdatedialog.h \
    src/utils/base64.hpp \
    src/utils/commands.h \
    src/widgets/dlgMountESP.h \
    src/widgets/dlgOCValidate.h \
    src/widgets/dlgParameters.h \
    src/widgets/dlgPreset.h \
    src/widgets/dlgdatabase.h \
    src/widgets/dlgkernelpatch.h \
    src/widgets/dlgmisc.h \
    src/widgets/dlgnewkeyfield.h \
    src/widgets/dlgpreference.h \
    src/watcher/filesystemwatcher.h \
    src/widgets/mainwindow.h \
    src/app/myapp.h \
    src/utils/plistparser.h \
    src/utils/plistserializer.h \
    src/utils/pugiconfig.hpp \
    src/utils/pugixml.hpp \
    src/widgets/recentfiles.h \
    src/widgets/syncocdialog.h \
    src/widgets/tooltip.h \
    src/utils/fileoperation.h

FORMS += \
    src/widgets/aboutdialog.ui \
    src/widgets/autoupdatedialog.ui \
    src/widgets/dlgMountESP.ui \
    src/widgets/dlgOCValidate.ui \
    src/widgets/dlgParameters.ui \
    src/widgets/dlgPreset.ui \
    src/widgets/dlgdatabase.ui \
    src/widgets/dlgkernelpatch.ui \
    src/widgets/dlgmisc.ui \
    src/widgets/dlgnewkeyfield.ui \
    src/widgets/dlgpreference.ui \
    src/widgets/mainwindow.ui \
    src/widgets/syncocdialog.ui

# Default rules for deployment.
# qnx: target.path = /tmp/$${TARGET}/bin
# else: unix:!android: target.path = /opt/$${TARGET}/bin
# !isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Info.plist \
    myapp.rc

RESOURCES += \
    qtocc.qrc

CONFIG(debug,debug|release) {
    DESTDIR = $$absolute_path($${_PRO_FILE_PWD_}/bin/debug)
} else {
    DESTDIR = $$absolute_path($${_PRO_FILE_PWD_}/bin/release)
}



