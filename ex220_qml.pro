QT += qml quick sql network

CONFIG += c++11

unix{
   DEFINES += _fullScreen UNIX_PLATFORM
   QT      += androidextras
}

macos | ios{
    QT      -= androidextras
}

SOURCES += main.cpp \
    tcpdataclient.cpp \
    tcpcmdclient.cpp

HEADERS += \
    tcpdataclient.h \
    micro.h \
    tcpcmdclient.h \
    header.h


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH += $$PWD/ \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    android-sources/AndroidManifest.xml


ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

