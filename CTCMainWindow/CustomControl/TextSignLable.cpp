#include "TextSignLable.h"
#include <QBoxLayout>
#include <QEvent>

namespace CTCWindows {
	namespace  Control {
		TextSignLable::TextSignLable(QWidget* parent)
			: QWidget(parent)
		{
			QVBoxLayout* pLayout = new QVBoxLayout(this);
			pLayout->setMargin(0);
			pLayout->setSpacing(0);
			this->setAttribute(Qt::WA_TranslucentBackground);
			this->setAttribute(Qt::WA_DeleteOnClose);
			this->setWindowFlags(Qt::FramelessWindowHint);
		}

		TextSignLable::~TextSignLable()
		{

		}

		void TextSignLable::InitLable(int nType)
		{
			if (nType == 0) {
				m_pLineEdit = new QLineEdit;
				m_pLineEdit->setFixedSize(160, 32);
				m_pLineEdit->setFont(QFont("ËÎÌו", 10));
				m_pLineEdit->setStyleSheet("QLineEdit{background-color: rgb(240, 240, 240);}");
				connect(m_pLineEdit, &QLineEdit::returnPressed, [this]() {
					emit addTextSignLable(m_pLineEdit->text());
					this->close();
				});
				this->layout()->addWidget(m_pLineEdit);
				this->setFixedSize(160, 32);
			}
		}

		TextSignLable* TextSignLable::CreateTextSignLable(QWidget* parent, int nType)
		{
			TextSignLable* pTextSignLable = new TextSignLable(parent);
			pTextSignLable->InitLable(nType);
			pTextSignLable->show();
			return pTextSignLable;
		}
	}
}
