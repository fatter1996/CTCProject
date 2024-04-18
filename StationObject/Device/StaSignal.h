#pragma once
#include "DeviceBase.h"

#define BTNDOWN_TRAIN    0x01    //列车按钮
#define BTNDOWN_SHUNT    0x02    //调车按钮
#define BTNDOWN_GUIDE    0x04    //引导按钮

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
            void Draw(const bool& isMulti = false);
            //绘制信号机按钮
            void DrawSignalButton();
            //绘制信号机灯位
            void DrawSignalLight();
            //绘制信号机按钮封锁
            void DrawSignalButtonLock();
            //绘制设备选中虚线框
            void DrawSelectRange();
            //绘制信号机灯位虚线框
            void DrawLightRange();
            //绘制信号机名称虚线框
            void DrawDeviceNameRange();
            //绘制信号机按钮虚线框
            void DrawButtonRange();
            //判断鼠标是否在事件范围内
            bool Contains(const QPoint& ptPos);
            //初始化信号灯颜色
            void InitSignalLightColor();
            //获取信号灯颜色
            void GetSignalLightColor();
            //鼠标是否在按钮上
            bool IsMouseWheel(const QPoint& ptPos);
            //初始化设备点击事件
            void InitClickEvent();
            //按钮点击事件
            void OnButtonClick();
            //命令清除
            void OrderClear();
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

            QRect m_rcLight1;    //灯位1范围
            QRect m_rcLight2;    //灯位2范围
            QColor m_cLightColor1;
            QColor m_cLightColor2;
            QRect m_rcTrainBtn;  //列车按钮范围
            QRect m_rcShuntBtn;  //调车按钮范围
            QRect m_rcGuideBtn;  //引导按钮范围

            //信号表示器
            bool m_bIsIsHaveBSQ = false;
            int m_nBSQNum = 0;
            int m_nBSQInterlockBus = 0;
            int m_nBSQModuAddr = 0;

            uint m_nBSQState = 0;
            QMap<int, std::function<void()>> m_mapLightColor;
        };
    }
}