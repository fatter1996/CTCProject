#ifndef SOCKETTCP_H
#define SOCKETTCP_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

namespace Socket {
    //TCP Client端
    class SocketTCP : public QObject
    {
        Q_OBJECT
    public:
        explicit SocketTCP(QObject* pParent = nullptr);
        ~SocketTCP();

    public:
        //初始化端口
        bool InitClient();
        void setLocalAddress(const QHostAddress& hAddress, const quint16& nPort);
        void setServerAddress(const QHostAddress& hAddress, const quint16& nPort);

    private:
        void timerEvent(QTimerEvent* event) override;

    signals:
        //接收数据信号
        void recvData(const QByteArray&, const QHostAddress&, const quint16&);

    public slots:
        //链接成功槽
        void onConnected();
        //断开链接槽
        void onDisconnected();
        //接收数据槽
        void onRecvData();
        //发送数据槽
        void onSendData(const QByteArray& dataArray);

    private:
        QTcpSocket* m_pTcpSocket = nullptr;

        int m_nTimer = -1;
        QHostAddress m_hLocalIp;
        uint m_nLocalPort = 0;
        QHostAddress m_hServerIp;
        uint m_nServerPort = 0;
    };
}

#endif // SOCKETTCP_H
