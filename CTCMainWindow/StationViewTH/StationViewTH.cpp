#include "StationViewTH.h"
#include "../StationViewTKY/StationViewTKY.h"
#include "../BaseWndClass/StationView/StationCtrlDisp.h"
#include "StationView/StationCtrlDispTH.h"
#include "StationView/StationMultiDispTH.h"
#include "StationViewKSK/StationView/StationLogDispKSK.h"
#include "ModuleWidget/StaRoutePlanTH.h"
#include "StationViewKSK/ModuleWidget/StaDispatchOrderKSK.h"
#include "StationViewKSK/ModuleWidget/StaVisibleSetKSK.h"
#include "Global.h"
#include <QDebug>
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace TH {
		using namespace Station;
		StationViewTH::StationViewTH(QWidget* parent)
			: CTCMainWindow(parent)
		{
			ui.setupUi(this);
		}

		StationViewTH::~StationViewTH()
		{}

		StationViewTH* StationViewTH::CreatStationView(QWidget* parent)
		{
			return new StationViewTH(parent);
		}

		BaseWnd::StaTrainDiagramWidget* StationViewTH::CreateStaTrainDiagramWidget()
		{
			return nullptr;
		}


		BaseWnd::StationCtrlDisp* StationViewTH::CreateStationCtrlDisp()
		{
			return new StationCtrlDispTH(this);
		}

		BaseWnd::StationMultiDisp* StationViewTH::CreateMultiStationDisp()
		{
			return new StationMultiDispTH(this);
		}

		BaseWnd::StationLogDisp* StationViewTH::CreateTrafficLogDisp()
		{
			return new CASCO::StationLogDispKSK(this);
		}

		BaseWnd::StaRoutePlan* StationViewTH::CreateStaRoutePlanWnd()
		{
			return new StaRoutePlanTH(this);
		}

		BaseWnd::StaDispatchOrder* StationViewTH::CreateStaDispatchOrder()
		{
			return new CASCO::StaDispatchOrderKSK(this);
		}

		BaseWnd::StaVisibleSet* StationViewTH::CreateStaVisibleSet()
		{
			return new CASCO::StaVisibleSetKSK(this);
		}

		void StationViewTH::InitStationViewMenuBar()
		{
			qDebug() << "InitStationViewMenuBarTKY";
			m_pMenuBar = new QMenuBar(this);

			MenuBarInfo* pMenuBarInfo1 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo1);
			pMenuBarInfo1->m_pMenu = m_pMenuBar->addMenu("系统(L)"); {
				connect(pMenuBarInfo1->addNewAction("登录", 1, 0), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo1->addNewAction("注销", 1, 1), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo1->addNewAction("统一登录和注销", 1, 2), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo1->addNewAction("退出", 1, 3), &QAction::triggered, [&]() {});
			}
			
			MenuBarInfo* pMenuBarInfo2 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo2);
			pMenuBarInfo2->m_pMenu = m_pMenuBar->addMenu("区段选择"); {
				connect(pMenuBarInfo2->addNewAction(MainStation()->getStationName(), 1, 0), &QAction::triggered, [&]() {});
			}
			
			MenuBarInfo* pMenuBarInfo3 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo3);
			pMenuBarInfo3->m_pMenu = m_pMenuBar->addMenu("车站选择"); {
				connect(pMenuBarInfo3->addNewAction(QString("区段视图:%1").arg(MainStation()->getStationName()), 1, 0), &QAction::triggered, [&]() {});
				pMenuBarInfo3->addSeparator();
				connect(pMenuBarInfo3->addNewAction(MainStation()->getStationName(), 1, 1, false, true, true), &QAction::triggered, [&]() {});
			}
			
			MenuBarInfo* pMenuBarInfo4 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo4);
			pMenuBarInfo4->m_pMenu = m_pMenuBar->addMenu("视图"); {
				connect(pMenuBarInfo4->addNewAction("实时报警信息", 1, 0), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo4->addNewAction("历史报警信息", 1, 1, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo4->addNewAction("进路序列", 1, 2, true, true, true), &QAction::triggered, [&](bool bChecked) {
					ShowStaRoutePlanWnd(bChecked);
				});
				pMenuBarInfo4->addNewAction("站场图缩放", 1, 3); {
					MenuInfo* pMenuInfo = pMenuBarInfo4->m_vecSubMenuInfo[3];
					connect(pMenuInfo->addNewAction("选择站场图", 2, 0), &QAction::triggered, [&]() {});
					connect(pMenuInfo->addNewAction("全景站场图", 2, 1), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::revert, STAVIEW); 
					});
					connect(pMenuInfo->addNewAction("放大站场图", 2, 2), &QAction::triggered, [&]() {
						setMouseState(MouseState::Amplify);
					});
					connect(pMenuInfo->addNewAction("缩小站场图", 2, 3), &QAction::triggered, [&]() {
						setMouseState(MouseState::Reduce); 
					});
					connect(pMenuInfo->addNewAction("缩放至整个站场图", 2, 3), &QAction::triggered, [&]() {});
					connect(pMenuInfo->addNewAction("还原站场图", 2, 3), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::revert, STAVIEW);
					});
				}
				pMenuBarInfo4->addNewAction("车次窗缩放", 1, 4); {
					MenuInfo* pMenuInfo = pMenuBarInfo4->m_vecSubMenuInfo[4];
					connect(pMenuInfo->addNewSubAction("放大车次窗", 2, 0), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::amplify, TRAINNUM);
					});
					connect(pMenuInfo->addNewSubAction("缩小车次窗", 2, 1), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::reduce, TRAINNUM);
					});
					connect(pMenuInfo->addNewSubAction("还原车次窗", 2, 2, false), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::revert, TRAINNUM);
					});
				}
			}

			MenuBarInfo* pMenuBarInfo5 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo5);
			pMenuBarInfo5->m_pMenu = m_pMenuBar->addMenu("显示选项"); {
			}

			MenuBarInfo* pMenuBarInfo6 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo6);
			pMenuBarInfo6->m_pMenu = m_pMenuBar->addMenu("功能"); {
			}

			MenuBarInfo* pMenuBarInfo7 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo7);
			pMenuBarInfo7->m_pMenu = m_pMenuBar->addMenu("窗口(W)"); {
				connect(pMenuBarInfo7->addNewAction("层叠窗口", 1, 0, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo7->addNewAction("显示调整", 1, 1, false), &QAction::triggered, [&]() {});
				pMenuBarInfo7->addSeparator();
				connect(pMenuBarInfo7->addNewAction("调车作业单", 1, 2, false), &QAction::triggered, [&]() {});
				pMenuBarInfo7->addSeparator();
				connect(pMenuBarInfo7->addNewAction("站场平面", 1, 3, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo7->addNewAction("行车日志", 1, 4, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo7->addNewAction("规章制度", 1, 5, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo7->addNewAction("信号员控制面板", 1, 6, false), &QAction::triggered, [&]() {});
			}
			
			MenuBarInfo* pMenuBarInfo8 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo8);
			pMenuBarInfo8->m_pMenu = m_pMenuBar->addMenu("帮助(H)"); {
				connect(pMenuBarInfo8->addNewAction("关于CTC-STPC", 1, 0), &QAction::triggered, [&]() {});
			}

			setMenuBar(m_pMenuBar);
		}

		void StationViewTH::InitStationViewToolBar()
		{
			qDebug() << "InitStationViewToolBarTKY";
			m_pStationViewToolBar = new QToolBar(this);
			connect(AddToolBarBtn(":/res/icon/", "选择站场图", STAVIEW_TOOL), &QPushButton::clicked, [&]() {});
			connect(AddToolBarBtn(":/res/icon/", "全景站场图", STAVIEW_TOOL), &QPushButton::clicked, [&]() {});
			connect(AddToolBarBtn(":/res/icon/enlarge", "放大站场图", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				setMouseState(MouseState::Amplify);
			});
			connect(AddToolBarBtn(":/res/icon/TKY/narrow.png", "缩小站场图", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				setMouseState(MouseState::Reduce);
			});
			connect(AddToolBarBtn(":/res/icon/TKY/narrow.png", "缩放至整个站场图", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				setMouseState(MouseState::Reduce);
			});
			connect(AddToolBarBtn(":/res/icon/TKY/revert.png", "还原站场图", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				MainStation()->setDiploid(DiploidOperate::revert, STAVIEW);
			});
			connect(AddToolBarBtn(":/res/icon/TKY/textEnlarge", "车次窗放大", STAVIEW_TOOL, true, true), &QPushButton::clicked, [=]() {
				MainStation()->setDiploid(DiploidOperate::amplify, TRAINNUM);
			});
			connect(AddToolBarBtn(":/res/icon/TKY/revert.png", "显示进路序列管理窗口", STAVIEW_TOOL), &QPushButton::clicked, [=](bool bChecked) {
				ShowStaRoutePlanWnd(bChecked);
			});
			
			connect(AddToolBarBtn(":/res/icon/TKY/staView.png", "单站界面", STAVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::TurnToStationCtrlDisp);
			connect(AddToolBarBtn(":/res/icon/TKY/stas.png", "站间透明", STAVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::TurnToStationMultiDisp);
			addToolBar(m_pStationViewToolBar);
			addToolBarBreak();
		}

		void StationViewTH::InitTrafficLogToolBar()
		{
			qDebug() << "InitTrafficLogToolBarTKY";
			m_pTrafficLogToolBar = m_pStationViewToolBar;
		}

		void StationViewTH::InitSignForToolBar()
		{
			qDebug() << "InitSignForToolBarTKY";
			m_pSignForToolBar = m_pStateToolBar;
		}

		void StationViewTH::InitStateToolBar()
		{
			qDebug() << "InitStateToolBarTKY";
			m_pStateToolBar = new QToolBar(this);
			m_pStateToolBar->setStyleSheet("QPushButton{background-color: rgb(210, 210, 210);}"
				"QLabel{background-color: rgb(240, 240, 240);}");

			//选路选排
			QPushButton* pRouteBtn = new QPushButton(m_pStateToolBar);
			pRouteBtn->setText("选路选排");
			pRouteBtn->setObjectName("routeBtn");
			pRouteBtn->setFixedHeight(36);
			pRouteBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			connect(pRouteBtn, &QPushButton::clicked, [&]() {});
			m_pStateToolBar->addWidget(pRouteBtn);

			//S道岔总锁
			QPushButton* pBlockBtnS = new QPushButton(m_pStateToolBar);
			pBlockBtnS->setText("S道岔总锁");
			pBlockBtnS->setObjectName("blockBtnS");
			pBlockBtnS->setFixedHeight(36);
			pBlockBtnS->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			connect(pBlockBtnS, &QPushButton::clicked, [&]() {});
			m_pStateToolBar->addWidget(pBlockBtnS);

			//X道岔总锁
			QPushButton* pBlockBtnX = new QPushButton(m_pStateToolBar);
			pBlockBtnX->setText("X道岔总锁");
			pBlockBtnX->setObjectName("blockBtnX");
			pBlockBtnX->setFixedHeight(36);
			pBlockBtnX->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			connect(pBlockBtnX, &QPushButton::clicked, [&]() {});
			m_pStateToolBar->addWidget(pBlockBtnX);

			//X道岔总锁
			QPushButton* pSlopeUnlock = new QPushButton(m_pStateToolBar);
			pSlopeUnlock->setText("坡道解锁");
			pSlopeUnlock->setObjectName("slopeUnlock");
			pSlopeUnlock->setFixedHeight(36);
			pSlopeUnlock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			connect(pSlopeUnlock, &QPushButton::clicked, [&]() {});
			m_pStateToolBar->addWidget(pSlopeUnlock);

			//撤销选中
			QPushButton* pUncheck = new QPushButton(m_pStateToolBar);
			pUncheck->setText("撤销选中");
			pUncheck->setObjectName("uncheck");
			pUncheck->setFixedHeight(36);
			pUncheck->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			connect(pUncheck, &QPushButton::clicked, [&]() {});
			m_pStateToolBar->addWidget(pUncheck);

			addToolBar(m_pStateToolBar);
		}

	}
}
