#ifndef SERVERTCP_H
#define SERVERTCP_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
namespace Socket {
    //TCP Server端
    class ServerTCP : public QObject
    {
        Q_OBJECT
    public:
        explicit ServerTCP(QObject* parent = nullptr);
        ~ServerTCP();

    public:
        //TCPServer
        QTcpServer* pTcpServer = nullptr;
        //连接的客户端
        QVector<QTcpSocket*> vectTcpSocket;

    public:
        //初始化端口
        bool InitServer(const QHostAddress& hAddress, const quint16& nPort);

    signals:
        //接收数据信号
        void recvDataSignal(const QByteArray& dataArray, const QHostAddress& hAddress, const quint16& nPort);

    public slots:
        //和客户端建立连接的槽
        void onClientConnect();
        //和客户端断开连接的槽
        void onDisconnected();
        //接收数据槽
        void onRecvDataSlot();
        //发送数据槽
        void onSendDataSlot(const QByteArray& dataArray, const QHostAddress& hAddress, const quint16& nPort);

    };

}

#endif // SERVERTCP_H
