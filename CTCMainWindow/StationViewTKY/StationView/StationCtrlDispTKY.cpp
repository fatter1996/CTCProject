
#include "StationCtrlDispTKY.h"
#include "../ModuleWidget/StaFunBtnToolBarTKY.h"
#pragma execution_character_set("utf-8")
#include "./CTCMainWindow/BaseWndClass/ModuleWidget/StaFunBtnToolBar.h"
#include <QDateTime>
#include "Global.h"
namespace CTCWindows {
	namespace CARS {
		int CTCWindows::CARS::StationCtrlDispTKY::m_nCountdown = 0;;
		StationCtrlDispTKY::StationCtrlDispTKY(QWidget* parent)
			: StationCtrlDisp(parent)
		{
			ui.setupUi(this);
			m_pLeftButton = new QPushButton(this);
			m_pLeftButton->move(5, 5);
			m_pLeftButton->setFixedHeight(50);
			m_pLeftButton->setFixedWidth(80);
			m_pLeftButton->setStyleSheet(
				"QPushButton {"
				"   background-color: transparent;"
				"   border: 4px solid red;"
				"   font-size: 15px;"
				"   font-family: ËÎÌו;"
				"	color:red;"
				"}"
			);
			
			m_nTimerId = startTimer(200);
			ui.scrollArea->setMouseTracking(true);
			m_pRightButton = new QPushButton(this);
			m_pRightButton->move(width() - m_pRightButton->width() - 25,5);
			m_pRightButton->setStyleSheet(
				"QPushButton {"
				"   background-color: transparent;"
				"   border: 4px solid white;"
				"   font-family: ËÎÌו;"
				"   font-size: 15px;"
				"	color:white;"
				"}"
			);
			m_pRightButton->setFixedHeight(50);
			m_pRightButton->setFixedWidth(80);
			m_pLeftButton->hide();

		}
		void StationCtrlDispTKY::timerEvent(QTimerEvent* event)
		{
			if (m_nTimerId == event->timerId()) {
				QDateTime currentDateTime = QDateTime::currentDateTime();
				QString timeStr = currentDateTime.toString("hh:mm:ss");
				m_accumulatedTime += 200;
				m_pRightButton->setText(timeStr);
				m_pLeftButton->setText(QString::number(m_nCountdown));
				if (Station::MainStation()->getSelectDevice().size() != 0) {
					m_nCountdown = 0;
					m_pLeftButton->hide(); 
					return StationCtrlDisp::timerEvent(event);;
				}
				if(m_accumulatedTime >= 1000) {
					m_accumulatedTime = 0;
					if (m_nCountdown > 0) {
						m_nCountdown--;
						m_pLeftButton->show();
					}
					else {
						if (m_nCountdown == 0) {
							m_pLeftButton->hide();
							m_pStaFunBtnToolBar->onFunBtnStateReset();
						}
					}
				}

			}
			return StationCtrlDisp::timerEvent(event);
		}
		StationCtrlDispTKY::~StationCtrlDispTKY()
		{
			killTimer(m_nTimerId);
		}

		void StationCtrlDispTKY::CreatStaFunBtnToolBar()
		{
			m_pStaFunBtnToolBar = new StaFunBtnToolBarTKY();
			this->layout()->addWidget(m_pStaFunBtnToolBar);
			connect(m_pStaFunBtnToolBar, &CTCWindows::BaseWnd::StaFunBtnToolBar::countdownStarts, [&] {
				setCountdown();
				});

		}

		void StationCtrlDispTKY::resizeEvent(QResizeEvent* event) {
			QWidget::resizeEvent(event);
			m_pRightButton->move(width() - m_pRightButton->width() - 25, 5);
		}

		void StationCtrlDispTKY::setCountdown()
		{
			m_nCountdown = 18;
		}
	}
}