#pragma once

#include <QMainWindow>
#include "ui_EditingInterfaceTKY.h"
#include "Global.h"

namespace CTCWindows {
	namespace CARS {
		class EditingInterfaceTKY : public QMainWindow
		{
			Q_OBJECT

		public:
			EditingInterfaceTKY(QWidget* parent = nullptr);
			~EditingInterfaceTKY();

		public:
			void setLinEditText(Station::Device::StaTextSign* pTextSign);

		private:
			QColor getColor(int nIndex);

		private:
			Ui::EditingInterfaceTKYClass ui;
			Station::Device::StaTextSign* m_pTextSign = nullptr;
		};
	}
}
