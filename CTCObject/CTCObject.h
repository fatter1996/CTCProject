#pragma once
#include "BaseWndClass/CTCMainWindow.h"
#include "SocketObject/socketudp.h"
#include "SocketObject/sockettcp.h"
#include "StationObject/StationObject.h"
#include "SealTechnique.h"
namespace CTCDoc {
    class CTCObject {

    public:
        CTCObject();
        ~CTCObject();

    public:
        QMainWindow* CreatCTCMainWnd();     //创建化主界面
        int ConfigFileInit();       //配置文件初始化
        int TtainTypeFileInit();       //车辆类型初始化
        void SocketComInit(); //通信初始化
        void StaOperationConnect();  //初始化站场操作信号槽
        bool UserLogin();

    private:
        int m_nStationViewType = 1; //界面类型;1-卡斯柯,2-铁科院
        CTCWindows::CTCMainWindow* m_pCTCMainWindow = nullptr;
        Station::MainStationObject* m_pMainStation = nullptr;
        //通信
        Socket::SocketUDP* m_pSocketUDP = nullptr;
        Socket::SocketTCP* m_pSocketTCP = nullptr;
        //站间透明
        QVector<Station::StationObject*> m_vecMultiStation;

        bool m_bShowToolbarBtn = true;
        bool m_bShowToolbarLabel = true;
    };
}
