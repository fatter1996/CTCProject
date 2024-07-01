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
            explicit DeviceBase(QObject* pParent = nullptr);
            ~DeviceBase();

        protected:
            virtual bool eventFilter(QObject* obj, QEvent* event);

        public:
            //初始化设备信息
            void InitDeviceInfoFromXml(QXmlStreamReader* pDeviceInfoReader, const QString& strDeviceType);
            void InitInitDeviceInfoFromJson(const QJsonObject& object, const QString& strKey);
        private:
            //读取设备属性
            void ReadDeviceAttributeFromXml(QXmlStreamReader* m_pDeviceInfoReader);
            void ReadDeviceAttributeFromJson(const QJsonObject& lampObject, const QString& strKey);
            //绘制设备名称
            void DrawDeviceName();
            //设备点击事件
            void onDeviceClick();

        public:
            //命令清除
            virtual void OrderClear() {}

        protected:
            //初始化设备属性
            virtual void InitDeviceAttribute() {}
            //站场绘制
            virtual void Draw(const bool& isMulti = false);
            //绘制设备选中虚线框
            virtual void DrawSelectRange() {}
            //绘制培训提示信息
            virtual void DrawCultivateTips() {}
            //判断鼠标是否在事件范围内
            virtual bool Contains(const QPoint& ptPos) { return false; }
            //鼠标是否在设备上
            virtual bool IsMouseWheel(const QPoint& ptPos) { return false; }
            //设备鼠标移动事件
            void onMouseMoveToDevice(const QPoint& ptPos);
            //初始化设备点击事件
            virtual void InitClickEvent() {}
            //获取设备名称颜色
            virtual QPen getDeviceNameColor() { return QPen(Qt::white); }
            //站场翻转
            virtual void setVollover(const QPoint& ptBase) {}
            //状态重置
            virtual void ResetDevState() {}

        public:
            void setState(const uint& nState) { m_nState = nState; }
            uint getType() { return m_nType; }
            void setStrType(const QString& strType) { m_strType = strType; }
            QString getStrType() { return m_strType; }
            QString getName() { return m_strName; }
            uint getCode() { return m_nCode; }
            void setRangeVisible(const bool& bRangeVisible) { m_bRangeVisible = bRangeVisible; }
            uint getState() { return m_nState; }
            ulong getAttr() { return m_nAttr; }
            uint getSXThroat() { return m_bUpDown; }
            void setShowTips(int nTipsType) { 
                m_bShowTips = true; 
                m_nTipsType = nTipsType;
            }

        public:
            static bool getElapsed() { return m_bElapsed; }
            static void setElapsed() { m_bElapsed = !m_bElapsed; }

        public: //工具函数
            static QRect QStringToQRect(QString strRect);
            static QPoint QStringToQPoint(QString strRect);
            //缩放
            static int Scale(const int& value, const int& nOffset = 0);
            static QPoint Scale(const QPoint& pt, QObject* pParent = nullptr, const bool bOutSide = false, const bool bTopLine = false);
            static QRect Scale(const QRect& rect, QObject* pParent = nullptr);
            static QRect OutSideRect(const QRect& rect, int rx, int ry);

        protected:
            QObject* m_pParent = nullptr;
            uint nIndex = 0;
            QMap<CTCWindows::FunType, std::function<void()>> m_mapClickEvent;
            QMap<QString, std::function<void(const QString& strElement)>> m_mapAttribute;
            //设备信息
            uint m_nType = 0;         //设备类型
            QString m_strName = "1";
            QString m_strType = "2";        //设备名称
            int m_nCode = -1;    //设备编号
            QPoint m_ptCenter;        //设备中心点
            bool m_bUpDown = false;       //上下行咽喉 true:S false:X
            QRect m_rcTextRect;       //设备名称文本区域
            QPoint m_ptName;
            uint m_nFontSize = 12;    //文字字体大小
            int m_nInterlockBus = 1;
            int m_nModuleCode = 0;
            ulong m_nAttr = 0;          //设备属性

            uint m_nState = 0;         //设备状态
            bool m_bRangeVisible = false; //是否高亮
            bool m_bShowName = true; //是否显示名称
            bool m_bShowTips = false; //是否显示提示内容

            //提示信息类型
            //信号机
            //0x01-列车进路, 0x02-调车进路,0x03-通过进路 高四位表示设备顺序
            //0x04-信号封锁,0x05-列按封锁, 0x06-调按封锁, 0x07-通按封锁
            //0x08-信号解锁,0x09-列按解锁, 0x0A-调按解锁, 0x0B-通按封锁
            //0x0C-列车引导
            //道岔 
            //0x10-道岔单操,0x11-道岔定位,0x12-道岔反位
            //0x14-道岔单锁,0x15-道岔单解,0x16-道岔封锁,0x17-道岔解封
            // 其他
            //0x20-分路不良,0x21-总取消,0x22-总人解,0x23-区故解,0x24-引导总锁
            int m_nTipsType = 0;    
            
        protected:
            static QPainter m_pPainter;    //绘制器
            static bool m_bElapsed;     //闪烁控制
            static int m_rcWheelDevCode;
            static int m_nWheelDevCode;
        };


        class StaSection : public DeviceBase {
            Q_OBJECT
        public:
            explicit StaSection(QObject* pParent = nullptr);
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
            QRect m_rcRespondRect;
        };

        
        class DeviceBtn {

        public:
            DeviceBtn();
            ~DeviceBtn();

        protected:
            //绘制按钮
            void DrawButton(QPainter& pPainter, const QRect rcButton, const QColor& cBtnColor, bool bBtnDown, int nType = 1, const QColor cBtnDownColor = COLOR_BTN_BLUE, const QColor cBtnElapsedColor = COLOR_BTN_WHITE);
            ////按钮鼠标移动事件
            //void onMouseMoveToButton(const QPoint& ptPos, const int& nCode);
            //按钮点击事件
            void OnButtonClick(DeviceBase* pDevice);
            virtual void SetBtnState() = 0;
            //按钮状态重置
            void BtnStateReset();

        protected:
            int m_nBtnState = 0; //按钮状态
            static int m_nFirstBtnType; //按钮类型(1 - 列车按钮; 2 - 调车按钮; 3 - 通过按钮; 4 - 引导按钮; 5 - 功能按钮)
            static int m_nSelectDevCode;
        };


        class DeviceArrow{

        public:
            DeviceArrow(QMap<QString, std::function<void(const QString& strElement)>> mapAttribute);
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
            explicit StaDistant(QObject* pParent = nullptr);
            ~StaDistant();

        protected:
            //站场绘制
            virtual void Draw(const bool& isMulti = false);
            //绘制信号灯
            virtual void DrawLight() = 0;
            //绘制文字
            virtual void DrawText() = 0;

        public:
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