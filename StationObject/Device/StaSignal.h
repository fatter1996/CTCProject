#pragma once
#include "DeviceBase.h"

#define BTNDOWN_TRAIN    0x01    //列车按钮
#define BTNDOWN_SHUNT    0x02    //调车按钮
#define BTNDOWN_GUIDE    0x04    //引导按钮

namespace Station {
    namespace Device {
        enum class SignalState : int {
            DS,      	//断丝
            B,          //白灯
            A,          //蓝灯
            L,          //绿灯
            H,          //红灯
            U,          //黄灯
            UU,		    //双黄
            LL,		    //双绿
            HB,		    //引导(红白)
            U2,		    //2黄
            LU,		    //绿黄
            BS,		    //白闪
            US,		    //黄闪
            LS,		    //绿闪
            HS,		    //红闪
            USU	    //黄闪黄
        };

        //信号机
        class StaSignal : public DeviceBase, public DeviceBtn
        {
        public:
            explicit StaSignal(QObject* pParent = nullptr);
            ~StaSignal();

        private:
            void InitAttributeMap() override;
            //初始化设备属性
            void InitDeviceAttribute() override;
            //站场绘制
            void Draw(bool isMulti = false) override;
            //绘制信号机按钮
            void DrawSignalButton();
            //绘制信号机灯位
            void DrawSignalLight();
            //绘制信号机按钮封锁
            void DrawSignalButtonLock();
            //绘制设备选中虚线框
            void DrawSelectRange() override;
            //绘制信号机灯位虚线框
            void DrawLightRange();
            //绘制信号机名称虚线框
            void DrawDeviceNameRange();
            //绘制信号机按钮虚线框
            void DrawButtonRange();
            //绘制培训提示信息
            void DrawCultivateTips() override;
            //判断鼠标是否在事件范围内
            bool Contains(const QPoint& ptPos) override;
            //初始化信号灯颜色
            void InitSignalLightColor();
            //获取信号灯颜色
            void GetSignalLightColor();
            //鼠标是否在设备上
            bool IsMouseWheel(const QPoint& ptPos) override;
            //初始化设备点击事件
            void InitClickEvent() override;
            //右键菜单
            void ShowDeviceMenu(const QPoint& ptPos) override;
            //设置按钮属性
            void SetBtnState() override;
            //命令清除
            void OrderClear(bool bClearTwinkle = false) override;
            //站场翻转
            void setVollover(const QPointF& ptBase) override;
            //状态重置
            void ResetDevState() override;

          
        public:
            int GetBtnState() { return m_nBtnState; }
            void setRelatedBtn(const DeviceBase* pDevice) { m_pRelatedBtn = const_cast<DeviceBase*>(pDevice); }
            bool IsIsHaveBSQ() const { return m_bIsIsHaveBSQ; }
            void setBSQState(const uint& nBSQState) { m_nBSQState = nBSQState; }
            void setDirection(const QString& strDirection) { m_strDirection = strDirection; }
            QString getDirection() const { return m_strDirection; }
            QString getXHDTYpe() { return m_strXHDType; }

        private:
            QPointF p12, p7, p8, p9, p10, p13, p14; //绘制坐标
            uint m_nXHDType = 0; //信号机类型
            QString m_strXHDType; //信号机类型
            qreal m_nRadius = 0; //信号机半径
            qreal m_nBtnRadius = 8; //按钮半径
            uint m_nSignalType = 0; //信号类型
            bool m_bRight = false;
            uint m_nD_B_C_Signal = 0;
            uint m_nDC_LC_Signal = 0;
            QString m_strSafeLamp;
            bool m_bHigh = true;
            bool m_bSingleDeng = true;
            bool m_bMD = true;
            bool m_bYDSD = false;
            QString m_strDirection;

            uint m_nSelectType = 0x0;  //选中类型(0-未选中; 0x01-选中信号机名称; 0x02-选中列车按钮; 0x04-选中调车按钮; 0x08-选中通过按钮; 0x1f-选中信号机灯位)
            uint m_nLightNum = 0;   //灯位个数
            DeviceBase* m_pRelatedBtn = nullptr; //关联通过按钮

            static int m_nBState;

            QRectF m_rcLightTotal;   //灯位总范围
            QRectF m_rcLight1;    //灯位1范围
            QRectF m_rcLight2;    //灯位2范围
            QColor m_cLightColor1;
            QColor m_cLightColor2;
            QRectF m_rcTrainBtn;  //列车按钮范围
            QRectF m_rcShuntBtn;  //调车按钮范围
            QRectF m_rcGuideBtn;  //引导按钮范围

            //信号表示器
            bool m_bIsIsHaveBSQ = false;
            int m_nBSQNum = 0;
            int m_nBSQInterlockBus = 0;
            int m_nBSQModuAddr = 0;

            uint m_nBSQState = 0;
            QMap<SignalState, std::function<void()>> m_mapLightColor;
        };
    }
}