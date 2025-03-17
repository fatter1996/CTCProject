#include "StaFunBtnToolBar.h"
#include "Global.h"
#include "CommonWidget/LeadSealDlg.h"
#include "CommonWidget/ModeChangeWnd.h"
#pragma execution_character_set("utf-8")
namespace CTCWindows {
	namespace BaseWnd {
		FunType StaFunBtnToolBar::m_SelectFunType = FunType::RouteBuild;
		OperObjType StaFunBtnToolBar::m_nOperObjType = OperObjType::Defult;

		StaFunBtnToolBar::StaFunBtnToolBar(QWidget* parent)
			: QWidget(parent)
		{

			m_nTimerID_500 = startTimer(500);
		}

		StaFunBtnToolBar::~StaFunBtnToolBar()
		{

		}

		void StaFunBtnToolBar::onButtonClicked(QAbstractButton* pButton)
		{
			ButtonClicked(m_mapFunBtnType[pButton]);
		}

		void StaFunBtnToolBar::ButtonClicked(FunType eSelectType)
		{
			Station::MainStationObject* Station = Station::MainStation();
			
			QString stationName = Station->getStationName();
			m_SelectFunType = eSelectType;
			switch (m_SelectFunType)
			{
			case CTCWindows::FunType::GuideBtn:			//引导按钮
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					Station::MainStationObject* Station = Station::MainStation();
					QString stationName = Station->getStationName();
					SealTechnique::InsertSealRecord(stationName, "引导按钮");
					
				}
				break;
			case CTCWindows::FunType::GuideClock:		//引导总锁
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					Station::MainStationObject* Station = Station::MainStation();
					QString stationName = Station->getStationName();
					SealTechnique::InsertSealRecord(stationName, "引导总锁");
					
				}
				break;
			case CTCWindows::FunType::TotalRelieve:	    //总人解
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					Station::MainStationObject* Station = Station::MainStation();
					QString stationName = Station->getStationName();
					SealTechnique::InsertSealRecord(stationName, "总人解");
				}
				break;
			case CTCWindows::FunType::RegionRelieve:	//区故解
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					Station::MainStationObject* Station = Station::MainStation();
					QString stationName = Station->getStationName();
					SealTechnique::InsertSealRecord(stationName, "区故解");
				}
				break;
			case CTCWindows::FunType::UnLighting:		//灭灯
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					Station::MainStationObject* Station = Station::MainStation();
					QString stationName = Station->getStationName();
					SealTechnique::InsertSealRecord(stationName, "灭灯");
				}
				break;
			case CTCWindows::FunType::RampUnlock:	    //坡道解锁
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					Station::MainStationObject* Station = Station::MainStation();
					QString stationName = Station->getStationName();
					SealTechnique::InsertSealRecord(stationName, "坡道解锁");
				}
				break;
			case CTCWindows::FunType::PoorRoute: {		//分路不良
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					Station::MainStationObject* Station = Station::MainStation();
					QString stationName = Station->getStationName();
					SealTechnique::InsertSealRecord(stationName,"分路不良" );
				
				}
			} break;
			default:
				break;
			}
			Station::MainStation()->ClearDevice();
		}

		void StaFunBtnToolBar::timerEvent(QTimerEvent* event)
		{
			if (event->timerId() == m_nTimerID_500) {
				if (m_pCommandClearBtn) {
					m_pCommandClearBtn->setEnabled(Station::MainStation()->getSelectDevice().size());
				}
				if (m_pCommandIssuedBtn) {
					m_pCommandIssuedBtn->setEnabled(Station::MainStation()->IsAllowStaOperation());
				}
			}
		}

		void StaFunBtnToolBar::onAuxiliaryMenuBtnClicked()
		{
			AuxiliaryMenuWnd* pAuxiliary = new AuxiliaryMenuWnd;
			pAuxiliary->setAttribute(Qt::WA_DeleteOnClose);
			InitAuxiliaryMenu(pAuxiliary);
			pAuxiliary->move(QPoint(QCursor::pos().x(), QCursor::pos().y() - pAuxiliary->height()));
			pAuxiliary->exec();
		}

		void StaFunBtnToolBar::onMethodConvertBtnClicked()
		{
			ModeChangeWnd* pModeChange = new ModeChangeWnd;
			pModeChange->setAttribute(Qt::WA_DeleteOnClose);
			pModeChange->Init(MODO_CHANGE);
			pModeChange->exec();
		}

		void StaFunBtnToolBar::onFunBtnStateReset()
		{
			if (!m_pButtonGroup) {
				return;
			}

			for (QAbstractButton* pButton : m_pButtonGroup->buttons()) {
				pButton->setEnabled(Station::MainStation()->IsAllowStaOperation());
			}
			
			if (Station::MainStation()->IsAllowStaOperation()) {
				if (m_pRouteBuildBtn) {
					m_pRouteBuildBtn->setChecked(true);
				}
				m_SelectFunType = FunType::RouteBuild;
			}
			else {
				for (QAbstractButton* pButton : m_pButtonGroup->buttons()) {
					pButton->setChecked(false);
				}
			}
		}
	}
}