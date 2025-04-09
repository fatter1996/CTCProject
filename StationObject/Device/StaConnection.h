#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //����
        class StaConnection : public StaDistant, public DeviceArrow, public DeviceBtn
        {
        public:
            explicit StaConnection(QObject* pParent = nullptr);
            ~StaConnection();

        private:
            void InitAttributeMap() override;
            //վ������
            void Draw(bool isMulti = false) override;
            //�����źŵ�
            void DrawLight() override;
            //��������
            void DrawText() override;
            //��ȡ��ͷ��ɫ
            void getArrowColor() override;
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
            int m_nFlag = 0;
            
            QPointF m_ptSignal;      //�ź�
            QPointF m_ptNear;        //�ӽ�
            QPointF m_ptTakeLook;    //�ղ�    
            QPointF m_ptAllow;       //����
            QPointF m_ptFileld;      //����
            QPointF m_ptAdjFileld;   //�ڳ�

            QRectF m_rcNearLight;
            QRectF m_rcFileldLight;
            QRectF m_rcAdjFileldLight;
            QRectF m_rcAllowBtn;

            QString m_strCLType;
        };
    }
}