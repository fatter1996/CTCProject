#pragma once

#include <QDialog>
#include "ui_StagePlanSign.h"
#include "CustomControl/TableView.h"

namespace CTCWindows {
	class StagePlanSign : public QDialog
	{
		Q_OBJECT
	public:
		StagePlanSign(QWidget* parent = nullptr);
		~StagePlanSign();

		void InitStagePlan();

	private:
		Ui::StagePlanSign ui;
		Control::TableView* m_pStagePlanSignTable = nullptr;	//列车进路表格
	};
}
