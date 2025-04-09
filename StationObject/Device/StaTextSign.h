#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //文字标注
        class StaTextSign : public DeviceBase
        {
        public:
            explicit StaTextSign(QObject* pParent = nullptr);
            ~StaTextSign();

        public:
            void InitAttributeMap() override;
            //站场绘制
            void Draw(bool isMulti = false) override;
            void InitTextSign(const QString& strText, const QPoint& ptPos, const QColor& colFont = Qt::black, const QColor& colBackground = Qt::white, int nSize = 10);
        
        private:
            void DrawTextSign();
        
        private:
            bool m_bIsShow = true;
            QString m_strText;
            QPoint m_ptShowPos;
            QColor m_colText;
            QColor m_colBackground;
            QRect m_rcBackground;
        };
    }
}
