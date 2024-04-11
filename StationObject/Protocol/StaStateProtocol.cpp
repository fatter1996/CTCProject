#include "StaStateProtocol.h"
#include "Global.h"
#include "../Device/StaDevice.h"
#include <QDebug>
namespace Station {
    namespace Protocol {
        using namespace Device;

        StaStateProtocol::StaStateProtocol(QMap<QString, QVector<DeviceBase*>>& mapDeviceVector) 
            : m_mapDeviceVector(mapDeviceVector)
        {
        
        }

        StaStateProtocol::~StaStateProtocol()
        {
        
        }

        void StaStateProtocol::UnpackStaViewState(QByteArray& dataAyyay)
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
            
            /*
            //通过按钮
            StaButton* pButton = nullptr;
            for (DeviceBase* pDevice : StationObject::m_vecButton) {
                pButton = dynamic_cast<StaButton*>(pDevice);
                if (!bAddByte) {
                    pButton->setState(dataAyyay[nFlag] & 0x0f);
                    if (pButton == StationObject::m_vecButton.at(StationObject::m_vecButton.size() - 1)) {
                        nFlag++;
                    }
                }
                else {
                    pButton->setState((dataAyyay[nFlag++] >> 4) & 0x0f);
                }
                bAddByte = !bAddByte;
            }
            
            
            
            
            //站联
            
            //电码化
            //自动闭塞
            StaAutoBlock* pAutoBlock = nullptr;
            for (DeviceBase* pDevice : StationObject::m_vecAutoBlock) {
                pAutoBlock = dynamic_cast<StaAutoBlock*>(pDevice);
                pAutoBlock->setArrowState(dataAyyay[nFlag] & 0x0f);
                pAutoBlock->setState((dataAyyay[nFlag] & 0xf0) + ((dataAyyay[nFlag + 1] & 0xff) << 8));
                pAutoBlock->setBtnState(dataAyyay[nFlag + 2]);
                pAutoBlock->setLeaveTrackState(dataAyyay[nFlag + 3]);
                nFlag += 4;
            }*/
        }

        DeviceBase* StaStateProtocol::getDeviceByCode(uint nCode)
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