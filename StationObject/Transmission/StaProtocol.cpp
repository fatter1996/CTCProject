#include "StaProtocol.h"
#include "Global.h"
#include "../Device/StaDevice.h"
#include <QDebug>

namespace Station {
    namespace Transmission {
        using namespace Device;

        StaProtocol::StaProtocol(QMap<QString, QVector<DeviceBase*>>& mapDeviceVector)
            : m_mapDeviceVector(mapDeviceVector)
        {
            m_mapUnPackOrder.insert(0x40, [=](const QByteArray& dataAyyay) { return UnpackStaViewState(dataAyyay); });
            m_mapUnPackOrder.insert(0x80, [=](const QByteArray& dataAyyay) { return UnpackLogin(dataAyyay); });
            m_mapUnPackOrder.insert(0x81, [=](const QByteArray& dataAyyay) { return UnpackCultivate(dataAyyay); });
        }

        StaProtocol::~StaProtocol()
        {
        
        }

        QByteArray StaProtocol::UnpackData(const QByteArray& dataAyyay)
        {
            return m_mapUnPackOrder[dataAyyay[10] & 0xFF](dataAyyay);
        }

        QByteArray StaProtocol::UnpackLogin(const QByteArray& dataAyyay)
        {
            return "";
        }

        QByteArray StaProtocol::UnpackStaViewState(const QByteArray& dataAyyay)
        {
            int nFlag = 3 + 4;
            //道岔
            {
                StaSwitch* pSwitch = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[SWITCH]) {
                    pSwitch = dynamic_cast<StaSwitch*>(pDevice);
                    pSwitch->setSwitchState(dataAyyay[nFlag] & 0x0f);
                    pSwitch->setState((dataAyyay[nFlag++] >> 4) & 0x0f);
                    pSwitch = nullptr;
                }
            }
            //区段
            {  
                bool bAddByte = false;
                StaSwitchSection* pSection = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[SECTION]) {
                    pSection = dynamic_cast<StaSwitchSection*>(pDevice);

                    DeviceBase* pSubDevice = nullptr;
                    for (int nSubDeviceCode : pSection->m_vecSectionsCode) {
                        pSubDevice = getDeviceByCode(nSubDeviceCode);
                        if (pSubDevice->getType() == 1) {
                        }
                    }

                    if (!bAddByte) {
                        pSubDevice->setState(dataAyyay[nFlag] & 0x0f);

                        if (pSection == m_mapDeviceVector[SECTION].at(m_mapDeviceVector[SECTION].size() - 1)) {
                            nFlag++;
                        }
                    }
                    else {
                        pSubDevice->setState((dataAyyay[nFlag] >> 4) & 0x0f);
                        nFlag++;
                    }
                    bAddByte = !bAddByte;
                }
            }
            //信号机
            {
                StaSignal* pSignal = nullptr;
                int nFlagTemp = nFlag + m_mapDeviceVector[SIGNALLAMP].size(); //信号机按钮
                int nFlagTemp2 = nFlagTemp + m_mapDeviceVector[SIGNALLAMP].size(); //信号表示器
                bool bAddByte = false;
                bool bAddByte2 = false;
                for (DeviceBase* pDevice : m_mapDeviceVector[SIGNALLAMP]) {
                    pSignal = dynamic_cast<StaSignal*>(pDevice);
                    //信号机表示信息
                    if(dataAyyay[nFlag] & 0xc0)
                        qDebug() << pSignal->getName() << (dataAyyay[nFlag] & 0xff);
                    pSignal->setState(dataAyyay[nFlag++] & 0xff);
                }
            }
            //半自动闭塞
            {
                StaSemiAutoBlock* pSemiAutoBlock = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[SEMIAUTOBLOCK]) {
                    pSemiAutoBlock = dynamic_cast<StaSemiAutoBlock*>(pDevice);

                    pSemiAutoBlock->setArrowState(dataAyyay[nFlag++] & 0xff);
                    //pSemiAutoBlock->setState(dataAyyay[nFlag++] & 0xff);
                }
            }
            //场联
            {
                StaConnection* pConnection = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[CONNECTION]) {
                    pConnection = dynamic_cast<StaConnection*>(pDevice);
                    qDebug() << pConnection->getName() << (dataAyyay[nFlag] & 0xff);
                    pConnection->setArrowState(dataAyyay[nFlag] & 0x33);
                    pConnection->setState(dataAyyay[nFlag++] & 0xcc);
                }
            }
            //机务段
            {
                bool bAddByte = false;
                StaLocomotive* pLocomotive = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[LOCOMOTIVE]) {
                    pLocomotive = dynamic_cast<StaLocomotive*>(pDevice);
                    if (!bAddByte) {
                        pLocomotive->setState(dataAyyay[nFlag] & 0x0f);
                        if (pLocomotive == m_mapDeviceVector[LOCOMOTIVE].at(m_mapDeviceVector[LOCOMOTIVE].size() - 1)) {
                            nFlag++;
                        }
                    }
                    else {
                        pLocomotive->setState((dataAyyay[nFlag++] >> 4) & 0x0f);
                    }
                    bAddByte = !bAddByte;
                }
            }
            //自动闭塞
            {
                StaAutoBlock* pAutoBlock = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[AUTOBLOCK]) {
                    pAutoBlock = dynamic_cast<StaAutoBlock*>(pDevice);
                    pAutoBlock->setArrowState(dataAyyay[nFlag] & 0x0f);
                    pAutoBlock->setState(dataAyyay[nFlag] & 0xf0);
                    pAutoBlock->setLeaveTrackState(dataAyyay[nFlag + 1]);
                    nFlag += 2;
                }
            }
            return "";
        }

        QByteArray StaProtocol::UnpackCultivate(const QByteArray& dataAyyay)
        {
            int length = dataAyyay[11] & 0xFF;
            QString strMsg = QString(dataAyyay.mid(12, length));
            QStringList strOrders = strMsg.split("@");
            QByteArray byteToInterLock;
            for (QString strOrder : strOrders) {
                QStringList subList = strOrder.split("-");
                if (subList[0] == "QDZY") {
                    byteToInterLock = subList[0].toLocal8Bit();
                }
                else if (subList[0] == "JLBL") {
                    QStringList devList = subList[1].split(",");
                }
            }
            return byteToInterLock;
        }

        DeviceBase* StaProtocol::getDeviceByCode(uint nCode)
        {
            for (DeviceBase* pDevice : m_mapDeviceVector[ALLDEVICE]) {
                if (pDevice->getCode() == nCode) {
                    return pDevice;
                }
            }
            return nullptr;
        }
    }
}