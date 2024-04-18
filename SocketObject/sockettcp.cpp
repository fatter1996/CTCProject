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

    bool SocketTCP::InitServer()
    {
        //m_pTcpSocket->bind(m_hLocalIp, m_nLocalPort);
        m_pTcpSocket->connectToHost(m_hCultivateIp, m_nCultivatePort);
        return true;
        //if (m_pTcpSocket->bind(m_hLocalIp, m_nLocalPort)) {
        //    qDebug() << QString("Init TCP socket %1:%2 succeeded!").arg(m_hLocalIp.toString()).arg(m_nLocalPort);
        //    //连接
        //    m_pTcpSocket->connectToHost(m_hCultivateIp, m_nCultivatePort);
        //    return true;
        //}
        //else {
        //    qCritical() << QString("Init TCP socket %1:%2 failed!").arg(m_hLocalIp.toString()).arg(m_nLocalPort);
        //    return false;
        //}
    }

    void SocketTCP::onConnected()
    {
        qDebug() << QString("TCP connect to %1:%2 succeeded!").arg(m_hCultivateIp.toString()).arg(m_nCultivatePort);
        if (m_nTimer >= 0) {
            killTimer(m_nTimer);
            m_nTimer = -1;
        }
        emit connected(m_hCultivateIp, m_nCultivatePort);
    }

    void SocketTCP::onDisconnected()
    {
        qDebug() << QString("TCP disconnected from %1:%2 !").arg(m_hCultivateIp.toString()).arg(m_nCultivatePort);
        emit disconnected(m_hCultivateIp, m_nCultivatePort);
        m_nTimer = startTimer(1000);
    }

    void SocketTCP::onRecvData()
    {
        emit recvData(m_pTcpSocket->readAll(), m_hCultivateIp, m_nCultivatePort);
    }

    void SocketTCP::onSendData(const QByteArray& dataArray)
    {
        m_pTcpSocket->write(dataArray);
    }

    void SocketTCP::timerEvent(QTimerEvent* event)
    {
        if (m_nTimer == event->timerId()) {
            //m_pTcpSocket->connectToHost(m_hCultivateIp, m_nCultivatePort);
        }
    }

    void SocketTCP::setLocalAddress(const QHostAddress& hAddress, const quint16& nPort)
    {
        m_hLocalIp = hAddress;
        m_nLocalPort = nPort;
    }

    void SocketTCP::setCultivateAddress(const QHostAddress& hAddress, const quint16& nPort)
    {
        m_hCultivateIp = hAddress;
        m_nCultivatePort = nPort;
    }
}

