#include "sockettcp.h"
#include <QTimerEvent>

namespace Socket {
    SocketTCP::SocketTCP(QObject* parent) : QObject(parent)
    {
        m_pTcpSocket = new QTcpSocket(this);
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

    bool SocketTCP::InitServer(const QHostAddress& hAddress, const quint16& nPort)
    {
        m_hAdress = hAddress;
        m_nPort = nPort;
        //连接
        m_pTcpSocket->connectToHost(m_hAdress, m_nPort);
        return true;
    }

    void SocketTCP::onConnected()
    {
        qDebug() << QString("TCP connect to %1:%2 succeeded!").arg(m_hAdress.toString()).arg(m_nPort);
        if (m_nTimer >= 0) {
            killTimer(m_nTimer);
            m_nTimer = -1;
        }
        emit connected(m_hAdress, m_nPort);
    }

    void SocketTCP::onDisconnected()
    {
        qDebug() << QString("TCP disconnected from %1:%2 !").arg(m_hAdress.toString()).arg(m_nPort);
        emit disconnected(m_hAdress, m_nPort);
        m_nTimer = startTimer(1000);
    }

    void SocketTCP::onRecvData()
    {
        emit recvData(m_pTcpSocket->readAll(), m_hAdress, m_nPort);
    }

    void SocketTCP::onSendDataSlot(const QByteArray& dataArray)
    {
        m_pTcpSocket->write(dataArray);
    }

    void SocketTCP::timerEvent(QTimerEvent* event)
    {
        if (m_nTimer == event->timerId()) {
            m_pTcpSocket->connectToHost(m_hAdress, m_nPort);
        }
    }
}

