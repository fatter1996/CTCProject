#pragma once
#include <QXmlStreamReader>
#include "Device/DeviceBase.h"
#include "Transmission/StaProtocol.h"
#include "Transmission/StaPacket.h"

namespace Station {

    struct UserInfo //用户信息
    {
        QString m_strName;
        QString m_strPassWord;
    };

    class StationObject : public QObject { 
        Q_OBJECT
    public:
        explicit StationObject(QObject* parent = nullptr);
        ~StationObject();

    private:
        void timerEvent(QTimerEvent* event);

    public:     //外部初始化
        void InitDeviceEventFilter(QWidget* pWidget);
        int ReadStationInfo(const QString& filePath); //解析"station.xml"
        void InitStaDevice();   //站场设备初始化

    private:    //内部初始化
        void ReadDeviceInfoHead(); //解析"station.xml" HEAD
        void ReadDeviceInfo();  //解析"station.xml" DEVICE
        void InitDeviceMap();

    public: 
        void SetDiploid(const int& n, double& nDiploid); //站场大小缩放
        void setStationName(const QString& strName) { m_strStationName = strName; }

    public slots:
        void onReciveData(const QByteArray& dataAyyay);
        void onOrderIssued();
        void onOrderClear();
        void UserLogin(QString strUserName, QString strPassword);

    signals:
        void SendDataToUDP(const QByteArray&);
        void SendDataToTCP(const QByteArray&);

    public:
        Device::DeviceBase* getDeviceByCode(const uint& nCode); //根据Code获取道岔
        QSize getStaFixedSize(); //获取站场实际大小
        uint getStationId() { return m_nStationId; }
        QString getStationName() { return m_strStationName; }

    public:
        static bool IsAllowStaOperation();
        static double getDiploid(const int& type);
        static QPoint getOffset();
        static void AddSelectDevice(Device::DeviceBase* pDevice);
        static QVector<Device::DeviceBase*>& getSelectDevice();
        static const UserInfo& getCurrUser();
        
    private:
        //站场设备数组
        QMap<QString, QVector<Device::DeviceBase*>> m_mapDeviceVector;
        QMap<QString, std::function<Device::DeviceBase* ()>> m_mapCreatDeviceVector;

    private:
        //站场设备解析
        QXmlStreamReader* m_pDeviceInfoReader = nullptr;
        //数据传输
        Transmission::StaProtocol* m_pProtocol = nullptr;
        Transmission::StaPacket* m_pStaPacket = nullptr;

    private:
        uint m_nStationId = 0;
        QString m_strStationName;
        int m_nStaWidth = 0;
        int m_nStaHeight = 0;
        bool m_bOverturn = false;         //站场是否翻转
        int m_nTimerId_500 = -1;

    private:
        static bool m_bExStaControl;   //非常站控
        static double m_nDiploid;          //站场图缩放系数
        static double m_nDiploidCheCi;     //车次缩放系数
        static double m_nDiploidMuti;      //站场图缩放系数-站间透明
        static double m_nDiploidCheCiMuti; //车次缩放系数-站间透明
        static QPoint m_ptOffset;       //站场图绘制偏移量
        static QVector<Device::DeviceBase*> m_vecSelectDevice;
        static UserInfo m_infoCurrUser;
    };
}
