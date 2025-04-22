#pragma once
#include "DeviceBase.h"
#include "SealTechnique.h"
namespace Station {
    namespace Device {
        enum class LowFrequency : int {
            L6 = 1,
            L5,
            L4,
            L3,
            L2,
            L,
            LU,
            LU2,
            U,
            U2,
            U2S,
            UU,
            UUS,
            HU,
            HB,
            H,
            O
        };
        enum class Direction : int {
            Right,
            Left
        };
        //股道
        class StaTrack : public StaSection, public DeviceTrain
        {
        public:
            explicit StaTrack(QObject* pParent = nullptr);
            ~StaTrack();

        private:
            void InitAttributeMap() override;
            //初始化设备属性
            void InitDeviceAttribute() override;
            //站场绘制
            void Draw(bool isMulti = false) override;
            //绘制股道背景
            void DrawTrackBack();
            //绘制股道
            void DrawTrack(const QPen& pen, const bool bOutSide = false, const int nOffset = 0);
            //绘制外边缘.
            void DrawDeviceOutSide() override;
            //绘制设备选中虚线框
            void DrawSelectRange() override;
            //绘制绝缘节
            void DrawInsulateNode() override;
            //判断鼠标是否在高亮显示范围内
            bool Contains(const QPoint& ptPos) override;
            //鼠标是否在设备上
            bool IsMouseWheel(const QPoint& ptPos) override;
            //设备鼠标移动事件
            void onMouseMoveToDevice(const QPoint& ptPos) override;
            //初始化设备点击事件
            void InitClickEvent() override;
            //右键菜单
            void ShowDeviceMenu(const QPoint& ptPos) override;
            //站场翻转
            void setVollover(const QPointF& ptBase) override;
            //状态重置
            void ResetDevState() override;
            //区段低频显示
            void DrawLowTriangulation();
            //绘制正常显示
            void DrawTriangulation(QColor pencolor,QColor Textcolor, Direction m_nDirection ,QString Text = "");

            void DrawMinTriangulation(QColor Pencolor, Direction m_nDirection);

            void DrawTriangulationLine();
            
        public:
            QString TrackType() { return m_strTrackType; }
            void SetTrack();
        private:
            static LowFrequency m_nLowFrequency;
            static Direction m_nDirection;
            SealTechnique* m_pSealTechnique = nullptr;
            QString m_strTrackType; //股道类型
            bool m_bSpeedLimit = false;       //是否限速（临时限速）
            bool m_bShuntFault = false;        //是否分路不良
            bool m_bShuntFaultIdle = false;   //是否分路不良空闲
        };
    }
}