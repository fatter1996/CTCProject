#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //����
        class StaSwitchSection : public DeviceBase {
            Q_OBJECT
        public:
            explicit StaSwitchSection(QObject* pParent = nullptr);
            ~StaSwitchSection();

        private:
            void InitAttributeMap() override;
            //վ����ת
            void setVollover(const QPointF& ptBase) override;

        private:
            uint m_nChildNum = 0;

        public:
            QVector<int> m_vecSectionsCode;
        };


        //ǧ��֮����ʶ
        class StaPermillSix : public DeviceBase {
            Q_OBJECT
        public:
            explicit StaPermillSix(QObject* pParent = nullptr);
            ~StaPermillSix();

        private:
            void InitAttributeMap() override;
            //վ������
            void Draw(bool isMulti = false) override;
            //����ǧ��֮����ʶ
            void DrawPermillSix();

            //վ����ת
            void setVollover(const QPointF& ptBase) override;

        private:
            QPointF p1, p2;
        };


        //������
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
            //վ������
            void Draw(bool isMulti = false) override;
            //���Ʒ�����
            void DrawNeutral();
            //վ����ת
            void setVollover(const QPointF& ptBase) override;

        private:
            QVector<NeutralLine*> m_vecNeutralLine;
            QVector<NeutralText*> m_vecNeutralText;
        };


        //��־
        class StaMark : public DeviceBase {
            Q_OBJECT
        public:
            explicit StaMark(QObject* pParent = nullptr);
            ~StaMark();

        private:
            void InitAttributeMap() override;
            //վ������
            void Draw(bool isMulti = false) override;
            //���Ʒ�����
            void DrawMark();
            //վ����ת
            void setVollover(const QPointF& ptBase) override;

        private:
            QColor m_cColor;
            QRectF m_rcMark;
        };
    }
}