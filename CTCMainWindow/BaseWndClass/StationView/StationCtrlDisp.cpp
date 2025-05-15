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
					if (mouseEvent->button() == Qt::LeftButton) {   //����������¼�
						onLeftbuttonRelease();
						event->accept();
						return true;
					}
					if (mouseEvent->button() == Qt::RightButton) {   //����Ҽ�����¼�
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
			pMenu->addAction(AddActionToMenu("��վѡ��", nullptr, true, false, false, pSubMenu1));

			QMenu* pSubMenu2 = new QMenu();
			pSubMenu2->addAction(AddActionToMenu("��վ", [](bool) {}, false));
			pSubMenu2->addAction(AddActionToMenu("ǰվ", [](bool) {}, false));
			pSubMenu2->addAction(AddActionToMenu("��վ", [](bool) {}, false));
			pSubMenu2->addAction(AddActionToMenu("βվ", [](bool) {}, false));
			pMenu->addAction(AddActionToMenu("��վ���", nullptr, true, pSubMenu2));

			pMenu->addSeparator();

			pMenu->addAction(AddActionToMenu("��������", [](bool bChecked) {
				MainWindow()->SetShowToolbar(bChecked, bChecked);
			}, true, true, MainWindow()->IsShowToolbar()));
			pMenu->addAction(AddActionToMenu("CTC��ť", [=](bool bChecked) {
				m_pStaFunBtnToolBar->setHidden(!bChecked);
			}, true, true, true));
			pMenu->addAction(AddActionToMenu("������ʾ", [=](bool) {
				MainWindow()->ShowVisibleSetWnd();
			}));
			pMenu->addAction(AddActionToMenu("��Ե��", [=](bool bChecked) {
				Station::MainStation()->SetVisible(Station::VisibleDev::insulationNode, bChecked);
			}, true, true, Station::MainStation()->IsVisible(Station::VisibleDev::insulationNode)));

			pMenu->addSeparator();

			pMenu->addAction(AddActionToMenu("��ԭ վ��ͼ", [=](bool) {
				Station::MainStation()->setDiploid(Station::DiploidOperate::revert, STAVIEW);
			}, !(Station::MainStation()->getDiploid(Station::DiploidRatio::StaDiploid) == 1.0)));
			pMenu->addAction(AddActionToMenu("�Ŵ� վ��ͼ", [=](bool) {
				MainWindow()->setMouseState(MouseState::Amplify);
			}));
			pMenu->addAction(AddActionToMenu("��С վ��ͼ", [=](bool) {
				MainWindow()->setMouseState(MouseState::Reduce);
			}));
			pMenu->addAction(AddActionToMenu("վ��ͼȫ��", [=](bool) {}));

			pMenu->addSeparator();

			pMenu->addAction(AddActionToMenu("��ԭ ���δ�", [=](bool) {
				Station::MainStation()->setDiploid(Station::DiploidOperate::revert, TRAINNUM);
			}, !(Station::MainStation()->getDiploid(Station::DiploidRatio::StaTrainNumDiploid) == 1.0)));
			pMenu->addAction(AddActionToMenu("�Ŵ� ���δ�", [=](bool) {
				Station::MainStation()->setDiploid(Station::DiploidOperate::amplify, TRAINNUM);
			}));
			pMenu->addAction(AddActionToMenu("��С ���δ�", [=](bool) {
				Station::MainStation()->setDiploid(Station::DiploidOperate::reduce, TRAINNUM);
			}));

			pMenu->addSeparator();

			pMenu->addAction(AddActionToMenu("���δ�λ��", [=](bool bChecked) {
				Station::MainStation()->SetVisible(Station::VisibleDev::trainNumWnd, bChecked);
			}, true, true, Station::MainStation()->IsVisible(Station::VisibleDev::trainNumWnd)));
			pMenu->addAction(AddActionToMenu("���ҳ��δ�", [=](bool) {}));
			
			pMenu->addSeparator();

			pMenu->addAction(AddActionToMenu("��ͨ���", [=](bool) { Station::MainStation()->PutThrough(15); }));
			pMenu->addAction(AddActionToMenu("������ͨ���30s", [=](bool) {Station::MainStation()->PutThrough(30); }));

			pMenu->addSeparator();

			pMenu->addAction(AddActionToMenu("�������", [=](bool) {}, false));
			pMenu->addAction(AddActionToMenu("�źŻ���С", [=](bool) {}, false));
			pMenu->addAction(AddActionToMenu("��ť�ͱ�ʾ�ƴ�С", [=](bool) {}, false));

			pMenu->addSeparator();

			pMenu->addAction(AddActionToMenu("�½����ֱ�ע", [=](bool) {
				Control::TextSignLable* pTextSignLable = Control::TextSignLable::CreateTextSignLable(StaPaintView());
				QPoint ptMouseT = StaPaintView()->mapFromGlobal(ptMouse);
				pTextSignLable->move(ptMouseT);
				connect(pTextSignLable, &Control::TextSignLable::addTextSignLable, [ptMouseT](const QString& strText) {
					Station::MainStation()->AddNewTextSign(strText, ptMouseT);
				});
			}));

			pMenu->addAction(AddActionToMenu("������ֱ�ע", [=](bool) {
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
				QAction* pAction1 = new QAction("�����´�");
				pMenu->addAction(pAction1);
				connect(pAction1, &QAction::triggered, Station::MainStation(), &Station::MainStationObject::onOrderIssued);
				QAction* pAction2 = new QAction("�������");
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