#include "StationViewTKY.h"
#include "StationView/StationCtrlDispTKY.h"
#include "StationView/StationMultiDispTKY.h"
#include "StationView/StationLogDispTKY.h"
#include "ModuleWidget/StaRoutePlanTKY.h"
#include "ModuleWidget/StaDispatchOrderTKY.h"
#include "ModuleWidget/StaVisibleSetTKY.h"
#include "CommonWidget/PointReport.h"
#include "Global.h"
#include <QDebug>
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace CARS {
		using namespace Station;
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

		BaseWnd::StationCtrlDisp* StationViewTKY::CreateStationCtrlDisp()
		{
			return new StationCtrlDispTKY(this);
		}

		BaseWnd::StationMultiDisp* StationViewTKY::CreateMultiStationDisp()
		{
			return new StationMultiDispTKY(this);
		}

		BaseWnd::StationLogDisp* StationViewTKY::CreateTrafficLogDisp()
		{
			return new StationLogDispTKY(this);
		}

		BaseWnd::StaRoutePlan* StationViewTKY::CreateStaRoutePlanWnd()
		{
			return new StaRoutePlanTKY(this);
		}

		BaseWnd::StaDispatchOrder* StationViewTKY::CreateStaDispatchOrder()
		{
			return new StaDispatchOrderTKY(this);
		}

		BaseWnd::StaVisibleSet* StationViewTKY::CreateStaVisibleSet()
		{
			return new StaVisibleSetTKY(this);
		}
		
		void StationViewTKY::InitStationViewMenuBar()
		{
			qDebug() << "InitStationViewMenuBarTKY";
			m_pMenuBar = new QMenuBar(this);

			MenuBarInfo* pMenuBarInfo1 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo1);
			pMenuBarInfo1->m_pMenu = m_pMenuBar->addMenu("系统(S)"); {
				connect(pMenuBarInfo1->addNewAction("交接班过表", 1, 0), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo1->addNewAction("静音", 1, 1), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo1->addNewAction("退出(X)", 1, 2), &QAction::triggered, [&]() {});
			}
			
			MenuBarInfo* pMenuBarInfo2 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo2);
			pMenuBarInfo2->m_pMenu = m_pMenuBar->addMenu("管理功能(M)"); {
				connect(pMenuBarInfo2->addNewAction("报点/填写日志", 1, 0), &QAction::triggered, [&]() {
					PointReport* pPointReport = new PointReport;
					pPointReport->setAttribute(Qt::WA_DeleteOnClose);
					pPointReport->exec();
				});

				connect(pMenuBarInfo2->addNewAction("现存车管理", 1, 1), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo2->addNewAction("确报查询", 1, 2), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo2->addNewAction("列控区间占用逻辑检查", 1, 3), &QAction::triggered, [&]() {});
			}

			MenuBarInfo* pMenuBarInfo3 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo3);
			pMenuBarInfo3->m_pMenu = m_pMenuBar->addMenu("调度命令(D)"); {
				connect(pMenuBarInfo3->addNewAction("调度命令管理", 1, 0), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("调度命令查询", 1, 1), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("行车凭证管理", 1, 2), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("行车凭证查询", 1, 3), &QAction::triggered, [&]() {});
			}

			MenuBarInfo* pMenuBarInfo4 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo4);
			pMenuBarInfo4->m_pMenu = m_pMenuBar->addMenu("站场显示(X)"); {
				connect(pMenuBarInfo4->addNewAction("刷新", 1, 0), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo4->addNewAction("放大", 1, 1), &QAction::triggered, [&]() {
					MainStation()->setDiploid(DiploidOperate::amplify, STAVIEW);
				});
				connect(pMenuBarInfo4->addNewAction("缩小", 1, 2), &QAction::triggered, [&]() {
					MainStation()->setDiploid(DiploidOperate::reduce, STAVIEW);
				});
				connect(pMenuBarInfo4->addNewAction("恢复", 1, 3), &QAction::triggered, [&]() {
					MainStation()->setDiploid(DiploidOperate::revert, STAVIEW);
				});
				connect(pMenuBarInfo4->addNewAction("车次放大", 1, 4), &QAction::triggered, [&]() {
					MainStation()->setDiploid(DiploidOperate::amplify, TRAINNUM); 
				});
				connect(pMenuBarInfo4->addNewAction("车次缩小", 1, 5), &QAction::triggered, [&]() {
					MainStation()->setDiploid(DiploidOperate::reduce, TRAINNUM);
				});
				connect(pMenuBarInfo4->addNewAction("车次恢复", 1, 6), &QAction::triggered, [&]() {
					MainStation()->setDiploid(DiploidOperate::revert, TRAINNUM);
				});
				connect(pMenuBarInfo4->addNewAction("放置提示圈", 1, 7), &QAction::triggered, [&]() {

				});
			}

			MenuBarInfo* pMenuBarInfo5 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo5);
			pMenuBarInfo5->m_pMenu = m_pMenuBar->addMenu("控制命令(C)"); {
				pMenuBarInfo5->addNewAction("进路命令", 1, 0); {
					MenuInfo* pMenuInfo = pMenuBarInfo5->m_vecSubMenuInfo[0];
					connect(pMenuInfo->addNewSubAction("排列进路", 2, 0), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::RouteBuild);
					});
					connect(pMenuInfo->addNewSubAction("总取消", 2, 1), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::RouteBuild); 
					});
					connect(pMenuInfo->addNewSubAction("总人解", 2, 2), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::TotalRelieve);
					});
					connect(pMenuInfo->addNewSubAction("信号重开", 2, 3), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::SignalReopen);
					});
					connect(pMenuInfo->addNewSubAction("引导", 2, 4), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::GuideBtn);
					});
					connect(pMenuInfo->addNewSubAction("故障解锁", 2, 5), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::RegionRelieve);
					});
					connect(pMenuInfo->addNewSubAction("坡道解锁", 2, 6), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::RampUnlock);
					});
				}
				pMenuBarInfo5->addNewAction("道岔命令", 1, 1); {
					MenuInfo* pMenuInfo = pMenuBarInfo5->m_vecSubMenuInfo[1];
					connect(pMenuInfo->addNewSubAction("道岔总定", 2, 0), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::TotalPosition);
					});
					connect(pMenuInfo->addNewSubAction("道岔总反", 2, 1), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::TotalReverse);
					});
					connect(pMenuInfo->addNewSubAction("道岔单锁", 2, 2), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::SingleLock);
					});
					connect(pMenuInfo->addNewSubAction("道岔解锁", 2, 3), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::SingleUnlock);
					});
					connect(pMenuInfo->addNewSubAction("道岔单封", 2, 4), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::Blockade);
					});
					connect(pMenuInfo->addNewSubAction("道岔解封", 2, 5), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::UnBlockade);
					});
				}

				pMenuBarInfo5->addNewAction("信号机命令", 1, 2); {
					MenuInfo* pMenuInfo = pMenuBarInfo5->m_vecSubMenuInfo[2];
					connect(pMenuInfo->addNewSubAction("按钮(信号)加封", 2, 0), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::Blockade);
					});
					connect(pMenuInfo->addNewSubAction("按钮(信号)解封", 2, 1), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::UnBlockade);
					});
					connect(pMenuInfo->addNewSubAction("点灯", 2, 2), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::Lighting);
					});
					connect(pMenuInfo->addNewSubAction("灭灯", 2, 3), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::UnLighting);
					});
				}
				connect(pMenuBarInfo5->addNewAction("设置引导总锁", 1, 3), &QAction::triggered, [&]() {
					m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::GuideClock);
				});
				connect(pMenuBarInfo5->addNewAction("取消引导总锁", 1, 4), &QAction::triggered, [&]() {
					m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::GuideClock);
				});
				connect(pMenuBarInfo5->addNewAction("上电解锁", 1, 5), &QAction::triggered, [&]() {
					
				});
				connect(pMenuBarInfo5->addNewAction("功能按钮", 1, 6), &QAction::triggered, [&]() {
					m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::FunBtn);
				});

				pMenuBarInfo5->addNewAction("封锁/解封", 1, 7); {
					MenuInfo* pMenuInfo = pMenuBarInfo5->m_vecSubMenuInfo[7];
					connect(pMenuInfo->addNewSubAction("封锁", 2, 0), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::Blockade);
					});
					connect(pMenuInfo->addNewSubAction("解封", 2, 1), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::UnBlockade);
					});
				}

				pMenuBarInfo5->addNewAction("停电/供电", 1, 8); {
					MenuInfo* pMenuInfo = pMenuBarInfo5->m_vecSubMenuInfo[8];
					connect(pMenuInfo->addNewSubAction("停电", 2, 0), &QAction::triggered, [&]() {

					});
					connect(pMenuInfo->addNewSubAction("供电", 2, 1), &QAction::triggered, [&]() {

					});
				}

				pMenuBarInfo5->addNewAction("分路不良", 1, 9); {
					MenuInfo* pMenuInfo = pMenuBarInfo5->m_vecSubMenuInfo[9];
					connect(pMenuInfo->addNewSubAction("设置分路不良", 2, 0), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::PoorRoute);
					});
					connect(pMenuInfo->addNewSubAction("取消分路不良", 2, 1), &QAction::triggered, [&]() {

					});
					connect(pMenuInfo->addNewSubAction("确认空闲", 2, 2), &QAction::triggered, [&]() {
						m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::IdleConfirm);
					});
					connect(pMenuInfo->addNewSubAction("不能确认空闲", 2, 3), &QAction::triggered, [&]() {

					});
				}

				pMenuBarInfo5->addNewAction("防溜", 1, 10); {
					MenuInfo* pMenuInfo = pMenuBarInfo5->m_vecSubMenuInfo[10];
					connect(pMenuInfo->addNewSubAction("设置防溜", 2, 0), &QAction::triggered, [&]() {
					});
					connect(pMenuInfo->addNewSubAction("取消防溜", 2, 1), &QAction::triggered, [&]() {
					});
				}

				connect(pMenuBarInfo5->addNewAction("统一设置封锁", 1, 11), &QAction::triggered, [&]() {

				});
				connect(pMenuBarInfo5->addNewAction("统一设置停供电", 1, 12), &QAction::triggered, [&]() {

				});
				connect(pMenuBarInfo5->addNewAction("命令下达", 1, 13), &QAction::triggered, [&]() {
					m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::CommandIssued);
				});
				connect(pMenuBarInfo5->addNewAction("命令清除", 1, 14), &QAction::triggered, [&]() {
					m_pStationCtrl->StaFunBtnBar()->ButtonClicked(FunType::CommandClear);
				});
			}
			
			this->setMenuBar(m_pMenuBar);
		}

		void StationViewTKY::InitStationViewToolBar()
		{
			qDebug() << "InitStationViewToolBarTKY";
			m_pStationViewToolBar = new QToolBar(this);
			connect(AddToolBarBtn(":/CTCProject/icon/TKY/log.png", "行车日志", STAVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::TurnToTrafficLogDisp);
			connect(AddToolBarBtn(":/CTCProject/icon/TKY/staTable.png", "进路序列窗口", STAVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::ShowStaRoutePlanWnd);
			connect(AddToolBarBtn(":/CTCProject/icon/TKY/set.png", "显示设置", STAVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::ShowVisibleSetWnd);
			connect(AddToolBarBtn(":/CTCProject/icon/TKY/enlarge", "放大", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				setMouseState(MouseState::Amplify); 
			});
			connect(AddToolBarBtn(":/CTCProject/icon/TKY/narrow.png", "缩小", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				setMouseState(MouseState::Reduce);
			});
			connect(AddToolBarBtn(":/CTCProject/icon/TKY/revert.png", "还原", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				MainStation()->setDiploid(DiploidOperate::revert, STAVIEW);
			});
			connect(AddToolBarBtn(":/CTCProject/icon/TKY/textEnlarge", "车次窗放大", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				MainStation()->setDiploid(DiploidOperate::amplify, TRAINNUM);
			});
			connect(AddToolBarBtn(":/CTCProject/icon/TKY/textNarrow.png", "车次窗缩小", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				MainStation()->setDiploid(DiploidOperate::reduce, TRAINNUM);
			});
			connect(AddToolBarBtn(":/CTCProject/icon/TKY/textRevert.png", "车次窗还原", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				MainStation()->setDiploid(DiploidOperate::revert, TRAINNUM);
			});
			connect(AddToolBarBtn(":/CTCProject/icon/TKY/staView.png", "单站界面", STAVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::TurnToStationCtrlDisp);
			connect(AddToolBarBtn(":/CTCProject/icon/TKY/stas.png", "站间透明", STAVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::TurnToStationMultiDisp);
			addToolBar(m_pStationViewToolBar);
		}

		void StationViewTKY::InitTrafficLogToolBar()
		{
			qDebug() << "InitTrafficLogToolBarTKY";
			m_pTrafficLogToolBar = m_pStationViewToolBar;
		}

		void StationViewTKY::InitSignForToolBar()
		{
			qDebug() << "InitSignForToolBarTKY";
			m_pSignForToolBar = new QToolBar(this);
			m_pSignForToolBar->setStyleSheet("QPushButton{background-color: rgb(210, 210, 210);}"
				"QLabel{background-color: rgb(240, 240, 240);}");
			//调度命令
			QPushButton* pDispatBtn = new QPushButton(m_pSignForToolBar);
			pDispatBtn->setText("调度命令");
			pDispatBtn->setObjectName("dispatchOrderBtn");
			pDispatBtn->setFixedSize(96, 28);
			connect(pDispatBtn, &QPushButton::clicked, this, &StationViewTKY::ShowDispatchOrderWnd);
			m_pSignForToolBar->addWidget(pDispatBtn);

			//间隔
			QLabel* pLabel = new QLabel(m_pSignForToolBar);
			pLabel->setFrameShape(QFrame::WinPanel);
			pLabel->setFrameShadow(QFrame::Sunken);
			pLabel->setFixedSize(160, 28);
			m_pSignForToolBar->addWidget(pLabel);
			//addToolBar(m_pSignForToolBar);
			insertToolBar(m_pStationViewToolBar, m_pSignForToolBar);
		}

		void StationViewTKY::InitStateToolBar()
		{

			qDebug() << "InitStateToolBarTKY";
			m_pStateToolBar = new QToolBar(this);
			m_pStateToolBar->setStyleSheet("QLabel{ background-color: rgb(0, 0, 0); }");
			m_pStateToolBar->layout()->setSpacing(2);
			//备机状态
			QLabel* pBackupLabel = new QLabel(m_pStateToolBar);
			pBackupLabel->setFrameShape(QFrame::WinPanel);
			pBackupLabel->setFrameShadow(QFrame::Sunken);
			pBackupLabel->setFont(QFont("微软雅黑", 11));
			pBackupLabel->setAlignment(Qt::AlignCenter);
			pBackupLabel->setText("备机状态");
			pBackupLabel->setObjectName("centerComLabel");
			pBackupLabel->setFixedSize(180, 28);
			pBackupLabel->setStyleSheet("QLabel{ color: yellow; }");
			m_pStateToolBar->addWidget(pBackupLabel);
			//控制状态
			QLabel* pControlLabel = new QLabel(m_pStateToolBar);
			pControlLabel->setFrameShape(QFrame::WinPanel);
			pControlLabel->setFrameShadow(QFrame::Sunken);
			pControlLabel->setFont(QFont("微软雅黑", 11));
			pControlLabel->setAlignment(Qt::AlignCenter);
			pControlLabel->setText("控制状态");
			pControlLabel->setObjectName("centerComLabel");
			pControlLabel->setFixedSize(180, 28);
			pControlLabel->setStyleSheet("QLabel{ color: green; }");
			m_pStateToolBar->addWidget(pControlLabel);
			//操作类型
			QLabel* pOperaTypeLabel = new QLabel(m_pStateToolBar);
			pOperaTypeLabel->setFrameShape(QFrame::WinPanel);
			pOperaTypeLabel->setFrameShadow(QFrame::Sunken);
			pOperaTypeLabel->setFont(QFont("微软雅黑", 11));
			pOperaTypeLabel->setAlignment(Qt::AlignCenter);
			pOperaTypeLabel->setText("排列进路");
			pOperaTypeLabel->setObjectName("centerComLabel");
			pOperaTypeLabel->setFixedSize(180, 28);
			pOperaTypeLabel->setStyleSheet("QLabel{ color: green; }");
			m_pStateToolBar->addWidget(pOperaTypeLabel);

			addToolBar(m_pStateToolBar);
		}
	}
}
