#include "ModeChangeWnd.h"
#include "Global.h"

namespace CTCWindows {
	ModeChangeWnd::ModeChangeWnd(QWidget* parent)
		: QDialog(parent)
	{
		ui.setupUi(this);
		setWindowIcon(QIcon("icon/icon.ico"));
		connect(ui.buttonGroup, qOverload<int, bool>(&QButtonGroup::buttonToggled), [&](int nIndex, bool bChecked) {
			for (ModeChangeRow* pModeChangeRow : m_vecModeChangeRow) {
				pModeChangeRow->SetRadioBtnChecked(nIndex, bChecked);
			}
		});
		connect(ui.checkBox, &QCheckBox::clicked, [&](bool bChecked) {
			for (ModeChangeRow* pModeChangeRow : m_vecModeChangeRow) {
				pModeChangeRow->SetCheckBtnChecked(bChecked);
			}
		});
	}

	ModeChangeWnd::~ModeChangeWnd()
	{
	}

	void ModeChangeWnd::InitModeChange(int nType, const QList<Station::MainStationObject*>& listStation)
	{
		m_listStation = listStation;
		if (nType == MODO_CHANGE) {
			ui.colFrame1->hide();
		}
		else if (nType == MANNER_CHANGE) {
			ui.colFrame4->hide();
		}
		else if (nType == MODO_CHANGE_AGREE) {
			ui.radioButton->setEnabled(false);
			ui.radioButton_2->setEnabled(false);
			ui.radioButton_3->setEnabled(false);
		}
		for (int i = 0; i < m_listStation.size(); i++) {
			ModeChangeRow* pModeChangeRow = new ModeChangeRow();
			pModeChangeRow->InitRow(listStation[i], nType);
			connect(pModeChangeRow, &ModeChangeRow::SubRadioBtnChecked, this, &ModeChangeWnd::onSubRadioBtnChecked);
			connect(pModeChangeRow, &ModeChangeRow::SubCheckBtnChecked, this, &ModeChangeWnd::onSubCheckBtnChecked);
			static_cast<QHBoxLayout*>(ui.frame->layout())->insertWidget(i + 1, pModeChangeRow);
			m_vecModeChangeRow.append(pModeChangeRow);
		}
		
		connect(ui.enterBtn, &QPushButton::clicked, [&]() {
			int nState = 0;
			for (int i = 0; i < m_listStation.size(); i++) {
				nState = m_vecModeChangeRow[i]->GetState(nType);
				if (nType == MODO_CHANGE_AGREE) {
					if (m_vecModeChangeRow[i]->getCurrState() == -1) {
						continue;
					}
					if (nState) {
						QByteArray btResult;
						if (Http::HttpClient::UpdataStaLimits(Station::Limits::ControlMode, m_vecModeChangeRow[i]->getCurrState(), btResult)) {
							m_listStation[i]->setStaLimits(Station::Limits::ControlMode, m_vecModeChangeRow[i]->getCurrState());
						}
					}
					m_listStation[i]->setStaLimits(Station::Limits::ApplyControlMode, -1);
					m_listStation[i]->SendPacketMsg(TARGET_TEACHER, 0x70, 0x05, nState);
				}
				else if (m_vecModeChangeRow[i]->getCurrState() != nState) {
					m_listStation[i]->SendPacketMsg(TARGET_TEACHER, 0x70, nType, nState);
					if (nType == MODO_CHANGE) {
						m_listStation[i]->setStaLimits(Station::Limits::ActiveApplyControlMode, nState);
					}
				}
			}
			this->close();
		});
		connect(ui.cancelBtn, &QPushButton::clicked, this, &ModeChangeWnd::close);
	}

	void ModeChangeWnd::onSubRadioBtnChecked()
	{
		ui.buttonGroup->setExclusive(false);
		for (QAbstractButton* pRadioBtn : ui.buttonGroup->buttons()) {
			static_cast<QRadioButton*>(pRadioBtn)->setChecked(false);
		}
		ui.buttonGroup->setExclusive(true);
	}

	void ModeChangeWnd::onSubCheckBtnChecked()
	{
		ui.checkBox->setChecked(false);
	}
}
