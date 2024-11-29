#include "StationMultiDisp.h"
#include "Global.h"
#include <QMouseEvent>

namespace CTCWindows {
	namespace BaseWnd {

		StationMultiDisp::StationMultiDisp(QWidget* parent)
		{
			m_nTimerId_500 = startTimer(500);
			installEventFilter(this);
			setMouseTracking(true);
		}

		StationMultiDisp::~StationMultiDisp()
		{
			killTimer(m_nTimerId_500);
		}

		void StationMultiDisp::InitMultiStation(QWidget* parent, QVector<Station::StationObject*> vecMultiStation)
		{
			m_vecMultiStation = vecMultiStation;
			QWidget* pSubWidget = nullptr;
			for (Station::StationObject* pStation : vecMultiStation) {
				pSubWidget = new QWidget(parent);
				pSubWidget->setObjectName(pStation->getStationName());
				pSubWidget->setFixedSize(pStation->getStaFixedSize());
				pSubWidget->move(0, 0);
				pSubWidget->installEventFilter(this);
				pSubWidget->setMouseTracking(true);
				pStation->InitDeviceEventFilter(pSubWidget);
			}
		}

		void StationMultiDisp::timerEvent(QTimerEvent* event)
		{
			if (event->timerId() == m_nTimerId_500) {
				for (Station::StationObject* pSatation : m_vecMultiStation) {
					if (pSatation->ShowWidget()) {
						pSatation->ShowWidget()->update();
					}
				}
			}
			return QWidget::timerEvent(event);
		}

		bool StationMultiDisp::eventFilter(QObject* obj, QEvent* event)
		{
			if (obj == this) {
				if (event->type() == QEvent::MouseButtonRelease) {
					QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {   //鼠标左键点击事件
						if (MainWindow()->getMouseState() == MouseState::Amplify) {
							Station::MainStation()->setDiploid(Station::DiploidOperate::amplify, TRAINNUM);
							event->accept();
							return true;
						}
						else if (MainWindow()->getMouseState() == MouseState::Reduce) {
							Station::MainStation()->setDiploid(Station::DiploidOperate::reduce, TRAINNUM);
							event->accept();
							return true;
						}
					}
					if (mouseEvent->button() == Qt::RightButton) {   //鼠标右键点击事件
						if (MainWindow()->getMouseState() == MouseState::Amplify ||
							MainWindow()->getMouseState() == MouseState::Reduce) {
							MainWindow()->setMouseState(MouseState::Default);
						}
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
	}
}