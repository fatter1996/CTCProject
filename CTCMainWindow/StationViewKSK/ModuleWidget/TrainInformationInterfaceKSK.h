#pragma once

#include <QWidget>
#include "ui_TrainInformationInterfaceKSK.h"
#include "CTCMainWindow/BaseWndClass/ModuleWidget/StaTreeNode.h"

namespace CTCWindows {
	namespace CASCO {
		class TrainInformationInterfaceKSK : public QWidget
		{
			Q_OBJECT

		public:
			TrainInformationInterfaceKSK(QWidget* parent = nullptr);
			~TrainInformationInterfaceKSK();
			void InitMyTree();
			void ConnectButton();
			QTreeWidgetItem* Item(QTreeWidgetItem* parent, QString text);
		signals:
			void CheckComplete(TrainInformationInterfaceKSK* pTrainInforma);
		private:
			CTCWindows::BaseWnd::StaTreeNode* m_pStaTreeNode = nullptr;
			Ui::TrainInformationInterfaceKSKClass ui;
		};
	}
}