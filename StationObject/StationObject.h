﻿#pragma once
#include <QXmlStreamReader>
#include "Device/DeviceBase.h"
#include "Transmission/StaProtocol.h"
#include "Transmission/StaPacket.h"
#include "../CultivateObject/Subject.h"
#include "GlobalStruct.h"

#define STAVIEW       1
#define TRAINNUM      2

namespace Station {

    enum class DiploidOperate : int {
        amplify,    //放大
        reduce,     //缩小
        revert      //还原
    };

    enum class DiploidRatio : int {
        StaDiploid,             //站场图缩放
        StaTrainNumDiploid,     //车次缩放
        MultiDiploid,            //站场图缩放-站间透明
        MultiTrainNumDiploid,    //车次缩放-站间透明
    };

    struct UserInfo //用户信息
    {
        QString m_strName;
        QString m_strPassWord;
    };

    class StationObject : public QObject { 
        Q_OBJECT
    public:
        explicit StationObject(QObject* pParent = nullptr);
        ~StationObject();

    public:     //外部初始化
        void InitDeviceEventFilter(QWidget* pWidget);
        int ReadStationInfo(const QString& filePath); //解析"station.xml"
        void InitStaDevice();   //站场设备初始化
        int ReadDeviceConfig(const QString& filePath); //解析"DeviceConfig.json"
        void SetVisible(VisibleDev devType, bool bVisible);
        bool IsVisible(VisibleDev devType);

    private:    //内部初始化
        void ReadDeviceInfoHead(); //解析"station.xml" HEAD
        void ReadDeviceInfo();  //解析"station.xml" DEVICE
        void InitDeviceMap();
        
    public:
        QVector<Device::DeviceBase*>& getDeviceVectorByType(QString strType) { return m_mapDeviceVector[strType]; }
        void setStationName(const QString& strName) { m_strStationName = strName; }
        Device::DeviceBase* getDeviceByCode(const uint& nCode);
        Device::DeviceBase* getDeviceByName(const QString& strName);
        QSize getStaFixedSize(); //获取站场实际大小
        uint getStationId() const { return m_nStationId; }
        QString getStationName() const { return m_strStationName; }
        QPoint getOffset() const { return m_ptOffset; }
        bool IsMainStation() const { return m_bMainStation; }
        QWidget* ShowWidget() const { return m_pShowWidget; }
    public:
        static void InitCreatDeviceMap();

    private:
        friend class MainStationObject;
        
    private:
        bool m_bMainStation = false;
        uint m_nStationId = 0;
        QString m_strStationName;
        QSize m_szStation;
        bool m_bOverturn = false;         //站场是否翻转
        QPoint m_ptOffset;       //站场图绘制偏移量
        QMap<QString, QVector<Device::DeviceBase*>> m_mapDeviceVector;
        QMap<VisibleDev, bool> m_mapVisible;
        QWidget* m_pShowWidget = nullptr;

    private:
        static QXmlStreamReader* m_pDeviceInfoReader;  //XML解析器
        static QMap<QString, std::function<Device::DeviceBase* (StationObject*)>> m_mapCreatDeviceVector;
        static int m_nTimerId_500;
    };

    
    class MainStationObject : public StationObject {
        Q_OBJECT
    public:
        explicit MainStationObject(QObject* pParent = nullptr);
        ~MainStationObject();

    private:
        void timerEvent(QTimerEvent* event) override;

    public:
        void OnLine();
        void SelectStationOrder();    //查询命令信息(调度命令,阶段计划,进路序列,车辆信息,行车日志等)
        void SelectStaLimits();    //查询权限信息
        void SelectStaStagePlan();   //查询阶段计划
        void SelectStaTrain();   //查询车辆信息
        void SelectStaTrainRoute();   //查询进路序列
        void SelectStaDispatch();   //查询调度命令
        void SelectStaTrainDispatch(StaDispatchOrder* pOrder);   //查询机车调度命令
        void SelectStaTrafficLog();   //查询行车日志
        void GetStationOrderByJosn(const QByteArray& btDataArray, Order type);

        int AddNewTrain(StaTrain* pStaTrain); //添加新车次
        int DeleteTrain(StaTrain* pStaTrain); //删除车次
        int SetTrainRunning(StaTrain* pStaTrain, bool bRunning); //设置车次启动或停稳
        int ChangeTrainNum(StaTrain* pStaTrain, QString strTrainNum); //设置车次启动或停稳
        int ChangeTrainAttr(StaTrain* pStaTrain, int nSpeed, QString strLocomotive, bool bElectric); //修改车次信息
        int AddNewStagePlan(StaStagePlan* pStaStagePlan); //添加阶段计划
        int AddNewTrainRoute(StaTrainRoute* pTrainRoute); //添加进路序列
        void InitCombinativeRoute(); //初始化组合进路
        int AddNewDispatchOrder(StaDispatchOrder* pDispatchOrder); //添加调度命令
        int AddNewTrafficLog(StaTrafficLog* pTrafficLog); //添加行车日志
        void CreatTrainRouteByTrafficLog(StaTrafficLog* pTrafficLog);

