#include "StationCtrlDisp.h"
#include "Global.h"
#include "CustomControl/TextSignLable.h"
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
						onLeftbuttonRelease();
						event->accept();
						return true;
					}
					if (mouseEvent->button() == Qt::RightButton) {   //鼠标右键点击事件
						onRightbuttonRelease();
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
				Station::Device::DeviceBase::setElapsed();
				StaPaintView()->update();
			}
			return QWidget::timerEvent(event);
		}

		QAction* StationCtrlDisp::AddActionToMenu(const QString& strText, std::function<void(bool)> callback, bool bEnabled, bool bCheckable, bool bChecked, QMenu* pSubMenu)
		{
			QAction* pAction = new QAction(strText);
			pAction->setEnabled(bEnabled);
			pAction->setCheckable(bCheckable);
			pAction->setChecked(bChecked);
			if (pSubMenu) {
				pAction->setMenu(pSubMenu);
			}
			else {
				connect(pAction, &QAction::triggered, callback);
			}
			return pAction;
		}

		void StationCtrlDisp::ShowRightMouseMenu(QMenu* pMenu, const QPoint& ptMouse)
		{
			QMenu* pSubMenu1 = new QMenu();
			pSubMenu1->addAction(AddActionToMenu(Station::MainStation()->getStationName(), [](bool) {}));
			pMenu->addAction(AddActionToMenu("车站选择", nullptr, true, false, false, pSubMenu1));

			QMenu* pSubMenu2 = new QMenu();
			pSubMenu2->addAction(AddActionToMenu("首站", [](bool) {}, false));
			pSubMenu2->addAction(AddActionToMenu("前站", [](bool) {}, false));
			pSubMenu2->addAction(AddActionToMenu("后站", [](bool) {}, false));
			pSubMenu2->addAction(AddActionToMenu("尾站", [](bool) {}, false));
			pMenu->addAction(AddActionToMenu("车站浏览", nullptr, true, pSubMenu2));

			pMenu->addSeparator();

			pMenu->addAction(AddActionToMenu("主工具条", [](bool bChecked) {
				MainWindow()->SetShowToolbar(bChecked, bChecked);
			}, true, true, MainWindow()->IsShowToolbar()));
			pMenu->addAction(AddActionToMenu("CTC按钮", [=](bool bChecked) {
				m_pStaFunBtnToolBar->setHidden(!bChecked);
			}, true, true, true));
			pMenu->addAction(AddActionToMenu("文字显示", [=](bool) {
				MainWindow()->ShowVisibleSetWnd();
			}));
			pMenu->addAction(AddActionToMenu("绝缘节", [=](bool bChecked) {
				Station::MainStation()->SetVisible(Station::VisibleDev::insulationNode, bChecked);
			}, true, true, Station::MainStation()->IsVisible(Station::VisibleDev::insulationNode)));

			pMenu->addSeparator();

			pMenu->addAction(AddActionToMenu("还原 站场图", [=](bool) {
				Station::MainStation()->setDiploid(Station::DiploidOperate::revert, STAVIEW);
			}, !(Station::MainStation()->getDiploid(Station::DiploidRatio::StaDiploid) == 1.0)));
			pMenu->addAction(AddActionToMenu("放大 站场图", [=](bool) {
				MainWindow()->setMouseState(MouseState::Amplify);
			}));
			pMenu->addAction(AddActionToMenu("缩小 站场图", [=](bool) {
				MainWindow()->setMouseState(MouseState::Reduce);
			}));
			pMenu->addAction(AddActionToMenu("站场图全景", [=](bool) {}));

			pMenu->addSeparator();

			pMenu->addAction(AddActionToMenu("还原 车次窗", [=](bool) {
				Station::MainStation()->setDiploid(Station::DiploidOperate::revert, TRAINNUM);
			}, !(Station::MainStation()->getDiploid(Station::DiploidRatio::StaTrainNumDiploid) == 1.0)));
			pMenu->addAction(AddActionToMenu("放大 车次窗", [=](bool) {
				Station::MainStation()->setDiploid(Station::DiploidOperate::amplify, TRAINNUM);
			}));
			pMenu->addAction(AddActionToMenu("缩小 车次窗", [=](bool) {
				Station::MainStation()->setDiploid(Station::DiploidOperate::reduce, TRAINNUM);
			}));

			pMenu->addSeparator();

			pMenu->addAction(AddActionToMenu("车次窗位置", [=](bool bChecked) {
				Station::MainStation()->SetVisible(Station::VisibleDev::trainNumWnd, bChecked);
			}, true, true, Station::MainStation()->IsVisible(Station::VisibleDev::trainNumWnd)));
			pMenu->addAction(AddActionToMenu("查找车次窗", [=](bool) {}));
			
			pMenu->addSeparator();

			pMenu->addAction(AddActionToMenu("接通光带", [=](bool) { Station::MainStation()->PutThrough(15); }));
			pMenu->addAction(AddActionToMenu("继续接通光带30s", [=](bool) {Station::MainStation()->PutThrough(30); }));

			pMenu->addSeparator();

			pMenu->addAction(AddActionToMenu("线条宽度", [=](bool) {}, false));
			pMenu->addAction(AddActionToMenu("信号机大小", [=](bool) {}, false));
			pMenu->addAction(AddActionToMenu("按钮和表示灯大小", [=](bool) {}, false));

			pMenu->addSeparator();

			pMenu->addAction(AddActionToMenu("新建文字标注", [=](bool) {
				Control::TextSignLable* pTextSignLable = Control::TextSignLable::CreateTextSignLable(StaPaintView());
				QPoint ptMouseT = StaPaintView()->mapFromGlobal(ptMouse);
				pTextSignLable->move(ptMouseT);
				connect(pTextSignLable, &Control::TextSignLable::addTextSignLable, [ptMouseT](const QString& strText) {
					Station::MainStation()->AddNewTextSign(strText, ptMouseT);
				});
			}));

			pMenu->addAction(AddActionToMenu("清空文字标注", [=](bool) {
				Station::MainStation()->ClearAllTextSign();
			}));
		}

		void StationCtrlDisp::onLeftbuttonRelease()
		{
			if (MainWindow()->getMouseState() == MouseState::Amplify) {
				Station::MainStation()->setDiploid(Station::DiploidOperate::amplify, STAVIEW);
			}
			else if (MainWindow()->getMouseState() == MouseState::Reduce) {
				Station::MainStation()->setDiploid(Station::DiploidOperate::reduce, STAVIEW);
			}
		}

		void StationCtrlDisp::onRightbuttonRelease()
		{
			if (MainWindow()->getMouseState() == MouseState::Amplify ||
				MainWindow()->getMouseState() == MouseState::Reduce) {
				MainWindow()->setMouseState(MouseState::Default);
			}
			QMenu* pMenu = new QMenu();
			pMenu->setAttribute(Qt::WA_DeleteOnClose);
			QPoint ptMouse = QCursor::pos();
			if (Station::MainStation()->getSelectDevice().size()) {
				QAction* pAction1 = new QAction("命令下达");
				pMenu->addAction(pAction1);
				connect(pAction1, &QAction::triggered, Station::MainStation(), &Station::MainStationObject::onOrderIssued);
				QAction* pAction2 = new QAction("命令清除");
				pMenu->addAction(pAction2);
				connect(pAction2, &QAction::triggered, []() {
					Station::MainStation()->onOrderClear(true);
				});
			}
			else {
				ShowRightMouseMenu(pMenu, ptMouse);
			}
			pMenu->exec(ptMouse);
		}
	}
}