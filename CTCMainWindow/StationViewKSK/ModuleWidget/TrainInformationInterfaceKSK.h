#pragma once

#include <QWidget>
#include "ui_TrainInformationInterfaceKSK.h"
#include "CTCMainWindow/BaseWndClass/ModuleWidget/StaTreeNode.h"
class TrainInformationInterfaceKSK : public QWidget
{
	Q_OBJECT

public:
	TrainInformationInterfaceKSK(QWidget *parent = nullptr);
	~TrainInformationInterfaceKSK();
	void InitMyTree();
	void ConnectButton();
	QTreeWidgetItem* Item(QTreeWidgetItem* parent, QString text);
signals:
	void CheckComplete(TrainInformationInterfaceKSK* pTrainInforma);
private:
	StaTreeNode* m_pStaTreeNode = nullptr;
	Ui::TrainInformationInterfaceKSKClass ui;
};
