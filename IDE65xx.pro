QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    about.cpp \
    baseplaintextedit.cpp \
    bookmarkwidget.cpp \
    cartconv.cpp \
    choosetopic.cpp \
    codeeditor.cpp \
    commodorestyle.cpp \
    common.cpp \
    darkfusionstyle.cpp \
    delegate.cpp \
    fabricstyle.cpp \
    hexsearchdialog.cpp \
    highlighter.cpp \
    hint.cpp \
    ideiconprovider.cpp \
    main.cpp \
    mainwindow.cpp \
    memoryviewer.cpp \
    popupmenu.cpp \
    qhexedit2/chunks.cpp \
    qhexedit2/commands.cpp \
    qhexedit2/qhexedit.cpp \
    settingswindow.cpp \
    singleapplication.cpp \
    tab.cpp \
    tdiskdirective.cpp \
    tfiledirective.cpp

HEADERS += \
    about.h \
    baseplaintextedit.h \
    bookmarkwidget.h \
    cartconv.h \
    choosetopic.h \
    codeeditor.h \
    commodorestyle.h \
    common.h \
    darkfusionstyle.h \
    delegate.h \
    fabricstyle.h \
    hexsearchdialog.h \
    highlighter.h \
    hint.h \
    ideiconprovider.h \
    mainwindow.h \
    memoryviewer.h \
    popupmenu.h \
    qhexedit2/chunks.h \
    qhexedit2/commands.h \
    qhexedit2/qhexedit.h \
    settingswindow.h \
    singleapplication.h \
    tab.h \
    tdiskdirective.h \
    tfiledirective.h \
    version.h

FORMS += \
    about.ui \
    cartconv.ui \
    choosetopic.ui \
    hexsearchdialog.ui \
    mainwindow.ui \
    settingswindow.ui \
    tdiskdirective.ui \
    tfiledirective.ui

# set custom icon on Windows only
#win32:RC_ICONS = icon/ide65xx.ico
win32:RC_FILE = version.rc

RESOURCES += \
    resource.qrc

DISTFILES += \
    version.rc

# macos app
ICON = icon/ide65xx.icns

TARGET = IDE65XX
