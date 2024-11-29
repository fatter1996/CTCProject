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
	}

	ModeChangeRow::~ModeChangeRow()
	{
	}

	void ModeChangeRow::InitRow(QString strName, int nType)
	{
		ui.staNameLabel->setText(strName);
		if (nType == MODO_CHANGE) {
			ui.colFrame1_2->hide();
			ui.radioBtn1->setText("���Ŀ���");
			ui.radioBtn2->setText("��վ����");
			ui.radioBtn3->setText("��վ����");
			int nControlMode = Station::MainStation()->getStaLimits(Station::Limits::ControlMode);
			if (nControlMode == 0) {
				ui.radioBtn1->setStyleSheet("color: rgb(255, 0, 0);");
				ui.radioBtn1->setChecked(true);
			}
			else if (nControlMode == 1) {
				ui.radioBtn2->setStyleSheet("color: rgb(255, 0, 0);");
				ui.radioBtn2->setChecked(true);
			}
			else if (nControlMode == 2) {
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

			int nControlMode = Station::MainStation()->getStaLimits(Station::Limits::ControlMode);   //����ģʽ, 0 - ���Ŀ��ƣ�1 - ��վ���ƣ�2 - ��վ����
			int nApplyControlMode = Station::MainStation()->getStaLimits(Station::Limits::ApplyControlMode);
			if (nControlMode == 0) {
				ui.radioBtn1->setStyleSheet("color: rgb(255, 0, 0);");
			}
			else if (nControlMode == 1) {
				ui.radioBtn2->setStyleSheet("color: rgb(255, 0, 0);");
			}
			else if (nControlMode == 2) {
				ui.radioBtn3->setStyleSheet("color: rgb(255, 0, 0);");
			}

			if (nApplyControlMode == 0) {
				ui.radioBtn1->setStyleSheet("color: rgb(255, 170, 0);");
				ui.radioBtn1->setChecked(true);
			}
			else if (nApplyControlMode == 1) {
				ui.radioBtn2->setStyleSheet("color: rgb(255, 170, 0);");
				ui.radioBtn2->setChecked(true);
			}
			else if (nApplyControlMode == 2) {
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

			int nPlanMode = Station::MainStation()->getStaLimits(Station::Limits::PlanMode);   //�ƻ�ģʽ��0 - �ֹ���·��1 - ��ͼ��·
			if (nPlanMode == 0) {
				ui.radioBtn1->setStyleSheet("color: rgb(255, 0, 0);");
				ui.radioBtn1->setChecked(true);
			}
			else if (nPlanMode == 1) {
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

	int ModeChangeRow::GetState(int nType, bool* bChecked)
	{
		if (nType == MODO_CHANGE_AGREE) {
			*bChecked = ui.checkBox1->isChecked();
		}
		return ui.buttonGroup->id(ui.buttonGroup->checkedButton());
	}
}
