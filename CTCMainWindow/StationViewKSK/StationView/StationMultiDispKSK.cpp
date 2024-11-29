#include "StationMultiDispKSK.h"

namespace CTCWindows {
	namespace CASCO {
		StationMultiDispKSK::StationMultiDispKSK(QWidget* parent)
			: StationMultiDisp(parent)
		{
			ui.setupUi(this);
		}

		StationMultiDispKSK::~StationMultiDispKSK()
		{
		}

		void StationMultiDispKSK::InitMultiStation(QVector<Station::StationObject*> vecMultiStation) 
		{
			return StationMultiDisp::InitMultiStation(ui.contentsWidget, vecMultiStation);
		}
	}
}
