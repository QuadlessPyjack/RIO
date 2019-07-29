QT -= gui
QT += network sql

CONFIG += c++11 console file_copies
CONFIG -= app_bundle
COPIES += win32:copyMySqlDriver win32:copyQtSqlDriver

win32:copyMySqlDriver.files = $$files(dist/win32/*.dll)
win32:copyQtSqlDriver.files = $$files(dist/win32/sqldrivers/*.dll)

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


win32:CONFIG(release, debug|release): {
    LIBS += -L$$OUT_PWD/../RIO_protocol/release/ -lRIO_protocol
    copyMySqlDriver.path  = $$OUT_PWD/../RIO_Server/release/
    copyQtSqlDriver.path  = $$OUT_PWD/../RIO_Server/release/sqldrivers/
}
else:win32:CONFIG(debug, debug|release): {
    LIBS += -L$$OUT_PWD/../RIO_protocol/debug/ -lRIO_protocol
    copyMySqlDriver.path  = $$OUT_PWD/../RIO_Server/debug/
    copyQtSqlDriver.path  = $$OUT_PWD/../RIO_Server/debug/sqldrivers/
}
else:unix: LIBS += -L$$OUT_PWD/../RIO_protocol/ -lRIO_protocol

INCLUDEPATH += $$PWD/../RIO_protocol
DEPENDPATH += $$PWD/../RIO_protocol
