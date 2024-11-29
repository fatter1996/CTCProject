#pragma once

#include <QDialog>
#include "CustomControl/TableView.h"
#include "ui_DispatchOrderSign.h"
#include "StationObject/GlobalStruct.h"

namespace CTCWindows {
	class DispatchOrderSign : public QDialog
	{
		Q_OBJECT
	public:
		DispatchOrderSign(QWidget* parent = nullptr);
		~DispatchOrderSign();

		void InitNewDispatchOrder(Station::StaDispatchOrder* pNewDispatchOrder);

	private:
		Ui::DispatchOrderSign ui;
		Control::TableView* m_pOrderedUnitTable = nullptr;	//列车进路表格
	};
}
