#include "StaStateProtocol.h"
#include "Global.h"
#include"../Device/StaDevice.h"

namespace Station {
    namespace Protocol {
        
        StaStateProtocol::StaStateProtocol()
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
                Device::StaSwitch* pSwitch = nullptr;
                for (Device::DeviceBase* pDevice : m_pStationObject->m_mapDeviceVector[SWITCH]) {
                    pSwitch = dynamic_cast<Device::StaSwitch*>(pDevice);
                    pSwitch->setSwitchState(dataAyyay[nFlag] & 0x0f);
                    pSwitch->setState((dataAyyay[nFlag++] >> 4) & 0x0f);
                    pSwitch = nullptr;
                }
            }
            //区段
            {  
                bool bAddByte = false;
                Device::StaSwitchSection* pSection = nullptr;
                for (Device::DeviceBase* pDevice : m_pStationObject->m_mapDeviceVector[SECTION]) {
                    pSection = dynamic_cast<Device::StaSwitchSection*>(pDevice);
                    if (!bAddByte) {
                        for (Device::DeviceBase* pSubDevice : pSection->m_vecSections) {
                            if (pSubDevice->getType() == 1) {
                                pSubDevice->setState(dataAyyay[nFlag] & 0x0f);
                            }
                        }

                        if (pSection == m_pStationObject->m_mapDeviceVector[SECTION].at(m_pStationObject->m_mapDeviceVector[SECTION].size() - 1)) {
                            nFlag++;
                        }
                    }
                    else {
                        for (Device::DeviceBase* pDevice : pSection->m_vecSections) {
                            if (pDevice->getType() == 1) {
                                pDevice->setState((dataAyyay[nFlag++] >> 4) & 0x0f);
                            }
                        }
                    }
                    bAddByte = !bAddByte;
                }
            }
            //信号机
            {
                Device::StaSignal* pSignal = nullptr;
                int nFlagTemp = nFlag + m_pStationObject->m_mapDeviceVector[SIGNALLAMP].size(); //信号机按钮
                int nFlagTemp2 = nFlagTemp + m_pStationObject->m_mapDeviceVector[SIGNALLAMP].size(); //信号表示器
                bool bAddByte = false;
                bool bAddByte2 = false;
                for (Device::DeviceBase* pDevice : m_pStationObject->m_mapDeviceVector[SIGNALLAMP]) {
                    pSignal = dynamic_cast<Device::StaSignal*>(pDevice);
                    //信号机表示信息
                    pSignal->setState(dataAyyay[nFlag++] & 0xff);
                    //信号机闪光状态
                    if (!bAddByte) {
                        pSignal->setBtnState(dataAyyay[nFlagTemp] & 0x0f);
                        if (pSignal == m_pStationObject->m_mapDeviceVector[SIGNALLAMP].at(m_pStationObject->m_mapDeviceVector[SIGNALLAMP].size() - 1)) {
                            nFlagTemp++;
                        }
                    }

                    else {
                        pSignal->setBtnState((dataAyyay[nFlagTemp++] >> 4) & 0x0f);
                    }
                    bAddByte = !bAddByte;
                    //信号表示器
                    if (pSignal->IsIsHaveBSQ()) {
                        if (!bAddByte2) {
                            pSignal->setBSQState(dataAyyay[nFlagTemp2] & 0x0f);
                            if (pSignal == m_pStationObject->m_mapDeviceVector[SIGNALLAMP].at(m_pStationObject->m_mapDeviceVector[SIGNALLAMP].size() - 1)) {
                                nFlagTemp2++;
                            }
                        }
                        else {
                            pSignal->setBSQState((dataAyyay[nFlagTemp2++] >> 4) & 0x0f);
                        }
                        bAddByte2 = !bAddByte2;
                    }
                }
                nFlag = nFlagTemp2;
            }
            
            /*
            //通过按钮
            Device::StaButton* pButton = nullptr;
            for (Device::DeviceBase* pDevice : m_pStationObject->m_vecButton) {
                pButton = dynamic_cast<Device::StaButton*>(pDevice);
                if (!bAddByte) {
                    pButton->setState(dataAyyay[nFlag] & 0x0f);
                    if (pButton == m_pStationObject->m_vecButton.at(m_pStationObject->m_vecButton.size() - 1)) {
                        nFlag++;
                    }
                }
                else {
                    pButton->setState((dataAyyay[nFlag++] >> 4) & 0x0f);
                }
                bAddByte = !bAddByte;
            }
            
            //半自动闭塞
            Device::StaSemiAutoBlock* pSemiAutoBlock = nullptr;
            for (Device::DeviceBase* pDevice : m_pStationObject->m_vecSemiAutoBlock) {
                pSemiAutoBlock = dynamic_cast<Device::StaSemiAutoBlock*>(pDevice);
                pSemiAutoBlock->setArrowState(dataAyyay[nFlag++] & 0xff);
                pSemiAutoBlock->setState(dataAyyay[nFlag++] & 0xff);
            }
            
            //场联
            Device::StaConnection* pConnection = nullptr;
            for (Device::DeviceBase* pDevice : m_pStationObject->m_vecConnection) {
                pConnection = dynamic_cast<Device::StaConnection*>(pDevice);
                pConnection->setArrowState(dataAyyay[nFlag++] & 0xff);
                pConnection->setState(dataAyyay[nFlag++] & 0xff);
            }
            //站联
            //机务段
            Device::StaLocomotive* pLocomotive = nullptr;
            for (Device::DeviceBase* pDevice : m_pStationObject->m_vecLocomotive) {
                pLocomotive = dynamic_cast<Device::StaLocomotive*>(pDevice);
                if (!bAddByte) {
                    pLocomotive->setState(dataAyyay[nFlag] & 0x0f);
                    if (pLocomotive == m_pStationObject->m_vecLocomotive.at(m_pStationObject->m_vecLocomotive.size() - 1)) {
                        nFlag++;
                    }
                }
                else {
                    pLocomotive->setState((dataAyyay[nFlag++] >> 4) & 0x0f);
                }
                bAddByte = !bAddByte;
            }
            //电码化
            //自动闭塞
            Device::StaAutoBlock* pAutoBlock = nullptr;
            for (Device::DeviceBase* pDevice : m_pStationObject->m_vecAutoBlock) {
                pAutoBlock = dynamic_cast<Device::StaAutoBlock*>(pDevice);
                pAutoBlock->setArrowState(dataAyyay[nFlag] & 0x0f);
                pAutoBlock->setState((dataAyyay[nFlag] & 0xf0) + ((dataAyyay[nFlag + 1] & 0xff) << 8));
                pAutoBlock->setBtnState(dataAyyay[nFlag + 2]);
                pAutoBlock->setLeaveTrackState(dataAyyay[nFlag + 3]);
                nFlag += 4;
            }*/
        }
    }
}