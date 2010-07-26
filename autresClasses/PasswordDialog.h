/*

Copyright © 2009-2010 Quentin RICHERT

Multiuso is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Multiuso is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Multiuso.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef HEADER_PASSWORD_DIALOG
#define HEADER_PASSWORD_DIALOG

#include "../CurrentIncludes.h"

enum Level
{
	VERY_HIGH,
	HIGH,
	GOOD,
	LOW,
	VERY_LOW,
	NUL
};

class SecurityLevel : public QWidget
{
	Q_OBJECT

	public:
		SecurityLevel()
		{
			setFixedHeight(25);
			m_level = NUL;
		}

		void setLevel(Level level)
		{
			m_level = level;

			update();
		}

	protected:
		void paintEvent(QPaintEvent *)
		{
			float level = 0.0;
			int minus = 2;

			switch (m_level)
			{
				case VERY_HIGH : level = 1; minus = 0; break;
				case HIGH : level = 0.75; break;
				case GOOD : level = 0.5; break;
				case LOW : level = 0.25; break;
				case VERY_LOW: level = 0.1; break;
				case NUL : level = 0; break;
			}

			QLinearGradient gradient(0, 0, width(), 0);
				gradient.setColorAt(0, Qt::darkGreen);
				gradient.setColorAt(0.1, "#27c103");
				gradient.setColorAt(0.25, Qt::green);
				gradient.setColorAt(0.5, Qt::yellow);
				gradient.setColorAt(0.75, "#ff6c00");
				gradient.setColorAt(1, Qt::red);

			QPainter painter(this);
				painter.setPen("#303030");
				painter.setBrush(gradient);
					painter.drawRect(0, 0, width() - 1, height() - 1);

				painter.setPen(Qt::NoPen);
					painter.fillRect(QRectF(1 + (width() * level), 1, (width() - (width() * level)) - minus, 23), Qt::white);
		}

	private:
		Level m_level;
};

class PasswordDialog : public QDialog
{
	Q_OBJECT

	public:
		PasswordDialog(QWidget *parent) : QDialog(parent)
		{
			setWindowTitle("Définition du mot de passe");
			setWindowIcon(QIcon(":/icones/preferences/password.png"));

			password = new QLineEdit;
				password->setEchoMode(QLineEdit::Password);
				connect(password, SIGNAL(textChanged(QString)), this, SLOT(slotPasswordChanged(QString)));

			passwordCheck = new QLineEdit;
				passwordCheck->setEchoMode(QLineEdit::Password);
				connect(passwordCheck, SIGNAL(textChanged(QString)), this, SLOT(slotPasswordCheck(QString)));

			level = new SecurityLevel;

			QFormLayout *layout = new QFormLayout;
				layout->addRow("Mot de passe :", password);
				layout->addRow("Confirmation :", passwordCheck);
				layout->addRow("Niveau de sécurité :", level);

			QPushButton *rejectButton = new QPushButton("Annuler");
				connect(rejectButton, SIGNAL(clicked()), this, SLOT(reject()));

			QHBoxLayout *buttons = new QHBoxLayout;
				buttons->addWidget(rejectButton);
				buttons->addWidget(Multiuso::closeButton(this, "OK"));
				buttons->setAlignment(Qt::AlignRight);

			QVBoxLayout *mainLayout = new QVBoxLayout(this);
				mainLayout->addLayout(layout);
				mainLayout->addLayout(buttons);
		}

		QString getPassword()
		{
			return password->text();
		}

	public slots:
		void slotPasswordChanged(QString pwd)
		{
			QRegExp rx1 = QRegExp("[0-9]+");
			QRegExp rx2 = QRegExp("[a-z]+");
			QRegExp rx3 = QRegExp("[A-Z]+");
			QRegExp rx4 = QRegExp("[^0-9a-zA-Z]+");

			if (pwd.contains(rx1) && pwd.contains(rx2) && pwd.contains(rx3) // 4 of them && >= 8
				&& pwd.contains(rx4) && pwd.length() >= 8)
					level->setLevel(VERY_HIGH);

			else if (pwd.contains(rx1) && pwd.contains(rx2) // 4 of them
				&& pwd.contains(rx3) && pwd.contains(rx4))
					level->setLevel(HIGH);

			else if ((pwd.contains(rx1) && pwd.contains(rx2) && pwd.contains(rx3)) // 3 of them
				|| (pwd.contains(rx1) && pwd.contains(rx2) && pwd.contains(rx4))
				|| (pwd.contains(rx1) && pwd.contains(rx3) && pwd.contains(rx4))
				|| (pwd.contains(rx2) && pwd.contains(rx3) && pwd.contains(rx4)))
					level->setLevel(GOOD);

			else if ((pwd.contains(rx1) && (pwd.contains(rx2))) // 2 of them
				|| (pwd.contains(rx1) && pwd.contains(rx3))
				|| (pwd.contains(rx1) && pwd.contains(rx4))
				|| (pwd.contains(rx2) && pwd.contains(rx3))
				|| (pwd.contains(rx2) && pwd.contains(rx4))
				|| (pwd.contains(rx3) && pwd.contains(rx4)))
					level->setLevel(LOW);

			else if (pwd.contains(rx1) || pwd.contains(rx2) // 1 of them
				|| pwd.contains(rx3) || pwd.contains(rx4))
					level->setLevel(VERY_LOW);

			else
				level->setLevel(NUL);

			slotPasswordCheck(passwordCheck->text());
		}

		void slotPasswordCheck(QString pwd)
		{
			if (pwd == password->text())
				passwordCheck->setStyleSheet("background-color : #bbffbb");
				
			else
				passwordCheck->setStyleSheet("background-color : #ffb3b3");
		}

		void accept()
		{
			if (password->text() == passwordCheck->text())
			{
				if (!password->text().isEmpty())
					QDialog::accept();

				else
					QMessageBox::critical(this, "Multiuso", "Il faut entrer un mot de passe !");
			}

			else
			{
				QMessageBox::critical(this, "Multiuso", "Les mots de passe diffèrent !");
			}
		}

	private:
		QLineEdit *password;
		QLineEdit *passwordCheck;

		SecurityLevel *level;
};

#endif
