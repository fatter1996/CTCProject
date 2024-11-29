#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //半自动闭塞
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
            //初始化设备属性
            void InitDeviceAttribute();
            //站场绘制
            void Draw(bool isMulti = false) override;
            //绘制接近/离去区段
            void DrawLeaveTrack(const StaLeaveTrack& track) ;
            //绘制信号灯
            void DrawLight() override;
            //绘制文字
            void DrawText() override;
            //车次预告窗
            void DrawRoutePreviewWnd();
            //判断鼠标是否在事件范围内
            bool Contains(const QPoint& ptPos) override;
            //鼠标是否在设备上
            bool IsMouseWheel(const QPoint& ptPos) override;
            //设备鼠标移动事件
            void onMouseMoveToDevice(const QPoint& ptPos) override;
            //初始化设备点击事件
            void InitClickEvent() override;
            //右键菜单
            void ShowDeviceMenu(const QPoint& ptPos) override;
            //设置按钮属性
            void SetBtnState() override;
            //获取接近区段颜色
            QColor getTrackColor(int nIndex);
            //获取箭头颜色
            void getArrowColor() override;
            //命令清除
            void OrderClear() override;
            //站场翻转
            void setVollover(const QPointF& ptBase) override;
            //状态重置
            void ResetDevState() override;

        public:
            //设置接近/离去区段状态
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

            //计时显示坐标
            QPointF m_ptZFZ_JS;
            QPointF m_ptJCFZ_JS;
            QPointF m_ptFCFZ_JS;

            uint m_nSelectBtnType = 0x0;  //选中类型(0-未选中; 0x01-总辅助; 0x02-接车辅助; 0x04-发车辅助)
            QString m_strDirection;
            QMap<QString, StaBlockLamp> m_mapBlockLamp;
            QPointF m_ptRouteWnd;
        };
    }
}