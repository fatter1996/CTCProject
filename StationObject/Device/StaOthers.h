#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //区段
        class StaSwitchSection : public DeviceBase {
            Q_OBJECT
        public:
            explicit StaSwitchSection(QObject* pParent = nullptr);
            ~StaSwitchSection();

        private:
            void InitAttributeMap() override;
            //站场翻转
            void setVollover(const QPointF& ptBase) override;

        private:
            uint m_nChildNum = 0;

        public:
            QVector<int> m_vecSectionsCode;
        };


        //千分之六标识
        class StaPermillSix : public DeviceBase {
            Q_OBJECT
        public:
            explicit StaPermillSix(QObject* pParent = nullptr);
            ~StaPermillSix();

        private:
            void InitAttributeMap() override;
            //站场绘制
            void Draw(bool isMulti = false) override;
            //绘制千分之六标识
            void DrawPermillSix();

            //站场翻转
            void setVollover(const QPointF& ptBase) override;

        private:
            QPointF p1, p2;
        };


        //分相区
        class StaNeutral : public DeviceBase {
            Q_OBJECT
        public:
            explicit StaNeutral(QObject* pParent = nullptr);
            ~StaNeutral();

        public:
            struct NeutralLine
            {
                QPointF ptStart;
                QPointF ptEnd;
            };
            struct NeutralText
            {
                QString strContent;
                QRectF rcText;
                QColor cTextColor;
            };

        private:
            void InitAttributeMap() override;
            //站场绘制
            void Draw(bool isMulti = false) override;
            //绘制分相区
            void DrawNeutral();
            //站场翻转
            void setVollover(const QPointF& ptBase) override;

        private:
            QVector<NeutralLine*> m_vecNeutralLine;
            QVector<NeutralText*> m_vecNeutralText;
        };


        //标志
        class StaMark : public DeviceBase {
            Q_OBJECT
        public:
            explicit StaMark(QObject* pParent = nullptr);
            ~StaMark();

        private:
            void InitAttributeMap() override;
            //站场绘制
            void Draw(bool isMulti = false) override;
            //绘制分相区
            void DrawMark();
            //站场翻转
            void setVollover(const QPointF& ptBase) override;

        private:
            QColor m_cColor;
            QRectF m_rcMark;
        };
    }
}