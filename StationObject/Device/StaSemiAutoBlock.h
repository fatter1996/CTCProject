#pragma once
#include "DeviceBase.h"
#include "DeviceTrain.h"

namespace Station {
    namespace Device {
        //���Զ�����
        class StaSemiAutoBlock : public StaDistant, public DeviceArrow, public DeviceBtn, public StaRoutePreview
        {
        public:
            explicit StaSemiAutoBlock(QObject* pParent = nullptr);
            ~StaSemiAutoBlock();

        private:
            void InitAttributeMap() override;
            void InitDeviceAttribute() override;
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
            //�Ҽ��˵�
            void ShowDeviceMenu(const QPoint& ptPos) override;
            //���ð�ť����
            void SetBtnState() override;
            //��ȡ��ͷ��ɫ
            void getArrowColor() override;
            //�������
            void OrderClear(bool bClearTwinkle = false) override;
            //վ����ת
            void setVollover(const QPointF& ptBase) override;
            //״̬����
            void ResetDevState() override;

            void AddBlockBtn(QString strType, const QString& strElement);

        private:
            QVector<StaBlockBtn> m_vecBlockBtn;
            uint m_nBlockType = 0;
            uint m_nSelectBtnType = 0x0;  //ѡ������(0-δѡ��; 0x01-����; 0x02-��ԭ; 0x04-�¹�)
        };
    }
}