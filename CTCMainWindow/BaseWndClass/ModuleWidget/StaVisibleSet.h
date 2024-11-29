#pragma once
#include <QDialog>
#include <QMap>

#include "../../../StationObject/GlobalStruct.h"

namespace CTCWindows {
	
	namespace BaseWnd {

		class StaVisibleSet : public QDialog
		{
			Q_OBJECT
		public:
			StaVisibleSet(QWidget* parent = nullptr);
			~StaVisibleSet();

		public:
			void CheckStateInit();

		public slots:
			void setDevVisible();

		protected:
			QMap<QString, Station::VisibleDev> m_mapVisibleDev;
		};
	}
}