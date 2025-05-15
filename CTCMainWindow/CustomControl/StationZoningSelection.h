#pragma once

#include <QMainWindow>
#include "ui_StationZoningSelection.h"
#include "./CTCMainWindow/BaseWndClass/CTCMainWindow.h"
namespace CTCWindows {
	namespace  Control {
		class StationZoningSelection : public QMainWindow
		{
			Q_OBJECT

		public:
			StationZoningSelection(QWidget* parent = nullptr);
			~StationZoningSelection();
			void SetCombobox(QStringList stationid, QStringList Comcobox_2, QString bt1, QString bt2, QString label1);
			QString getStationId();
			QString getSubzone();
		private:
			Ui::StationZoningSelectionClass ui;
		};
	}
}