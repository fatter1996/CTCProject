#pragma once
#include <QWidget>
#include <QEvent>
#include <QMap>
#include <QAbstractButton>

namespace CTCWindows {
    //功能按钮类型
    enum class FunType : int {
        RouteBuild,     //进路建立
        TotalCancel,    //总取消
        SignalReopen,   //信号重开
        GuideBtn,       //引导按钮
        GuideClock,     //引导总锁
        TotalRelieve,   //总人解
        RegionRelieve,  //区故解
        TotalPosition,  //总定位
        TotalReverse,   //总反位
        SingleLock,     //单锁
        SingleUnlock,   //单解
        Blockade,       //封锁
        UnBlockade,     //解封
        FunBtn,         //功能按钮
        RampUnlock,     //坡道解锁
        PoorRoute,      //分路不良
        IdleConfirm,    //确认空闲
        Lighting,       //点灯
        UnLighting,     //灭灯
        AuxiliaryMenu,  //辅助菜单
        CommandClear,   //命令清除
        CommandIssued,  //命令下达
        MethodConvert   //方式转换
    };

    //单站界面基类
    class StaFunBtnToolBar : public QWidget
    {
        Q_OBJECT
    public:
        StaFunBtnToolBar(QWidget* parent = nullptr);
        ~StaFunBtnToolBar();

    public:
        virtual void FunBtnStateReset() = 0;

    public slots:
        void onButtonClicked(QAbstractButton* pButton);
        void onOrderClear(bool checked);
        void onOrderIssued(bool checked);

    signals:
        void OrderClear();
        void OrderIssued();

    protected:
        QMap<QAbstractButton*, FunType> m_mapFunBtnType;

    public:
        static FunType m_SelectFunType;
        static QMap<FunType, int> m_mapStaOrderCode;
    }; 

    FunType getCurrFunType();
    int getFunBtnOrderCode();
}