#include "sockettcp.h"
#include <QTimerEvent>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace Socket {
    SocketTCP::SocketTCP(QObject* pParent) : QObject(pParent)
    {
        m_pTcpSocket = new QTcpSocket(this);
        int socketDescriptor = m_pTcpSocket->socketDescriptor();
        if (socketDescriptor != -1) {
            // 设置 SO_LINGER 选项
            int reuseAddr = 1;
            // 使用 setsockopt 设置选项
            if (setsockopt(socketDescriptor, SOL_SOCKET, SO_LINGER, (const char*)&reuseAddr, sizeof(reuseAddr)) < 0) {
                // 处理错误
                perror("setsockopt SO_LINGER");
            }
        }

        connect(m_pTcpSocket, &QTcpSocket::connected, this, &SocketTCP::onConnected);
        connect(m_pTcpSocket, &QTcpSocket::readyRead, this, &SocketTCP::onRecvData);
        connect(m_pTcpSocket, &QTcpSocket::disconnected, this, &SocketTCP::onDisconnected);
    }

    SocketTCP::~SocketTCP()
    {
        if (m_pTcpSocket) {
            delete m_pTcpSocket;
            m_pTcpSocket = nullptr;
        }
    }

    bool SocketTCP::InitClient()
    {
       if (1/*m_pTcpSocket->bind(QHostAddress::Any, m_nLocalPort)*/) {
           //qDebug() << QString("Init TCP socket %1:%2 succeeded!").arg(m_hLocalIp.toString()).arg(m_nLocalPort);
           //连接
           m_pTcpSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
           m_pTcpSocket->connectToHost(m_hServerIp, m_nServerPort);
           m_nTimer = startTimer(3000);
           return true;
       }
       else {
           qDebug() << QString("Init TCP socket %1:%2 failed!").arg(m_hLocalIp.toString()).arg(m_nLocalPort);
           return false;
       }
    }

    void SocketTCP::onConnected()
    {
        qDebug() << QString("TCP connect to %1:%2 succeeded!").arg(m_hServerIp.toString()).arg(m_nServerPort);
        if (m_nTimer >= 0) {
            killTimer(m_nTimer);
            m_nTimer = -1;
        }
    }

    void SocketTCP::onDisconnected()
    {
        qDebug() << QString("TCP disconnected from %1:%2 !").arg(m_hServerIp.toString()).arg(m_nServerPort);
        m_nTimer = startTimer(1000);
    }

    void SocketTCP::onRecvData()
    {
        emit recvData(m_pTcpSocket->readAll(), m_hServerIp, m_nServerPort);
    }

    void SocketTCP::onSendData(const QByteArray& dataArray)
    {
        m_pTcpSocket->write(dataArray);
    }

    void SocketTCP::timerEvent(QTimerEvent* event)
    {
        if (m_nTimer == event->timerId()) {
            m_pTcpSocket->connectToHost(m_hServerIp, m_nServerPort);
        }
        return QObject::timerEvent(event);
    }

    void SocketTCP::setLocalAddress(const QHostAddress& hAddress, const quint16& nPort)
    {
        m_hLocalIp = hAddress;
        m_nLocalPort = nPort;
    }

    void SocketTCP::setServerAddress(const QHostAddress& hAddress, const quint16& nPort)
    {
        m_hServerIp = hAddress;
        m_nServerPort = nPort;
    }
}

