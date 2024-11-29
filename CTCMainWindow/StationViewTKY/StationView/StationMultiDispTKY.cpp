#include "StationMultiDispTKY.h"

namespace CTCWindows {
	namespace CARS {
		StationMultiDispTKY::StationMultiDispTKY(QWidget* parent)
			: StationMultiDisp(parent)
		{
			ui.setupUi(this);
		}

		StationMultiDispTKY::~StationMultiDispTKY()
		{
		}

		void StationMultiDispTKY::InitMultiStation(QVector<Station::StationObject*> vecMultiStation)
		{
			return StationMultiDisp::InitMultiStation(ui.contentsWidget, vecMultiStation);
		}
	}
}
