#include "servertcp.h"

namespace Socket {
    ServerTCP::ServerTCP(QObject* pParent) : QObject(pParent)
    {
        pTcpServer = new QTcpServer(this);
    }

    ServerTCP::~ServerTCP()
    {
        //清除
        vectTcpSocket.clear();
        if (pTcpServer) {
            //关闭服务
            pTcpServer->close();
            pTcpServer = nullptr;
        }
    }

    bool ServerTCP::InitServer(const QHostAddress& hAddress, const quint16& nPort)
    {
        //绑定本地IP和端口
        if (pTcpServer->listen(hAddress, nPort)) {
            qDebug() << QString("init TCP listen %1:%2 succeeded!").arg(hAddress.toString()).arg(nPort);
            connect(pTcpServer, &QTcpServer::newConnection, this, &ServerTCP::onClientConnect);
            return true;
        }
        else {
            qCritical() << QString("init TCP listen %1:%2 failed!").arg(hAddress.toString()).arg(nPort);
        }
        return false;
    }

    //和客户端建立连接的槽
    void ServerTCP::onClientConnect()
    {
        //取出连接好的套接字
        QTcpSocket* pSocketClient = pTcpServer->nextPendingConnection();
        qDebug() << QString("%1:%2 connect succeeded!").arg(pSocketClient->peerAddress().toString().split("::ffff:")[1]).arg(pSocketClient->peerPort());
        //连接信号槽
        connect(pSocketClient, &QTcpSocket::readyRead, this, &ServerTCP::onRecvDataSlot);
        connect(pSocketClient, &QTcpSocket::disconnected, this, &ServerTCP::onDisconnected);
        //增加该链接
        vectTcpSocket.append(pSocketClient);
    }
    //和客户端断开连接的槽
    void ServerTCP::onDisconnected()
    {
        //获取信号发送者
        QTcpSocket* pSocketClient = qobject_cast<QTcpSocket*>(sender());

        for (QTcpSocket* pSocket : vectTcpSocket) {
            if (pSocket->socketDescriptor() == pSocketClient->socketDescriptor()) {
                //移除客户端链接
                vectTcpSocket.removeOne(pSocket);
                qDebug() << QString("%1:%2 disconnected!")
                    .arg(pSocketClient->peerAddress().toString().split("::ffff:")[1])
                    .arg(QString::number(pSocketClient->peerPort()));
                return;
            }
        }
    }
    //接收数据槽
    void ServerTCP::onRecvDataSlot()
    {
        //获取信号发送者
        QTcpSocket* pSocketClient = qobject_cast<QTcpSocket*>(sender());
        emit recvDataSignal(pSocketClient->readAll(), pSocketClient->peerAddress(), pSocketClient->peerPort());
    }
    //发送数据槽
    void ServerTCP::onSendDataSlot(const QByteArray& dataArray, const QHostAddress& hAddress, const quint16& nPort)
    {
        for (QTcpSocket* pSocket : vectTcpSocket) {
            if (pSocket->localAddress() == hAddress && pSocket->localPort() == nPort) {
                pSocket->write(dataArray);
                continue;
            }
        }
    }
}


