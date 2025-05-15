#include "StationZoningSelection.h"
#include "Global.h"
#include "./StationObject/Device/StaDevice.h"
#pragma execution_character_set("utf-8")
namespace CTCWindows {
	namespace  Control {
		StationZoningSelection::StationZoningSelection(QWidget* parent)
			: QMainWindow(parent)
		{
			ui.setupUi(this);
		}

		StationZoningSelection::~StationZoningSelection()
		{}

		void StationZoningSelection::SetCombobox(QStringList stationid, QStringList Comcobox_2, QString bt1, QString bt2, QString label1)
		{
			ui.stationcomboBox->clear();
			ui.subzonecomboBox->clear();
			ui.stationcomboBox->addItems(stationid);
			ui.subzonecomboBox->addItems(Comcobox_2);
			ui.label_2->setText(label1);
			ui.pushButton->setText(bt1);
			ui.pushButton_2->setText(bt2);
			connect(ui.pushButton, &QPushButton::clicked, [&]() {qDebug() << getStationId(); });
			connect(ui.pushButton_2, &QPushButton::clicked, [&]() {qDebug() << getSubzone(); });
		}

		QString StationZoningSelection::getStationId()
		{
			return ui.stationcomboBox->currentText();
		}

		QString StationZoningSelection::getSubzone()
		{
			return ui.subzonecomboBox->currentText();
		}
	}
}