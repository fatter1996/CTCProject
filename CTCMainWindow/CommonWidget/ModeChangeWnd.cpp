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

	void ModeChangeWnd::Init(int nType)
	{
		ModeChangeRow* pModeChangeRow = new ModeChangeRow();
		pModeChangeRow->InitRow(Station::MainStation()->getStationName(), nType);
		if (nType == MODO_CHANGE) {
			ui.colFrame1->hide();
		}
		if (nType == MANNER_CHANGE) {
			ui.colFrame4->hide();
		}

		connect(pModeChangeRow, &ModeChangeRow::SubRadioBtnChecked, this, &ModeChangeWnd::onSubRadioBtnChecked);
		connect(pModeChangeRow, &ModeChangeRow::SubCheckBtnChecked, this, &ModeChangeWnd::onSubCheckBtnChecked);
		
		static_cast<QHBoxLayout*>(ui.frame->layout())->insertWidget(
			ui.frame->layout()->indexOf(ui.frame_2) + 1, pModeChangeRow);
		m_vecModeChangeRow.append(pModeChangeRow);
		
		connect(ui.enterBtn, &QPushButton::clicked, [&]() {
			bool bChecked = true;
			int nState = pModeChangeRow->GetState(nType, &bChecked);
			if (nType == MODO_CHANGE) {
				Station::MainStation()->SendPacketMsg(TARGET_TEACHER, 0x70, 0x03, nState);
			}
			else if (nType == MODO_CHANGE_AGREE) {
				QByteArray btResult;
				if (Http::HttpClient::UpdataStaLimits(Station::Limits::PlanMode, nState, btResult)) {
					Station::MainStation()->setStaLimits(Station::Limits::ControlMode, nState);
				}
			}
			else if (nType == MANNER_CHANGE) {
				Station::MainStation()->SendPacketMsg(TARGET_TEACHER, 0x70, 0x04, nState);
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
