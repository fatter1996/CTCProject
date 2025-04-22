#include "EditingInterfaceTKY.h"
#include <QPushButton>
#include <QDebug>
#include "Global.h"
EditingInterfaceTKY::EditingInterfaceTKY(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	connect(ui.pushButton, &QPushButton::clicked, [=] {

		emit CTCWindows::MainWindow()->ModifyContent(ui.lineEdit->text(), ui.comboBox->currentIndex(), ui.comboBox_2->currentIndex(),pTextSign);
	
		this->close();
		});
	connect(ui.pushButton_2, &QPushButton::clicked, [=] {
			this->close();
		});
}

EditingInterfaceTKY::~EditingInterfaceTKY()
{}

void EditingInterfaceTKY::setLinEditText(QString Text,Station::Device::StaTextSign* TextSign)
{
	ui.lineEdit->setPlaceholderText(Text);
	pTextSign = TextSign;
}