        StaTrain* getStaTrainById(int nTrainId);
        StaTrainRoute* getStaTrainRouteById(int nRouteId);
        QVector<StaTrainRoute*>  getStaTrainRouteByTrain(int nTrainID);
        StaTrainRoute* getStaTrainRouteByRowIndex(int nRow);
        StaTrafficLog* getStaTrafficLogById(int nLogId);

        void SendPacketMsg(int nTargetCode, int nOrderType = 0, int nAttr1 = -1, int nAttr2 = -1, QByteArray btAttr3 = QByteArray());
        void SubmitCurSubject(); //提交当前题目
        void AddSelectDevice(Device::DeviceBase* pDevice); 
        bool IsAllowStaOperation(); //是否可操作
        void CompareResult(const QByteArray& dataAyyay);

        void StationReset();
        void ClearDevice();

    public:
        QVector<Device::DeviceBase*>& getSelectDevice() { return m_vecSelectDevice; }
        const UserInfo& getCurrUser() const { return m_infoCurrUser; }
        const QString& getCultivateOrder() const { return m_strOrderToInterLock; }
        void setUserId(int nUserId) { m_nUserId = nUserId; }
        int getUserId() const { return m_nUserId; }
        void setCurSubject(CultivateObject::Subject* pSubject);
        CultivateObject::Subject* getCurSubject() const { return m_pCurSubject; }
        bool IsOverturn() const { return m_bOverturn; }
        
        QVector<StaTrain*>& TrainList() { return m_vecStaTrain; }
        QVector<StaStagePlan*>& StagePlanList() { return m_vecStaStagePlan; }
        QVector<StaTrainRoute*>& TrainRouteList() { return m_vecStaTrainRoute; }
        QVector<StaDispatchOrder*>& DispatchOrderList() { return m_vecStaDispatchOrder; }
        QVector<StaTrafficLog*>& TrafficLogList() { return m_vecStaTrafficLog; }
        StaStagePlan* NewStagePlan() { return m_pNewStagePlan; }
        void ClearNewStagePlan() { m_pNewStagePlan = nullptr; };
        StaDispatchOrder* NewDispatchOrder() { return m_pNewDispatchOrder; }
        void ClearNewDispatchOrder() { m_pNewDispatchOrder = nullptr; };

        void setDiploid(DiploidOperate operate, int nType = STAVIEW); //站场大小缩放
        double getDiploid(DiploidRatio ratio) const { return m_mapDiploidRatio[ratio]; }

    public:
        struct StaLimits {
            int nRouteLimits = 0; //进路权限，0 - 联锁，1 - 在CTC，2 - 占线板
            bool bAutoSignStage = false;   //自动签收阶段计划
            bool bExStaControl = false;   //非常站控
            int nControlMode = 0;   //控制模式, 0 - 中心控制，1 - 车站控制，2 - 车站调车
            int nApplyControlMode = -1;   //申请控制模式, 0 - 中心控制，1 - 车站控制，2 - 车站调车
            int nPlanMode = 0;  //计划模式，0 - 手工排路，1 - 按图排路
            bool nPlanControl = false;  //计划控制
        };
        int getStaLimits(Limits type);
        void setStaLimits(Limits type, int nValue);

    public slots:
        void onReciveData(const QByteArray& dataAyyay);
        void onOrderIssued();
        void onOrderClear();
        void onUserLogin(QString strUserName, QString strPassword);

    signals:
        void FunBtnStateReset();
        void SendDataToUDP(const QByteArray&);
        void SendDataToTCP(const QByteArray&);
        void TrainRouteUpData();
        void TrafficLogTableUpData();

    private:
        //数据传输
        Transmission::StaProtocol* m_pProtocol = nullptr;
        Transmission::StaPacket* m_pStaPacket = nullptr;

    private:
        StaLimits m_StaLimits;
        QVector<Device::DeviceBase*> m_vecSelectDevice;
        UserInfo m_infoCurrUser;
        QString m_strOrderToInterLock;
        int m_nUserId = 0;
        CultivateObject::Subject* m_pCurSubject = nullptr;
        bool m_bAutoSendPlan = true;
        int m_nCountdown = 0;
        int m_nTimerId_1000 = -1;
        QVector<StaTrain*> m_vecStaTrain;
        QVector<StaStagePlan*> m_vecStaStagePlan;
        QVector<StaTrainRoute*> m_vecStaTrainRoute;
        QVector<StaDispatchOrder*> m_vecStaDispatchOrder;
        QVector<StaTrafficLog*> m_vecStaTrafficLog;

        QMap<Order, std::function<StaOrder*()>> m_mapCreatStationOrder;
        QMap<Order, std::function<void(void*, const QJsonObject&)>> m_mapStationOrder;
        QMap<DiploidRatio, double> m_mapDiploidRatio;

        StaStagePlan* m_pNewStagePlan = nullptr;
        StaDispatchOrder* m_pNewDispatchOrder = nullptr;
    };
}
