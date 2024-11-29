#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //���Զ�����
        class StaAutoBlock : public StaDistant, public DeviceArrow, public DeviceBtn, public DeviceTrain
        {
        public:
            explicit StaAutoBlock(QObject* pParent = nullptr);
            ~StaAutoBlock();

        public:
            struct StaLeaveTrack
            {
                uint m_nIndex = 0;
                QString m_strName;
                QPointF m_ptName;
                QRectF m_rcTrack;
                uint m_nState = 0;
            };

            struct StaBlockLamp
            {
                QString m_strName;
                QRectF m_rcName;
                QRectF m_rcLamp;
                uint m_nState = 0;
            };

        private:
            //��ʼ���豸����
            void InitDeviceAttribute();
            //վ������
            void Draw(bool isMulti = false) override;
            //���ƽӽ�/��ȥ����
            void DrawLeaveTrack(const StaLeaveTrack& track) ;
            //�����źŵ�
            void DrawLight() override;
            //��������
            void DrawText() override;
            //����Ԥ�洰
            void DrawRoutePreviewWnd();
            //�ж�����Ƿ����¼���Χ��
            bool Contains(const QPoint& ptPos) override;
            //����Ƿ����豸��
            bool IsMouseWheel(const QPoint& ptPos) override;
            //�豸����ƶ��¼�
            void onMouseMoveToDevice(const QPoint& ptPos) override;
            //��ʼ���豸����¼�
            void InitClickEvent() override;
            //�Ҽ��˵�
            void ShowDeviceMenu(const QPoint& ptPos) override;
            //���ð�ť����
            void SetBtnState() override;
            //��ȡ�ӽ�������ɫ
            QColor getTrackColor(int nIndex);
            //��ȡ��ͷ��ɫ
            void getArrowColor() override;
            //�������
            void OrderClear() override;
            //վ����ת
            void setVollover(const QPointF& ptBase) override;
            //״̬����
            void ResetDevState() override;

        public:
            //���ýӽ�/��ȥ����״̬
            void setLeaveTrackState(int nState);

        private:
            QRectF m_rcZFZBtn;
            QRectF m_rcJCFZBtn;
            QRectF m_rcFCFZBtn;
            QRectF m_rcGFBtn;
            QRectF m_rcFZLight;

            QPointF m_ptZFZText;
            QPointF m_ptJCFZText;
            QPointF m_ptFCFZText;
            QPointF m_ptFZText;
            QPointF m_ptGFText;

            uint m_nLampNum = 0;
            bool m_bLeave = false;
            QPointF m_ptLampCenter;
            QPointF m_ptInterUsed;
            QVector<StaLeaveTrack> m_vecStaLeaveTrack;
            QString m_strAutoBlockType;

            //��ʱ��ʾ����
            QPointF m_ptZFZ_JS;
            QPointF m_ptJCFZ_JS;
            QPointF m_ptFCFZ_JS;

            uint m_nSelectBtnType = 0x0;  //ѡ������(0-δѡ��; 0x01-�ܸ���; 0x02-�ӳ�����; 0x04-��������)
            QString m_strDirection;
            QMap<QString, StaBlockLamp> m_mapBlockLamp;
            QPointF m_ptRouteWnd;
        };
    }
}