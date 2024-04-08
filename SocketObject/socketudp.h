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
        explicit SocketUDP(QObject* parent = nullptr);
        ~SocketUDP();

    public:
        //申明socket通道
        QUdpSocket* m_pUdpSocket = nullptr;

    public:
        //初始化端口
        bool InitSocket();
        void timerEvent(QTimerEvent* event);

    public:
        void setLocalAddress(const QHostAddress& hAddress, const quint16& nPort);
        void setInterlockAddress(const QHostAddress& hAddress, const quint16& nPort);

    signals:
        //接收数据信号
        void recvDataSignal(QByteArray dataArray);

    public slots:
        //接收数据槽
        void onRecvData();
        //发送数据槽
        void onSendData(const QByteArray& dataArray, const QHostAddress& hAddress, const quint16& nPort);

    private:
        int nTimerId = -1;
        QHostAddress m_hLocalIp;
        uint m_nLocalPort = 0;
        QHostAddress m_hInterlockIp;
        uint m_nInterlockPort = 0;
    };
}

#endif // SOCKETUDP_H
