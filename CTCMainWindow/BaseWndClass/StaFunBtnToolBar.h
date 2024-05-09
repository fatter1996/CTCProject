#pragma once

#include <QWidget>
#include <QEvent>
#include <QMap>
#include <QAbstractButton>
#include"../BaseWndClassInterface/StationViewInterface.h"

namespace CTCWindows {
    //功能按钮类型
    enum class FunType : int {
        RouteBuild      = 0x01,     //进路建立
        TotalCancel     = 0x04,     //总取消
        SignalReopen    = 0x03,     //信号重开
        GuideBtn        = 0x02,     //引导按钮
        GuideClock      = 0x05,     //引导总锁
        TotalRelieve    = 0x0c,     //总人解
        RegionRelieve   = 0x0d,     //区故解
        TotalPosition   = 0x06,     //总定位
        TotalReverse    = 0x07,     //总反位
        SingleLock      = 0x08,     //单锁
        SingleUnlock    = 0x09,     //单解
        Blockade        = 0x0a,     //封锁
        UnBlockade      = 0x0b,     //解封
        FunBtn          = 0x20,     //功能按钮
        RampUnlock      = 0x10,     //坡道解锁
        PoorRoute       = 0x11,     //分路不良
        IdleConfirm     = 0x02,     //确认空闲
        Lighting        = 0x0e,     //点灯
        UnLighting      = 0x0f,     //灭灯
        AuxiliaryMenu   = 0x30,     //辅助菜单
        CommandClear    = 0x21,     //命令清除
        CommandIssued   = 0x22,     //命令下达
        MethodConvert   = 0x40      //方式转换
    };

    enum class OperObjType : int {
        Defult          = 0x00,
        Train           = 0x01,     //列车
        Shunt           = 0x02,     //调车
        Though          = 0x03,     //通过按钮
        Flexibility     = 0x04,     //变通按钮
        Guide           = 0x05,     //引导按钮
        
        Switch          = 0x11,     //道岔
        Track           = 0x12,     //股道
        Signal          = 0x13,     //信号灯
        
        Ascend          = 0x21,     //上行咽喉
        Descend         = 0x22,     //下行咽喉

        Blockage        = 0x31,     //闭塞

        Restoration     = 0x32,     //复原
        Accident        = 0x33,     //事故

        TotalAux        = 0x41,     //总辅助
        PickUpAux       = 0x42,     //接车辅助
        DepartureAux    = 0x43,     //发车辅助

        Allow           = 0x51,     //允许操动道岔
        Hump            = 0x52      //驼峰
    };

    //单站界面基类
    class StaFunBtnToolBar : public StationViewInterface
    {
        Q_OBJECT
    public:
        StaFunBtnToolBar(QWidget* parent = nullptr);
        ~StaFunBtnToolBar();

    public slots:
        void onButtonClicked(QAbstractButton* pButton);

    protected:
        QMap<QAbstractButton*, FunType> m_mapFunBtnType;

    public:
        static FunType m_SelectFunType;
        static OperObjType m_nOperObjType;
    }; 

    FunType getCurrFunType();
    void setOperObjType(OperObjType eType);
    OperObjType getOperObjType();
}