#include "StaFunBtnToolBar.h"
#include "Global.h"

namespace CTCWindows {
	namespace BaseWnd {
		FunType StaFunBtnToolBar::m_SelectFunType = FunType::RouteBuild;
		OperObjType StaFunBtnToolBar::m_nOperObjType = OperObjType::Defult;

		StaFunBtnToolBar::StaFunBtnToolBar(QWidget* parent)
			: QWidget(parent)
		{

		}

		StaFunBtnToolBar::~StaFunBtnToolBar()
		{

		}

		void StaFunBtnToolBar::onButtonClicked(QAbstractButton* pButton)
		{
			m_SelectFunType = m_mapFunBtnType[pButton];
			emit SelectDeviceClear();
		}
	}
}