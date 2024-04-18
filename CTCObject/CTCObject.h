#pragma once
#include "CTCMainWindow/CTCMainWindow.h"
#include "SocketObject/socketudp.h"
#include "SocketObject/sockettcp.h"
#include "StationObject/StationObject.h"

namespace CTCDoc {
    class CTCObject {

    public:
        CTCObject();
        ~CTCObject();

    public:
        QMainWindow* CreatCTCMainWnd();     //创建化主界面
        int ConfigFileInit();       //配置文件初始化
        void StaOperationConnect();  //初始化站场操作信号槽
        bool UserLogin();
            
    private:
        int m_nStationViewType = 1; //界面类型;1-卡斯柯,2-铁科院
        CTCWindows::CTCMainWindow* m_pCTCMainWindow = nullptr;
        Station::StationObject* m_pMainStation = nullptr;

    private:
        //通信
        Socket::SocketUDP* socketUDP = nullptr;
        Socket::SocketTCP* socketTCP = nullptr;
    };

}
