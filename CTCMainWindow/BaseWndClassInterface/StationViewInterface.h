#pragma once
#include <QWidget>

namespace CTCWindows {
	class StationViewInterface : public QWidget
	{
		Q_OBJECT
	public:
		StationViewInterface() {}
		virtual ~StationViewInterface() = default; // 虚析构函数

	public:
		virtual void FunBtnStateReset() = 0;

	signals:
		void OrderClear();
		void OrderIssued();

	public slots:
		virtual void onOrderClear(); //命令清除
		virtual void onOrderIssued(); //命令下达

	};
}