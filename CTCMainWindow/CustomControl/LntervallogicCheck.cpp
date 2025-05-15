#include "LntervallogicCheck.h"
#include "qtabwidget.h"
#include <QLabel>
#include <QDebug>
#include "./CTCMainWindow/BaseWndClass/CTCMainWindow.h"
#include "Global.h"
#include "./StationObject/Device/DeviceBase.h"
#include "./StationObject/Device/StaDevice.h"
#include <QPushButton>
#pragma execution_character_set("utf-8")
namespace CTCWindows {
	namespace  Control {
		LntervallogicCheck::LntervallogicCheck(QWidget* parent)
			: QMainWindow(parent)
		{
			ui.setupUi(this);
			setFixedSize(this->width(), this->height());
			m_pSectionOccupation = new	StationZoningSelection;
			m_pSectionTotalUnlock = new	StationZoningSelection;
			m_pSectionLogicalCheckOn = new	StationZoningSelection;
			m_pSectionLogicalCheckOff = new StationZoningSelection;

			setWindowIcon(QIcon(":/CTCProject/icon/icon.ico"));

			QWidget* OccupationWidget = ui.tabWidget->widget(0);
			QVBoxLayout* Occupation = new QVBoxLayout(OccupationWidget);
			QWidget* TotalUnlockWidget = ui.tabWidget->widget(1);
			QVBoxLayout* TotalUnlocklayout = new QVBoxLayout(TotalUnlockWidget);
			QWidget* CheckOnWidget = ui.tabWidget->widget(2);
			QVBoxLayout* CheckOnlayout = new QVBoxLayout(CheckOnWidget);
			QWidget* CheckOffWidget = ui.tabWidget->widget(3);
			QVBoxLayout* CheckOfflayout = new QVBoxLayout(CheckOffWidget);
			QVector<QString> vecTrackname;
			QVector<QString> vecJZTrackname;
			QStringList stationTrack;
			QStringList stationJZTrack;
			QStringList stationid;
			Station::Device::StaSignal* pSignal = nullptr;
			Station::Device::StaTrack* pTrack = nullptr;

			Occupation->addWidget(m_pSectionOccupation);
			connect(ui.tabWidget, &QTabWidget::currentChanged, this, &LntervallogicCheck::setTitle);
			TotalUnlocklayout->addWidget(m_pSectionTotalUnlock);
			CheckOnlayout->addWidget(m_pSectionLogicalCheckOn);
			CheckOfflayout->addWidget(m_pSectionLogicalCheckOff);

			stationid.append(Station::MainStation()->getStationName());

			for (Station::Device::DeviceBase* pDevice : Station::MainStation()->getDeviceVectorByType(SIGNALLAMP)) {
				pSignal = dynamic_cast<Station::Device::StaSignal*>(pDevice);
				qDebug() << pSignal->getType() << pSignal->getName() << pSignal->getXHDTYpe();
				if (pSignal->getXHDTYpe() == "JZ_XHJ") {
					stationJZTrack.append(pSignal->getName());
				}
			}

			for (Station::Device::DeviceBase* pDevice : Station::MainStation()->getDeviceVectorByType(TRACK)) {
				pTrack = dynamic_cast<Station::Device::StaTrack*>(pDevice);
				qDebug() << pTrack->getStrType() << pTrack->getName() << pTrack->getSXThroat() << pTrack->getAttr() << pTrack->getElapsed() << pTrack->TrackType();
				if (pTrack->TrackType() == "JJ_QD") {
					stationTrack.append(pTrack->getName());
				}
			}

			m_pSectionOccupation->SetCombobox(stationid, stationTrack, "无车验证", "无车执行", "闭塞分区");
			m_pSectionTotalUnlock->SetCombobox(stationid, stationJZTrack, "总解锁验证", "总解锁执行", "区间方向口");
			m_pSectionLogicalCheckOn->SetCombobox(stationid, stationJZTrack, "开启验证", "开启执行", "区间方向口");
			m_pSectionLogicalCheckOff->SetCombobox(stationid, stationJZTrack, "关闭验证", "关闭执行", "区间方向口");

			ui.tabWidget->setCurrentIndex(0);
		}

		LntervallogicCheck::~LntervallogicCheck()
		{}

		void LntervallogicCheck::setTitle(int index)
		{
			QString tabLabel = ui.tabWidget->tabText(index);
			this->setWindowTitle(tabLabel);
			ui.textEdit->setText(QString("测试	%1").arg(index));
		}
	}
}