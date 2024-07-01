#pragma once
#include <QWidget>
#include "../../BaseWndClass/RoutePlanWidget/StaRoutePlan.h"
#include "ui_StaRoutePlanTKY.h"

namespace CTCWindows {
	namespace CARS {
		class StaRoutePlanTKY : public BaseWnd::StaRoutePlan
		{
			Q_OBJECT
		public:
			StaRoutePlanTKY(QWidget* parent = nullptr);
			~StaRoutePlanTKY();

			QSize sizeHint() const { return QSize(width(), minimumHeight()); }

		private:
			Ui::StaRoutePlanTKY ui;
		};
	}
}
