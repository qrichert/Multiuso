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

#ifndef HEADER_LOGINDIALOG
#define HEADER_LOGINDIALOG

#include "../CurrentIncludes.h"

class LoginDialog : public QDialog
{
	Q_OBJECT

	public:
		LoginDialog(QIcon icon, QWidget *parent = 0) : QDialog(parent)
		{
			setWindowTitle("Identification");
			setWindowIcon(icon);

			pseudo = new QLineEdit;
			
			password = new QLineEdit;
				password->setEchoMode(QLineEdit::Password);
				
			QFormLayout *layout = new QFormLayout;
				layout->addRow("Pseudo :", pseudo);
				layout->addRow("Mot de Passe :", password);
				
			QVBoxLayout *mainLayout = new QVBoxLayout(this);
				mainLayout->addWidget(new QLabel("Utilisez les identifiants de votre compte<br />de messagerie Multiuso."));
				mainLayout->addLayout(layout);
				mainLayout->addLayout(Multiuso::dialogButtons(this, "Annuler", "OK"));
		}
		
		QString getPseudo()
		{
			return pseudo->text();
		}
		
		QString getPassword()
		{
			return password->text();
		}
		
	public slots:
		void accept()
		{
			if (!pseudo->text().isEmpty()
				&& !password->text().isEmpty())
					QDialog::accept();
					
			else
				QDialog::reject();
		}
		
	private:
		QLineEdit *pseudo;
		QLineEdit *password;
};

#endif
