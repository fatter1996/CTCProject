#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //半自动闭塞
        class StaSemiAutoBlock : public StaDistant, public DeviceArrow, public DeviceBtn
        {
        public:
            explicit StaSemiAutoBlock(QObject* pParent = nullptr);
            ~StaSemiAutoBlock();

        private:
            //站场绘制
            void Draw(bool isMulti = false) override;
            //绘制信号灯
            void DrawLight() override;
            //绘制文字
            void DrawText() override;
            //判断鼠标是否在事件范围内
            bool Contains(const QPoint& ptPos) override;
            //鼠标是否在设备上
            bool IsMouseWheel(const QPoint& ptPos) override;
            //初始化设备点击事件
            void InitClickEvent() override;
            //设置按钮属性
            void SetBtnState() override;
            //获取箭头颜色
            void getArrowColor() override;
            //命令清除
            void OrderClear(int nType = 0) override;
            //站场翻转
            void setVollover(const QPointF& ptBase) override;
            //状态重置
            void ResetDevState() override;

            void AddBlockBtn(QString strType, const QString& strElement);

        private:
            QVector<StaBlockBtn> m_vecBlockBtn;
            QRectF m_rcFrame;
            QString m_strDirection;
            uint m_nBlockType = 0;
            uint m_nSelectBtnType = 0x0;  //选中类型(0-未选中; 0x01-闭塞; 0x02-复原; 0x04-事故)
        };
    }
}