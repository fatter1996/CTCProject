#include "SealTechnique.h"
#include "QHeaderView"
#include "QStringListModel"
#include <QDebug>
#pragma execution_character_set("utf-8")
QMap<QString, QMap<QString, int>> SealTechnique::m_mStationSeal;

SealTechnique::SealTechnique(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
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
	QStringListModel* model = new QStringListModel(this);
	model->setStringList(stationNames);
	ui.listView->setModel(model);
	ui.listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QObject::connect(ui.listView, &QListView::pressed, [=](QModelIndex pos) {
		QString key = model->data(pos, Qt::DisplayRole).toString();
		QMap<QString, int> value = m_mStationSeal.value(key);
		SetTableWidget(value);
	});
}

void SealTechnique::SetTableWidget(QMap<QString, int> SealTechnique)
{
	ui.tableWidget->setRowCount(0);
	ui.tableWidget->horizontalHeader()->setVisible(true);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget->verticalHeader()->setVisible(false);
	QStringList predefinedTexts = { "X总辅助",   "X接车辅助",  "X发车辅助",
									"XF总辅助",  "XF接车辅助", "XF发车辅助" ,
									"SF总辅助",  "SF接车辅助", "SF发车辅助",
									"S总辅助",   "S接车辅助",  "S发车辅助" ,
									"X引导总锁", "X允许改方",
									"S引导总锁", "S允许改方",
									"故障通知",  "总人解" ,    "区故解"};

	QTableWidgetItem* textItem = nullptr;
	QTableWidgetItem* valueItem = nullptr;
	int value = 0;
	int row = 0;
	for (const QString& text : predefinedTexts) {

		row = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(row);
		textItem = new QTableWidgetItem(text);
		ui.tableWidget->setItem(row, 0, textItem);

		value = SealTechnique.value(text, 0);
		valueItem = new QTableWidgetItem(QString::number(value));
		ui.tableWidget->setItem(row, 1, valueItem);
	}
	
}

void SealTechnique::SetMapSeal(StationSeal m_nStationSeal,int frequency)
{
	switch (m_nStationSeal)
	{
	case StationSeal::XZ:
		m_mSealTechnique.insert("X总辅助", frequency);
		break;
	case StationSeal::XJ:
		m_mSealTechnique.insert("X接车辅助", frequency);
		break;
	case StationSeal::XF:
		m_mSealTechnique.insert("X发车辅助", frequency);
		break;
	case StationSeal::XFZ:
		m_mSealTechnique.insert("XF总辅助", frequency);
		break;
	case StationSeal::XFJ:
		m_mSealTechnique.insert("XF接车辅助", frequency);
		break;
	case StationSeal::XFF:
		m_mSealTechnique.insert("XF发车辅助", frequency);
		break;
	case StationSeal::SFZ:
		m_mSealTechnique.insert("SF总辅助", frequency);
		break;
	case StationSeal::SFJ:
		m_mSealTechnique.insert("SF接车辅助", frequency);
		break;
	case StationSeal::SFF:
		m_mSealTechnique.insert("SF发车辅助", frequency);
		break;
	case StationSeal::SZ:
		m_mSealTechnique.insert("S总辅助", frequency);
		break;
	case StationSeal::SJ:
		m_mSealTechnique.insert("S接车辅助", frequency);
		break;
	case StationSeal::SF:
		m_mSealTechnique.insert("S发车辅助", frequency);
		break;
	case StationSeal::XY:
		m_mSealTechnique.insert("X引导总锁", frequency);
		break;
	case StationSeal::XG:
		m_mSealTechnique.insert("X允许改方", frequency);
		break;
	case StationSeal::SY:
		m_mSealTechnique.insert("S引导总锁", frequency);
		break;
	case StationSeal::SG:
		m_mSealTechnique.insert("S允许改方", frequency);
		break;
	case StationSeal::GZ:
		m_mSealTechnique.insert("故障通知", frequency);
		break;
	case StationSeal::ZRJ:
		m_mSealTechnique.insert("总人解", frequency);
		break;
	case StationSeal::QGJ:
		m_mSealTechnique.insert("区故解", frequency);
		break;
	case StationSeal::ZDW:
		m_mSealTechnique.insert("总定位", frequency);
		break;
	case StationSeal::ZFW:
		m_mSealTechnique.insert("总反位", frequency);
		break;
	case StationSeal::DS:
		m_mSealTechnique.insert("单锁", frequency);
		break;
	case StationSeal::DJ:
		m_mSealTechnique.insert("单解", frequency);
		break;
	case StationSeal::FS:
		m_mSealTechnique.insert("封锁", frequency);
		break;
	case StationSeal::JF:
		m_mSealTechnique.insert("解封", frequency);
		break;
	case StationSeal::PDJS:
		m_mSealTechnique.insert("坡道解锁", frequency);
		break;
	case StationSeal::FLBL:
		m_mSealTechnique.insert("分路不良", frequency);
		break;
	case StationSeal::DD:
		m_mSealTechnique.insert("点灯", frequency);
		break;
	case StationSeal::MD:
		m_mSealTechnique.insert("灭灯", frequency);
		break;
	default:
		break;
	}
}

void SealTechnique::SetStationSeal(QString station)
{
	m_mStationSeal.insert(station, m_mSealTechnique);
}
