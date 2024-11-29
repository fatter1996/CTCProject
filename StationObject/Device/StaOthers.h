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
    }
}