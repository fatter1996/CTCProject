#include "TrafficMsgLog.h"

namespace CTCWindows {
	TrafficMsgLog::TrafficMsgLog(QWidget* parent)
		: QWidget(parent)
	{
		ui.setupUi(this);
	}

	TrafficMsgLog::~TrafficMsgLog()
	{
	
	}

	void TrafficMsgLog::AddDispatchDeskMsg(QString strText)
	{
		ui.textBrowser->append(strText);
	}

	void TrafficMsgLog::AddAdjacentStationMsg(QString strText)
	{
		
		ui.textBrowser_2->append(strText);
	}
}
