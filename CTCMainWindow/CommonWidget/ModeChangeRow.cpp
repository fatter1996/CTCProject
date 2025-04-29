#include "ModeChangeRow.h"
#include "Global.h"
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	ModeChangeRow::ModeChangeRow(QWidget* parent)
		: QWidget(parent)
	{
		ui.setupUi(this);
		connect(ui.buttonGroup, qOverload<int>(&QButtonGroup::buttonClicked), [&]() {
			emit SubRadioBtnChecked();
		});

		connect(ui.checkBox1, &QCheckBox::clicked, [&](bool bChecked) {
			emit SubCheckBtnChecked();
		});
		ui.buttonGroup->setId(ui.radioBtn1, 0);
		ui.buttonGroup->setId(ui.radioBtn2, 1);
		ui.buttonGroup->setId(ui.radioBtn3, 2);
	}

	ModeChangeRow::~ModeChangeRow()
	{
	}

	void ModeChangeRow::InitRow(Station::MainStationObject* pStation, int nType)
	{
		ui.staNameLabel->setText(pStation->getStationName());
		if (nType == MODO_CHANGE) {
			ui.colFrame1_2->hide();
			ui.radioBtn1->setText("中心控制");
			ui.radioBtn2->setText("车站控制");
			ui.radioBtn3->setText("车站调车");
			m_nCurrState = pStation->getStaLimits(Station::Limits::ControlMode);
			if (m_nCurrState == 0) {
				ui.radioBtn1->setStyleSheet("color: rgb(255, 0, 0);");
				ui.radioBtn1->setChecked(true);
			}
			else if (m_nCurrState == 1) {
				ui.radioBtn2->setStyleSheet("color: rgb(255, 0, 0);");
				ui.radioBtn2->setChecked(true);
			}
			else if (m_nCurrState == 2) {
				ui.radioBtn3->setStyleSheet("color: rgb(255, 0, 0);");
				ui.radioBtn3->setChecked(true);
			}
		}
		else if (nType == MODO_CHANGE_AGREE) {
			ui.checkBox1->setText("同意");
			ui.checkBox1->setChecked(true);
			ui.radioBtn1->setText("中心控制");
			ui.radioBtn2->setText("车站控制");
			ui.radioBtn3->setText("车站调车");

			ui.radioBtn1->setEnabled(false);
			ui.radioBtn2->setEnabled(false);
			ui.radioBtn3->setEnabled(false);

			m_nCurrState = pStation->getStaLimits(Station::Limits::ControlMode);   //控制模式, 0 - 中心控制，1 - 车站控制，2 - 车站调车
			
			if (m_nCurrState == 0) {
				ui.radioBtn1->setStyleSheet("color: rgb(255, 0, 0);");
				ui.radioBtn1->setChecked(true);
			}
			else if (m_nCurrState == 1) {
				ui.radioBtn2->setStyleSheet("color: rgb(255, 0, 0);");
				ui.radioBtn2->setChecked(true);
			}
			else if (m_nCurrState == 2) {
				ui.radioBtn3->setStyleSheet("color: rgb(255, 0, 0);");
				ui.radioBtn3->setChecked(true);
			}

			m_nCurrState = pStation->getStaLimits(Station::Limits::ApplyControlMode);
			if (m_nCurrState == 0) {
				ui.radioBtn1->setStyleSheet("color: rgb(255, 170, 0);");
				ui.radioBtn1->setChecked(true);
			}
			else if (m_nCurrState == 1) {
				ui.radioBtn2->setStyleSheet("color: rgb(255, 170, 0);");
				ui.radioBtn2->setChecked(true);
			}
			else if (m_nCurrState == 2) {
				ui.radioBtn3->setStyleSheet("color: rgb(255, 170, 0);");
				ui.radioBtn3->setChecked(true);
			}
		}
		else if (nType == MANNER_CHANGE) {
			ui.colFrame4_2->hide();
			ui.checkBox1->setText("计划控制");
			ui.checkBox1->setChecked(true);
			ui.radioBtn1->setText("按图排路");
			ui.radioBtn2->setText("手工排路");

			m_nCurrState = pStation->getStaLimits(Station::Limits::PlanMode);   //计划模式，0 - 手工排路，1 - 按图排路
			if (m_nCurrState == 0) {
				ui.radioBtn1->setStyleSheet("color: rgb(255, 0, 0);");
				ui.radioBtn1->setChecked(true);
			}
			else if (m_nCurrState == 1) {
				ui.radioBtn2->setStyleSheet("color: rgb(255, 0, 0);");
				ui.radioBtn2->setChecked(true);
			}
		}
	}
	
	void ModeChangeRow::SetRadioBtnChecked(int nIndex, bool bChecked)
	{
		ui.buttonGroup->button(nIndex)->setChecked(bChecked);;
	}

	void ModeChangeRow::SetCheckBtnChecked(bool bChecked)
	{
		ui.checkBox1->setChecked(bChecked);
	}

	int ModeChangeRow::GetState(int nType)
	{
		if (nType == MODO_CHANGE_AGREE) {
			return ui.checkBox1->isChecked();
		}
		else {
			return ui.buttonGroup->id(ui.buttonGroup->checkedButton());
		}
	}
}
