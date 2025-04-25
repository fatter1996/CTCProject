#include "EditingInterfaceTKY.h"
#include <QPushButton>
#include <QDebug>
#include "Global.h"

namespace CTCWindows {
	namespace CARS {
		EditingInterfaceTKY::EditingInterfaceTKY(QWidget* parent)
			: QMainWindow(parent)
		{
			ui.setupUi(this);

			connect(ui.pushButton, &QPushButton::clicked, [=] {
				m_pTextSign->InitTextSign(ui.lineEdit->text(), m_pTextSign->getShowPos(), getColor(ui.comboBox->currentIndex()), getColor(ui.comboBox_2->currentIndex()));
				this->close();
			});
			connect(ui.pushButton_2, &QPushButton::clicked, this, &EditingInterfaceTKY::close);
		}

		EditingInterfaceTKY::~EditingInterfaceTKY()
		{
		
		}

		void EditingInterfaceTKY::setLinEditText(Station::Device::StaTextSign* pTextSign)
		{
			ui.lineEdit->setPlaceholderText(pTextSign->getText());
			m_pTextSign = pTextSign;
		}

		QColor EditingInterfaceTKY::getColor(int nIndex)
		{
			switch (nIndex)
			{
			case 0:		return Qt::black;
			case 1:		return Qt::white;
			case 2:		return Qt::red;
			case 3:		return Qt::yellow;
			case 4:		return Qt::blue;
			case 5:		return Qt::green;
			default:	return Qt::black;
			}
		}
	}
}
