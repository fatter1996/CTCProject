#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //�շ�
        class StaHump : public StaDistant, public DeviceBtn
        {
        public:
            explicit StaHump(QObject* pParent = nullptr);
            ~StaHump();

        private:
            //վ������
            void Draw(bool isMulti = false) override;
            //�����շ�
            void DrawHump();
            //�����źŵ�
            void DrawLight() override;
            //��������
            void DrawText() override;
            //�ж�����Ƿ����¼���Χ��
            bool Contains(const QPoint& ptPos) override;
            //����Ƿ����豸��
            bool IsMouseWheel(const QPoint& ptPos) override;
            //��ʼ���豸����¼�
            void InitClickEvent() override;
            //���ð�ť����
            void SetBtnState() override;
            //վ����ת
            void setVollover(const QPointF& ptBase) override;
            //״̬����
            void ResetDevState() override;

        private:
            QPointF p1, p2, p3, p4, p5, p6, p7, p8, p9; //��������
            QPointF m_ptCutOff; //�ж�����
            QPointF m_ptAllow; //��������
            QPointF m_ptTakeLook; //�ղ�
            QRectF m_rcButton;
            QRectF m_rcCutOff; //�ж�����
            QRectF m_rcAllow; //��������
            QRectF m_rcTakeLook; //�ղ�
            QRectF m_rcSignal;
        };
    }
}