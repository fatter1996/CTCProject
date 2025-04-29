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
			ui.radioBtn1->setText("���Ŀ���");
			ui.radioBtn2->setText("��վ����");
			ui.radioBtn3->setText("��վ����");
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
			ui.checkBox1->setText("ͬ��");
			ui.checkBox1->setChecked(true);
			ui.radioBtn1->setText("���Ŀ���");
			ui.radioBtn2->setText("��վ����");
			ui.radioBtn3->setText("��վ����");

			ui.radioBtn1->setEnabled(false);
			ui.radioBtn2->setEnabled(false);
			ui.radioBtn3->setEnabled(false);

			m_nCurrState = pStation->getStaLimits(Station::Limits::ControlMode);   //����ģʽ, 0 - ���Ŀ��ƣ�1 - ��վ���ƣ�2 - ��վ����
			
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
			ui.checkBox1->setText("�ƻ�����");
			ui.checkBox1->setChecked(true);
			ui.radioBtn1->setText("��ͼ��·");
			ui.radioBtn2->setText("�ֹ���·");

			m_nCurrState = pStation->getStaLimits(Station::Limits::PlanMode);   //�ƻ�ģʽ��0 - �ֹ���·��1 - ��ͼ��·
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
