#pragma once
#include "CTCMainWindow/CTCMainWindow.h"
#include "SocketObject/socketudp.h"

namespace CTCDoc {
    class CTCObject {

    public:
        CTCObject();
        ~CTCObject();

    public:
        QMainWindow* CreatCTCMainWnd();     //创建化主界面
        int ConfigInit();       //配置文件初始化

    private:
        int m_nStationViewType = 1; //界面类型;1-卡斯柯,2-铁科院
        
    public:
        CTCWindows::CTCMainWindow* m_pCTCMainWindow = nullptr;
        
    private:
        //通信
        Socket::SocketUDP* socketUDP = nullptr;
    };

}
