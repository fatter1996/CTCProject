#include "StaVisibleSetTKY.h"

namespace CTCWindows {
	namespace CARS {
		StaVisibleSetTKY::StaVisibleSetTKY(QWidget* parent)
			: BaseWnd::StaVisibleSet(parent)
		{
			m_mapVisibleDev = {
				{ "button", Station::VisibleDev::button },
				{ "trackName", Station::VisibleDev::trackName },
				{ "unswitchSectionName", Station::VisibleDev::unswitchSectionName},
				{ "sectionName", Station::VisibleDev::sectionName},
				{ "auxiliaryLineName", Station::VisibleDev::auxiliaryLineName},
				{ "switchName", Station::VisibleDev::switchName},
				{ "direction", Station::VisibleDev::direction},
				{ "phaseSeparaName", Station::VisibleDev::phaseSeparaName},
				{ "entrySignalName", Station::VisibleDev::entrySignalName},
				{ "exitSignalName", Station::VisibleDev::exitSignalName},
				{ "shuntSignalName", Station::VisibleDev::shuntSignalName},
				{ "sectionSignalName", Station::VisibleDev::sectionSignalName},
				{ "stateLempName", Station::VisibleDev::stateLempName},
				{ "occludeLempName", Station::VisibleDev::occludeLempName},
				{ "RBCChangeName", Station::VisibleDev::RBCChangeName},
				{ "stateLemp", Station::VisibleDev::stateLemp},
				{ "phaseSepara", Station::VisibleDev::phaseSepara},
				{ "notes", Station::VisibleDev::notes},
				{ "shuntSignal", Station::VisibleDev::shuntSignal},
				{ "sectionSignal", Station::VisibleDev::sectionSignal},
				{ "occludeLemp", Station::VisibleDev::occludeLemp},
				{ "RBCChange", Station::VisibleDev::RBCChange},
				{ "auxiliaryLine", Station::VisibleDev::auxiliaryLine},
				{ "insulationNode", Station::VisibleDev::insulationNode},
				{ "platform", Station::VisibleDev::platform},
				{ "endMarker", Station::VisibleDev::endMarker},
				{ "preview", Station::VisibleDev::preview},
				{ "permillSix", Station::VisibleDev::permillSix}
			};
			ui.setupUi(this);
			CheckStateInit();

			connect(ui.confirmBtn, &QPushButton::clicked, this, &StaVisibleSetTKY::setDevVisible);
			connect(ui.cancelBtn, &QPushButton::clicked, [&]() { this->close(); });
		}

		StaVisibleSetTKY::~StaVisibleSetTKY()
		{

		}
	}
}
