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
			installEventFilter(this);
			m_pCountdownLabel = new QLabel(this);
			m_pCountdownLabel->setFixedSize(48, 48);
			m_pCountdownLabel->setAlignment(Qt::AlignCenter);
			m_pCountdownLabel->move(16, 72);
			m_pCountdownLabel->setStyleSheet(
				"background-color: rgba(128,128,128,160);"
				"border: 2px solid rgba(128,128,128,200);;"
				"font-size: 15px;"
				"font-family: 宋体;"
				"color:red;"
			);
			m_pCountdownLabel->hide();

			m_nTimerId = startTimer(1000);
			
			m_pTimeLabel = new QLabel(this);
			m_pTimeLabel->setFixedSize(80, 48);
			m_pTimeLabel->setAlignment(Qt::AlignCenter);
			m_pTimeLabel->move(width() - 108, 72);
			m_pTimeLabel->setStyleSheet(
				"background-color: rgba(128,128,128,160);"
				"border: 2px solid rgba(128,128,128,200);;"
				"font-family: 宋体;"
				"font-size: 15px;"
				"color:white;"
			);
			
		}

		StationViewTKY::~StationViewTKY()
		{
			killTimer(m_nTimerId);
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

				connect(pMenuBarInfo2->addNewAction("现存车管理", 1, 1), &QAction::triggered, [&]() {
					if (m_pVehicleManage == nullptr) {
						m_pVehicleManage = new VehicleManage;
					}
					m_pVehicleManage->show();
					});
				connect(pMenuBarInfo2->addNewAction("确报查询", 1, 2), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo2->addNewAction("列控区间占用逻辑检查", 1, 3), &QAction::triggered, [&]() {
					if (m_pLntervallogic == nullptr) {
						m_pLntervallogic = new LntervallogicCheck;
					}
					m_pLntervallogic->show();

					});
			}

			MenuBarInfo* pMenuBarInfo3 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo3);
			pMenuBarInfo3->m_pMenu = m_pMenuBar->addMenu("调度命令(D)"); {
				connect(pMenuBarInfo3->addNewAction("调度命令管理", 1, 0), &QAction::triggered, [&]() {
					BaseWnd::StaDispatchOrder* pDispatchOrderWnd = CreateStaDispatchOrder();
					pDispatchOrderWnd->setAttribute(Qt::WA_DeleteOnClose);
					pDispatchOrderWnd->ViewPermission(Station::LimitsOfAuthority::admin);
					pDispatchOrderWnd->exec();
				});
				connect(pMenuBarInfo3->addNewAction("调度命令查询", 1, 1), &QAction::triggered, [&]() {
					BaseWnd::StaDispatchOrder* pDispatchOrderWnd = CreateStaDispatchOrder();
					pDispatchOrderWnd->setAttribute(Qt::WA_DeleteOnClose);
					pDispatchOrderWnd->ViewPermission(Station::LimitsOfAuthority::employee);
					pDispatchOrderWnd->exec();
				});
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
			MenuBarInfo* pMenuBarInfo6 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo6);
			pMenuBarInfo6->m_pMenu = m_pMenuBar->addMenu("查询(S)"); {
				connect(pMenuBarInfo6->addNewAction("告警信息", 1, 0) , &QAction::triggered, [&]() {
					
					m_pStaAlarm = new StaAlarmWindowTKY;
					m_pStaAlarm->show();

					});
				connect(pMenuBarInfo6->addNewAction("历史告警信息", 1, 1) , & QAction::triggered, [&]() {
					
					});
				pMenuBarInfo6->m_pMenu->addSeparator();
				connect(pMenuBarInfo6->addNewAction("查询行车日志", 1, 2), &QAction::triggered, [&]() {
					});
				connect(pMenuBarInfo6->addNewAction("查询车次", 1, 3), &QAction::triggered, [&]() {
					});
				pMenuBarInfo6->m_pMenu->addSeparator();
				connect(pMenuBarInfo6->addNewAction("调车作业单查询", 1, 4), &QAction::triggered, [&]() {
					});
				connect(pMenuBarInfo6->addNewAction("运行图查询", 1,5), &QAction::triggered, [&]() {
					});
				pMenuBarInfo6->m_pMenu->addSeparator();
				connect(pMenuBarInfo6->addNewAction("铅封计数查询", 1, 6), &QAction::triggered, [&]() {


					});
			}
			MenuBarInfo* pMenuBarInfo7 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo7);
			pMenuBarInfo7->m_pMenu = m_pMenuBar->addMenu("打印(P)"); {
				connect(pMenuBarInfo7->addNewAction("打印行车日志", 1, 0), &QAction::triggered, [&]() {
					});
				pMenuBarInfo7->m_pMenu->addSeparator();
				connect(pMenuBarInfo7->addNewAction("打印阶段计划", 1, 1), &QAction::triggered, [&]() {
					});
				connect(pMenuBarInfo7->addNewAction("打印上行计划", 1, 2), &QAction::triggered, [&]() {
					});
				connect(pMenuBarInfo7->addNewAction("打印下行计划", 1, 3), &QAction::triggered, [&]() {
					});
				pMenuBarInfo7->m_pMenu->addSeparator();
				connect(pMenuBarInfo7->addNewAction("清空打印任务", 1, 4), &QAction::triggered, [&]() {
					});
				connect(pMenuBarInfo7->addNewAction("打印计划设置", 1, 5), &QAction::triggered, [&]() {
					});

			}
			MenuBarInfo* pMenuBarInfo8 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo8);
			pMenuBarInfo8->m_pMenu = m_pMenuBar->addMenu("选项(W)"); {
				connect(pMenuBarInfo8->addNewAction("显示报点表格", 1, 0), &QAction::triggered, [&]() {
					});
			
				connect(pMenuBarInfo8->addNewAction("显示站场显示", 1, 1), &QAction::triggered, [&]() {
					});
				pMenuBarInfo8->m_pMenu->addSeparator();
				connect(pMenuBarInfo8->addNewAction("自动报点", 1, 2), &QAction::triggered, [&]() {
					});
				connect(pMenuBarInfo8->addNewAction("车次号自识别", 1, 3), &QAction::triggered, [&]() {
					});
				pMenuBarInfo8->m_pMenu->addSeparator();
				
				pMenuBarInfo8->addNewAction("无线车次上表", 1, 4); {
					MenuInfo* pMenuInfo = pMenuBarInfo8->m_vecSubMenuInfo[4];
					connect(pMenuInfo->addNewSubAction("机车类型", 2, 0), &QAction::triggered, [&]() {
						});
					connect(pMenuInfo->addNewSubAction("机车号", 2, 1), &QAction::triggered, [&]() {
						});
					connect(pMenuInfo->addNewSubAction("车长姓名", 2, 2), &QAction::triggered, [&]() {
						});
					connect(pMenuInfo->addNewSubAction("司机代号", 2, 3), &QAction::triggered, [&]() {
						});
					connect(pMenuInfo->addNewSubAction("总重", 2, 4), &QAction::triggered, [&]() {
						});
					connect(pMenuInfo->addNewSubAction("换长", 2, 5), &QAction::triggered, [&]() {
						});
					connect(pMenuInfo->addNewSubAction("辆数", 2, 6), &QAction::triggered, [&]() {
						});
				}
				pMenuBarInfo8->m_pMenu->addSeparator();
				connect(pMenuBarInfo8->addNewAction("股道不一致报警", 1, 5), &QAction::triggered, [&]() {
					});
				pMenuBarInfo8->m_pMenu->addSeparator();
				connect(pMenuBarInfo8->addNewAction("行车日志", 1, 6), &QAction::triggered, [&]() {
					CTCMainWindow::TurnToTrafficLogDisp();
					});
				connect(pMenuBarInfo8->addNewAction("列车/甩挂编组", 1, 7), &QAction::triggered, [&]() {
					});
				connect(pMenuBarInfo8->addNewAction("电话记录", 1, 8), &QAction::triggered, [&]() {
					});
				connect(pMenuBarInfo8->addNewAction("邻站站名", 1, 9), &QAction::triggered, [&]() {
					});
				connect(pMenuBarInfo8->addNewAction("图定时间", 1, 10), &QAction::triggered, [&]() {
					});
				connect(pMenuBarInfo8->addNewAction("邻站发车", 1, 11), &QAction::triggered, [&]() {
					});

			}
			MenuBarInfo* pMenuBarInfo9 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo9);
			pMenuBarInfo9->m_pMenu = m_pMenuBar->addMenu("系统维护(W)"); {
				connect(pMenuBarInfo9->addNewAction("本机倒为备机", 1, 0), &QAction::triggered, [&]() {
					});
				pMenuBarInfo9->m_pMenu->addSeparator();
				connect(pMenuBarInfo9->addNewAction("测试无线设备", 1, 1), &QAction::triggered, [&]() {
					});
				connect(pMenuBarInfo9->addNewAction("声音测试", 1, 2), &QAction::triggered, [&]() {
					});
				connect(pMenuBarInfo9->addNewAction("校 时", 1, 3), &QAction::triggered, [&]() {
					});
				pMenuBarInfo9->m_pMenu->addSeparator();
				connect(pMenuBarInfo9->addNewAction("下载工作日志", 1, 4), &QAction::triggered, [&]() {
					});
				connect(pMenuBarInfo9->addNewAction("更新配置文件", 1, 5), &QAction::triggered, [&]() {
					});
				pMenuBarInfo9->m_pMenu->addSeparator();
				connect(pMenuBarInfo9->addNewAction("登销记模板维护", 1, 6), &QAction::triggered, [&]() {
					});
				pMenuBarInfo9->m_pMenu->addSeparator();
				connect(pMenuBarInfo9->addNewAction("辅助模拟标注", 1, 7), &QAction::triggered, [&]() {
					});
			}
			MenuBarInfo* pMenuBarInfo10 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo10);
			pMenuBarInfo10->m_pMenu = m_pMenuBar->addMenu("帮助(H)"); {

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
			pDispatBtn->setObjectName("dispatchBtn");
			pDispatBtn->setFixedSize(96, 28);
			connect(pDispatBtn, &QPushButton::clicked, this, &StationViewTKY::ShowDispatchOrderWnd);
			m_pSignForToolBar->addWidget(pDispatBtn);

			//间隔
			QLabel* pLabel = new QLabel(m_pSignForToolBar);
			pLabel->setFrameShape(QFrame::WinPanel);
			pLabel->setFrameShadow(QFrame::Sunken);
			pLabel->setFixedSize(160, 28);
			m_pSignForToolBar->addWidget(pLabel);
			m_pStationViewToolBar->installEventFilter(this);
			m_pSignForToolBar->installEventFilter(this);
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
			m_pStateToolBar->installEventFilter(this);
			addToolBar(m_pStateToolBar);
		}

		bool StationViewTKY::eventFilter(QObject* obj, QEvent* event) {
			if (event->type() == QEvent::Resize) {
				m_pTimeLabel->move(width() - 108, 72);
			}
			return QObject::eventFilter(obj, event);
		}
		BaseWnd::StaTraindiagramwidget* StationViewTKY::CreateStaTraindiagramwidget()
		{
			return nullptr;
		}
		void StationViewTKY::InitStaTraindiagramwidget()
		{
		}

		void StationViewTKY::timerEvent(QTimerEvent* event)
		{
			if (TimerId == event->timerId()) {
				upDateTime();
			}
			return QMainWindow::timerEvent(event);
		}

		QString StationViewTKY::getWeekday(const QDateTime& dateTime) {
			int weekday = dateTime.date().dayOfWeek();
			switch (weekday) {
			case 1:
				return "星期一";
			case 2:
				return "星期二";
			case 3:
				return "星期三";
			case 4:
				return "星期四";
			case 5:
				return "星期五";
			case 6:
				return "星期六";
			case 7:
				return "星期日";
			default:
				return "";
			}
		}
		void StationViewTKY::upDateTime()
		{
			QDateTime currentDateTime = QDateTime::currentDateTime();
			QString timeStr = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
			TimeLabel->setText(QString("%1  %2").arg(timeStr).arg(getWeekday(currentDateTime)));

		}


		void StationViewTKY::InitBottomToolBar()
		{
			m_pBottomToolBar = new QToolBar(this);
			m_pBottomToolBar->installEventFilter(this);
			QPushButton* pDispatBtn = new QPushButton("行车日志");
			pDispatBtn->setCheckable(true);
			pDispatBtn->setFixedSize(64, 20);
			pDispatBtn->setStyleSheet("QPushButton {background-color: rgb(250, 250, 250);}");
			m_pBottomToolBar->addWidget(pDispatBtn);
			connect(pDispatBtn, &QPushButton::clicked, this, &CTCMainWindow::TurnToTrafficLogDisp);

			QPushButton* pTrainformaBtn = new QPushButton("列车编组");
			pTrainformaBtn->setCheckable(true);
			pTrainformaBtn->setFixedSize(64, 20);
			pTrainformaBtn->setStyleSheet("QPushButton {background-color: rgb(250, 250, 250);}");
			m_pBottomToolBar->addWidget(pTrainformaBtn);
			connect(pTrainformaBtn, &QPushButton::clicked, this, [&] {});

			QPushButton* pTelephonerecordBtn = new QPushButton("电话记录");
			pTelephonerecordBtn->setCheckable(true);
			pTelephonerecordBtn->setFixedSize(64, 20);
			pTelephonerecordBtn->setStyleSheet("QPushButton {background-color: rgb(250, 250, 250);}");
			m_pBottomToolBar->addWidget(pTelephonerecordBtn);
			connect(pTelephonerecordBtn, &QPushButton::clicked, this, [&] {});

			QPushButton* pAdjacentStationBtn = new QPushButton("邻站站名");
			pAdjacentStationBtn->setCheckable(true);
			pAdjacentStationBtn->setFixedSize(64, 20);
			pAdjacentStationBtn->setStyleSheet("QPushButton {background-color: rgb(250, 250, 250);}");
			m_pBottomToolBar->addWidget(pAdjacentStationBtn);
			connect(pAdjacentStationBtn, &QPushButton::clicked, this, [&] {});

			QPushButton* pMapTimeBtn = new QPushButton("图定时间");
			pMapTimeBtn->setCheckable(true);
			pMapTimeBtn->setFixedSize(64, 20);
			pMapTimeBtn->setStyleSheet("QPushButton {background-color: rgb(250, 250, 250);}");
			m_pBottomToolBar->addWidget(pMapTimeBtn);
			connect(pMapTimeBtn, &QPushButton::clicked, this, [&] {});

			QPushButton* pAdjacentStationDepartureBtn = new QPushButton("邻站发车");
			pAdjacentStationDepartureBtn->setCheckable(true);
			pAdjacentStationDepartureBtn->setFixedSize(64, 20);
			pAdjacentStationDepartureBtn->setStyleSheet("QPushButton {background-color: rgb(250, 250, 250);}");
			m_pBottomToolBar->addWidget(pAdjacentStationDepartureBtn);
			connect(pAdjacentStationDepartureBtn, &QPushButton::clicked, this, [&] {});
			addToolBar(Qt::BottomToolBarArea, m_pBottomToolBar);
		}

		void StationViewTKY::InitStatusBar()
		{
			m_pStatusBar = new QStatusBar(this);
			m_pStatusBar->setFixedHeight(28);

			QLabel* permanentLabel = new QLabel;
			permanentLabel->setStyleSheet("border: 0.5px solid #CCCCCC;");
			permanentLabel->setFixedWidth(160);
			permanentLabel->setAlignment(Qt::AlignVCenter);
			permanentLabel->setText("中国铁道科学研究院");

			m_pBottomTimeLabel = new QLabel;
			m_pBottomTimeLabel->setStyleSheet("border: 0.5px solid #CCCCCC;");
			m_pBottomTimeLabel->setFixedWidth(180);
			m_pBottomTimeLabel->setAlignment(Qt::AlignVCenter);

			QLabel* permanentLabe2 = new QLabel;
			permanentLabe2->setStyleSheet("border: 0.5px solid #CCCCCC;");
			permanentLabe2->setFixedWidth(120);
			permanentLabe2->setAlignment(Qt::AlignVCenter);
			permanentLabe2->setText(QString("本站名：%1").arg(Station::MainStation()->getStationName()));

			m_pStatusBar->addWidget(permanentLabel);
			m_pStatusBar->addWidget(m_pBottomTimeLabel);
			m_pStatusBar->addWidget(permanentLabe2);
			setStatusBar(m_pStatusBar);
		}
		
	}
}
