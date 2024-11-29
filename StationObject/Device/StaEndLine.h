#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //�෢��
        class StaEndLine : public DeviceBase
        {
        public:
            explicit StaEndLine(QObject* pParent = nullptr);
            ~StaEndLine();

        private:
            //վ������
            void Draw(bool isMulti = false) override;

            //�����źŵ�
            void DrawEndLine();
            
            //վ����ת
            void setVollover(const QPointF& ptBase) override;
            //״̬����
            void ResetDevState() override;

        private:
            QPointF p1, p2, p3, p4, p5, p6, p7; //��������
        };
    }
}