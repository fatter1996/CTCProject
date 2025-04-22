#include "StaAlarmWindowKSK.h"
#include <QIcon>
StaAlarmWindowKSK::StaAlarmWindowKSK(QWidget *parent)
	: StaAlarmWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle("CTC--¸æ¾¯");
	setWindowIcon(QIcon(":/CTCProject/icon/icon.ico"));
}

StaAlarmWindowKSK::~StaAlarmWindowKSK()
{}
