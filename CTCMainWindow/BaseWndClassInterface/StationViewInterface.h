#pragma once
#include <QWidget>

namespace CTCWindows {
	class StationViewInterface : public QWidget
	{
		Q_OBJECT
	public:
		StationViewInterface(QWidget* pParent = nullptr) {}
		virtual ~StationViewInterface() = default; // 虚析构函数

	public slots:
		virtual void onFunBtnStateReset() {}

	signals:
		void OrderClear();
		void SelectDeviceClear();
		void OrderIssued();
	};
}