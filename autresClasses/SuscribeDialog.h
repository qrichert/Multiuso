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

#ifndef HEADER_SUSCRIBEDIALOG
#define HEADER_SUSCRIBEDIALOG

#include <QtNetwork>
#include "../CurrentIncludes.h"
#include "autresClasses/PasswordDialog.h"

class SuscribeDialog : public QWidget // Not QObject because PasswordDialog excpects a QWidget as parent.
{
	Q_OBJECT

	public:
		SuscribeDialog(QWidget *parent = 0) : QWidget(parent)
		{
			suscribe();
		}

		void suscribe()
		{
			QDialog *infosDialog = new QDialog(this);
				
				QLineEdit *linePseudo = new QLineEdit;
				QLineEdit *lineFirstName = new QLineEdit;
				QLineEdit *lineLastName = new QLineEdit;

				QFormLayout *layoutLines = new QFormLayout;
					layoutLines->addRow("Pseudo (25 carac. max.) :", linePseudo);
					layoutLines->addRow("Prénom :", lineFirstName);
					layoutLines->addRow("Nom :", lineLastName);

				QPushButton *buttonReject = new QPushButton("Annuler");
					connect(buttonReject, SIGNAL(clicked()), infosDialog, SLOT(reject()));

				QHBoxLayout *buttonsLayout = new QHBoxLayout;
					buttonsLayout->addWidget(buttonReject);
					buttonsLayout->addWidget(Multiuso::closeButton(infosDialog, "OK"));
					buttonsLayout->setAlignment(Qt::AlignRight);

				QVBoxLayout *layoutDialog = new QVBoxLayout(infosDialog);
					layoutDialog->addWidget(new QLabel("Ces informations serviront à vos contacts pour identifier "
								"vos messages<br />et ne seront en aucun divulguées à votre insu."));
					layoutDialog->addLayout(layoutLines);
					layoutDialog->addLayout(buttonsLayout);

			if (infosDialog->exec() == QDialog::Rejected)
			{
				infosDialog->deleteLater();

				emit deleteRequested(this);

				return;
			}

			else if (linePseudo->text().isEmpty()
					|| linePseudo->text().length() > 25
					|| lineFirstName->text().isEmpty()
					|| lineLastName->text().isEmpty())
			{
				QMessageBox::critical(this, "Multiuso", "Vous devez remplir correctement tous les champs !");

				emit deleteRequested(this);
				
				return;
			}

			else if (linePseudo->text().contains("&")
					|| linePseudo->text().contains("="))
			{
				QMessageBox::critical(this, "Multiuso", "Votre pseudo ne peut contenir de '&' ou de '=' !");

				emit deleteRequested(this);
				
				return;
			}

			PasswordDialog *pwdDialog = new PasswordDialog(this);

			if (pwdDialog->exec() == QDialog::Rejected)
			{
				emit deleteRequested(this);

				return;
			}
			
			QString password = pwdDialog->getPassword();

			pwdDialog->deleteLater();

			suscribe(linePseudo->text(), password, lineFirstName->text(), lineLastName->text());
		}

		void suscribe(QString pseudo, QString pwd, QString firstName, QString lastName)
		{
			firstName = Multiuso::toName(firstName);
			lastName = Multiuso::toName(lastName);

			pwd = QCryptographicHash::hash(pwd.toAscii(), QCryptographicHash::Sha1).toHex();

			QNetworkRequest request(QCoreApplication::organizationDomain() + "suscribe.php?request=suscribe"
												"&pseudo=" + pseudo +
												"&pwd=" + pwd +
												"&first_name=" + firstName +
												"&last_name=" + lastName);

			QNetworkAccessManager *manager = new QNetworkAccessManager(this);

			suscribeReply = manager->get(request);
				connect(suscribeReply, SIGNAL(finished()), this, SLOT(getSuscribeReply()));
				connect(suscribeReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(getSuscribeReply(QNetworkReply::NetworkError)));
		}

	public slots:
		void getSuscribeReply()
		{
			QFile reply(Multiuso::tempPath() + "/reply");
				reply.open(QIODevice::WriteOnly | QIODevice::Text);
					reply.write(suscribeReply->readAll());
				reply.close();

			suscribeReply->deleteLater();

			QTextStream stream(&reply);

			reply.open(QIODevice::ReadOnly | QIODevice::Text);

				while (!stream.atEnd())
				{
					QString line = stream.readLine();

					if (line.startsWith("ERROR:"))
					{
						int error = line.replace(QRegExp("ERROR:([0-9]+)"), "\\1").toInt();

						switch (error)
						{
							case 0: QMessageBox::information(this, "Multiuso", "Inscription réussie !<br />Vous pouvez maintenant vous connecter à votre compte !"); break;
							case 2: QMessageBox::critical(this, "Multiuso", "Ce pseudo est déjà utilisé !"); break;
							default: QMessageBox::critical(this, "Multiuso", "Erreur iconnue !"); break;
						}
					}
				}

			reply.close();
			reply.remove();

			emit deleteRequested(this);
		}

		void getSuscribeReply(QNetworkReply::NetworkError)
		{
			QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

			if (reply != 0)
			{
				suscribeReply->abort();
				suscribeReply->deleteLater();
			}

			QMessageBox::critical(this, "Multiuso", "Impossible d'accéder à la page d'inscription, réessayez plus tard.");
				
			emit deleteRequested(this);
		}

	signals:
		void deleteRequested(SuscribeDialog *dialog);

	private:
		QNetworkReply *suscribeReply;
};

#endif
