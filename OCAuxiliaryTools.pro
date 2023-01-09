QT     += core gui
QT     += xml
QT     += network
CONFIG +=sdk_no_version_check

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

TRANSLATIONS += \
            src/cn.ts

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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

SOURCES += \
    src/BalloonTip.cpp \
    src/Method.cpp \
    src/Plist.cpp \
    src/PlistDate.cpp \
    src/aboutdialog.cpp \
    src/autoupdatedialog.cpp \
    src/commands.cpp \
    src/dlgMountESP.cpp \
    src/dlgOCValidate.cpp \
    src/dlgParameters.cpp \
    src/dlgPreset.cpp \
    src/dlgdatabase.cpp \
    src/dlgkernelpatch.cpp \
    src/dlgmisc.cpp \
    src/dlgnewkeyfield.cpp \
    src/dlgpreference.cpp \
    src/filesystemwatcher.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/myapp.cpp \
    src/plistparser.cpp \
    src/plistserializer.cpp \
    src/pugixml.cpp \
    src/recentfiles.cpp \
    src/syncocdialog.cpp \
    src/tooltip.cpp

FORMS += \
    src/aboutdialog.ui \
    src/autoupdatedialog.ui \
    src/dlgMountESP.ui \
    src/dlgOCValidate.ui \
    src/dlgParameters.ui \
    src/dlgPreset.ui \
    src/dlgdatabase.ui \
    src/dlgkernelpatch.ui \
    src/dlgmisc.ui \
    src/dlgnewkeyfield.ui \
    src/dlgpreference.ui \
    src/mainwindow.ui \
    src/syncocdialog.ui

HEADERS += \
    src/BalloonTip.h \
    src/Method.h \
    src/Plist.hpp \
    src/PlistDate.hpp \
    src/aboutdialog.h \
    src/autoupdatedialog.h \
    src/base64.hpp \
    src/commands.h \
    src/dlgMountESP.h \
    src/dlgOCValidate.h \
    src/dlgParameters.h \
    src/dlgPreset.h \
    src/dlgdatabase.h \
    src/dlgkernelpatch.h \
    src/dlgmisc.h \
    src/dlgnewkeyfield.h \
    src/dlgpreference.h \
    src/filesystemwatcher.h \
    src/mainwindow.h \
    src/myapp.h \
    src/plistparser.h \
    src/plistserializer.h \
    src/pugiconfig.hpp \
    src/pugixml.hpp \
    src/recentfiles.h \
    src/syncocdialog.h \
    src/tooltip.h



