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
        explicit SocketTCP(QObject* parent = nullptr);
        ~SocketTCP();

    public:
        QTcpSocket* m_pTcpSocket = nullptr;
        QHostAddress m_hAdress;
        quint16 m_nPort = 0;
        int m_nTimer = -1;
    public:
        //初始化端口
        bool InitServer(const QHostAddress& hAddress, const quint16& nPort);

    private:
        void timerEvent(QTimerEvent* event);

    signals:
        //接收数据信号
        void recvData(const QByteArray& dataArray, const QHostAddress& hAddress, const quint16& nPort);
        //断开链接信号
        void disconnected(const QHostAddress& hAddress, const quint16& nPort);
        //链接成功信号
        void connected(const QHostAddress& hAddress, const quint16& nPort);

    public slots:
        //链接成功槽
        void onConnected();
        //断开链接槽
        void onDisconnected();
        //接收数据槽
        void onRecvData();
        //发送数据槽
        void onSendDataSlot(const QByteArray& dataArray);
    };
}

#endif // SOCKETTCP_H
