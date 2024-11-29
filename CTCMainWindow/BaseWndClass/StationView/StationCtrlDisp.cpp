#include "StationCtrlDisp.h"
#include "Global.h"
#include <QMouseEvent>
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace BaseWnd {
		StationCtrlDisp::StationCtrlDisp(QWidget* parent)
		{
			m_nTimerId_500 = startTimer(500);
			installEventFilter(this);
			setMouseTracking(true);
		}

		StationCtrlDisp::~StationCtrlDisp()
		{
			killTimer(m_nTimerId_500);
		}

		bool StationCtrlDisp::eventFilter(QObject* obj, QEvent* event)
		{
			if (obj == this) {
				if (event->type() == QEvent::MouseButtonRelease) {
					QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {   //鼠标左键点击事件
						if (MainWindow()->getMouseState() == MouseState::Amplify) {
							Station::MainStation()->setDiploid(Station::DiploidOperate::amplify, STAVIEW);
							event->accept();
							return true;
						}
						else if (MainWindow()->getMouseState() == MouseState::Reduce) {
							Station::MainStation()->setDiploid(Station::DiploidOperate::reduce, STAVIEW);
							event->accept();
							return true;
						}
					}
					if (mouseEvent->button() == Qt::RightButton) {   //鼠标右键点击事件
						if (MainWindow()->getMouseState() == MouseState::Amplify ||
							MainWindow()->getMouseState() == MouseState::Reduce) {
							MainWindow()->setMouseState(MouseState::Default);
						}
						QMenu* pMenu = new QMenu();
						pMenu->setAttribute(Qt::WA_DeleteOnClose);

						if (Station::MainStation()->getSelectDevice().size()) {
							QAction* pAction1 = new QAction("命令下达");
							pMenu->addAction(pAction1);
							connect(pAction1, &QAction::triggered, Station::MainStation(), &Station::MainStationObject::onOrderIssued);
							QAction* pAction2 = new QAction("命令清除");
							pMenu->addAction(pAction2);
							connect(pAction2, &QAction::triggered, Station::MainStation(), &Station::MainStationObject::onOrderClear);
						}
						else {
							QAction* pAction1 = new QAction("车站选择");
							QMenu* pSubMenu1 = new QMenu();
							QAction* pSubAction1 = new QAction(Station::MainStation()->getStationName());
							pSubMenu1->addAction(pSubAction1);
							connect(pSubAction1, &QAction::triggered, []() {});
							pAction1->setMenu(pSubMenu1);
							pMenu->addAction(pAction1);
							QAction* pAction2 = new QAction("车站浏览");
							QMenu* pSubMenu2 = new QMenu();
							QAction* pSubAction21 = new QAction("首站");
							pSubAction21->setEnabled(false);
							pSubMenu2->addAction(pSubAction21);
							connect(pSubAction21, &QAction::triggered, []() {});
							QAction* pSubAction22 = new QAction("前站");
							pSubAction22->setEnabled(false);
							pSubMenu2->addAction(pSubAction22);
							connect(pSubAction22, &QAction::triggered, []() {});
							QAction* pSubAction23 = new QAction("后站");
							pSubAction23->setEnabled(false);
							pSubMenu2->addAction(pSubAction23);
							connect(pSubAction23, &QAction::triggered, []() {});
							QAction* pSubAction24 = new QAction("尾站");
							pSubAction24->setEnabled(false);
							pSubMenu2->addAction(pSubAction24);
							connect(pSubAction24, &QAction::triggered, []() {});
							pAction2->setMenu(pSubMenu2);
							pMenu->addAction(pAction2);
							pMenu->addSeparator();
							QAction* pAction3 = new QAction("主工具条");
							pAction3->setEnabled(false);
							pMenu->addAction(pAction3);
							connect(pAction3, &QAction::triggered, []() {});
							QAction* pAction4 = new QAction("CTC按钮");
							pAction4->setCheckable(true);
							pAction4->setChecked(!m_pStaFunBtnToolBar->isHidden());
							pMenu->addAction(pAction4);
							connect(pAction4, &QAction::triggered, [&](bool checked) {
								m_pStaFunBtnToolBar->setHidden(!checked); });
							QAction* pAction5 = new QAction("文字显示");
							pMenu->addAction(pAction5);
							connect(pAction5, &QAction::triggered, MainWindow(), &CTCMainWindow::ShowVisibleSetWnd);
							QAction* pAction6 = new QAction("绝缘节");
							pAction6->setCheckable(true);
							pAction6->setChecked(Station::MainStation()->IsVisible(Station::VisibleDev::insulationNode));
							pMenu->addAction(pAction6);
							connect(pAction6, &QAction::triggered, [](bool bChecked) {
								Station::MainStation()->SetVisible(Station::VisibleDev::insulationNode, bChecked); 
							});
							pMenu->addSeparator();
							QAction* pAction7 = new QAction("还原 站场图");
							pAction7->setEnabled(!(Station::MainStation()->getDiploid(Station::DiploidRatio::StaDiploid) == 1.0));
							pMenu->addAction(pAction7);
							connect(pAction7, &QAction::triggered, []() {
								Station::MainStation()->setDiploid(Station::DiploidOperate::revert, STAVIEW);  
							});
							QAction* pAction8 = new QAction("放大 站场图");
							pMenu->addAction(pAction8);
							connect(pAction8, &QAction::triggered, []() {
								MainWindow()->setMouseState(MouseState::Amplify);  
							});
							QAction* pAction9 = new QAction("缩小 站场图");
							pMenu->addAction(pAction9);
							connect(pAction9, &QAction::triggered, []() {
								MainWindow()->setMouseState(MouseState::Reduce);  
							});
							QAction* pAction10 = new QAction("站场图全景");
							pMenu->addAction(pAction10);
							connect(pAction10, &QAction::triggered, []() {});
							pMenu->addSeparator();
							QAction* pAction11 = new QAction("还原 车次窗");
							pAction11->setEnabled(!(Station::MainStation()->getDiploid(Station::DiploidRatio::StaTrainNumDiploid) == 1.0));
							pMenu->addAction(pAction11);
							connect(pAction11, &QAction::triggered, []() {
								Station::MainStation()->setDiploid(Station::DiploidOperate::revert, TRAINNUM);  
							});
							QAction* pAction12 = new QAction("放大 车次窗");
							pMenu->addAction(pAction12);
							connect(pAction12, &QAction::triggered, []() {
								Station::MainStation()->setDiploid(Station::DiploidOperate::amplify, TRAINNUM);
							});
							QAction* pAction13 = new QAction("缩小 车次窗");
							pMenu->addAction(pAction13);
							connect(pAction13, &QAction::triggered, []() {
								Station::MainStation()->setDiploid(Station::DiploidOperate::reduce, TRAINNUM);
							});
							pMenu->addSeparator();
							QAction* pAction14 = new QAction("车次窗位置");
							pAction14->setCheckable(true);
							pAction14->setChecked(Station::MainStation()->IsVisible(Station::VisibleDev::trainNumWnd));
							pMenu->addAction(pAction14);
							connect(pAction14, &QAction::triggered, [](bool bChecked) {
								Station::MainStation()->SetVisible(Station::VisibleDev::trainNumWnd, bChecked);
							});
							QAction* pAction15 = new QAction("查找车次窗");
							pMenu->addAction(pAction15);
							connect(pAction15, &QAction::triggered, []() {});
							pMenu->addSeparator();
							QAction* pAction16 = new QAction("接通光带");
							pMenu->addAction(pAction16);
							connect(pAction16, &QAction::triggered, []() {});
							QAction* pAction17 = new QAction("继续接通光带30s");
							pMenu->addAction(pAction17);
							connect(pAction17, &QAction::triggered, []() {});
							pMenu->addSeparator();
							QAction* pAction18 = new QAction("线条宽度");
							pAction18->setEnabled(false);
							pMenu->addAction(pAction18);
							connect(pAction18, &QAction::triggered, []() {});
							QAction* pAction19 = new QAction("信号机大小");
							pAction19->setEnabled(false);
							pMenu->addAction(pAction19);
							connect(pAction19, &QAction::triggered, []() {});
							QAction* pAction20 = new QAction("按钮和表示灯大小");
							pAction20->setEnabled(false);
							pMenu->addAction(pAction20);
							connect(pAction20, &QAction::triggered, []() {});
						}
						pMenu->exec(QCursor::pos());
					}
				}

				if (event->type() == QEvent::MouseMove) {
					if (MainWindow()->getMouseState() == MouseState::Amplify) {
						setCursor(QCursor(QPixmap(":/CTCProject/icon/mouseEnlarge.png")));
					}
					else if (MainWindow()->getMouseState() == MouseState::Reduce) {
						setCursor(QCursor(QPixmap(":/CTCProject/icon/mouseNarrow.png")));
					}
					else if (MainWindow()->getMouseState() == MouseState::AddTrain) {
						setCursor(QCursor(QPixmap(":/CTCProject/icon/trainHand.png")));
					}
					else {
						unsetCursor();
					}
				}
			}
			return QWidget::eventFilter(obj, event);
		}

		void StationCtrlDisp::timerEvent(QTimerEvent* event)
		{
			if (event->timerId() == m_nTimerId_500) {
				StaPaintView()->update();
			}
			return QWidget::timerEvent(event);
		}
	}
}