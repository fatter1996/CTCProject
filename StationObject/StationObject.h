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
        

    public: //获取站场设备
        Device::DeviceBase* getDeviceByCode(uint nCode); //根据Code获取道岔

    public: //站场大小
        void SetDiploid(const int n, double& nDiploid); //站场大小缩放
        QSize getStaFixedSize(); //获取站场实际大小

    public:
        void setStationName(const QString& strName) { m_strStationName = strName; }
        uint StationID() { return m_nStationId; }
        QString StationName() { return m_strStationName; }

    public:
        void UnpackDataPacket(QByteArray& dataAyyay);

    public:
        //站场设备数组
        QMap<QString, QVector<Device::DeviceBase*>> m_mapDeviceVector;

    public:
        double m_nDiploid = 1.0;          //站场图缩放系数
        double m_nDiploidCheCi = 1.0;     //车次缩放系数
        double m_nDiploidMuti = 1.0;      //站场图缩放系数-站间透明
        double m_nDiploidCheCiMuti = 1.0; //车次缩放系数-站间透明
        QPoint m_ptOffset = { 0,0 };       //站场图绘制偏移量
        bool m_bOverturn = false;         //站场是否翻转

    private:
        QMap<QString, std::function<Device::DeviceBase* ()>> m_mapCreatDeviceVector;

    private:
        //站场设备解析器
        QXmlStreamReader* m_pDeviceInfoReader = nullptr;
        Protocol::StaStateProtocol* m_pProtocol = nullptr;

    private:
        uint m_nStationId = 0;
        QString m_strStationName;
        int m_nStaWidth = 0;
        int m_nStaHeight = 0;
        uint m_nWorkType = 0;    //工作状态(主用-0x5A; 备用-0xA5; 预备-0x55; 其它-0x0)
    };
}
