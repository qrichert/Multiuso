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

#include "Messagerie.h"

Messagerie::Messagerie(QWidget *parent = 0) : QDialog(parent), currentPseudo("")
{
	setWindowTitle("Multiuso - Messagerie");
	setWindowIcon(QIcon(":/icones/actions/actionMessagerie.png"));
	resize (305, 250);

	connectionWidget = new ConnectionWidget;
		connect(connectionWidget, SIGNAL(connectRequest()), this, SLOT(connectPeople()));

	messagesWidget = new MessagesWidget;
		connect(messagesWidget, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));

	mainWidget = new QStackedWidget;
		mainWidget->addWidget(connectionWidget);
		mainWidget->addWidget(messagesWidget);

	mainLayout = new QVBoxLayout(this);
		mainLayout->addWidget(mainWidget);
		mainLayout->setContentsMargins(7, 7, 7, 7);
}

void Messagerie::updateMessagesWidget()
{
	messagesWidget->setPseudo(currentPseudo);
	messagesWidget->setFirstName(currentFirstName);
	messagesWidget->setLastName(currentLastName);
	messagesWidget->setMessages(messages);
}

void Messagerie::connectPeople()
{
	currentPseudo = connectionWidget->pseudo();
	
	QString password = connectionWidget->password();
		password = QCryptographicHash::hash(password.toAscii(), QCryptographicHash::Sha1);

	QNetworkRequest request(QCoreApplication::organizationDomain() + "messages.php?request=login"
									"&pseudo=" + currentPseudo +
									"&pwd=" + password);

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	replyConnection = manager->get(request);
		connect(replyConnection, SIGNAL(finished()), this, SLOT(getConnectionReply()));
		connect(replyConnection, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(getConnectionReply(QNetworkReply::NetworkError)));
}

void Messagerie::getConnectionReply()
{
	messages.clear();

	bool ok = true;

	QFile reply(Multiuso::tempPath() + "/reply");
		reply.open(QIODevice::WriteOnly | QIODevice::Text);
			reply.write(replyConnection->readAll());
		reply.close();

		replyConnection->deleteLater();

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
					case 1: ok = false; QMessageBox::critical(this, "Multiuso", "Pseudo ou mot de passe incorrect !"); break;
					default: ok = false; QMessageBox::critical(this, "Multiuso", "Erreur iconnue !"); break;
				}
			}

			else if (line.startsWith("FIRST_NAME:"))
			{
				currentFirstName = line;
				currentLastName = line;

					currentFirstName.replace(QRegExp("FIRST_NAME:(.+)LAST_NAME:(.+)"), "\\1");
					currentLastName.replace(QRegExp("FIRST_NAME:(.+)LAST_NAME:(.+)"), "\\2");
			}

			else if (line.startsWith("ID:"))
			{
				Message message;
					message.id = line;
					message.from = line;
					message.date = line;
					message.message = line;
					
						message.id.replace(QRegExp("ID:(.+)FROM:(.+)DATE:(.+)MESSAGE:(.+)"), "\\1");
						message.from.replace(QRegExp("ID:(.+)FROM:(.+)DATE:(.+)MESSAGE:(.+)"), "\\2");
						message.date.replace(QRegExp("ID:(.+)FROM:(.+)DATE:(.+)MESSAGE:(.+)"), "\\3");
						message.message.replace(QRegExp("ID:(.+)FROM:(.+)DATE:(.+)MESSAGE:(.+)"), "\\4");
							message.message.replace("<br />", "\n");
							message.message.replace("&lt;", "<");
							message.message.replace("&amp;", "&");

				messages << message;
			}
		}

	reply.close();
	reply.remove();

	if (ok)
	{
		resize(Multiuso::screenWidth() / 2, Multiuso::screenHeight() / 2);
		
		connectionWidget->saveInfos();
		mainWidget->setCurrentIndex(1);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		updateMessagesWidget();
	}
}

void Messagerie::getConnectionReply(QNetworkReply::NetworkError)
{
	QMessageBox::critical(this, "Multiuso", "Impossible d'accéder à la page de connexion, réessayez plus tard.");
}

void Messagerie::slotDisconnected()
{
	resize (305, 250);
	mainWidget->setCurrentIndex(0);
	mainLayout->setContentsMargins(7, 7, 7, 7);
}

