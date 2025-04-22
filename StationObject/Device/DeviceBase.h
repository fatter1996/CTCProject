#pragma once
#include <QObject>
#include <QEvent>
#include <QPainter>
#include <QWidget>
#include <QXmlStreamReader>
#include <QRectF>
#include <QMap>
#include <QMap>
#include "StationObject/GlobalStruct.h"
#include "BaseWndClass/ModuleWidget/StaFunBtnToolBar.h"

#define TRACK_WIDTH     4    //股道宽度
#define COLOR_BTN_BLUE		 QColor("#0000EE")
#define COLOR_BTN_WHITE      QColor("#FAFAFA")



namespace Station {
    namespace Device {

        class DeviceBase;
        typedef QMap<QString, std::function<void(DeviceBase* pDevice, const QString& strElement)>> AttrMap;
        typedef QMap<CTCWindows::FunType, std::function<void(DeviceBase* pDevice)>> ClickEventMap;

        class DeviceScale {

        public:
            explicit DeviceScale();
            ~DeviceScale() {}

        public: //工具函数
            double Scale(double value, double nOffset = 0);
            QPointF Scale(const QPointF& pt, const bool bOutSide = false, const bool bTopLine = false);
            QRectF Scale(const QRectF& rect);
            static QRectF QStringToQRectF(const QString& strRect);
            static QPointF QStringToQPointF(const QString& strPoint);
            static QRectF OutSideRect(const QRectF& rect, int rx, int ry);

        protected:
            bool m_bMainStation = false;
            QObject* m_pParent = nullptr;
                 
        };


        class DeviceBase : public QObject, virtual public DeviceScale {
            Q_OBJECT
        public:
            explicit DeviceBase(QObject* pParent = nullptr);
            ~DeviceBase();

        protected:
            virtual bool eventFilter(QObject* obj, QEvent* event) override;
        public:
            virtual void InitAttributeMap() = 0;
            //初始化设备信息
            void InitDeviceInfoFromXml(QXmlStreamReader* pDeviceInfoReader, const QString& strDeviceType);
            void InitDeviceInfoFromTxt(QStringList& strInfoList, const QString& strDeviceType);
            void InitDeviceInfoFromJson(const QJsonObject& object);

        private:
            
            //读取设备属性
            void ReadDeviceAttributeFromXml(QXmlStreamReader* m_pDeviceInfoReader);
            void ReadDeviceAttributeFromJson(const QJsonObject& lampObject, const QString& strKey);
            void ReadDeviceAttributeFromTxt(const QString& strKey, const QString& strValue);
            //绘制设备名称
            void DrawDeviceName();
            //设备点击事件
            void onDeviceClick();


        public:
            //命令清除
            virtual void OrderClear(bool bClearTwinkle = false) {}

        protected:
            
            //初始化设备属性
            virtual void InitDeviceAttribute() {}
            //站场绘制
            virtual void Draw(bool isMulti = false);
            //绘制设备选中虚线框
            virtual void DrawSelectRange() {}
            //绘制培训提示信息
            virtual void DrawCultivateTips() {}
            //判断鼠标是否在事件范围内
            virtual bool Contains(const QPoint& ptPos) { return false; }
            //鼠标是否在设备上
            virtual bool IsMouseWheel(const QPoint& ptPos) { return false; }
            //设备鼠标移动事件
            virtual void onMouseMoveToDevice(const QPoint& ptPos);
            //初始化设备点击事件
            virtual void InitClickEvent() {}
            //右键菜单
            virtual void ShowDeviceMenu(const QPoint& ptPos) {}
            //获取设备名称颜色
            virtual QPen getDeviceNameColor() { return QPen(Qt::white); }
            //站场翻转
            virtual void setVollover(const QPointF& ptBase) {}
            //状态重置
            virtual void ResetDevState() {}

        public:
            void setState(const uint& nState) { 
                m_nState = nState; 
                if (m_strName == "8" && nState == 8) {
                    m_nState = nState;
                }
            }
            uint getType() const { return m_nType; }
            void setStrType(const QString& strType) { m_strType = strType; }
            QString getStrType() const { return m_strType; }
            QString getName() const { return m_strName; }
            uint getCode() const { return m_nCode; }
            void setRangeVisible(bool bRangeVisible) { m_bRangeVisible = bRangeVisible; }
            uint getState() const { return m_nState; }
            ulong getAttr() const { return m_nAttr; }
            uint getSXThroat() const { return m_bUpDown; }
            void setShowTips(int nTipsType) { 
                m_bShowTips = true; 
                m_nTipsType = nTipsType;
            }

        public:
            static bool getElapsed() { return m_bElapsed; }
            static void setElapsed() { m_bElapsed = !m_bElapsed; }

        

