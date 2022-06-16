QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    common/tatapp.cpp \
    common/tatlogger.cpp \
    common/tatsettings.cpp \
    main.cpp \
    tatwindow.cpp \
    tools/convtoqtimgtool.cpp \
    tools/httpclientfortimeouttesttool.cpp \
    tools/httpserverfortimeouttesttool.cpp \
    tools/war3editortool.cpp

HEADERS += \
    common/tatapp.h \
    common/tatlogger.h \
    common/tatsettings.h \
    tatwindow.h \
    tools/convtoqtimgtool.h \
    tools/httpclientfortimeouttesttool.h \
    tools/httpserverfortimeouttesttool.h \
    tools/war3editortool.h

FORMS += \
    tatwindow.ui \
    tools/convtoqtimgtool.ui \
    tools/httpclientfortimeouttesttool.ui \
    tools/httpserverfortimeouttesttool.ui \
    tools/war3editortool.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += \
    $$PWD/common \
    $$PWD/tools

#
win32 {
    #
    LIBS += \
        -lUser32 \
        -lOle32 \
        -lGdi32 \
        -lWinmm \
        -lDbghelp \
        -lShlwapi \
        -lWininet \
        -lWs2_32 \
        -lIphlpapi

    # Program can handle larger than 2GB memories
    QMAKE_LFLAGS_WINDOWS += /LARGEADDRESSAWARE
    QMAKE_LFLAGS_CONSOLE += /LARGEADDRESSAWARE

    # charset
    QMAKE_CXXFLAGS += /source-charset:utf-8 \
                      /execution-charset:utf-8

    # 在release模式下也生成pdb文件,用于调试.
    QMAKE_LFLAGS += /MAP /MAPINFO:EXPORTS
    QMAKE_CFLAGS += /Zi
    QMAKE_LFLAGS += /debug /opt:ref
    QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
}
