#include "StationMultiDispTH.h"

namespace CTCWindows {
	namespace TH {
		StationMultiDispTH::StationMultiDispTH(QWidget* parent)
			: StationMultiDisp(parent)
		{
			ui.setupUi(this);
		}

		StationMultiDispTH::~StationMultiDispTH()
		{
		}

		void StationMultiDispTH::InitMultiStation(QVector<Station::StationObject*> vecMultiStation)
		{
			return StationMultiDisp::InitMultiStation(ui.contentsWidget, vecMultiStation);
		}
	}
}
