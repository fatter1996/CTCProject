#include "SealTechnique.h"
#include <QHeaderView>
#include <QStringListModel>
#include <QDebug>
#include "Global.h"
#include "./StationObject/StationObject.h"
#include "./StationObject/Device/DeviceBase.h"

#pragma execution_character_set("utf-8")

namespace CTCWindows {
	QMap<QString, QMap<QString, int>> SealTechnique::m_mapStationSeal;

	SealTechnique::SealTechnique(QWidget* parent)
		: QWidget(parent)
	{
		ui.setupUi(this);
		InitListView();
		InitTableWidget();
		InitSealRecord(Station::MainStation());
		UpdaTableWidget(Station::MainStation());
	}

	SealTechnique::~SealTechnique()
	{
	
	}

	void SealTechnique::InitTableWidget()
	{
		ui.tableWidget->verticalHeader()->hide();
		QVector<Control::TableViewHeadInfo> vecHeadInfo1 = {
			{ "按钮名称", 96 },
			{ "铅封计数", 56 }
		};
		ui.tableWidget->SetHeadData(vecHeadInfo1, HHead);
	}

	void SealTechnique::UpdaTableWidget(Station::StationObject* pStation)
	{
		QVector<QStringList> vecTableData;
		QStringList strRowTableData;
		if (!m_mapStationSeal.contains(pStation->getStationName())) {
			return;
		}
		for (const QString& strKey : m_mapStationSeal[pStation->getStationName()].keys()) {
			strRowTableData.clear();
			strRowTableData << strKey << QString("%1").arg(m_mapStationSeal[pStation->getStationName()][strKey]);
			vecTableData.append(strRowTableData);
		}
		ui.tableWidget->ResetTableRows(vecTableData);
	}

	void SealTechnique::InitListView()
	{
		QStringListModel* model = new QStringListModel(this);
		model->setStringList(QStringList() << Station::MainStation()->getStationName());
		ui.listView->setModel(model);
		ui.listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
		QObject::connect(ui.listView, &QListView::clicked, [=](QModelIndex pos) {
			InitSealRecord(Station::MainStation());
			UpdaTableWidget(Station::MainStation());
		});
		ui.listView->setCurrentIndex(model->index(0));
	}

	void SealTechnique::InitSealRecord(Station::StationObject* pStation)
	{
		m_mapStationSeal.clear();
		QString strStationName = pStation->getStationName();
		for (Station::Device::DeviceBase* pAutoBlock : Station::MainStation()->getDeviceVectorByType(AUTOBLOCK)) {
			m_mapStationSeal[strStationName].insert(pAutoBlock->getName() + "总辅助", 0);
			m_mapStationSeal[strStationName].insert(pAutoBlock->getName() + "接车辅助", 0);
			m_mapStationSeal[strStationName].insert(pAutoBlock->getName() + "发车辅助", 0);
			if (dynamic_cast<Station::Device::StaAutoBlock*>(pAutoBlock)->IsHaveAllowBtn()) {
				m_mapStationSeal[strStationName].insert(pAutoBlock->getName() + "允许改方", 0);
			}
		}
		for (Station::Device::DeviceBase* pSemiAutoBlock : Station::MainStation()->getDeviceVectorByType(SEMIAUTOBLOCK)) {
			m_mapStationSeal[strStationName].insert(pSemiAutoBlock->getName() + "事故", 0);
			m_mapStationSeal[strStationName].insert(pSemiAutoBlock->getName() + "复原", 0);
			m_mapStationSeal[strStationName].insert(pSemiAutoBlock->getName() + "闭塞", 0);
		}
		m_mapStationSeal[strStationName].insert("S引导总锁", 0);
		m_mapStationSeal[strStationName].insert("X引导总锁", 0);

		m_mapStationSeal[strStationName].insert("引导按钮", 0);
		m_mapStationSeal[strStationName].insert("总人解", 0);
		m_mapStationSeal[strStationName].insert("区故解", 0);
		m_mapStationSeal[strStationName].insert("坡道解锁", 0);
		m_mapStationSeal[strStationName].insert("分路不良", 0);
		m_mapStationSeal[strStationName].insert("点灯", 0);
		m_mapStationSeal[strStationName].insert("灭灯", 0);
	}

	void SealTechnique::InsertSealRecord(QString strStation, QString strStationSeal)
	{
		if (!m_mapStationSeal.contains(strStation)) {
			return;
		}
		if (!m_mapStationSeal[strStation].contains(strStationSeal)) {
			return;
		}
		m_mapStationSeal[strStation][strStationSeal] += 1;
	}
}
