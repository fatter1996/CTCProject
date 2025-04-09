#include "StaAlarmWindowTKY.h"
#include "qicon.h"
#include <QHeaderView>
#include <QPushButton>
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace CARS {
		StaAlarmWindowTKY::StaAlarmWindowTKY(QWidget* parent)
			: StaAlarmWindow(parent)
		{
			ui.setupUi(this);
			setWindowTitle("CTC--¸æ¾¯");
			setWindowIcon(QIcon(":/CTCProject/icon/icon.ico"));
			ui.other->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
			ui.firstorder->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
			connect(ui.soundoff, &QPushButton::clicked, [&]() {});
			connect(ui.verify, &QPushButton::clicked, [&]() {});
		}

		StaAlarmWindowTKY::~StaAlarmWindowTKY()
		{
		}
	}
}