#include "CarLocationEdit.h"
#include <QIcon>
#include <QPushButton>
#pragma execution_character_set("utf-8")
#include <QMessageBox>
#include <QDebug>
namespace CTCWindows {
	namespace  Control {
		CarLocationEdit::CarLocationEdit(QWidget* parent)
			: QWidget(parent)
		{
			ui.setupUi(this);

			setWindowIcon(QIcon(":/CTCProject/icon/icon.ico"));
			setWindowTitle("现在车地点编辑");
			setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

			InitConnect();
		}

		CarLocationEdit::~CarLocationEdit()
		{}

		void CarLocationEdit::InitConnect()
		{
			connect(ui.OK, &QPushButton::clicked, [&]() {
				QMessageBox msgBox;
				msgBox.setWindowTitle("提示");
				msgBox.setText("确认后二次打开“现在车管理”时生效");
				msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
				msgBox.setDefaultButton(QMessageBox::Ok);
				int ret = msgBox.exec();
				if (ret == QMessageBox::Ok) {
					qDebug() << "用户点击了确认";
					close();
				}
				else {
					msgBox.setWindowTitle("提示");
					msgBox.setText("已取消");
					msgBox.setStandardButtons(QMessageBox::Ok);
					msgBox.exec();
					qDebug() << "用户点击了取消或关闭窗口";
					close();
				}
				});
			connect(ui.cancel, &QPushButton::clicked, [&]() {
				qDebug() << "用户点击了取消或关闭窗口";
				close();
				});
			connect(ui.delete_2, &QPushButton::clicked, [&]() {});
			connect(ui.up, &QPushButton::clicked, [&]() {});
			connect(ui.edit, &QPushButton::clicked, [&]() {});
			connect(ui.down, &QPushButton::clicked, [&]() {});
			connect(ui.readdition, &QPushButton::clicked, [&]() {});
		}
	}
}
