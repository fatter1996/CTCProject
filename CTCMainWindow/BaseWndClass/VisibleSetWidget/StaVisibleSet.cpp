#include "StaVisibleSet.h"
#include <QCheckBox>
#include "Global.h"

namespace CTCWindows {
	namespace BaseWnd {
		StaVisibleSet::StaVisibleSet(QWidget* parent)
		{
			
		}

		StaVisibleSet::~StaVisibleSet()
		{
		
		}

		void StaVisibleSet::CheckStateInit()
		{
			for (QCheckBox* pCheck : findChildren<QCheckBox*>()) {
				if (m_mapVisibleDev.contains(pCheck->objectName())) {
					pCheck->setChecked(Station::MainStation()->IsVisible(m_mapVisibleDev[pCheck->objectName()]));
				}
			}
		}

		void StaVisibleSet::setDevVisible()
		{
			for (QCheckBox* pCheck : findChildren<QCheckBox*>()) {
				if (m_mapVisibleDev.contains(pCheck->objectName())) {
					Station::MainStation()->SetVisible(m_mapVisibleDev[pCheck->objectName()], pCheck->isChecked());
				}
			}
			this->close();
		}
	}
}