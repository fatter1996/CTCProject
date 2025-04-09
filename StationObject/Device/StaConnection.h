#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //场联
        class StaConnection : public StaDistant, public DeviceArrow, public DeviceBtn
        {
        public:
            explicit StaConnection(QObject* pParent = nullptr);
            ~StaConnection();

        private:
            void InitAttributeMap() override;
            //站场绘制
            void Draw(bool isMulti = false) override;
            //绘制信号灯
            void DrawLight() override;
            //绘制文字
            void DrawText() override;
            //获取箭头颜色
            void getArrowColor() override;
            //判断鼠标是否在事件范围内
            bool Contains(const QPoint& ptPos) override;
            //鼠标是否在设备上
            bool IsMouseWheel(const QPoint& ptPos) override;
            //初始化设备点击事件
            void InitClickEvent() override;
            //设置按钮属性
            void SetBtnState() override;
            //站场翻转
            void setVollover(const QPointF& ptBase) override;
            //状态重置
            void ResetDevState() override;

        private:
            int m_nFlag = 0;
            
            QPointF m_ptSignal;      //信号
            QPointF m_ptNear;        //接近
            QPointF m_ptTakeLook;    //照查    
            QPointF m_ptAllow;       //允许
            QPointF m_ptFileld;      //本场
            QPointF m_ptAdjFileld;   //邻场

            QRectF m_rcNearLight;
            QRectF m_rcFileldLight;
            QRectF m_rcAdjFileldLight;
            QRectF m_rcAllowBtn;

            QString m_strCLType;
        };
    }
}