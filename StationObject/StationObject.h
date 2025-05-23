﻿#pragma once
#include <QXmlStreamReader>
#include "Device/DeviceBase.h"
#include "Transmission/StaProtocol.h"
#include "Transmission/StaPacket.h"
#include "../CultivateObject/Subject.h"
#include "GlobalStruct.h"
#include <QStringList>
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
        int ReadStationInfoByXml(const QString& filePath); //解析"station.xml"
        int ReadStationInfoByTxt(const QString& filePath); //解析"station.txt"
        void InitStaDevice();   //站场设备初始化
        int ReadDeviceConfig(const QString& filePath); //解析"DeviceConfig.json"
        void SetVisible(VisibleDev devType, bool bVisible);
        bool IsVisible(VisibleDev devType);
        void AddNewTextSign(const QString& strText, const QPoint& ptPos, const QColor& colFont = Qt::black, const QColor& colBackground = Qt::white, int nSize = 10);
        void DeleteTextSign(Device::StaTextSign* pTextSign);
        void ClearAllTextSign();
      
    private:    //内部初始化
        void ReadDeviceInfoHead(); //解析"station.xml" HEAD
        void ReadDeviceInfo();  //解析"station.xml" DEVICE
        void InitDeviceMap();
        
    public:
        QVector<Device::DeviceBase*>& getDeviceVectorByType(QString strType) { return m_mapDeviceVector[strType]; }
        void setStationName(const QString& strName) { m_strStationName = strName; }
        QString getStationName() { return m_strStationName; }
        Device::DeviceBase* getDeviceByCode(const uint& nCode, QString StrType = "Device");
        Device::DeviceBase* getDeviceByName(const QString& strName, QString StrType = "Device");
        Device::DeviceBase* getSwitchBySectionCode(int nCode);
        QSize getStaFixedSize(); //获取站场实际大小
        void setStaFixedSize(int nWidth, int nHeight);
        uint getStationId() const { return m_nStationId; }
        void setStationId(uint nStationID)  { m_nStationId = nStationID; }
        QString getStationName() const { return m_strStationName; }
        QPoint getOffset() const { return m_ptOffset; }
        bool IsMainStation() const { return m_bMainStation; }
        QWidget* ShowWidget() const { return m_pShowWidget; }
        QMap<QString, QVector<int>> getTrackAdjSingal() { return m_mapTrackAdjSingalCode; }

    public:
        static void InitCreatDeviceMap();
 
    private:
        friend class MainStationObject;
        
    private:
        QMap<QString, QVector<int>> m_mapTrackAdjSingalCode;
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
        static int m_nUpState;
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
        int ReadInterLock(const QString& filePath);   //解析"InterlockTable.txt"
        void InterLockfileAnalysis(QString strLine);
        int ReadChartConfig(const QString& filePath); //解析"Chart_Conversion.json"

    public:
        int AddNewTrain(StaTrain* pStaTrain); //添加新车次
        int DeleteTrain(StaTrain* pStaTrain); //删除车次
        int SetTrainRunning(StaTrain* pStaTrain, bool bRunning); //设置车次启动或停稳
        int ChangeTrainNum(StaTrain* pStaTrain, const QString& strTrainNum); //修改车次号
        int ChangeTrainAttr(StaTrain* pStaTrain, int nSpeed, const QString& strLocomotive, bool bElectric); //修改车次信息
        int AddNewStagePlan(StaStagePlan* pStaStagePlan); //添加阶段计划
        int AddNewTrainRoute(StaTrainRoute* pTrainRoute); //添加进路序列
        void InitCombinativeRoute(); //初始化组合进路
        int AddNewDispatchOrder(StaDispatchOrder* pDispatchOrder); //添加调度命令
        int AddNewTrafficLog(StaTrafficLog* pTrafficLog); //添加行车日志
        void CreatTrainRouteByTrafficLog(StaTrafficLog* pTrafficLog);
        void TrainRouteTrigger(StaTrainRoute* pCurRoute, const QVector<StaTrainRoute*>& vecRouteList, const QString& strType);
        void TrainRouteTriggerChange(const QVector<StaTrainRoute*>& vecRouteList, bool bAutoTouch);
        bool TrainRouteTrackChange(const QVector<StaTrainRoute*>& vecRouteList, Device::DeviceBase* pTrack);
        bool TrainRouteSignalChange(const QVector<StaTrainRoute*>& vecRouteList, Device::DeviceBase* pSignal);
        bool DeleteTrainRoute(const QVector<StaTrainRoute*>& vecRouteList);
        QString getTriggerString(const QVector<StaTrainRoute*>& vecRouteList);

    public:
        StaTrain* getStaTrainById(int nTrainId);
        StaTrain* getStaTempTrainById(int nTrainId);
        StaTrainRoute* getStaTrainRouteById(int nRouteId);
        QVector<StaTrainRoute*>  getStaTrainRouteByTrain(int nTrainID);
        StaTrainRoute* getStaTrainRouteByRowIndex(int nRow);
        StaTrafficLog* getStaTrafficLogById(int nLogId);
        StaTrafficLog* getStaTrafficLogByTrain(int nTrainID);
        void RemoveTrain(StaTrain* pTrain);

    public:
        void SubmitCurSubject(); //提交当前题目
        void AddSelectDevice(Device::DeviceBase* pDevice); 
        bool IsAllowStaOperation(); //是否可操作
        bool IsAllowShuntTrainOnly(); //是否允许调车
        void CompareResult(const QByteArray& dataArray);

    public:
        void SendPacketMsg(int nTargetCode, int nOrderType = 0, int nAttr1 = -1, int nAttr2 = -1, QByteArray btAttr3 = QByteArray());
        void StationReset();
        void ClearDevice(bool bClearTwinkle = false);
        void PutThrough(int nTime);

    public:
        QVector<Device::DeviceBase*>& getSelectDevice() { return m_vecSelectDevice; }
        const UserInfo& getCurrUser() const { return m_infoCurrUser; }
        const QString& getCultivateOrder() const { return m_strOrderToInterLock; }
        void setUserId(int nUserId) { m_nUserId = nUserId; }
        int getUserId() const { return m_nUserId; }
        void setCurSubject(CultivateObject::Subject* pSubject);
        CultivateObject::Subject* getCurSubject() const { return m_pCurSubject; }
        bool IsOverturn() const { return m_bOverturn; }
        
        const QVector<StaTrain*>& TrainList() const { return m_vecStaTrain; }
        void AddTrain(StaTrain* pTrain) { m_vecStaTrain.append(pTrain); }
        void AddTempTrain(StaTrain* pTrain) { m_vecStaTempTrain.append(pTrain); }
        void RemoveTempTrain(StaTrain* pTrain) { m_vecStaTempTrain.removeOne(pTrain); }
        void RemoveTrainRoute(StaTrainRoute* pTrainRoute) { m_vecStaTrainRoute.removeOne(pTrainRoute); }
        const QVector<StaStagePlan*>& StagePlanList() const { return m_vecStaStagePlan; }
        const QVector<StaTrainRoute*>& TrainRouteList() const { return m_vecStaTrainRoute; }
        const QVector<StaDispatchOrder*>& DispatchOrderList() const { return m_vecStaDispatchOrder; }
        const QVector<StaTrafficLog*>& TrafficLogList() const { return m_vecStaTrafficLog; }
        void RemoveTrafficLog(StaTrafficLog* pTrafficLog) { m_vecStaTrafficLog.removeOne(pTrafficLog); }
        StaStagePlan* NewStagePlan() const { return m_pNewStagePlan; }
        void ClearNewStagePlan() { m_pNewStagePlan = nullptr; };
        StaDispatchOrder* NewDispatchOrder() const { return m_pNewDispatchOrder; }
        void ClearNewDispatchOrder() { m_pNewDispatchOrder = nullptr; };
        void setDiploid(DiploidOperate operate, int nType = STAVIEW); //站场大小缩放
        double getDiploid(DiploidRatio ratio) const { return m_mapDiploidRatio[ratio]; }
        QVector<Device::SignalBtn*> getSignalBtn() const { return m_vecSignalBtn; }

        QVector<StaTrafficLog*> getvecTrafficLog() const { return m_vecStaTrafficLog; }
        QMap<int, QString> getPassengeTrain() const { return m_mapPassengeTrain; }
        QMap<int, QString> getFreighTrain() const { return m_mapFreighTrain; }
        QMap<int, QString> getTrainType() const { return m_mapTrainType; }
        void AddPassengeTrain(int nIndex, QString strtype) { m_mapPassengeTrain.insert(nIndex, strtype); }
        void AddFreighTrain(int nIndex, QString strtype) { m_mapFreighTrain.insert(nIndex, strtype); }
        void AddTrainType(int nIndex, QString strtype) { m_mapTrainType.insert(nIndex, strtype); }
        void AddTrainRoute(StaTrainRoute* pRoute) { m_vecStaTrainRoute.append(pRoute); }
        bool getAutoSendPlan() const { return m_bAutoSendPlan; }
        TrainDiagramInfo getTrainDiagram() const{ return m_TrainDiagram; }

    public:
        struct StaLimits {
            int nRouteLimits = 0; //进路权限，0 - 联锁，1 - 在CTC，2 - 占线板
            bool bAutoSignStage = false;   //自动签收阶段计划
            bool bExStaControl = false;   //非常站控
            int nControlMode = 0;   //控制模式, 0 - 中心控制，1 - 车站控制，2 - 车站调车
            int nApplyControlMode = -1;   //申请控制模式, 0 - 中心控制，1 - 车站控制，2 - 车站调车
            bool nPlanMode = 0;  //计划模式，0 - 手工排路，1 - 按图排路
            bool nPlanControl = false;  //计划控制
            int nActiveApplyControlMode = -1;
        };

        int getStaLimits(Limits type);
        void setStaLimits(Limits type, int nValue);

    public slots:
        void onReciveData(const QByteArray& dataArray);
        void onOrderIssued();
        void onOrderClear(bool bClearTwinkle = false);
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
        TrainDiagramInfo m_TrainDiagram;

        QVector<Device::DeviceBase*> m_vecSelectDevice;

        QMap<int, QString> m_mapPassengeTrain;  //货车类型
        QMap<int, QString> m_mapFreighTrain;    //列车类型
        QMap<int, QString> m_mapTrainType;      //运行类型

        UserInfo m_infoCurrUser;
        QString m_strOrderToInterLock;
        int m_nUserId = 0;
        CultivateObject::Subject* m_pCurSubject = nullptr;
        bool m_bAutoSendPlan = true;
        QVector<StaTrain*> m_vecStaTrain;
        QVector<StaTrain*> m_vecStaTempTrain;
        QVector<StaStagePlan*> m_vecStaStagePlan;
        QVector<StaTrainRoute*> m_vecStaTrainRoute;
        QVector<StaDispatchOrder*> m_vecStaDispatchOrder;
        QVector<StaTrafficLog*> m_vecStaTrafficLog;

        QMap<Order, std::function<StaOrder*()>> m_mapCreatStationOrder;
        QMap<Order, std::function<void(void*, const QJsonObject&)>> m_mapStationOrder;
        QMap<DiploidRatio, double> m_mapDiploidRatio;
        QVector<Device::SignalBtn*> m_vecSignalBtn;

        StaStagePlan* m_pNewStagePlan = nullptr;
        StaDispatchOrder* m_pNewDispatchOrder = nullptr;
    };
}
