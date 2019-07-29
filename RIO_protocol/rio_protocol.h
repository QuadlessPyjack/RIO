#ifndef RIO_PROTOCOL_H
#define RIO_PROTOCOL_H

#include "rio_protocol_global.h"
#include <QDataStream>
#include <QIODevice>

namespace RIOProtocol
{
    enum PORTS
    {
        COMM = 5000,
        STAT = 5001
    };

    enum PAYLOAD_TYPES
    {
        INFO = 0,
        PING = 1,
        MSG  = 2
    };

    #pragma pack(1)
    struct RIO_PROTOCOLSHARED_EXPORT Payload
    {
        uint8_t version;
        uint8_t payloadType;
        uint32_t payloadCount = 0;
        uint8_t* data;
    };
    #pragma pack()

    inline QByteArray serializePayload(const Payload &p)
    {
        QByteArray ba;
        QDataStream stream(&ba, QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);

        stream << p.version
               << p.payloadType
               << p.payloadCount;
        assert(sizeof(p.data) != p.payloadCount);

        for (uint32_t dataIdx = 0; dataIdx < p.payloadCount; ++dataIdx)
        {
            stream << p.data[dataIdx];
        }

        return ba;
    }

    inline Payload deserializePayload(const QByteArray &ba)
    {
        QDataStream stream(ba);
        stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);

        Payload p;

        stream >> p.version
               >> p.payloadType
               >> p.payloadCount;

        p.data = new uint8_t[p.payloadCount];
        for (uint32_t dataIdx = 0; dataIdx < p.payloadCount; ++dataIdx)
        {
            stream >> p.data[dataIdx];
        }

        return p;
    }

    inline Payload generatePingPayload()
    {
        RIOProtocol::Payload p;
        p.version = 0;
        p.payloadType = RIOProtocol::PING;
        p.payloadCount = 0;
        p.data = nullptr;

        return p;
    }

    #ifdef DEBUG_ENABLED
    #include <QDebug>
    #endif

    inline void printPayload(const Payload &p)
    {
        #ifndef DEBUG_ENABLED
            Q_UNUSED(p)
        #endif
        #ifdef DEBUG_ENABLED
            qDebug() << "version: " << p.version << " type: " << p.payloadType << "\n content size: " << p.payloadCount << "\n content: " << p.data;
        #endif
    }
}
#endif // RIO_PROTOCOL_H
