#include "StationViewKSK.h"
#include "../StationViewTKY/StationViewTKY.h"
#include "../BaseWndClass/StationView/StationCtrlDisp.h"
#include "StationView/StationCtrlDispKSK.h"
#include "StationView/StationMultiDispKSK.h"
#include "StationView/StationLogDispKSK.h"
#include "ModuleWidget/StaRoutePlanKSK.h"
#include "ModuleWidget/StaDispatchOrderKSK.h"
#include "ModuleWidget/StaVisibleSetKSK.h"
#include "Global.h"
#include <QDebug>
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace CASCO {
		using namespace Station;
		StationViewKSK::StationViewKSK(QWidget* parent)
			: CTCMainWindow(parent)
		{
			ui.setupUi(this);
		}

		StationViewKSK::~StationViewKSK()
		{
		
		}

		StationViewKSK* StationViewKSK::CreatStationView(QWidget* parent)
		{
			return new StationViewKSK(parent);
		}

		void StationViewKSK::InitbottomTrafficLogToolBar()
		{
		}

		BaseWnd::StationCtrlDisp* StationViewKSK::CreateStationCtrlDisp()
		{
			return new StationCtrlDispKSK(this);
		}

		BaseWnd::StationMultiDisp* StationViewKSK::CreateMultiStationDisp()
		{
			return new StationMultiDispKSK(this);
		}

		BaseWnd::StationLogDisp* StationViewKSK::CreateTrafficLogDisp()
		{
			return new StationLogDispKSK(this);
		}

		BaseWnd::StaRoutePlan* StationViewKSK::CreateStaRoutePlanWnd()
		{
			return new StaRoutePlanKSK(this);
		}

		BaseWnd::StaDispatchOrder* StationViewKSK::CreateStaDispatchOrder()
		{
			return new StaDispatchOrderKSK(this);
		}

		BaseWnd::StaVisibleSet* StationViewKSK::CreateStaVisibleSet()
		{
			return new StaVisibleSetKSK(this);
		} 

		void StationViewKSK::InitStationViewMenuBar()
		{
			qDebug() << "InitStationViewMenuBarTKY";
			m_pMenuBar = new QMenuBar(this);

			MenuBarInfo* pMenuBarInfo1 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo1);
			pMenuBarInfo1->m_pMenu = m_pMenuBar->addMenu("登录(L)"); {
				connect(pMenuBarInfo1->addNewAction("用户登录", 1, 0), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo1->addNewAction("用户注销", 1, 1), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo1->addNewAction("退出", 1, 2), &QAction::triggered, [&]() {});
			}

			MenuBarInfo* pMenuBarInfo2 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo2);
			pMenuBarInfo2->m_pMenu = m_pMenuBar->addMenu("显示(S)"); {
				pMenuBarInfo2->addNewAction("工具栏", 1, 0); {
					MenuInfo* pMenuInfo = pMenuBarInfo2->m_vecSubMenuInfo[0];
					connect(pMenuInfo->addNewSubAction("标准按钮", 2, 0, true, true, true), &QAction::triggered, [=](bool bChecked) {
						m_pStationViewToolBar->setHidden(!bChecked);
						m_pTrafficLogToolBar->setHidden(!bChecked);
					});
					connect(pMenuInfo->addNewSubAction("签收栏", 2, 1, true, true, true), &QAction::triggered, [=](bool bChecked) {
						m_pSignForToolBar->setHidden(!bChecked);
						m_pStateToolBar->setHidden(!bChecked);
					});
					connect(pMenuInfo->addNewSubAction("显示系统信息窗口", 2, 2, true, true, false), &QAction::triggered, [&](bool bChecked) {});
					connect(pMenuInfo->addNewSubAction("显示进路序列管理窗口", 2, 3, true, true, true), &QAction::triggered, [=](bool bChecked) {
						ShowStaRoutePlanWnd(bChecked);
						m_pRoutePlanAction = pMenuBarInfo2->getSubActionByIndex(2, 0, 3)->m_pAction;
					});
				}
				pMenuBarInfo2->addNewAction("站场图", 1, 1); {
					MenuInfo* pMenuInfo = pMenuBarInfo2->m_vecSubMenuInfo[1];
					connect(pMenuInfo->addNewSubAction("放大站场图", 2, 0), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::amplify, STAVIEW); 
					});
					connect(pMenuInfo->addNewSubAction("缩小站场图", 2, 1), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::reduce, STAVIEW); 
					});
					connect(pMenuInfo->addNewSubAction("还原站场图", 2, 2, false), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::revert, STAVIEW); 
					});
					m_stuMenuSyncAction.m_pStaRevertToolAction = pMenuBarInfo2->getSubActionByIndex(2, 1, 2)->m_pAction;
					connect(pMenuInfo->addNewSubAction("放大车次窗", 2, 3), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::amplify, TRAINNUM); 
					});
					connect(pMenuInfo->addNewSubAction("缩小车次窗", 2, 4), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::reduce, TRAINNUM); 
					});
					connect(pMenuInfo->addNewSubAction("还原车次窗", 2, 5, false), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::revert, TRAINNUM); 
					});
					m_stuMenuSyncAction.m_pStaRevertToolAction = pMenuBarInfo2->getSubActionByIndex(2, 1, 5)->m_pAction;
					connect(pMenuInfo->addNewSubAction("文字显示", 2, 6), &QAction::triggered, this, &CTCMainWindow::ShowVisibleSetWnd);
					connect(pMenuInfo->addNewSubAction("绝缘节显示", 2, 7, true, true, true), &QAction::triggered, [&](bool bChecked) {
						MainStation()->SetVisible(VisibleDev::insulationNode, bChecked); 
					});
					connect(pMenuInfo->addNewSubAction("列车按钮显示", 2, 8, true, true, true), &QAction::triggered, [&](bool bChecked) {
						MainStation()->SetVisible(VisibleDev::trainButton, bChecked); 
					});
					connect(pMenuInfo->addNewSubAction("调车按钮显示", 2, 9, true, true, true), &QAction::triggered, [&](bool bChecked) {
						MainStation()->SetVisible(VisibleDev::shuntButton, bChecked); 
					});
					connect(pMenuInfo->addNewSubAction("进路序列窗显示", 2, 10, true, true, true), &QAction::triggered, [&](bool bChecked) {
						MainStation()->SetVisible(VisibleDev::preview, bChecked); 
					});
					connect(pMenuInfo->addNewSubAction("显示状态条", 2, 6, false, true, true), &QAction::triggered, [&]() {});
					connect(pMenuInfo->addNewSubAction("调车进路显示", 2, 6, false, true, true), &QAction::triggered, [&]() {});
					connect(pMenuInfo->addNewSubAction("道岔位置", 2, 6, false, true, true), &QAction::triggered, [&]() {});
				}
				pMenuBarInfo2->addSeparator();
				pMenuBarInfo2->addNewAction("行车日志", 1, 2); {
					MenuInfo* pMenuInfo = pMenuBarInfo2->m_vecSubMenuInfo[2];
					connect(pMenuInfo->addNewSubAction("显示表头", 2, 0, true, true, true), &QAction::triggered, 
						m_pStationLog, &BaseWnd::StationLogDisp::ShowTableHead);
					connect(pMenuInfo->addNewSubAction("显示邻站", 2, 1, false), &QAction::triggered, [&]() {});
					pMenuBarInfo2->m_pMenu->addSeparator();
					connect(pMenuInfo->addNewSubAction("扩大", 2, 2, false), &QAction::triggered, [&]() {});
					connect(pMenuInfo->addNewSubAction("缩小", 2, 3, false), &QAction::triggered, [&]() {});
				}
			}
			
			MenuBarInfo* pMenuBarInfo3 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo3);
			pMenuBarInfo3->m_pMenu = m_pMenuBar->addMenu("工具(U)"); {
				connect(pMenuBarInfo3->addNewAction("调度命令管理", 1, 0), &QAction::triggered, this, &CTCMainWindow::ShowDispatchOrderWnd);
				connect(pMenuBarInfo3->addNewAction("甩挂作业", 1, 1, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("列车编组(速报表)", 1, 2), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("站存车(现存车)", 1, 3), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("列控命令", 1, 4, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("车站直接限速", 1, 5), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("运统46", 1, 6), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("战场图回放", 1, 7, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("列控区间占用逻辑检查", 1, 8), &QAction::triggered, [&]() {});
				pMenuBarInfo3->addSeparator();
				connect(pMenuBarInfo3->addNewAction("用户管理", 1, 9, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("选项", 1, 10), &QAction::triggered, [&]() {});
				pMenuBarInfo3->addSeparator();
				connect(pMenuBarInfo3->addNewAction("确报查询", 1, 11, false), &QAction::triggered, [&]() {});
			}
			

			MenuBarInfo* pMenuBarInfo5 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo5);
			pMenuBarInfo5->m_pMenu = m_pMenuBar->addMenu("调试(D)"); {
				connect(pMenuBarInfo5->addNewAction("刷新站场图", 1, 0, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo5->addNewAction("测试调度命令", 1, 1, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo5->addNewAction("发送XML信息", 1, 2, false), &QAction::triggered, [&]() {});
				pMenuBarInfo5->addSeparator();
				connect(pMenuBarInfo5->addNewAction("语音静音", 1, 3), &QAction::triggered, [&]() {});
			}

			MenuBarInfo* pMenuBarInfo6 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo6);
			pMenuBarInfo6->m_pMenu = m_pMenuBar->addMenu("窗口(W)"); {
				connect(pMenuBarInfo6->addNewAction("层叠窗口", 1, 0, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo6->addNewAction("显示调整", 1, 1, false), &QAction::triggered, [&]() {});
				pMenuBarInfo6->addSeparator();
				connect(pMenuBarInfo6->addNewAction("调车作业单", 1, 2, false), &QAction::triggered, [&]() {});
				pMenuBarInfo6->addSeparator();
				connect(pMenuBarInfo6->addNewAction("站场平面", 1, 3, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo6->addNewAction("行车日志", 1, 4, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo6->addNewAction("规章制度", 1, 5, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo6->addNewAction("信号员控制面板", 1, 6, false), &QAction::triggered, [&]() {});
			}
			

			MenuBarInfo* pMenuBarInfo7 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo7);
			pMenuBarInfo7->m_pMenu = m_pMenuBar->addMenu("帮助(H)"); {
				connect(pMenuBarInfo7->addNewAction("关于CTC-STPC", 1, 0), &QAction::triggered, [&]() {});
			}
			
			setMenuBar(m_pMenuBar);
		}

		void StationViewKSK::InitStationViewToolBar()
		{
			qDebug() << "InitStationViewToolBarTKY";
			m_pStationViewToolBar = new QToolBar(this);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/multi.png", "站间透明", STAVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::TurnToStationMultiDisp);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/station.png", "单站界面", STAVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::TurnToStationCtrlDisp);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/textSet.png", "显示设置", STAVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::ShowVisibleSetWnd);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/nodes.png", "显示绝缘节", STAVIEW_TOOL, true, true), &QPushButton::clicked, [&](bool bChecked) {
				MainStation()->SetVisible(VisibleDev::insulationNode, bChecked);
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/trainBtn.png", "显示列车按钮", STAVIEW_TOOL, true, true), &QPushButton::clicked, [&](bool bChecked) {
				MainStation()->SetVisible(VisibleDev::trainButton, bChecked); 
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/shuntBtn.png", "显示调车按钮", STAVIEW_TOOL, true, true), &QPushButton::clicked, [&](bool bChecked) {
				MainStation()->SetVisible(VisibleDev::shuntButton, bChecked); 
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/trainNum.png", "显示车次窗", STAVIEW_TOOL, true), &QPushButton::clicked, [&](bool bChecked) {
				MainStation()->SetVisible(VisibleDev::trainNumWnd, bChecked);
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/forecast.png", "显示车次预告窗", STAVIEW_TOOL, true, true), &QPushButton::clicked, [&](bool bChecked) {
				MainStation()->SetVisible(VisibleDev::preview, bChecked);
			});
			AddToolBarSeparator(STAVIEW_TOOL);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/text.png", "文字标注", STAVIEW_TOOL), &QPushButton::clicked, [&]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/flash.png", "终端按钮闪烁", STAVIEW_TOOL), &QPushButton::clicked, [&]() {});
			AddToolBarSeparator(STAVIEW_TOOL);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/amplify.png", "放大", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				setMouseState(MouseState::Amplify); 
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/reduce.png", "缩小", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				setMouseState(MouseState::Reduce); 
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/revert.png", "还原", STAVIEW_TOOL, false, false, false, ":/CTCProject/icon/CASCO/revert2.png"), &QPushButton::clicked, [=]() {
				MainStation()->setDiploid(DiploidOperate::revert, STAVIEW); 
			});
			m_stuMenuSyncAction.m_pStaRevertBtn = m_pStationViewToolBar->findChild<QPushButton*>("revert");
			AddToolBarSeparator(STAVIEW_TOOL);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/textAmplify", "车次窗放大", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				MainStation()->setDiploid(DiploidOperate::amplify, TRAINNUM); 
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/textReduce.png", "车次窗缩小", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				MainStation()->setDiploid(DiploidOperate::reduce, TRAINNUM); 
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/textRevert.png", "车次窗还原", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				MainStation()->setDiploid(DiploidOperate::revert, TRAINNUM); 
			});
			AddToolBarSeparator(STAVIEW_TOOL);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/find.png", "查找车次", STAVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/return.png", "行车日志", STAVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::TurnToTrafficLogDisp);
			addToolBar(m_pStationViewToolBar);
			addToolBarBreak();
		}

		void StationViewKSK::InitTrafficLogToolBar()
		{
			qDebug() << "InitTrafficLogToolBarTKY";
			m_pTrafficLogToolBar = new QToolBar(this);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/print.png", "打印", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/preview.png", "打印预览", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/diagram.png", "运行图", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/newTrain.png", "新增车辆", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/user.png", "用户管理", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/history.png", "历史报表查询", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/view.png", "视图", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/upward.png", "显示上行车次", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/downward.png", "显示下行车次", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/allTrain.png", "显示上下行车次", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/agree.png", "同意邻站发车", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/adjacent.png", "邻站同意发车", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/plan.png", "会让计划", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/findLog.png", "查找车次", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			AddToolBarSeparator(LOGVIEW_TOOL);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/return.png", "返回到站场图", LOGVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::TurnToStationCtrlDisp);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/shunt.png", "调车作业单", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/help.png", "关于", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			addToolBar(m_pTrafficLogToolBar);
			addToolBarBreak();
		}

		void StationViewKSK::InitSignForToolBar()
		{
			qDebug() << "InitSignForToolBarTKY";
			m_pSignForToolBar = new QToolBar(this);
			m_pSignForToolBar->setStyleSheet("QPushButton{background-color: rgb(210, 210, 210);}"
				"QLabel{background-color: rgb(240, 240, 240);}");

			//调度命令
			QPushButton* pDispatBtn = new QPushButton(m_pSignForToolBar);
			pDispatBtn->setText("调度命令");
			pDispatBtn->setObjectName("dispatchBtn");
			pDispatBtn->setFixedSize(96, 28);
			connect(pDispatBtn, &QPushButton::clicked, this, &CTCMainWindow::ShowDispatchOrderSignWnd);
			m_pSignForToolBar->addWidget(pDispatBtn);
			//间隔
			QLabel* pLabel = new QLabel(m_pSignForToolBar);
			pLabel->setFrameShape(QFrame::WinPanel);
			pLabel->setFrameShadow(QFrame::Sunken);
			pLabel->setFixedSize(144, 28);
			m_pSignForToolBar->addWidget(pLabel);
			
			addToolBar(m_pSignForToolBar);
		}

		void StationViewKSK::InitStateToolBar()
		{
			qDebug() << "InitStateToolBarTKY";
			m_pStateToolBar = new QToolBar(this);
			m_pStateToolBar->setStyleSheet("QPushButton{background-color: rgb(210, 210, 210);}"
				"QLabel{background-color: rgb(240, 240, 240);}");

			//占线板
			QPushButton* pDispatBtn = new QPushButton(m_pStateToolBar);
			pDispatBtn->setText("占线板(F9)");
			pDispatBtn->setObjectName("boardBtn");
			pDispatBtn->setFixedSize(96, 28);
			connect(pDispatBtn, &QPushButton::clicked, []() {});
			m_pStateToolBar->addWidget(pDispatBtn);
			pDispatBtn->hide();
			//阶段计划
			QPushButton* pStagePlanBtn = new QPushButton(m_pStateToolBar);
			pStagePlanBtn->setText("阶段计划");
			pStagePlanBtn->setObjectName("stagePlanBtn");
			pStagePlanBtn->setFixedSize(96, 28);
			connect(pStagePlanBtn, &QPushButton::clicked, this, &CTCMainWindow::ShowStagePlanSignWnd);
			m_pStateToolBar->addWidget(pStagePlanBtn);
			//发送计划
			QPushButton* pSendPlanBtn = new QPushButton(m_pStateToolBar);
			pSendPlanBtn->setText("发送计划");
			pSendPlanBtn->setObjectName("sendPlanBtn");
			pSendPlanBtn->setFixedSize(96, 28);
			connect(pSendPlanBtn, &QPushButton::clicked, []() {});
			m_pStateToolBar->addWidget(pSendPlanBtn);
			//阶段记事
			QPushButton* pStageNotesBtn = new QPushButton(m_pStateToolBar);
			pStageNotesBtn->setText("阶段记事");
			pStageNotesBtn->setObjectName("stageNotesBtn");
			pStageNotesBtn->setFixedSize(96, 28);
			connect(pStageNotesBtn, &QPushButton::clicked, []() {});
			m_pStateToolBar->addWidget(pStageNotesBtn);
			//时间显示
			QLabel* pTimeLabel = new QLabel(m_pStateToolBar);
			pTimeLabel->setFrameShape(QFrame::WinPanel);
			pTimeLabel->setFrameShadow(QFrame::Sunken);
			pTimeLabel->setFont(QFont("微软雅黑", 11));
			pTimeLabel->setAlignment(Qt::AlignCenter);
			pTimeLabel->setText(QDateTime::currentDateTime().toString("yyyy-mm-dd hh:MM:ss"));
			pTimeLabel->setObjectName("timeLabel");
			pTimeLabel->setFixedSize(180, 28);
			m_pStateToolBar->addWidget(pTimeLabel);
			//与同步站机通信
			QLabel* pSyncComLabel = new QLabel(m_pStateToolBar);
			pSyncComLabel->setFrameShape(QFrame::WinPanel);
			pSyncComLabel->setFrameShadow(QFrame::Sunken);
			pSyncComLabel->setFont(QFont("微软雅黑", 11));
			pSyncComLabel->setAlignment(Qt::AlignCenter);
			pSyncComLabel->setText("与同步站机通信中断");
			pSyncComLabel->setObjectName("centerComLabel");
			pSyncComLabel->setFixedSize(180, 28);
			m_pStateToolBar->addWidget(pSyncComLabel);
			//与中心通信
			QLabel* pCenterComLabel = new QLabel(m_pStateToolBar);
			pCenterComLabel->setFrameShape(QFrame::WinPanel);
			pCenterComLabel->setFrameShadow(QFrame::Sunken);
			pCenterComLabel->setFont(QFont("微软雅黑", 11));
			pCenterComLabel->setAlignment(Qt::AlignCenter);
			pCenterComLabel->setText("与中心通信中断");
			pCenterComLabel->setObjectName("centerComLabel");
			pCenterComLabel->setFixedSize(144, 28);
			m_pStateToolBar->addWidget(pCenterComLabel);
			//与车站服务器通信
			QLabel* pServerComLabel = new QLabel(m_pStateToolBar);
			pServerComLabel->setFrameShape(QFrame::WinPanel);
			pServerComLabel->setFrameShadow(QFrame::Sunken);
			pServerComLabel->setFont(QFont("微软雅黑", 11));
			pServerComLabel->setAlignment(Qt::AlignCenter);
			pServerComLabel->setText("与车站服务器通信中断");
			pServerComLabel->setObjectName("serverComLabel");
			pServerComLabel->setFixedSize(180, 28);
			m_pStateToolBar->addWidget(pServerComLabel);

			addToolBar(m_pStateToolBar);
		}

	}
}
