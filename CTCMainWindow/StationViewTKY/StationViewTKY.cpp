#include "StationViewTKY.h"
#include <QDebug>
#include <QMenuBar>

#pragma execution_character_set("utf-8")

namespace CTCWindows {
	StationViewTKY::StationViewTKY(QWidget* parent)
		: CTCMainWindow(parent)
	{
		ui.setupUi(this);
	}

	StationViewTKY::~StationViewTKY()
	{

	}

	StationViewTKY* StationViewTKY::CreatStationView(QWidget* parent)
	{
		return new StationViewTKY(parent);
	}

	StationCtrlDispTKY* StationViewTKY::CreateStationCtrlDisp()
	{
		qDebug() << "CreateStationCtrlDispTKY";
		m_pStationCtrl = new StationCtrlDispTKY(this);
		ui.centralWidget->layout()->addWidget(m_pStationCtrl);
		return m_pStationCtrl;
	}

	StationMultiDispTKY* StationViewTKY::CreateMultiStationDisp()
	{
		qDebug() << "CreateMultiStationDispTKY";
		return nullptr;
	}

	StationLogDispTKY* StationViewTKY::CreateTrafficLogManage()
	{
		qDebug() << "CreateTrafficLogManageTKY";
		return nullptr;
	}

	void StationViewTKY::InitStationViewMenuBar()
	{
		qDebug() << "InitStationViewMenuBarTKY";
        m_pMenuBar = new QMenuBar(this);

        MenuBarInfo* menuBarInfo1 = new MenuBarInfo;
        menuBarInfo1->pMenu = m_pMenuBar->addMenu("系统(S)"); {
            QAction* pAction1 = menuBarInfo1->addNewAction(this, 1, 0, "交接班过表");
            connect(pAction1, &QAction::triggered, [&]() {

            });

            QAction* pAction2 = menuBarInfo1->addNewAction(this, 1, 1, "静音");
            connect(pAction2, &QAction::triggered, [&]() {

            });

            QAction* pAction3 = menuBarInfo1->addNewAction(this, 1, 2, "退出(X)");
            connect(pAction3, &QAction::triggered, [&]() {

            });
        }
        m_vecMenuBarInfo.append(menuBarInfo1);

        MenuBarInfo* menuBarInfo2 = new MenuBarInfo;
        menuBarInfo2->pMenu = m_pMenuBar->addMenu("管理功能(M)"); {
            QAction* pAction1 = menuBarInfo2->addNewAction(this, 1, 0, "报点/填写日志");
            connect(pAction1, &QAction::triggered, [&]() {

                });

            QAction* pAction2 = menuBarInfo2->addNewAction(this, 1, 1, "现在车管理");
            connect(pAction2, &QAction::triggered, [&]() {

            });

            QAction* pAction3 = menuBarInfo2->addNewAction(this, 1, 2, "确报查询");
            connect(pAction3, &QAction::triggered, [&]() {

            });

			QAction* pAction4 = menuBarInfo2->addNewAction(this, 1, 3, "列控区间占用逻辑检查");
			connect(pAction4, &QAction::triggered, [&]() {

			});
        }
        m_vecMenuBarInfo.append(menuBarInfo2);
            
		MenuBarInfo* menuBarInfo3 = new MenuBarInfo;
		menuBarInfo3->pMenu = m_pMenuBar->addMenu("调度命令(D)"); {
			QAction* pAction1 = menuBarInfo3->addNewAction(this, 1, 0, "调度命令管理");
			connect(pAction1, &QAction::triggered, [&]() {

			});

			QAction* pAction2 = menuBarInfo3->addNewAction(this, 1, 1, "调度命令查询");
			connect(pAction2, &QAction::triggered, [&]() {

			});

			QAction* pAction3 = menuBarInfo3->addNewAction(this, 1, 2, "行车凭证管理");
			connect(pAction3, &QAction::triggered, [&]() {

			});

			QAction* pAction4 = menuBarInfo3->addNewAction(this, 1, 3, "行车凭证查询");
			connect(pAction4, &QAction::triggered, [&]() {

			});
		}
		m_vecMenuBarInfo.append(menuBarInfo3);
            
		MenuBarInfo* menuBarInfo4 = new MenuBarInfo;
		menuBarInfo4->pMenu = m_pMenuBar->addMenu("站场显示(X)"); {
			QAction* pAction1 = menuBarInfo4->addNewAction(this, 1, 0, "刷新");
			connect(pAction1, &QAction::triggered, [&]() {

			});

			QAction* pAction2 = menuBarInfo4->addNewAction(this, 1, 1, "放大");
			connect(pAction2, &QAction::triggered, [&]() {

			});

			QAction* pAction3 = menuBarInfo4->addNewAction(this, 1, 2, "缩小");
			connect(pAction3, &QAction::triggered, [&]() {

			});

			QAction* pAction4 = menuBarInfo4->addNewAction(this, 1, 3, "恢复");
			connect(pAction4, &QAction::triggered, [&]() {

			});

			QAction* pAction5 = menuBarInfo4->addNewAction(this, 1, 4, "车次放大");
			connect(pAction5, &QAction::triggered, [&]() {

			});

			QAction* pAction6 = menuBarInfo4->addNewAction(this, 1, 5, "车次缩小");
			connect(pAction6, &QAction::triggered, [&]() {

			});

			QAction* pAction7 = menuBarInfo4->addNewAction(this, 1, 6, "车次恢复");
			connect(pAction7, &QAction::triggered, [&]() {

			});

			QAction* pAction8 = menuBarInfo4->addNewAction(this, 1, 7, "防止提示圈");
			connect(pAction8, &QAction::triggered, [&]() {

			});
		}
		m_vecMenuBarInfo.append(menuBarInfo4);

        MenuBarInfo* menuBarInfo5 = new MenuBarInfo;
        menuBarInfo5->pMenu = m_pMenuBar->addMenu("控制命令(C)"); {
            menuBarInfo5->addNewAction(this, 1, 0, "进路命令"); {
                MenuInfo* menuInfo = menuBarInfo5->vecSubMenuInfo[0];
                
				QAction* pAction1 = menuInfo->addNewSubAction(this, 2, 0, "排列进路");
                connect(pAction1, &QAction::triggered, [&]() {

                });

                QAction* pAction2 = menuInfo->addNewSubAction(this, 2, 1, "总取消");
                connect(pAction2, &QAction::triggered, [&]() {

                });

                QAction* pAction3 = menuInfo->addNewSubAction(this, 2, 2, "总人解");
                connect(pAction3, &QAction::triggered, [&]() {

                });

                QAction* pAction4 = menuInfo->addNewSubAction(this, 2, 3, "信号重开");
                connect(pAction4, &QAction::triggered, [&]() {

                });

                QAction* pAction5 = menuInfo->addNewSubAction(this, 2, 4, "引导");
                connect(pAction5, &QAction::triggered, [&]() {

                });

                QAction* pAction6 = menuInfo->addNewSubAction(this, 2, 5, "故障解锁");
                connect(pAction6, &QAction::triggered, [&]() {

                });

                QAction* pAction7 = menuInfo->addNewSubAction(this, 2, 6, "坡道解锁");
                connect(pAction7, &QAction::triggered, [&]() {

                });
            }

            menuBarInfo5->addNewAction(this, 1, 1, "道岔命令"); {
                MenuInfo* menuInfo = menuBarInfo5->vecSubMenuInfo[1];
				
				QAction* pAction1 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 0, "道岔总定");
				connect(pAction1, &QAction::triggered, [&]() {

				});

				QAction* pAction2 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 1, "道岔总反");
				connect(pAction2, &QAction::triggered, [&]() {

				});

				QAction* pAction3 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 2, "道岔单锁");
				connect(pAction3, &QAction::triggered, [&]() {

				});
                
