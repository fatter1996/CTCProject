#include "SealTechnique.h"
#include "QHeaderView"
#include "QStringListModel"
#include <QDebug>
#include "./StationObject/StationObject.h"
#include "./StationObject/Device/DeviceBase.h"
#include "Global.h"
#pragma execution_character_set("utf-8")
QMap<QString, QMap<QString, int>> SealTechnique::m_mStationSeal;
SealTechnique* SealTechnique::m_pSealTechnique = nullptr;
SealTechnique::SealTechnique(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	InsertSealRecord(Station::MainStation()->getStationName(), "");
	ui.tableWidget->setColumnWidth(1, 100);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	

}

SealTechnique::~SealTechnique()
{}

void SealTechnique::SetTable()
{

	QStringList stationNames = m_mStationSeal.keys();
	stationNames = m_mStationSeal.keys();
	if (m_mStationSeal.size() == 0) {
		stationNames.join( Station::MainStation()->getStationName());
	}

	QStringListModel* model = new QStringListModel(this);
	model->setStringList(stationNames);
	ui.listView->setModel(model);
	QMap<QString, int> value = m_mStationSeal.value(0);
	SetTableWidget(value, Station::MainStation()->ReadMapDevice());
	ui.listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QObject::connect(ui.listView, &QListView::pressed, [=](QModelIndex pos) {
		QStringList predefinedTexts;
		Station::MainStationObject* Station = Station::MainStation();
	
		predefinedTexts = Station->ReadMapDevice();
	
		QString key = model->data(pos, Qt::DisplayRole).toString();
		QMap<QString, int> value = m_mStationSeal.value(key);
		SetTableWidget(value, predefinedTexts);
	});
}

void SealTechnique::SetTableWidget(QMap<QString, int> SealTechnique, QStringList predefinedTexts)
{

	ui.tableWidget->setRowCount(0);
	ui.tableWidget->horizontalHeader()->setVisible(true);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget->verticalHeader()->setVisible(false);
	QTableWidgetItem* textItem = nullptr;
	QTableWidgetItem* valueItem = nullptr;
	int value = 0;
	int row = 0;

	QStringList BtnStringList = {"功能按钮","引导按钮","引导总锁","总人解","区故解","灭灯","坡道解锁","分路不良"};

								
	for (const QString& text : predefinedTexts) {

		row = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(row);
		textItem = new QTableWidgetItem(text);
		ui.tableWidget->setItem(row, 0, textItem);

		value = SealTechnique.value(text, 0);
		valueItem = new QTableWidgetItem(QString::number(value));
		ui.tableWidget->setItem(row, 1, valueItem);
	}

	for (const QString& text : BtnStringList) {

		row = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(row);
		textItem = new QTableWidgetItem(text);
		ui.tableWidget->setItem(row, 0, textItem);

		value = SealTechnique.value(text, 0);
		valueItem = new QTableWidgetItem(QString::number(value));
		ui.tableWidget->setItem(row, 1, valueItem);
	}
	
}


void SealTechnique::InsertSealRecord(QString station, QString StationSeal)
{
	m_mStationSeal[station][StationSeal] += 1;
}
