#include "StaVisibleSetKSK.h"

namespace CTCWindows {
	namespace CASCO {
		StaVisibleSetKSK::StaVisibleSetKSK(QWidget* parent)
			: BaseWnd::StaVisibleSet(parent)
		{
			m_mapVisibleDev = {
				{ "entrySignalName", Station::VisibleDev::entrySignalName },
				{ "trackName", Station::VisibleDev::trackName },
				{ "stateLempName", Station::VisibleDev::stateLempName },
				{ "exitSignalName", Station::VisibleDev::exitSignalName },
				{ "unswitchSectionName", Station::VisibleDev::unswitchSectionName },
				{ "shuntSignalName", Station::VisibleDev::shuntSignalName },
				{ "sectionName", Station::VisibleDev::sectionName },
				{ "text", Station::VisibleDev::text },
				{ "sectionSignalName", Station::VisibleDev::sectionSignalName },
				{ "switchName", Station::VisibleDev::switchName },
				{ "insulationNode", Station::VisibleDev::insulationNode },
				{ "stationName", Station::VisibleDev::stationName },
				{ "switchSectionName", Station::VisibleDev::switchSectionName },
				{ "button", Station::VisibleDev::funButtonName },
				{ "direction", Station::VisibleDev::direction },
				{ "squeezeLamp", Station::VisibleDev::squeezeLamp },
				{ "counter", Station::VisibleDev::counter},
				{ "routeWndFrame", Station::VisibleDev::previewFrame}
			};
			ui.setupUi(this);
			CheckStateInit();
			connect(ui.confirmBtn, &QPushButton::clicked, this, &StaVisibleSetKSK::setDevVisible);
		}

		StaVisibleSetKSK::~StaVisibleSetKSK()
		{
		}
	}
}