        protected:
            uint nIndex = 0;
            //设备信息
            uint m_nType = 0;         //设备类型
            QString m_strType;
            QString m_strName;
            int m_nCode = -1;    //设备编号
            QPointF m_ptCenter;        //设备中心点
            bool m_bUpDown = false;       //上下行咽喉 true:S false:X
            QRectF m_rcTextRect;       //设备名称文本区域
            QPointF m_ptName;
            uint m_nFontSize = 14;    //文字字体大小
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
            static QMap<QString, ClickEventMap> m_mapClickEvent;
            static QMap<QString, AttrMap> m_mapAttribute;
            static QPainter m_pPainter;    //绘制器
            static bool m_bElapsed;     //闪烁控制
            static int m_nWheelDevCode;
        };


        class StaSection : public DeviceBase {
            Q_OBJECT
        public:
            explicit StaSection(QObject* pParent = nullptr);
            ~StaSection();

        protected:
            virtual void InitAttributeMap() override;
            //站场绘制
            virtual void Draw(bool isMulti = false) override;
            //绘制外边缘
            virtual void DrawDeviceOutSide() = 0;
            //绘制绝缘节
            virtual void DrawInsulateNode() = 0;

        protected:
            //绘制股道
            void DrawTrackLine(const QPen& pen, const QPointF& ptStart, const QPointF& ptEnd, const bool bOutSide = false, const int nOffset = 0);
            //获取股道颜色
            QColor getTrackColor();

        protected:
            uint m_nZ = 0; //折点数
            QPointF p1, p2, p3, p4, p12, p34, pz12, pz34; //绘制坐标
            QRectF m_rcRespondRect;
            uint m_nPowerCut = 0;  //是否停电(1-定位,2-反位,4-岔前)(可组合)


        };

        
        class DeviceBtn : virtual public DeviceScale {

        public:
            explicit DeviceBtn();
            ~DeviceBtn();

        protected:
            //绘制按钮
            void DrawButton(QPainter& pPainter, const QRectF rcButton, const QColor& cBtnColor, bool bBtnDown, int nType = 1, 
                const QColor cBtnDownColor = COLOR_BTN_BLUE, const QColor cBtnElapsedColor = COLOR_BTN_WHITE);
            //按钮点击事件
            void OnButtonClick();
            virtual void SetBtnState() = 0;
            //按钮状态重置
            void BtnStateReset();
        public:
            void setBtnState(int nState) { m_nBtnState = m_nBtnState; }

        protected:
            int m_nBtnState = 0; //按钮状态
            static int m_nFirstBtnType; //按钮类型(1 - 列车按钮; 2 - 调车按钮; 3 - 通过按钮; 4 - 引导按钮; 5 - 功能按钮)
            static int m_nSelectDevCode;
        };


        class DeviceArrow : virtual public DeviceScale {

        public:
            DeviceArrow();
            ~DeviceArrow();

        protected:
            void InitArrowAttributeMap(QString strType, QMap<QString, AttrMap>& mapAttribute);
            //绘制箭头
            void DrawArrow(QPainter& pPainter);

        protected:
            virtual void getArrowColor() = 0;

        public:
            void setArrowState(const uint& nState) { m_nArrowState = nState; }

        protected:
            QPointF ptArrow;
            QPointF p11, p12, p13, p14, p15, p16, p17;
            QPointF p21, p22, p23, p24, p25, p26, p27;

            QColor m_cColor1;
            QColor m_cColor2;

            uint m_nArrowState = 0; //箭头状态
        };


        class StaDistant : public DeviceBase {
            Q_OBJECT
        public:
            struct StaBlockBtn
            {
                QString m_strName;
                QRectF m_rcName;
                QRectF m_rcBtn;
                QPointF m_ptCountdown;
                QColor m_cTextColor;
            };

            struct StaBlockLamp
            {
                QString m_strName;
                QRectF m_rcName;
                QRectF m_rcLamp;
                uint m_nState = 0;
                QColor m_cTextColor;
            };

        public:
            explicit StaDistant(QObject* pParent = nullptr);
            ~StaDistant();

        protected:
            virtual void InitAttributeMap() override;
            //站场绘制
            virtual void Draw(bool isMulti = false) override;
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


        class DeviceTrain : virtual public DeviceScale {

        public:
            DeviceTrain();
            ~DeviceTrain();

        public:
            //设置车次
            void SetTrain(StaTrain* pTrain) { m_pTrain = pTrain; }
            //车次移动
            void MoveTo(DeviceTrain* pNext);

        protected:
            //绘制车次窗
            void DrawTrainFrame(QPainter& pPainter);
            //绘制车次
            void DrawTrain(QPainter& pPainter);
            void DrawTrainFrame(QPainter& pPainter, QRectF rcFrame);
            void DrawTrainHead(QPainter& pPainter, QRectF rcFrame);
            void DrawTrainTail(QPainter& pPainter, QRectF rcFrame);
            void ShowTrainMenu(QPoint ptPos, int nCode);
            void onMouseMoveToTrainFrame(const QPoint& ptPos);

        public:
            struct TrainFrame {
                QRectF m_rcFrame;
                bool m_bContains = false;
            };
            
        protected:
            QVector<TrainFrame*> m_vecTrainFrame;
            StaTrain* m_pTrain = nullptr;
            bool m_bTrainLeave = false;
            static int m_nInTrainFrame;
        };
    }
}