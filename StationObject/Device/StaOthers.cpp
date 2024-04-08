#include "StaOthers.h"
#include "StaSwitch.h"
#include "Global.h"

namespace Station {
    namespace Device {
        StaSwitchSection::StaSwitchSection(QObject* parent)
        {
            m_mapAttribute.insert("m_nChildNum", [&](const QString& strElement) { m_nChildNum = strElement.toUInt(); });
            m_mapAttribute.insert("m_nChild", [&](const QString& strElement) {
                if (nIndex < m_nChildNum) {
                    m_vecSections.append(m_pStationObject->getDeviceByCode(strElement.toUInt()));
                }
                });
        }

        StaSwitchSection::~StaSwitchSection()
        {

        }

        void StaSwitchSection::ClearStateInSection(const DeviceBase* pMainDevice, DeviceBase* pDevice)
        {
            StaSwitch* pSwitch = dynamic_cast<StaSwitch*>(pDevice);
            pSwitch->setState(0);

            StaSwitch* pNextSwitch = dynamic_cast<StaSwitch*>(pDevice);
            if (dynamic_cast<StaSwitch*>(pDevice)->getSwitchState() & SWITCH_STATE_DW) {
                pNextSwitch = dynamic_cast<StaSwitch*>(m_pStationObject->getDeviceByCode(pSwitch->getDWSwitchCode()));
            }
            else if (dynamic_cast<StaSwitch*>(pDevice)->getSwitchState() & SWITCH_STATE_FW) {
                pNextSwitch = dynamic_cast<StaSwitch*>(m_pStationObject->getDeviceByCode(pSwitch->getFWSwitchCode()));
            }

            if (pNextSwitch && pNextSwitch == pMainDevice) {
                pNextSwitch = dynamic_cast<StaSwitch*>(m_pStationObject->getDeviceByCode(pSwitch->getCQSwitchCode()));
            }

            if (pNextSwitch) {
                pNextSwitch->setState(0);
                ClearStateInSection(pSwitch, pNextSwitch);
            }
        }

        void StaSwitchSection::setVollover(const QPoint& ptBase)
        {

        }

        void StaSwitchSection::ResetDevState()
        {

        }



        StaPermillSix::StaPermillSix(QObject* parent)
        {
            m_mapAttribute.insert("p1", [&](const QString& strElement) { p1 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p2", [&](const QString& strElement) { p2 = QStringToQPoint(strElement); });
        }

        StaPermillSix::~StaPermillSix()
        {
        
        }

        void StaPermillSix::Draw(const bool& bElapsed, const bool& isMulti)
        {
            if (!m_pPainter)
                return;

            DrawPermillSix();

            return DeviceBase::Draw(bElapsed, isMulti);
        }
        
        void StaPermillSix::DrawPermillSix()
        {
            m_pPainter->setPen(QPen(COLOR_TRACK_BLUE, 2));
            m_pPainter->drawLine(p1, m_ptCenter);
            m_pPainter->drawLine(m_ptCenter, p2);
        }

        void StaPermillSix::setVollover(const QPoint& ptBase)
        {
        
        }

        void StaPermillSix::ResetDevState()
        {
        
        }
    }
}