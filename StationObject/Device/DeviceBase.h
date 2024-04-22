#pragma once
#include <QObject>
#include <QEvent>
#include <QPainter>
#include <QWidget>
#include <QXmlStreamReader>
#include <QRect>
#include <QMap>

#include "../../CTCMainWindow/BaseWndClass/StaFunBtnToolBar.h"

#define TRACK_WIDTH     4    //股道宽度
#define COLOR_BTN_BLUE		 QColor("#0000EE")
#define COLOR_BTN_WHITE      QColor("#FAFAFA")


namespace Station {
    namespace Device {
        class DeviceBase : public QObject {
            Q_OBJECT
        public:
            explicit DeviceBase(QObject* parent = nullptr);
            ~DeviceBase();

        protected:
            virtual bool eventFilter(QObject* obj, QEvent* event);

        public:
            //初始化设备信息
            void InitDeviceInfo(QXmlStreamReader* m_pDeviceInfoReader, const QString& strDeviceType);

        private:
            //读取设备属性
            void ReadDeviceAttribute(QXmlStreamReader* m_pDeviceInfoReader);
            //绘制设备名称
            void DrawDeviceName();
            //设备点击事件
            void onDeviceClick();

        public:
            //命令清除
            virtual void OrderClear();

        protected:
            //初始化设备属性
            virtual void InitDeviceAttribute();
            //站场绘制
            virtual void Draw(const bool& isMulti = false);
            //绘制设备选中虚线框
            virtual void DrawSelectRange();
            //判断鼠标是否在事件范围内
            virtual bool Contains(const QPoint& ptPos);
            //初始化设备点击事件
            virtual void InitClickEvent();
            //获取设备名称颜色
            virtual QPen getDeviceNameColor();
            //站场翻转
            virtual void setVollover(const QPoint& ptBase) = 0;
            //状态重置
            virtual void ResetDevState() = 0;

        public:
            void setState(const uint& nState);
            uint getType() { return m_nType; }
            QString getName() { return m_strName; }
            uint getCode() { return m_nCode; }
            void setRangeVisible(const bool& bRangeVisible) { m_bRangeVisible = bRangeVisible; }
            uint getState() { return m_nState; }

        public:
            static bool getElapsed();
            static void setElapsed();

        public: //工具函数
            static QRect QStringToQRect(QString strRect);
            static QPoint QStringToQPoint(QString strRect);
            //缩放
            static int Scale(const int& value, const int& nOffset = 0);
            static QPoint Scale(const QPoint& pt, const bool bOutSide = false, const bool bTopLine = false);
            static QRect Scale(const QRect& rect);
            static QRect OutSideRect(const QRect& rect, int rx, int ry);

        protected:
            uint nIndex = 0;
            QMap<QString, std::function<void(const QString& strElement)>> m_mapAttribute;
            QMap<CTCWindows::FunType, std::function<void()>> m_mapClickEvent;

            //设备信息
            uint m_nType = 0;         //设备类型
            QString m_strName;        //设备名称
            int m_nCode = -1;    //设备编号
            QPoint m_ptCenter;        //设备中心点
            uint m_nSX = 0;           //上下行咽喉 1:S 0:X
            QRect m_rcTextRect;       //设备名称文本区域
            QPoint m_ptName;
            uint m_nFontSize = 12;    //文字字体大小
            int m_nInterlockBus = 1;
            int m_nModuleCode = 0;
            ulong m_nAttr = 0;          //设备属性

            uint m_nState = 0;         //设备状态
            bool m_bRangeVisible = false; //是否高亮

            static int m_rcWheelDevCode;

        protected:
            static QPainter m_pPainter;    //绘制器
            static bool m_bElapsed;     //闪烁控制
        };


        class StaSection : public DeviceBase {
            Q_OBJECT
        public:
            explicit StaSection(QObject* parent = nullptr);
            ~StaSection();

        protected:
            //站场绘制
            virtual void Draw(const bool& isMulti = false);
            //绘制外边缘
            virtual void DrawDeviceOutSide() = 0;
            //绘制绝缘节
            virtual void DrawInsulateNode() = 0;

        protected:
            //绘制股道
            void DrawTrackLine(const QPen& pen, const QPoint& ptStart, const QPoint& ptEnd, const bool bOutSide = false, const int nOffset = 0);
            //获取股道颜色
            QColor getTrackColor();

        protected:
            uint m_nZ = 0; //折点数
            QPoint p1, p2, p3, p4, p12, p34, pz12, pz34; //绘制坐标

            bool m_bShowInsulateNode = true;  //是否显示绝缘节
            bool m_bInsulateNodeChange = true;  //是否显示绝缘节

            QRect m_rcRespondRect;
        };

        
        class DeviceBtn {

        public:
            DeviceBtn();
            ~DeviceBtn();

        protected:
            //绘制按钮
            void DrawButton(QPainter& pPainter, const QRect rcButton, const QColor& cBtnColor, bool bBtnDown, int nType = 1, const QColor cBtnDownColor = COLOR_BTN_BLUE, const QColor cBtnElapsedColor = COLOR_BTN_WHITE);
            //按钮鼠标移动事件
            void onMouseMoveToButton(const QPoint& ptPos, const int& nCode);
            //鼠标是否在按钮上
            virtual bool IsMouseWheel(const QPoint& ptPos) = 0;
            //按钮点击事件
            virtual void OnButtonClick() = 0;
            //按钮状态重置
            void BtnStateReset();

        protected:
            int m_nBtnState = 0; //按钮状态
            static int m_nFirstBtnType; //按钮类型(1 - 列车按钮; 2 - 调车按钮; 3 - 通过按钮; 4 - 引导按钮; 5 - 功能按钮)
            static int m_nSelectDevCode;
        };


        class DeviceArrow{

        public:
            DeviceArrow(QMap<QString, std::function<void(const QString&)>>& m_mapAttribute);
            ~DeviceArrow();

        protected:
            //绘制箭头
            void DrawArrow(QPainter& pPainter);

        protected:
            virtual void getArrowColor() = 0;

        public:
            void setArrowState(const uint& nState) { m_nArrowState = nState; }

        protected:
            QPoint p11, p12, p13, p14, p15, p16, p17;
            QPoint p21, p22, p23, p24, p25, p26, p27;

            QColor m_cColor1;
            QColor m_cColor2;

            uint m_nArrowState = 0; //箭头状态
        };


        class StaDistant : public DeviceBase {
            Q_OBJECT
        public:
            explicit StaDistant(QObject* parent = nullptr);
            ~StaDistant();

        protected:
            //站场绘制
            virtual void Draw(const bool& isMulti = false);
            //绘制信号灯
            virtual void DrawLight() = 0;
            //绘制文字
            virtual void DrawText() = 0;

        protected:
            QString m_strRelayQD;
            QString m_strCJ_qModule;
            QString m_strCJ_hModule;
            QString m_strCJ_qModule2;
            QString m_strCJ_hModule2;
            uint m_nInStart = 0;
            uint m_nInStart2 = 0;
            uint m_nOutStart = 0;
            uint m_nOutStart2 = 0;
        };
    }
}