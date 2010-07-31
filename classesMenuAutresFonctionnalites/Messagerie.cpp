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

Messagerie::Messagerie(QWidget *parent = 0) : QDialog(parent), currentPseudo(""), currentPassword(""),
							currentFirstName(""), currentLastName("")
{
	setWindowTitle("Multiuso - Messagerie");
	setWindowIcon(QIcon(":/icones/actions/actionMessagerie.png"));
	resize (305, 250);

	connectionWidget = new ConnectionWidget;
		connect(connectionWidget, SIGNAL(connectRequest()), this, SLOT(connectPeople()));

	messagesWidget = new MessagesWidget;
		connect(messagesWidget, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
		connect(messagesWidget, SIGNAL(addContactRequested(QString)), this, SLOT(addContact(QString)));
		connect(messagesWidget, SIGNAL(removeContactRequested(QString)), this, SLOT(removeContact(QString)));
		connect(messagesWidget, SIGNAL(reloadRequested()), this, SLOT(connectPeople()));
		connect(messagesWidget, SIGNAL(sendMessageRequested(QString, QString)), this, SLOT(sendMessage(QString, QString)));
		connect(messagesWidget, SIGNAL(removeMessageRequested(QString)), this, SLOT(removeMessage(QString)));

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
	messagesWidget->setContacts(contacts);
}

void Messagerie::connectPeople()
{
	connectionWidget->setDisabled(true);

	currentPseudo = connectionWidget->pseudo();
	
	currentPassword = connectionWidget->password();
		currentPassword = QCryptographicHash::hash(currentPassword.toAscii(), QCryptographicHash::Sha1);

	QNetworkRequest request(QCoreApplication::organizationDomain() + "messages.php?request=login"
									"&pseudo=" + currentPseudo +
									"&pwd=" + currentPassword);

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	replyConnection = manager->get(request);
		connect(replyConnection, SIGNAL(finished()), this, SLOT(getConnectionReply()));
		connect(replyConnection, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(getConnectionReply(QNetworkReply::NetworkError)));
}

void Messagerie::getConnectionReply()
{
	messages.clear();
	contacts.clear();

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
							message.message.replace("|0088lt;|", "<");
							message.message.replace("|0088amp;|", "&");

				messages << message;
			}

			else if (line.startsWith("CONTACT_ID:"))
			{
				Contact contact;
					contact.id = line;
					contact.pseudo = line;
					contact.firstName = line;
					contact.lastName = line;

						contact.id.replace(QRegExp("CONTACT_ID:(.+)CONTACT:(.+)FIRST_NAME:(.+)LAST_NAME:(.+)"), "\\1");
						contact.pseudo.replace(QRegExp("CONTACT_ID:(.+)CONTACT:(.+)FIRST_NAME:(.+)LAST_NAME:(.+)"), "\\2");
						contact.firstName.replace(QRegExp("CONTACT_ID:(.+)CONTACT:(.+)FIRST_NAME:(.+)LAST_NAME:(.+)"), "\\3");
						contact.lastName.replace(QRegExp("CONTACT_ID:(.+)CONTACT:(.+)FIRST_NAME:(.+)LAST_NAME:(.+)"), "\\4");

				contacts << contact;
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

	else
	{
		connectionWidget->setDisabled(false);
	}
}

void Messagerie::getConnectionReply(QNetworkReply::NetworkError)
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

	if (reply != 0)
		reply->abort();

	connectionWidget->setDisabled(false);
	
	QMessageBox::critical(this, "Multiuso", "Impossible d'accéder à la page de connexion, réessayez plus tard.");
}

void Messagerie::slotDisconnected()
{
	messages.clear();
	contacts.clear();

	resize (305, 250);
	mainWidget->setCurrentIndex(0);
	mainLayout->setContentsMargins(7, 7, 7, 7);
	connectionWidget->setDisabled(false);
}

void Messagerie::addContact(QString pseudo)
{
	if (pseudo == this->currentPseudo)
	{
		QMessageBox::information(this, "Multiuso",
				"Ô rage ! Ô désespoir ! Ô bouletitude ennemie !<br />"
				"N'as tu donc tant vécu que pour être ton seul ami ?");

		return;
	}

	QNetworkRequest request(QCoreApplication::organizationDomain() + "messages.php?request=add_contact"
									"&pseudo=" + currentPseudo +
									"&pwd=" + currentPassword +
									"&contact=" + pseudo);

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	replyContacts = manager->get(request);
		connect(replyContacts, SIGNAL(finished()), this, SLOT(getContactReply()));
		connect(replyContacts, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(getContactReply(QNetworkReply::NetworkError)));
}

void Messagerie::getContactReply()
{
	bool ok = true;

	QFile reply(Multiuso::tempPath() + "/reply");
		reply.open(QIODevice::WriteOnly | QIODevice::Text);
			reply.write(replyContacts->readAll());
		reply.close();

		replyContacts->deleteLater();

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
					case 0: QMessageBox::information(this, "Multiuso", "Contact ajouté avec succès !"); break;
					case 1: ok = false; QMessageBox::critical(this, "Multiuso", "Pseudo ou mot de passe incorrect !"); break;
					case 3: ok = false; QMessageBox::critical(this, "Multiuso", "Cet utilisateur n'existe pas !"); break;
					case 4: ok = false; QMessageBox::critical(this, "Multiuso", "Cet utilisateur fait déjà partie de vos contacts !"); break;
					default: ok = false; QMessageBox::critical(this, "Multiuso", "Erreur iconnue !"); break;
				}
			}
		}

		reply.close();
		reply.remove();

	if (ok)
		messagesWidget->reload();
}

