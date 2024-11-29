#include "StaDispatchOrderKSK.h"
#include <QTabBar>
namespace CTCWindows {
	namespace CASCO {
		StaDispatchOrderKSK::StaDispatchOrderKSK(QWidget* parent)
			: BaseWnd::StaDispatchOrder(parent)
		{
			ui.setupUi(this);
			ui.tabWidget->tabBar()->hide();
		}

		StaDispatchOrderKSK::~StaDispatchOrderKSK()
		{}
	}
}
