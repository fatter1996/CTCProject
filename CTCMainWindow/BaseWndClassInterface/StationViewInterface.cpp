#include "StationViewInterface.h"

namespace CTCWindows {
	void StationViewInterface::onOrderClear()
	{
		FunBtnStateReset();
		emit OrderClear();
	}

	void StationViewInterface::onOrderIssued()
	{
		//FunBtnStateReset();
		emit OrderIssued();
	}
}