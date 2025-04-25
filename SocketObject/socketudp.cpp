#include "socketudp.h"
#pragma execution_character_set("utf-8")

namespace Socket {
    SocketUDP::SocketUDP(QObject* pParent) : QObject(pParent)
    {
        m_pUdpSocket = new QUdpSocket();
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
            qDebug() << QString("Init UDP socket %1:%2 succeeded!").arg(m_hLocalIp.toString()).arg(m_nLocalPort);
            connect(m_pUdpSocket, &QUdpSocket::readyRead, this, &SocketUDP::onRecvData);
            return true;
        }
        else {
            qCritical() << QString("Init UDP socket %1:%2 failed!").arg(m_hLocalIp.toString()).arg(m_nLocalPort);
            return false;
        }
    }

    void SocketUDP::onRecvData()
    {
        QByteArray recvDataArray;
        QHostAddress clientAddress;
        quint16 nPort = 0;

        while (m_pUdpSocket->hasPendingDatagrams()) {
            recvDataArray.resize(m_pUdpSocket->pendingDatagramSize());
            m_pUdpSocket->readDatagram(recvDataArray.data(), recvDataArray.size(), &clientAddress, &nPort);
            //发送信号
            if (recvDataArray.size() >= 16) {
                emit recvData(recvDataArray);
            } 
        }
    }

    //发送数据槽
    void SocketUDP::onSendData(const QByteArray& dataArray)
    {
        m_pUdpSocket->writeDatagram(dataArray, m_hServerIp, m_nServerPort);
    }

    void SocketUDP::setLocalAddress(const QHostAddress& hAddress, const quint16& nPort)
    {
        m_hLocalIp = hAddress;
        m_nLocalPort = nPort;
    }

    void SocketUDP::setServerAddress(const QHostAddress& hAddress, const quint16& nPort)
    {
        m_hServerIp = hAddress;
        m_nServerPort = nPort;
    }
}
