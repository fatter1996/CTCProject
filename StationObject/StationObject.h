#pragma once
#include <QXmlStreamReader>
#include "Device/DeviceBase.h"
#include "Protocol/StaStateProtocol.h"

namespace Station {
    class StationObject { 

    public:
        StationObject();
        ~StationObject();

    public:     //外部初始化
        void InitDeviceEventFilter(QWidget* pWidget);
        int ReadStationInfo(const QString& filePath); //解析"station.xml"
        void InitStaDevice();   //站场设备初始化

    private:    //内部初始化
        void ReadDeviceInfoHead(); //解析"station.xml" HEAD
        void ReadDeviceInfo();  //解析"station.xml" DEVICE

    public: 
        void SetDiploid(const int n, double& nDiploid); //站场大小缩放
        void setStationName(const QString& strName) { m_strStationName = strName; }

    public:
        void UnpackData(QByteArray& dataAyyay);

    public:
        Device::DeviceBase* getDeviceByCode(uint nCode); //根据Code获取道岔
        QSize getStaFixedSize(); //获取站场实际大小
        uint getStationId() { return m_nStationId; }
        QString getStationName() { return m_strStationName; }

    public:
        static bool IsAllowStaOperation();

    private:
        QMap<QString, std::function<Device::DeviceBase* ()>> m_mapCreatDeviceVector;

    private:
        //站场设备解析器
        QXmlStreamReader* m_pDeviceInfoReader = nullptr;
        Protocol::StaStateProtocol* m_pProtocol = nullptr;
       
    private:
        //站场设备数组
        QMap<QString, QVector<Device::DeviceBase*>> m_mapDeviceVector;

    private:
        uint m_nStationId = 0;
        QString m_strStationName;
        int m_nStaWidth = 0;
        int m_nStaHeight = 0;

    private:
        static bool m_bExStaControl;   //非常站控

    public:
        static double m_nDiploid;          //站场图缩放系数
        static double m_nDiploidCheCi;     //车次缩放系数
        static double m_nDiploidMuti;      //站场图缩放系数-站间透明
        static double m_nDiploidCheCiMuti; //车次缩放系数-站间透明
        static QPoint m_ptOffset;       //站场图绘制偏移量
        static bool m_bOverturn;         //站场是否翻转
    };
}
