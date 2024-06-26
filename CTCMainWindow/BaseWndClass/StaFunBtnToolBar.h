#pragma once

#include <QWidget>
#include <QEvent>
#include <QMap>
#include <QAbstractButton>

namespace CTCWindows {
    //功能按钮类型
    enum class FunType : int {
        RouteBuild = 0x01,      //进路建立
        GuideBtn,               //引导按钮
        SignalReopen,           //信号重开
        TotalCancel,            //总取消
        GuideClock,             //引导总锁
        TotalPosition,          //总定位
        TotalReverse,           //总反位
        SingleLock,             //单锁
        SingleUnlock,           //单解
        Blockade,               //封锁
        UnBlockade,             //解封
        TotalRelieve,           //总人解
        RegionRelieve,          //区故解
        Lighting,               //点灯
        UnLighting,             //灭灯
        RampUnlock,             //坡道解锁
        PoorRoute,              //分路不良
        IdleConfirm,            //确认空闲
        FunBtn = 0x20,          //功能按钮
        CommandClear,           //命令清除
        CommandIssued,          //命令下达
        AuxiliaryMenu = 0x30,   //辅助菜单
        MethodConvert = 0x40    //方式转换
    };

    enum class OperObjType : int {
        Defult = 0x00,
        Train,            //列车
        Shunt,            //调车
        Though,           //通过按钮
        Flexibility,      //变通按钮
        Guide,            //引导按钮
                          
        Switch = 0x11,    //道岔
        Track,            //股道
        Signal,           //信号灯
                          
        Ascend = 0x21,    //上行咽喉
        Descend,          //下行咽喉
                          
        Blockage = 0x31,  //闭塞
        Restoration,      //复原
        Accident,         //事故
                          
        TotalAux = 0x41,   //总辅助
        PickUpAux,        //接车辅助
        DepartureAux,     //发车辅助
                          
        Allow = 0x51,     //允许操动道岔
        Hump              //驼峰
    };

    namespace BaseWnd {
        //单站界面功能按钮栏基类
        class StaFunBtnToolBar : public QWidget
        {
            Q_OBJECT
        public:
            StaFunBtnToolBar(QWidget* parent = nullptr);
            ~StaFunBtnToolBar();

        public slots:
            void onButtonClicked(QAbstractButton* pButton);
            virtual void onFunBtnStateReset() = 0;

        signals:
            void OrderClear();
            void SelectDeviceClear();
            void OrderIssued();

        protected:
            QMap<QAbstractButton*, FunType> m_mapFunBtnType;

        public:
            static FunType getCurrFunType() { return m_SelectFunType; }
            static void setOperObjType(OperObjType eType) { m_nOperObjType = eType; }
            static OperObjType getOperObjType() { return m_nOperObjType; }

        protected:
            static FunType m_SelectFunType;
            static OperObjType m_nOperObjType;
        };
    }
}