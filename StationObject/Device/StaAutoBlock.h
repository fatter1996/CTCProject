#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //半自动闭塞
        class StaAutoBlock : public StaDistant, public DeviceArrow, public DeviceBtn
        {
        public:
            explicit StaAutoBlock(QObject* pParent = nullptr);
            ~StaAutoBlock();

        public:
            struct StaLeaveTrack
            {
                uint m_nIndex = 0;
                QString m_strName;
                QPoint m_ptName;
                QRect m_rcTrack;
                uint m_nState = 0;
            };

        private:
            //初始化设备属性
            void InitDeviceAttribute();
            //站场绘制
            void Draw(const bool& isMulti = false);
            //绘制接近/离去区段
            void DrawLeaveTrack(const StaLeaveTrack& track);
            //绘制信号灯
            void DrawLight();
            //绘制文字
            void DrawText();
            //判断鼠标是否在事件范围内
            bool Contains(const QPoint& ptPos);
            //鼠标是否在设备上
            bool IsMouseWheel(const QPoint& ptPos);
            //初始化设备点击事件
            void InitClickEvent();
            //设置按钮属性
            void SetBtnState();
            //获取接近区段颜色
            QColor getTrackColor(int nIndex);
            //获取箭头颜色
            void getArrowColor();
            //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();

        public:
            //设置接近/离去区段状态
            void setLeaveTrackState(int nState);

        private:
            QRect m_rcZFZBtn;
            QRect m_rcJCFZBtn;
            QRect m_rcFCFZBtn;
            QRect m_rcFZLight;

            QPoint m_ptZFZText;
            QPoint m_ptJCFZText;
            QPoint m_ptFCFZText;
            QPoint m_ptFZText;

            uint m_nLampNum = 0;
            bool m_bLeave = false;
            QPoint m_ptLampCenter;
            QPoint m_ptInterUsed;
            QVector<StaLeaveTrack> m_vecStaLeaveTrack;
            QString m_strAutoBlockType;

            //计时显示坐标
            QPoint m_ptZFZ_JS;
            QPoint m_ptJCFZ_JS;
            QPoint m_ptFCFZ_JS;

            uint m_nSelectBtnType = 0x0;  //选中类型(0-未选中; 0x01-总辅助; 0x02-接车辅助; 0x04-发车辅助)
        };
    }
}