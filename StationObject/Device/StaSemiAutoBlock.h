#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //���Զ�����
        class StaSemiAutoBlock : public StaDistant, public DeviceArrow, public DeviceBtn
        {
        public:
            explicit StaSemiAutoBlock(QObject* pParent = nullptr);
            ~StaSemiAutoBlock();

        private:
            //վ������
            void Draw(bool isMulti = false) override;
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
            //��ȡ��ͷ��ɫ
            void getArrowColor() override;
            //�������
            void OrderClear(int nType = 0) override;
            //վ����ת
            void setVollover(const QPointF& ptBase) override;
            //״̬����
            void ResetDevState() override;

        private:
            QPointF m_ptBSText;
            QPointF m_ptFYText;
            QPointF m_ptSGText;

            QRectF m_rcBSBtn;
            QRectF m_rcFYBtn;
            QRectF m_rcSGBtn;

            uint m_nBlockType = 0;
            uint m_nSelectBtnType = 0x0;  //ѡ������(0-δѡ��; 0x01-����; 0x02-��ԭ; 0x04-�¹�)
        };
    }
}