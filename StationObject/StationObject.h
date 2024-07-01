#pragma once
#include <QXmlStreamReader>
#include "Device/DeviceBase.h"
#include "Transmission/StaProtocol.h"
#include "Transmission/StaPacket.h"
#include "../CultivateObject/Subject.h"
#include "GlobalStruct.h"

namespace Station {

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

    private:
        void timerEvent(QTimerEvent* event);

    public:     //外部初始化
        void InitDeviceEventFilter(QWidget* pWidget);
        int ReadStationInfo(const QString& filePath); //解析"station.xml"
        void InitStaDevice();   //站场设备初始化
        int ReadOtherConfig(const QString& filePath); //解析"OtherConfig.json"
        void SetVisible(VisibleDev devType, bool bVisible);
        bool IsVisible(VisibleDev devType);

    private:    //内部初始化
        void ReadDeviceInfoHead(); //解析"station.xml" HEAD
        void ReadDeviceInfo();  //解析"station.xml" DEVICE
        void InitDeviceMap();
        
    public:
        QVector<Device::DeviceBase*>& getDeviceVectorByType(QString strType) { return m_mapDeviceVector[strType]; }
        void SetDiploid(const int& n, double& nDiploid); //站场大小缩放
        void setStationName(const QString& strName) { m_strStationName = strName; }
        Device::DeviceBase* getDeviceByCode(const uint& nCode);
        Device::DeviceBase* getDeviceByName(const QString& strName);
        QSize getStaFixedSize(); //获取站场实际大小
        uint getStationId() { return m_nStationId; }
        QString getStationName() { return m_strStationName; }
        QPoint getOffset() { return m_ptOffset; }

    public:
        static void InitCreatDeviceMap();
        static double getDiploid(const int& type);

    private:
        friend class MainStationObject;
        
    private:
        uint m_nStationId = 0;
        QString m_strStationName;
        int m_nStaWidth = 0;
        int m_nStaHeight = 0;
        bool m_bOverturn = false;         //站场是否翻转
        QPoint m_ptOffset;       //站场图绘制偏移量
        QMap<QString, QVector<Device::DeviceBase*>> m_mapDeviceVector;
        QMap<VisibleDev, bool> m_mapVisible;

    private:
       
        static QXmlStreamReader* m_pDeviceInfoReader;  //XML解析器
        static QMap<QString, std::function<Device::DeviceBase* (StationObject*)>> m_mapCreatDeviceVector;
        static double m_nDiploid;          //站场图缩放系数
        static double m_nDiploidCheCi;     //车次缩放系数
        static double m_nDiploidMuti;      //站场图缩放系数-站间透明
        static double m_nDiploidCheCiMuti; //车次缩放系数-站间透明
        static int m_nTimerId_500;
    };

    
    class MainStationObject : public StationObject {
        Q_OBJECT
    public:
        explicit MainStationObject(QObject* pParent = nullptr);
        ~MainStationObject();

    public:
        void SelectStationOrder();    //查询命令信息(调度命令,阶段计划,进路序列,车辆信息,行车日志等)
        void SelectStaStagePlan();   //查询阶段计划
        void SelectStaTrain();   //查询车辆信息
        void SelectStaTrainRoute();   //查询进路序列
        void SelectStaDispatch();   //查询调度命令
        void SelectStaTrainDispatch(StaDispatchOrder* pOrder);   //查询机车调度命令
        void GetStationOrderByJosn(const QByteArray& btDataArray, Order type);

        int AddNewTrain(StaTrain* pStaTrain); //添加新车次
        int AddNewStagePlan(StaStagePlan* pStaStagePlan); //添加阶段计划
        int AddNewTrainRoute(StaTrainRoute* pTrainRoute); //添加进路序列
        int AddNewDispatchOrder(StaDispatchOrder* pDispatchOrder); //添加调度命令

        void SubmitCurSubject(); //提交当前题目
        void AddSelectDevice(Device::DeviceBase* pDevice); 
        bool IsAllowStaOperation(); //是否可操作

    public:
        QVector<Device::DeviceBase*>& getSelectDevice() { return m_vecSelectDevice; }
        const UserInfo& getCurrUser() { return m_infoCurrUser; }
        const QString& getCultivateOrder() { return m_strOrderToInterLock; }
        void setUserId(int nUserId) { m_nUserId = nUserId; }
        int getUserId() { return m_nUserId; }
        void setCurSubject(CultivateObject::Subject* pSubject);
        CultivateObject::Subject* getCurSubject() { return m_pCurSubject; }
        void setDevSelected(bool bSelected = true);
        bool getDevSelected() { return m_bDevSelected; }
        StaTrain* getStaTrainById(int nTrainId);
        QVector<StaTrainRoute*> TrainRouteList() { return m_vecStaTrainRoute; }
        QVector<StaDispatchOrder*> DispatchOrderList() { return m_vecStaDispatchOrder; }

    public slots:
        void onReciveData(const QByteArray& dataAyyay);
        void onOrderIssued();
        void onOrderClear();
        void onSelectDeviceClear();
        void onUserLogin(QString strUserName, QString strPassword);

    signals:
        void FunBtnStateReset();
        void SendDataToUDP(const QByteArray&);
        void SendDataToTCP(const QByteArray&);

    private:
        //数据传输
        Transmission::StaProtocol* m_pProtocol = nullptr;
        Transmission::StaPacket* m_pStaPacket = nullptr;

    private:
        bool m_bExStaControl = false;   //非常站控
        QVector<Device::DeviceBase*> m_vecSelectDevice;
        UserInfo m_infoCurrUser;
        QString m_strOrderToInterLock;
        int m_nUserId = 0;
        CultivateObject::Subject* m_pCurSubject = nullptr;
        bool m_bDevSelected = false;

        QVector<StaTrain*> m_vecStaTrain;
        QVector<StaStagePlan*> m_vecStaStagePlan;
        QVector<StaTrainRoute*> m_vecStaTrainRoute;
        QVector<StaDispatchOrder*> m_vecStaDispatchOrder;

        QMap<Order, std::function<StaOrder*()>> m_mapCreatStationOrder;
        QMap<Order, std::function<void(void*, const QJsonObject&)>> m_mapStationOrder;
    };
}
