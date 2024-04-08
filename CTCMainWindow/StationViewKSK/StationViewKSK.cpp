#include "StationViewKSK.h"
#include <QDebug>
#include <QMenuBar>

#pragma execution_character_set("utf-8")

namespace CTCWindows {
	StationViewKSK::StationViewKSK(QWidget* parent)
		: CTCMainWindow(parent)
	{
		ui.setupUi(this);
	}

	StationViewKSK::~StationViewKSK()
	{
	
	}

	StationViewKSK* StationViewKSK::CreatStationView(QWidget * parent)
	{
		return new StationViewKSK(parent);
	}

	StationCtrlDispKSK* StationViewKSK::CreateStationCtrlDisp()
	{
		qDebug() << "CreateStationCtrlDisp";
		return nullptr;
	}

	StationMultiDispKSK* StationViewKSK::CreateMultiStationDisp()
	{
		qDebug() << "CreateMultiStationDisp";
		return nullptr;
	}

	StationLogDispKSK* StationViewKSK::CreateTrafficLogManage()
	{
		qDebug() << "CreateTrafficLogManage";
		return nullptr;
	}

	void StationViewKSK::InitStationViewMenuBar()
	{
		qDebug() << "InitStationViewMenuBar";
        m_pMenuBar = new QMenuBar(this);

        MenuBarInfo* menuBarInfo1 = new MenuBarInfo;
        menuBarInfo1->pMenu = m_pMenuBar->addMenu("登录"); {
            QAction* pAction1 = menuBarInfo1->addNewAction(this, 1, 0, "用户登录");
            connect(pAction1, &QAction::triggered, [&]() {
                
            });

            QAction* pAction2 = menuBarInfo1->addNewAction(this, 1, 1, "用户注销");
            connect(pAction2, &QAction::triggered, [&]() {
                
            });

            QAction* pAction3 = menuBarInfo1->addNewAction(this, 1, 2, "退出");
            connect(pAction3, &QAction::triggered, [&]() {
                
            });
        }
        m_vecMenuBarInfo.append(menuBarInfo1);

        MenuBarInfo* menuBarInfo2 = new MenuBarInfo;
        menuBarInfo2->pMenu = m_pMenuBar->addMenu("显示"); {
            menuBarInfo2->addNewAction(this, 1, 0, "工具栏"); {
                MenuInfo* menuInfo = menuBarInfo2->vecSubMenuInfo[0];
                QAction* pAction1 = menuInfo->addNewSubAction(this, 2, 0, "标准按钮");
                connect(pAction1, &QAction::triggered, [&]() {
                    
                });

                QAction* pAction2 = menuInfo->addNewSubAction(this, 2, 1, "签收栏");
                connect(pAction2, &QAction::triggered, [&]() {

                });

                QAction* pAction3 = menuInfo->addNewSubAction(this, 2, 2, "显示系统信息窗口", true, true);
                connect(pAction3, &QAction::triggered, [&]() {

                 });

                QAction* pAction4 = menuInfo->addNewSubAction(this, 2, 3, "显示进路序列管理窗口", true, true, bRoutepreWndShow);
                connect(pAction4, &QAction::triggered, [&]() {

                });
            }

            menuBarInfo2->addNewAction(this, 1, 1, "站场图"); {
                MenuInfo* menuInfo = menuBarInfo2->vecSubMenuInfo[1]; 
                /*站场图操作*/ {
                    QAction* pAction1 = menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 0, "放大站场图", true, true);
                    QAction* pAction2 = menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 1, "缩小站场图", true, true);
                    QAction* pAction3 = menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 2, "还原站场图", false);

                    connect(pAction1, &QAction::triggered, [&]() {

                    });

                    connect(pAction2, &QAction::triggered, [&]() {

                    });

                    connect(pAction3, &QAction::triggered, [&]() {

                    });
                }
                menuBarInfo2->addSeparator(); 
                /*车次窗操作*/ {
                    QAction* pAction1 = menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 3, "放大车次窗");
                    QAction* pAction2 = menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 4, "缩小车次窗");
                    QAction* pAction3 = menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 5, "还原车次窗", false);

                    connect(pAction1, &QAction::triggered, [&]() {

                    });

                    connect(pAction2, &QAction::triggered, [&]() {

                    });

                    connect(pAction3, &QAction::triggered, [&]() {

                    });
                }
                menuBarInfo2->addSeparator();
                /*显示内容*/ {
                    QAction* pAction1 = menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 6, "文字显示");
                    connect(pAction1, &QAction::triggered, [&]() {
                        
                    });

                    QAction* pAction2 = menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 7, "绝缘节显示", true, true, bShow_JYJ);
                    connect(pAction2, &QAction::triggered, [&]() {

                    });

                    QAction* pAction3 = menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 8, "列车按钮显示", true, true, bShow_LCAN);
                    connect(pAction3, &QAction::triggered, [&]() {
                        
                    });

                    QAction* pAction4 = menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 9, "调车按钮显示", true, true, bShow_DCAN);
                    connect(pAction4, &QAction::triggered, [&]() {
                        
                    });

                    QAction* pAction5 = menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 10, "进路序列窗显示", true, true, bShow_JLYG);
                    connect(pAction5, &QAction::triggered, [&]() {

                    });

                    menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 11, "显示状态条", false, true, true);
                    menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 12, "调车进路显示", false, true, true);
                    menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 13, "道岔位置", false, true, true);
                }
            }

            menuBarInfo2->addNewAction(this, 1, 2, "行车日志"); {
                MenuInfo* menuInfo = menuBarInfo2->vecSubMenuInfo[2];

                QAction* pAction1 = menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 0, "显示表头", true, true, true);
                connect(pAction1, &QAction::triggered, [&]() {
                    
                });

                menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 1, "显示邻站", false);
                menuInfo->addSeparator();
                menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 2, "扩大", false);
                menuInfo->addNewSubAction(menuBarInfo2->pMenu, 2, 2, "缩小", false);
            }

        }
        m_vecMenuBarInfo.append(menuBarInfo2);

        MenuBarInfo* menuBarInfo3 = new MenuBarInfo;
        menuBarInfo3->pMenu = m_pMenuBar->addMenu("工具"); {
            QAction* pAction1 = menuBarInfo3->addNewAction(this, 1, 0, "调度命令管理");
            connect(pAction1, &QAction::triggered, [&]() {

            });

            menuBarInfo3->addNewAction(this, 1, 1, "甩挂作业", false);

            QAction* pAction2 = menuBarInfo3->addNewAction(this, 1, 2, "列车编组(速报表)");
            connect(pAction2, &QAction::triggered, [&]() {

            });

            QAction* pAction3 = menuBarInfo3->addNewAction(this, 1, 3, "站存车(现存车)");
            connect(pAction3, &QAction::triggered, [&]() {

            });

            menuBarInfo3->addNewAction(this, 1, 4, "列控命令", false);

            QAction* pAction4 = menuBarInfo3->addNewAction(this, 1, 5, "车站直接限速");
            connect(pAction4, &QAction::triggered, [&]() {

            });

            QAction* pAction5 = menuBarInfo3->addNewAction(this, 1, 6, "运统46");
            connect(pAction5, &QAction::triggered, [&]() {

            });

            menuBarInfo3->addNewAction(this, 1, 7, "站场图回放", false);

            QAction* pAction6 = menuBarInfo3->addNewAction(this, 1, 8, "列控区间占用逻辑检查");
            connect(pAction6, &QAction::triggered, [&]() {

            });

            menuBarInfo3->addSeparator();
            menuBarInfo3->addNewAction(this, 1, 9, "用户管理", false);

            QAction* pAction7 = menuBarInfo3->addNewAction(this, 1, 10, "选项");
            connect(pAction7, &QAction::triggered, [&]() {

            });

            menuBarInfo3->addSeparator();
            menuBarInfo3->addNewAction(this, 1, 11, "确报查询", false);
        }
        m_vecMenuBarInfo.append(menuBarInfo3);

        MenuBarInfo* menuBarInfo4 = new MenuBarInfo;
        menuBarInfo4->pMenu = m_pMenuBar->addMenu("调试"); {
            menuBarInfo4->addNewAction(this, 1, 0, "刷新站场图", false);
            menuBarInfo4->addNewAction(this, 1, 1, "测试调度命令", false);
            menuBarInfo4->addNewAction(this, 1, 2, "发送XML信息", false);
            menuBarInfo4->addSeparator();
            QAction* pAction = menuBarInfo4->addNewAction(this, 1, 3, "语音静音");
            connect(pAction, &QAction::triggered, [&]() {

            });
        }
        m_vecMenuBarInfo.append(menuBarInfo4);

        MenuBarInfo* menuBarInfo5 = new MenuBarInfo;
        menuBarInfo5->pMenu = m_pMenuBar->addMenu("窗口");
        {
            menuBarInfo5->addNewAction(this, 1, 0, "层叠窗口", false);
            menuBarInfo5->addNewAction(this, 1, 1, "显示调整", false);
            menuBarInfo5->addSeparator();
            QAction* pAction = menuBarInfo5->addNewAction(this, 1, 2, "调车作业单", false);
            connect(pAction, &QAction::triggered, [&]() {

            });

            menuBarInfo5->addSeparator();
            menuBarInfo5->addNewAction(this, 1, 3, "站场平面", false);
            menuBarInfo5->addNewAction(this, 1, 4, "行车日志", false);
            menuBarInfo5->addNewAction(this, 1, 5, "规章制度", false);
            menuBarInfo5->addNewAction(this, 1, 6, "信号员控制面板", false);
        }
        m_vecMenuBarInfo.append(menuBarInfo5);

        MenuBarInfo* menuBarInfo6 = new MenuBarInfo;
        menuBarInfo6->pMenu = m_pMenuBar->addMenu("帮助");
        {
            QAction* pAction = menuBarInfo6->addNewAction(this, 1, 0, "关于CTC-STPC");
            connect(pAction, &QAction::triggered, [&]() {

            });
        }
        m_vecMenuBarInfo.append(menuBarInfo6);

        this->setMenuBar(m_pMenuBar);
	}
	void StationViewKSK::InitStationViewToolBar()
	{
		qDebug() << "InitStationViewToolBarKSK";
	}
	void StationViewKSK::InitTrafficLogToolBar()
	{
		qDebug() << "InitTrafficLogToolBarKSK";
	}
	void StationViewKSK::InitSignForToolBar()
	{
		qDebug() << "InitSignForToolBarKSK";
	}

    void StationViewKSK::InitStateToolBar()
    {
        qDebug() << "InitStateToolBarKSK";
    }
}
