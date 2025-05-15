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
			setWindowTitle("���ڳ��ص�༭");
			setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

			InitConnect();
		}

		CarLocationEdit::~CarLocationEdit()
		{}

		void CarLocationEdit::InitConnect()
		{
			connect(ui.OK, &QPushButton::clicked, [&]() {
				QMessageBox msgBox;
				msgBox.setWindowTitle("��ʾ");
				msgBox.setText("ȷ�Ϻ���δ򿪡����ڳ�����ʱ��Ч");
				msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
				msgBox.setDefaultButton(QMessageBox::Ok);
				int ret = msgBox.exec();
				if (ret == QMessageBox::Ok) {
					qDebug() << "�û������ȷ��";
					close();
				}
				else {
					msgBox.setWindowTitle("��ʾ");
					msgBox.setText("��ȡ��");
					msgBox.setStandardButtons(QMessageBox::Ok);
					msgBox.exec();
					qDebug() << "�û������ȡ����رմ���";
					close();
				}
				});
			connect(ui.cancel, &QPushButton::clicked, [&]() {
				qDebug() << "�û������ȡ����رմ���";
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
