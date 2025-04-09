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
			pMenuBarInfo1->m_pMenu = m_pMenuBar->addMenu("��¼(L)"); {
				connect(pMenuBarInfo1->addNewAction("�û���¼", 1, 0), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo1->addNewAction("�û�ע��", 1, 1), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo1->addNewAction("�˳�", 1, 2), &QAction::triggered, [&]() {});
			}

			MenuBarInfo* pMenuBarInfo2 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo2);
			pMenuBarInfo2->m_pMenu = m_pMenuBar->addMenu("��ʾ(S)"); {
				pMenuBarInfo2->addNewAction("������", 1, 0); {
					MenuInfo* pMenuInfo = pMenuBarInfo2->m_vecSubMenuInfo[0];
					connect(pMenuInfo->addNewSubAction("��׼��ť", 2, 0, true, true, true), &QAction::triggered, [=](bool bChecked) {
						m_pStationViewToolBar->setHidden(!bChecked);
						m_pTrafficLogToolBar->setHidden(!bChecked);
					});
					connect(pMenuInfo->addNewSubAction("ǩ����", 2, 1, true, true, true), &QAction::triggered, [=](bool bChecked) {
						m_pSignForToolBar->setHidden(!bChecked);
						m_pStateToolBar->setHidden(!bChecked);
					});
					connect(pMenuInfo->addNewSubAction("��ʾϵͳ��Ϣ����", 2, 2, true, true, false), &QAction::triggered, [&](bool bChecked) {});
					connect(pMenuInfo->addNewSubAction("��ʾ��·���й�����", 2, 3, true, true, true), &QAction::triggered, [=](bool bChecked) {
						ShowStaRoutePlanWnd(bChecked);
						m_pRoutePlanAction = pMenuBarInfo2->getSubActionByIndex(2, 0, 3)->m_pAction;
					});
				}
				pMenuBarInfo2->addNewAction("վ��ͼ", 1, 1); {
					MenuInfo* pMenuInfo = pMenuBarInfo2->m_vecSubMenuInfo[1];
					connect(pMenuInfo->addNewSubAction("�Ŵ�վ��ͼ", 2, 0), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::amplify, STAVIEW); 
					});
					connect(pMenuInfo->addNewSubAction("��Сվ��ͼ", 2, 1), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::reduce, STAVIEW); 
					});
					connect(pMenuInfo->addNewSubAction("��ԭվ��ͼ", 2, 2, false), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::revert, STAVIEW); 
					});
					m_stuMenuSyncAction.m_pStaRevertToolAction = pMenuBarInfo2->getSubActionByIndex(2, 1, 2)->m_pAction;
					connect(pMenuInfo->addNewSubAction("�Ŵ󳵴δ�", 2, 3), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::amplify, TRAINNUM); 
					});
					connect(pMenuInfo->addNewSubAction("��С���δ�", 2, 4), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::reduce, TRAINNUM); 
					});
					connect(pMenuInfo->addNewSubAction("��ԭ���δ�", 2, 5, false), &QAction::triggered, [&]() {
						MainStation()->setDiploid(DiploidOperate::revert, TRAINNUM); 
					});
					m_stuMenuSyncAction.m_pStaRevertToolAction = pMenuBarInfo2->getSubActionByIndex(2, 1, 5)->m_pAction;
					connect(pMenuInfo->addNewSubAction("������ʾ", 2, 6), &QAction::triggered, this, &CTCMainWindow::ShowVisibleSetWnd);
					connect(pMenuInfo->addNewSubAction("��Ե����ʾ", 2, 7, true, true, true), &QAction::triggered, [&](bool bChecked) {
						MainStation()->SetVisible(VisibleDev::insulationNode, bChecked); 
					});
					connect(pMenuInfo->addNewSubAction("�г���ť��ʾ", 2, 8, true, true, true), &QAction::triggered, [&](bool bChecked) {
						MainStation()->SetVisible(VisibleDev::trainButton, bChecked); 
					});
					connect(pMenuInfo->addNewSubAction("������ť��ʾ", 2, 9, true, true, true), &QAction::triggered, [&](bool bChecked) {
						MainStation()->SetVisible(VisibleDev::shuntButton, bChecked); 
					});
					connect(pMenuInfo->addNewSubAction("��·���д���ʾ", 2, 10, true, true, true), &QAction::triggered, [&](bool bChecked) {
						MainStation()->SetVisible(VisibleDev::preview, bChecked); 
					});
					connect(pMenuInfo->addNewSubAction("��ʾ״̬��", 2, 6, false, true, true), &QAction::triggered, [&]() {});
					connect(pMenuInfo->addNewSubAction("������·��ʾ", 2, 6, false, true, true), &QAction::triggered, [&]() {});
					connect(pMenuInfo->addNewSubAction("����λ��", 2, 6, false, true, true), &QAction::triggered, [&]() {});
				}
				pMenuBarInfo2->addSeparator();
				pMenuBarInfo2->addNewAction("�г���־", 1, 2); {
					MenuInfo* pMenuInfo = pMenuBarInfo2->m_vecSubMenuInfo[2];
					connect(pMenuInfo->addNewSubAction("��ʾ��ͷ", 2, 0, true, true, true), &QAction::triggered, 
						m_pStationLog, &BaseWnd::StationLogDisp::ShowTableHead);
					connect(pMenuInfo->addNewSubAction("��ʾ��վ", 2, 1, false), &QAction::triggered, [&]() {});
					pMenuBarInfo2->m_pMenu->addSeparator();
					connect(pMenuInfo->addNewSubAction("����", 2, 2, false), &QAction::triggered, [&]() {});
					connect(pMenuInfo->addNewSubAction("��С", 2, 3, false), &QAction::triggered, [&]() {});
				}
			}
			
			MenuBarInfo* pMenuBarInfo3 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo3);
			pMenuBarInfo3->m_pMenu = m_pMenuBar->addMenu("����(U)"); {
				connect(pMenuBarInfo3->addNewAction("�����������", 1, 0), &QAction::triggered, this, &CTCMainWindow::ShowDispatchOrderWnd);
				connect(pMenuBarInfo3->addNewAction("˦����ҵ", 1, 1, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("�г�����(�ٱ���)", 1, 2), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("վ�泵(�ִ泵)", 1, 3), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("�п�����", 1, 4, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("��վֱ������", 1, 5), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("��ͳ46", 1, 6), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("ս��ͼ�ط�", 1, 7, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("�п�����ռ���߼����", 1, 8), &QAction::triggered, [&]() {});
				pMenuBarInfo3->addSeparator();
				connect(pMenuBarInfo3->addNewAction("�û�����", 1, 9, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo3->addNewAction("ѡ��", 1, 10), &QAction::triggered, [&]() {});
				pMenuBarInfo3->addSeparator();
				connect(pMenuBarInfo3->addNewAction("ȷ����ѯ", 1, 11, false), &QAction::triggered, [&]() {});
			}
			

			MenuBarInfo* pMenuBarInfo5 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo5);
			pMenuBarInfo5->m_pMenu = m_pMenuBar->addMenu("����(D)"); {
				connect(pMenuBarInfo5->addNewAction("ˢ��վ��ͼ", 1, 0, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo5->addNewAction("���Ե�������", 1, 1, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo5->addNewAction("����XML��Ϣ", 1, 2, false), &QAction::triggered, [&]() {});
				pMenuBarInfo5->addSeparator();
				connect(pMenuBarInfo5->addNewAction("��������", 1, 3), &QAction::triggered, [&]() {});
			}

			MenuBarInfo* pMenuBarInfo6 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo6);
			pMenuBarInfo6->m_pMenu = m_pMenuBar->addMenu("����(W)"); {
				connect(pMenuBarInfo6->addNewAction("�������", 1, 0, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo6->addNewAction("��ʾ����", 1, 1, false), &QAction::triggered, [&]() {});
				pMenuBarInfo6->addSeparator();
				connect(pMenuBarInfo6->addNewAction("������ҵ��", 1, 2, false), &QAction::triggered, [&]() {});
				pMenuBarInfo6->addSeparator();
				connect(pMenuBarInfo6->addNewAction("վ��ƽ��", 1, 3, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo6->addNewAction("�г���־", 1, 4, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo6->addNewAction("�����ƶ�", 1, 5, false), &QAction::triggered, [&]() {});
				connect(pMenuBarInfo6->addNewAction("�ź�Ա�������", 1, 6, false), &QAction::triggered, [&]() {});
			}
			

			MenuBarInfo* pMenuBarInfo7 = new MenuBarInfo;
			m_vecMenuBarInfo.append(pMenuBarInfo7);
			pMenuBarInfo7->m_pMenu = m_pMenuBar->addMenu("����(H)"); {
				connect(pMenuBarInfo7->addNewAction("����CTC-STPC", 1, 0), &QAction::triggered, [&]() {});
			}
			
			setMenuBar(m_pMenuBar);
		}

		void StationViewKSK::InitStationViewToolBar()
		{
			qDebug() << "InitStationViewToolBarTKY";
			m_pStationViewToolBar = new QToolBar(this);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/multi.png", "վ��͸��", STAVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::TurnToStationMultiDisp);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/station.png", "��վ����", STAVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::TurnToStationCtrlDisp);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/textSet.png", "��ʾ����", STAVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::ShowVisibleSetWnd);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/nodes.png", "��ʾ��Ե��", STAVIEW_TOOL, true, true), &QPushButton::clicked, [&](bool bChecked) {
				MainStation()->SetVisible(VisibleDev::insulationNode, bChecked);
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/trainBtn.png", "��ʾ�г���ť", STAVIEW_TOOL, true, true), &QPushButton::clicked, [&](bool bChecked) {
				MainStation()->SetVisible(VisibleDev::trainButton, bChecked); 
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/shuntBtn.png", "��ʾ������ť", STAVIEW_TOOL, true, true), &QPushButton::clicked, [&](bool bChecked) {
				MainStation()->SetVisible(VisibleDev::shuntButton, bChecked); 
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/trainNum.png", "��ʾ���δ�", STAVIEW_TOOL, true), &QPushButton::clicked, [&](bool bChecked) {
				MainStation()->SetVisible(VisibleDev::trainNumWnd, bChecked);
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/forecast.png", "��ʾ����Ԥ�洰", STAVIEW_TOOL, true, true), &QPushButton::clicked, [&](bool bChecked) {
				MainStation()->SetVisible(VisibleDev::preview, bChecked);
			});
			AddToolBarSeparator(STAVIEW_TOOL);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/text.png", "���ֱ�ע", STAVIEW_TOOL), &QPushButton::clicked, [&]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/flash.png", "�ն˰�ť��˸", STAVIEW_TOOL), &QPushButton::clicked, [&]() {});
			AddToolBarSeparator(STAVIEW_TOOL);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/amplify.png", "�Ŵ�", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				setMouseState(MouseState::Amplify); 
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/reduce.png", "��С", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				setMouseState(MouseState::Reduce); 
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/revert.png", "��ԭ", STAVIEW_TOOL, false, false, false, ":/CTCProject/icon/CASCO/revert2.png"), &QPushButton::clicked, [=]() {
				MainStation()->setDiploid(DiploidOperate::revert, STAVIEW); 
			});
			m_stuMenuSyncAction.m_pStaRevertBtn = m_pStationViewToolBar->findChild<QPushButton*>("revert");
			AddToolBarSeparator(STAVIEW_TOOL);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/textAmplify", "���δ��Ŵ�", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				MainStation()->setDiploid(DiploidOperate::amplify, TRAINNUM); 
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/textReduce.png", "���δ���С", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				MainStation()->setDiploid(DiploidOperate::reduce, TRAINNUM); 
			});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/textRevert.png", "���δ���ԭ", STAVIEW_TOOL), &QPushButton::clicked, [=]() {
				MainStation()->setDiploid(DiploidOperate::revert, TRAINNUM); 
			});
			AddToolBarSeparator(STAVIEW_TOOL);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/find.png", "���ҳ���", STAVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/return.png", "�г���־", STAVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::TurnToTrafficLogDisp);
			addToolBar(m_pStationViewToolBar);
			addToolBarBreak();
		}

		void StationViewKSK::InitTrafficLogToolBar()
		{
			qDebug() << "InitTrafficLogToolBarTKY";
			m_pTrafficLogToolBar = new QToolBar(this);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/print.png", "��ӡ", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/preview.png", "��ӡԤ��", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/diagram.png", "����ͼ", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/newTrain.png", "��������", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/user.png", "�û�����", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/history.png", "��ʷ�����ѯ", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/view.png", "��ͼ", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/upward.png", "��ʾ���г���", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/downward.png", "��ʾ���г���", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/allTrain.png", "��ʾ�����г���", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/agree.png", "ͬ����վ����", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/adjacent.png", "��վͬ�ⷢ��", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/plan.png", "���üƻ�", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/findLog.png", "���ҳ���", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			AddToolBarSeparator(LOGVIEW_TOOL);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/return.png", "���ص�վ��ͼ", LOGVIEW_TOOL), &QPushButton::clicked, this, &CTCMainWindow::TurnToStationCtrlDisp);
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/shunt.png", "������ҵ��", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			connect(AddToolBarBtn(":/CTCProject/icon/CASCO/help.png", "����", LOGVIEW_TOOL), &QPushButton::clicked, [=]() {});
			addToolBar(m_pTrafficLogToolBar);
			addToolBarBreak();
		}

		void StationViewKSK::InitSignForToolBar()
		{
			qDebug() << "InitSignForToolBarTKY";
			m_pSignForToolBar = new QToolBar(this);
			m_pSignForToolBar->setStyleSheet("QPushButton{background-color: rgb(210, 210, 210);}"
				"QLabel{background-color: rgb(240, 240, 240);}");

			//��������
			QPushButton* pDispatBtn = new QPushButton(m_pSignForToolBar);
			pDispatBtn->setText("��������");
			pDispatBtn->setObjectName("dispatchBtn");
			pDispatBtn->setFixedSize(96, 28);
			connect(pDispatBtn, &QPushButton::clicked, this, &CTCMainWindow::ShowDispatchOrderSignWnd);
			m_pSignForToolBar->addWidget(pDispatBtn);
			//���
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

			//ռ�߰�
			QPushButton* pDispatBtn = new QPushButton(m_pStateToolBar);
			pDispatBtn->setText("ռ�߰�(F9)");
			pDispatBtn->setObjectName("boardBtn");
			pDispatBtn->setFixedSize(96, 28);
			connect(pDispatBtn, &QPushButton::clicked, []() {});
			m_pStateToolBar->addWidget(pDispatBtn);
			pDispatBtn->hide();
			//�׶μƻ�
			QPushButton* pStagePlanBtn = new QPushButton(m_pStateToolBar);
			pStagePlanBtn->setText("�׶μƻ�");
			pStagePlanBtn->setObjectName("stagePlanBtn");
			pStagePlanBtn->setFixedSize(96, 28);
			connect(pStagePlanBtn, &QPushButton::clicked, this, &CTCMainWindow::ShowStagePlanSignWnd);
			m_pStateToolBar->addWidget(pStagePlanBtn);
			//���ͼƻ�
			QPushButton* pSendPlanBtn = new QPushButton(m_pStateToolBar);
			pSendPlanBtn->setText("���ͼƻ�");
			pSendPlanBtn->setObjectName("sendPlanBtn");
			pSendPlanBtn->setFixedSize(96, 28);
			connect(pSendPlanBtn, &QPushButton::clicked, []() {});
			m_pStateToolBar->addWidget(pSendPlanBtn);
			//�׶μ���
			QPushButton* pStageNotesBtn = new QPushButton(m_pStateToolBar);
			pStageNotesBtn->setText("�׶μ���");
			pStageNotesBtn->setObjectName("stageNotesBtn");
			pStageNotesBtn->setFixedSize(96, 28);
			connect(pStageNotesBtn, &QPushButton::clicked, []() {});
			m_pStateToolBar->addWidget(pStageNotesBtn);
			//ʱ����ʾ
			QLabel* pTimeLabel = new QLabel(m_pStateToolBar);
			pTimeLabel->setFrameShape(QFrame::WinPanel);
			pTimeLabel->setFrameShadow(QFrame::Sunken);
			pTimeLabel->setFont(QFont("΢���ź�", 11));
			pTimeLabel->setAlignment(Qt::AlignCenter);
			pTimeLabel->setText(QDateTime::currentDateTime().toString("yyyy-mm-dd hh:MM:ss"));
			pTimeLabel->setObjectName("timeLabel");
			pTimeLabel->setFixedSize(180, 28);
			m_pStateToolBar->addWidget(pTimeLabel);
			//��ͬ��վ��ͨ��
			QLabel* pSyncComLabel = new QLabel(m_pStateToolBar);
			pSyncComLabel->setFrameShape(QFrame::WinPanel);
			pSyncComLabel->setFrameShadow(QFrame::Sunken);
			pSyncComLabel->setFont(QFont("΢���ź�", 11));
			pSyncComLabel->setAlignment(Qt::AlignCenter);
			pSyncComLabel->setText("��ͬ��վ��ͨ���ж�");
			pSyncComLabel->setObjectName("centerComLabel");
			pSyncComLabel->setFixedSize(180, 28);
			m_pStateToolBar->addWidget(pSyncComLabel);
			//������ͨ��
			QLabel* pCenterComLabel = new QLabel(m_pStateToolBar);
			pCenterComLabel->setFrameShape(QFrame::WinPanel);
			pCenterComLabel->setFrameShadow(QFrame::Sunken);
			pCenterComLabel->setFont(QFont("΢���ź�", 11));
			pCenterComLabel->setAlignment(Qt::AlignCenter);
			pCenterComLabel->setText("������ͨ���ж�");
			pCenterComLabel->setObjectName("centerComLabel");
			pCenterComLabel->setFixedSize(144, 28);
			m_pStateToolBar->addWidget(pCenterComLabel);
			//�복վ������ͨ��
			QLabel* pServerComLabel = new QLabel(m_pStateToolBar);
			pServerComLabel->setFrameShape(QFrame::WinPanel);
			pServerComLabel->setFrameShadow(QFrame::Sunken);
			pServerComLabel->setFont(QFont("΢���ź�", 11));
			pServerComLabel->setAlignment(Qt::AlignCenter);
			pServerComLabel->setText("�복վ������ͨ���ж�");
			pServerComLabel->setObjectName("serverComLabel");
			pServerComLabel->setFixedSize(180, 28);
			m_pStateToolBar->addWidget(pServerComLabel);

			addToolBar(m_pStateToolBar);
		}

	}
}
