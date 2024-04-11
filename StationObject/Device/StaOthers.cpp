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
                    m_vecSectionsCode.append(strElement.toUInt());
                }
            });
        }

        StaSwitchSection::~StaSwitchSection()
        {

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
            DrawPermillSix();
            return DeviceBase::Draw(bElapsed, isMulti);
        }
        
        void StaPermillSix::DrawPermillSix()
        {
            m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
            m_pPainter.drawLine(p1, m_ptCenter);
            m_pPainter.drawLine(m_ptCenter, p2);
        }

        void StaPermillSix::setVollover(const QPoint& ptBase)
        {
        
        }

        void StaPermillSix::ResetDevState()
        {
        
        }
    }
}