				QAction* pAction4 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 3, "道岔解锁");
				connect(pAction4, &QAction::triggered, [&]() {

				});

				QAction* pAction5 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 4, "道岔单封");
				connect(pAction5, &QAction::triggered, [&]() {

				});

				QAction* pAction6 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 5, "道岔解封");
				connect(pAction6, &QAction::triggered, [&]() {

				});
            }

            menuBarInfo5->addNewAction(this, 1, 2, "信号机命令"); {
                MenuInfo* menuInfo = menuBarInfo5->vecSubMenuInfo[2];

                QAction* pAction1 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 0, "按钮(信号)加封");
                connect(pAction1, &QAction::triggered, [&]() {

                });

				QAction* pAction2 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 1, "按钮(信号)解封");
				connect(pAction2, &QAction::triggered, [&]() {

				});

				QAction* pAction3 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 2, "点灯");
				connect(pAction3, &QAction::triggered, [&]() {

				});

				QAction* pAction4 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 3, "灭灯");
				connect(pAction4, &QAction::triggered, [&]() {

				});
            }

			QAction* pAction1 = menuBarInfo5->addNewAction(this, 1, 3, "设置引导总锁");
			connect(pAction1, &QAction::triggered, [&]() {

			});

			QAction* pAction2 = menuBarInfo5->addNewAction(this, 1, 4, "取消引导总锁");
			connect(pAction2, &QAction::triggered, [&]() {

			});

			QAction* pAction3 = menuBarInfo5->addNewAction(this, 1, 5, "上电解锁");
			connect(pAction3, &QAction::triggered, [&]() {

			});

			QAction* pAction4 = menuBarInfo5->addNewAction(this, 1, 6, "功能按钮");
			connect(pAction4, &QAction::triggered, [&]() {

			});

			menuBarInfo5->addNewAction(this, 1, 7, "封锁/解封"); {
				MenuInfo* menuInfo = menuBarInfo5->vecSubMenuInfo[7];

				QAction* pAction1 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 0, "封锁");
				connect(pAction1, &QAction::triggered, [&]() {

				});

				QAction* pAction2 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 1, "解封");
				connect(pAction2, &QAction::triggered, [&]() {

				});
			}

			menuBarInfo5->addNewAction(this, 1, 8, "停电/供电"); {
				MenuInfo* menuInfo = menuBarInfo5->vecSubMenuInfo[8];

				QAction* pAction1 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 0, "停电");
				connect(pAction1, &QAction::triggered, [&]() {

				});

				QAction* pAction2 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 1, "供电");
				connect(pAction2, &QAction::triggered, [&]() {

				});
			}

			menuBarInfo5->addNewAction(this, 1, 9, "分路不良"); {
				MenuInfo* menuInfo = menuBarInfo5->vecSubMenuInfo[9];

				QAction* pAction1 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 0, "设置分路不良");
				connect(pAction1, &QAction::triggered, [&]() {

				});

				QAction* pAction2 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 1, "取消分路不良");
				connect(pAction2, &QAction::triggered, [&]() {

				});

				QAction* pAction3 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 2, "确认空闲");
				connect(pAction3, &QAction::triggered, [&]() {

				});

				QAction* pAction4 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 3, "不能确认空闲");
				connect(pAction4, &QAction::triggered, [&]() {

				});
			}

			menuBarInfo5->addNewAction(this, 1, 10, "防溜"); {
				MenuInfo* menuInfo = menuBarInfo5->vecSubMenuInfo[10];

				QAction* pAction1 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 0, "设置防溜");
				connect(pAction1, &QAction::triggered, [&]() {

				});

				QAction* pAction2 = menuInfo->addNewSubAction(menuBarInfo5->pMenu, 2, 1, "取消防溜");
				connect(pAction2, &QAction::triggered, [&]() {

				});
			}

			QAction* pAction5 = menuBarInfo5->addNewAction(this, 1, 11, "统一设置封锁");
			connect(pAction5, &QAction::triggered, [&]() {

			});

			QAction* pAction6 = menuBarInfo5->addNewAction(this, 1, 12, "统一设置停供电");
			connect(pAction6, &QAction::triggered, [&]() {

			});

			QAction* pAction7 = menuBarInfo5->addNewAction(this, 1, 13, "命令下达");
			connect(pAction7, &QAction::triggered, [&]() {

			});

			QAction* pAction8 = menuBarInfo5->addNewAction(this, 1, 14, "命令清除");
			connect(pAction8, &QAction::triggered, [&]() {

			});
        }
        m_vecMenuBarInfo.append(menuBarInfo5);

        this->setMenuBar(m_pMenuBar);
	}
	
	void StationViewTKY::InitStationViewToolBar()
	{
		qDebug() << "InitStationViewToolBarTKY";
	}
	
	void StationViewTKY::InitTrafficLogToolBar()
	{
		qDebug() << "InitTrafficLogToolBarTKY";
	}
	
	void StationViewTKY::InitSignForToolBar()
	{
		qDebug() << "InitSignForToolBarTKY";
	}

	void StationViewTKY::InitStateToolBar()
	{
		qDebug() << "InitStateToolBarTKY";
	}
}
