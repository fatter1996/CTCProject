#pragma once
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
namespace CTCWindows {
	namespace  Control {
		class TextSignLable : public QWidget
		{
			Q_OBJECT

		public:
			TextSignLable(QWidget* parent = nullptr);
			~TextSignLable();

			void InitLable(int nType = 0);

		public:
			static TextSignLable* CreateTextSignLable(QWidget* parent, int nType = 0);
		signals:
			void addTextSignLable(const QString&);

		private:
			QLineEdit* m_pLineEdit = nullptr;
		};
	}
}
