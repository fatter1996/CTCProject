#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //信号机
        class StaSignal : public DeviceBase, public DeviceBtn
        {
        public:
            explicit StaSignal(QObject* parent = nullptr);
            ~StaSignal();

        private:
            virtual bool eventFilter(QObject* obj, QEvent* event);

        private:
            //初始化设备属性
            void InitDeviceAttribute();
            //站场绘制
            void Draw(const bool& bElapsed, const bool& isMulti = false);
            //绘制信号机按钮
            void DrawSignalButton(const bool& bElapsed);
            //绘制信号机灯位
            void DrawSignalLight(const bool& bElapsed);
            //绘制设备选中虚线框
            void DrawSelectRange();
            //绘制信号机灯位虚线框
            void DrawLightRange();
            //绘制信号机名称虚线框
            void DrawDeviceNameRange();
            //绘制信号机按钮虚线框
            void DrawButtonRange();

            //获取信号灯颜色
            void GetSignalLightColor(QColor& cColor1, QColor& cColor2, const bool& bElapsed);
            //获取设备名称颜色
            QPen getDeviceNameColor(const bool& bElapsed);
            //按钮点击事件
            void OnButtonClick();
            //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();

        public:
            void setRelatedBtn(const DeviceBase* pDevice) { m_pRelatedBtn = const_cast<DeviceBase*>(pDevice); }
            bool IsIsHaveBSQ() { return m_bIsIsHaveBSQ; }
            void setBSQState(const uint& nBSQState) { m_nBSQState = nBSQState; }
        private:
            QPoint p12, p7, p8, p9, p10, p13, p14; //绘制坐标
            uint m_nXHDType = 0; //信号机类型
            uint m_nRadius = 0; //信号机半径
            uint m_nSignalType = 0; //信号类型

            uint m_nD_B_C_Signal = 0;
            uint m_nDC_LC_Signal = 0;
            QString m_strSafeLamp;
            bool m_bHigh = true;
            bool m_bSingleDeng = true;
            bool m_bMD = true;

            bool bShowBtn = true; //是否显示按钮
            uint m_nSelectType = 0x0;  //选中类型(0-未选中; 0x01-选中信号机名称; 0x02-选中列车按钮; 0x04-选中调车按钮; 0x08-选中通过按钮; 0x1f-选中信号机灯位)
            uint m_nLightNum = 0;   //灯位个数
            DeviceBase* m_pRelatedBtn = nullptr; //关联通过按钮

            QRect n_rcLight1;    //灯位1范围
            QRect n_rcLight2;    //灯位2范围
            QRect n_rcTrainBtn;  //列车按钮范围
            QRect n_rcShuntBtn;  //调车按钮范围
            QRect n_rcGuideBtn;  //引导按钮范围

            //信号表示器
            bool m_bIsIsHaveBSQ = false;
            int m_nBSQNum = 0;
            int m_nBSQInterlockBus = 0;
            int m_nBSQModuAddr = 0;

            uint m_nBSQState = 0;
        };
    }
}