#include "StaEndLine.h"
#include "Global.h"

namespace Station {
    namespace Device {
        StaEndLine::StaEndLine(QObject* pParent)
            : DeviceBase(pParent)
        {
            
        }

        StaEndLine::~StaEndLine()
        {

        }

        void StaEndLine::InitAttributeMap()
        {
            if (m_mapAttribute.contains(m_strType)) {
                return;
            }
            AttrMap mapAttrFun;
            m_mapAttribute.insert(m_strType, mapAttrFun);
            m_mapAttribute[m_strType].insert("p1", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaEndLine*>(pDevice)->p1 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p2", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaEndLine*>(pDevice)->p2 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p3", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaEndLine*>(pDevice)->p3 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p4", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaEndLine*>(pDevice)->p4 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p5", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaEndLine*>(pDevice)->p5 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p6", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaEndLine*>(pDevice)->p6 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p7", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaEndLine*>(pDevice)->p7 = QStringToQPointF(strElement); });
            return DeviceBase::InitAttributeMap();
        }

        void StaEndLine::InitDeviceAttribute()
        {
            if (p5.isNull()) {
                if (m_nType == 41) {
                    p5 = QPointF(p4.x() - 5, p4.y());
                }
                else if (m_nType == 42) {
                    p5 = QPointF(p4.x() + 5, p4.y());
                }
            }
            if (p6.isNull()) {
                p6 = QPointF(p3.x(), p3.y() - 5);
            }
            if (p7.isNull()) {
                p7 = QPointF(p3.x(), p3.y() + 5);
            }
        }

        void StaEndLine::Draw(bool isMulti)
        {
            if (MainStation()->IsVisible(VisibleDev::endMarker)) {
                DrawEndLine();
            }
            return DeviceBase::Draw(isMulti);
        }

        void StaEndLine::DrawEndLine()
        {
            m_pPainter.setPen(QPen(COLOR_TRACK_WHITE, 2));
            m_pPainter.drawLine(Scale(p1), Scale(p6));
            m_pPainter.drawLine(Scale(p2), Scale(p7));
            m_pPainter.drawLine(Scale(p6), Scale(p7));
            m_pPainter.drawLine(Scale(p4), Scale(p5));
            m_pPainter.drawLine(Scale(p3), Scale(QPointF(p4.x(), p3.y())));
        }

        void StaEndLine::setVollover(const QPointF& ptBase)
        {

        }

        void StaEndLine::ResetDevState()
        {

        }
    }
}