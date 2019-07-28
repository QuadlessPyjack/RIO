QT += quick network widgets charts
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
    sslclient.cpp \
    appmanager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

HEADERS += \
    sslclient.h \
    appmanager.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../RIO_protocol/release/ -lRIO_protocol
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../RIO_protocol/debug/ -lRIO_protocol
else:unix: LIBS += -L$$OUT_PWD/../RIO_protocol/ -lRIO_protocol

INCLUDEPATH += $$PWD/../RIO_protocol
DEPENDPATH += $$PWD/../RIO_protocol
