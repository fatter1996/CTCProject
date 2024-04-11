#include "socketudp.h"
#pragma execution_character_set("utf-8")

namespace Socket {
    SocketUDP::SocketUDP(QObject* parent) : QObject(parent)
    {
        m_pUdpSocket = new QUdpSocket();
        nTimerId = startTimer(1000);
    }

    SocketUDP::~SocketUDP()
    {
        m_pUdpSocket->close();
        delete m_pUdpSocket;
        m_pUdpSocket = nullptr;
    }

    bool SocketUDP::InitSocket()
    {
        //绑定本地接收UDP socket通道
        if (m_pUdpSocket->bind(m_hLocalIp, m_nLocalPort)) {
            qDebug() << QString("init UDP socket %1:%2 succeeded!").arg(m_hLocalIp.toString()).arg(m_nLocalPort);
            connect(m_pUdpSocket, &QUdpSocket::readyRead, this, &SocketUDP::onRecvData);
            return true;
        }
        else {
            qCritical() << QString("init UDP socket %1:%2 failed!").arg(m_hLocalIp.toString()).arg(m_nLocalPort);
            return false;
        }
    }

    void SocketUDP::onRecvData()
    {
        QByteArray recvData;
        QHostAddress clientAddress;
        quint16 nPort = 0;

        while (m_pUdpSocket->hasPendingDatagrams()) {
            recvData.resize(m_pUdpSocket->pendingDatagramSize());
            m_pUdpSocket->readDatagram(recvData.data(), recvData.size(), &clientAddress, &nPort);
            //qDebug() << "接收数据：" << recvData.toHex();
            //发送信号
            if(recvData.size() >= 16)
                emit recvDataSignal(recvData);
        }
    }

    //发送数据槽
    void SocketUDP::onSendData(const QByteArray& dataArray, const QHostAddress& hAddress, const quint16& nPort)
    {
        m_pUdpSocket->writeDatagram(dataArray, QHostAddress(hAddress), nPort);
    }

    void SocketUDP::timerEvent(QTimerEvent* event)
    {
        if (nTimerId == event->timerId()) {
            onSendData("HellowWorld!", m_hInterlockIp, m_nInterlockPort);
        }
    }

    void SocketUDP::setLocalAddress(const QHostAddress& hAddress, const quint16& nPort)
    {
        m_hLocalIp = hAddress;
        m_nLocalPort = nPort;
    }

    void SocketUDP::setInterlockAddress(const QHostAddress& hAddress, const quint16& nPort)
    {
        m_hInterlockIp = hAddress;
        m_nInterlockPort = nPort;
    }
}