void Messagerie::getContactReply(QNetworkReply::NetworkError)
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

	if (reply != 0)
		reply->abort();

	QMessageBox::critical(this, "Multiuso", "Impossible d'accéder à la page de gestion des contacts, réessayez plus tard.");
}

void Messagerie::removeContact(QString id)
{
	QNetworkRequest request(QCoreApplication::organizationDomain() + "messages.php?request=remove_contact"
									"&pseudo=" + currentPseudo +
									"&pwd=" + currentPassword +
									"&contact=" + id);

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	replyRContacts = manager->get(request);
		connect(replyRContacts, SIGNAL(finished()), this, SLOT(getRContactReply()));
		connect(replyRContacts, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(getRContactReply(QNetworkReply::NetworkError)));

}

void Messagerie::getRContactReply()
{
	bool ok = true;

	QFile reply(Multiuso::tempPath() + "/reply");
		reply.open(QIODevice::WriteOnly | QIODevice::Text);
			reply.write(replyRContacts->readAll());
		reply.close();

		replyRContacts->deleteLater();

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
					case 0: QMessageBox::information(this, "Multiuso", "Contact supprimé avec succès !"); break;
					case 1: ok = false; QMessageBox::critical(this, "Multiuso", "Pseudo ou mot de passe incorrect !"); break;
					case 5: ok = false; QMessageBox::critical(this, "Multiuso", "Cet utilisateur ne fait pas partie de vos contacts !"); break;
					default: ok = false; QMessageBox::critical(this, "Multiuso", "Erreur iconnue !"); break;
				}
			}
		}

		reply.close();
		reply.remove();

	if (ok)
		messagesWidget->reload();
}

void Messagerie::getRContactReply(QNetworkReply::NetworkError)
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

	if (reply != 0)
		reply->abort();

	QMessageBox::critical(this, "Multiuso", "Impossible d'accéder à la page de gestion des contacts, réessayez plus tard.");
}

void Messagerie::sendMessage(QString pseudo, QString message)
{
	QNetworkRequest request(QCoreApplication::organizationDomain() + "messages.php?request=add"
									"&pseudo=" + currentPseudo +
									"&pwd=" + currentPassword +
									"&receiver=" + pseudo +
									"&msg=" + message);

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	replyMessages = manager->get(request);
		connect(replyMessages, SIGNAL(finished()), this, SLOT(getSendMessageReply()));
		connect(replyMessages, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(getSendMessageReply(QNetworkReply::NetworkError)));
}

void Messagerie::getSendMessageReply()
{
	QFile reply(Multiuso::tempPath() + "/reply");
		reply.open(QIODevice::WriteOnly | QIODevice::Text);
			reply.write(replyMessages->readAll());
		reply.close();

		replyMessages->deleteLater();

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
					case 0: QMessageBox::information(this, "Multiuso", "Message envoyé avec succès !"); break;
					case 1: QMessageBox::critical(this, "Multiuso", "Pseudo ou mot de passe incorrect !"); break;
					case 3: QMessageBox::critical(this, "Multiuso", "Cet utilisateur n'existe pas !"); break;
					default: QMessageBox::critical(this, "Multiuso", "Erreur iconnue !"); break;
				}
			}
		}

		reply.close();
		reply.remove();
}

void Messagerie::getSendMessageReply(QNetworkReply::NetworkError)
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

	if (reply != 0)
		reply->abort();

	QMessageBox::critical(this, "Multiuso", "Impossible d'accéder à la page de gestion des messages, réessayez plus tard.");
}

void Messagerie::removeMessage(QString id)
{
	QNetworkRequest request(QCoreApplication::organizationDomain() + "messages.php?request=remove"
									"&pseudo=" + currentPseudo +
									"&pwd=" + currentPassword +
									"&id=" + id);

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	replyRMessages = manager->get(request);
		connect(replyRMessages, SIGNAL(finished()), this, SLOT(getRemoveMessageReply()));
		connect(replyRMessages, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(getRemoveMessageReply(QNetworkReply::NetworkError)));
}

void Messagerie::getRemoveMessageReply()
{
	bool ok = true;

	QFile reply(Multiuso::tempPath() + "/reply");
		reply.open(QIODevice::WriteOnly | QIODevice::Text);
			reply.write(replyRMessages->readAll());
		reply.close();

		replyRMessages->deleteLater();

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
					case 0: break;
					case 1: ok = false; QMessageBox::critical(this, "Multiuso", "Pseudo ou mot de passe incorrect !"); break;
					default: ok = false; QMessageBox::critical(this, "Multiuso", "Erreur iconnue !"); break;
				}
			}
		}

		reply.close();
		reply.remove();
		
	if (ok)
		messagesWidget->reload();
}

void Messagerie::getRemoveMessageReply(QNetworkReply::NetworkError)
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

	if (reply != 0)
		reply->abort();

	QMessageBox::critical(this, "Multiuso", "Impossible d'accéder à la page de gestion des messages, réessayez plus tard.");
}
