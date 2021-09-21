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

TRANSLATIONS += cn.ts

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Method.cpp \
    Plist.cpp \
    PlistDate.cpp \
    aboutdialog.cpp \
    autoupdatedialog.cpp \
    commands.cpp \
    dlgMountESP.cpp \
    dlgOCValidate.cpp \
    dlgParameters.cpp \
    dlgdatabase.cpp \
    main.cpp \
    mainwindow.cpp \
    myapp.cpp \
    plistparser.cpp \
    plistserializer.cpp \
    pugixml.cpp \
    recentfiles.cpp \
    tooltip.cpp

HEADERS += \
    Method.h \
    Plist.hpp \
    PlistDate.hpp \
    aboutdialog.h \
    autoupdatedialog.h \
    base64.hpp \
    commands.h \
    dlgMountESP.h \
    dlgOCValidate.h \
    dlgParameters.h \
    dlgdatabase.h \
    mainwindow.h \
    myapp.h \
    plistparser.h \
    plistserializer.h \
    pugiconfig.hpp \
    pugixml.hpp \
    recentfiles.h \
    tooltip.h

FORMS += \
    aboutdialog.ui \
    autoupdatedialog.ui \
    dlgMountESP.ui \
    dlgOCValidate.ui \
    dlgParameters.ui \
    dlgdatabase.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

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



