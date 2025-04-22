#pragma once

#include <QObject>
#include <QWidget>
namespace CTCWindows {
	namespace BaseWnd {
		class StaAlarmWindow : public QWidget
		{
			Q_OBJECT

		public:
			StaAlarmWindow(QWidget* parent);
			~StaAlarmWindow();
		};
	}
}