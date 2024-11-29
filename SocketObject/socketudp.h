#ifndef SOCKETUDP_H
#define SOCKETUDP_H

#include <QEvent>
#include <QObject>
#include <QUdpSocket>

namespace Socket {
    //UDP通信
    class SocketUDP : public QObject
    {
        Q_OBJECT
    public:
        explicit SocketUDP(QObject* pParent = nullptr);
        ~SocketUDP();

    public:
        //初始化端口
        bool InitSocket();

    public:
        void setLocalAddress(const QHostAddress& hAddress, const quint16& nPort);
        void setServerAddress(const QHostAddress& hAddress, const quint16& nPort);
        
    signals:
        //接收数据信号
        void recvData(const QByteArray&);

    public slots:
        //接收数据槽
        void onRecvData();
        //发送数据槽
        void onSendData(const QByteArray& dataArray);

    private:
        //socket通道
        QUdpSocket* m_pUdpSocket = nullptr;
        QHostAddress m_hLocalIp;
        uint m_nLocalPort = 0;
        QHostAddress m_hServerIp;
        uint m_nServerPort = 0;
    };
}

#endif // SOCKETUDP_H
