QT -= gui
QT += network sql

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
    connectionmanager.cpp \
    persistence.cpp \
    sslserver.cpp

RESOURCES += \
    resources.qrc

HEADERS += \
    persistence.h \
    connectionmanager.h \
    sslserver.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../RIO_protocol/release/ -lRIO_protocol
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../RIO_protocol/debug/ -lRIO_protocol
else:unix: LIBS += -L$$OUT_PWD/../RIO_protocol/ -lRIO_protocol

INCLUDEPATH += $$PWD/../RIO_protocol
DEPENDPATH += $$PWD/../RIO_protocol
