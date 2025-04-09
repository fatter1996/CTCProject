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
			case CTCWindows::FunType::GuideBtn:			//������ť
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					Station::MainStationObject* Station = Station::MainStation();
					QString stationName = Station->getStationName();
					SealTechnique::InsertSealRecord(stationName, "������ť");
					
				}
				break;
			case CTCWindows::FunType::GuideClock:		//��������
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					Station::MainStationObject* Station = Station::MainStation();
					QString stationName = Station->getStationName();
					SealTechnique::InsertSealRecord(stationName, "��������");
					
				}
				break;
			case CTCWindows::FunType::TotalRelieve:	    //���˽�
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					Station::MainStationObject* Station = Station::MainStation();
					QString stationName = Station->getStationName();
					SealTechnique::InsertSealRecord(stationName, "���˽�");
				}
				break;
			case CTCWindows::FunType::RegionRelieve:	//���ʽ�
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					Station::MainStationObject* Station = Station::MainStation();
					QString stationName = Station->getStationName();
					SealTechnique::InsertSealRecord(stationName, "���ʽ�");
				}
				break;
			case CTCWindows::FunType::UnLighting:		//���
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					Station::MainStationObject* Station = Station::MainStation();
					QString stationName = Station->getStationName();
					SealTechnique::InsertSealRecord(stationName, "���");
				}
				break;
			case CTCWindows::FunType::RampUnlock:	    //�µ�����
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					Station::MainStationObject* Station = Station::MainStation();
					QString stationName = Station->getStationName();
					SealTechnique::InsertSealRecord(stationName, "�µ�����");
				}
				break;
			case CTCWindows::FunType::PoorRoute: {		//��·����
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					Station::MainStationObject* Station = Station::MainStation();
					QString stationName = Station->getStationName();
					SealTechnique::InsertSealRecord(stationName,"��·����" );
				
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