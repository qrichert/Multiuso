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

#ifndef HEADER_MESSAGERIE
#define HEADER_MESSAGERIE

#include <QtNetwork>
#include "../CurrentIncludes.h"
#include "autresClasses/PasswordDialog.h"

class SuscribeLabel : public QLabel
{
	Q_OBJECT

	public:
		SuscribeLabel() : QLabel("<center>Créer un compte</center>")
		{
			setStyleSheet("color:orange;");
			setCursor(Qt::ArrowCursor);
		}

	protected:
		void mousePressEvent(QMouseEvent *)
		{
			emit clicked();
		}

		void enterEvent(QEvent *)
		{
			setStyleSheet("color:red;");
			setCursor(Qt::PointingHandCursor);
		}

		void leaveEvent(QEvent *)
		{
			setStyleSheet("color:orange;");
			setCursor(Qt::ArrowCursor);
		}

	signals:
		void clicked();
};

class ConnectionWidget : public QWidget
{
	Q_OBJECT

	public:
		ConnectionWidget()
		{
			m_pseudo = new QLineEdit;

			m_password = new QLineEdit;
				m_password->setEchoMode(QLineEdit::Password);
			
			QFormLayout *layoutConnection = new QFormLayout;
				layoutConnection->addRow("Utilisateur :", m_pseudo);
				layoutConnection->addRow("Mot de passe :", m_password);

			QGroupBox *groupConnection = new QGroupBox("Connexion");
				groupConnection->setLayout(layoutConnection);

			m_rememberMe = new QCheckBox;

			m_rememberMyPassword = new QCheckBox;
	
			QFormLayout *layoutInfos = new QFormLayout;
				layoutInfos->addRow("Se souvenir de moi : ", m_rememberMe);
				layoutInfos->addRow("Se souvenir du mot de passe : ", m_rememberMyPassword);

			SuscribeLabel *labelSuscribe = new SuscribeLabel;
				connect(labelSuscribe, SIGNAL(clicked()), this, SLOT(suscribe()));

			QPushButton *buttonConnect = new QPushButton("Se connecter");
				connect(buttonConnect, SIGNAL(clicked()), this, SIGNAL(connectRequest()));
			
			QVBoxLayout *mainLayout = new QVBoxLayout(this);
				mainLayout->addWidget(groupConnection);
				mainLayout->addLayout(layoutInfos);
				mainLayout->addWidget(labelSuscribe);
				mainLayout->addWidget(buttonConnect);
				mainLayout->setAlignment(Qt::AlignCenter);


			setInfos();
		}

		void setInfos()
		{
			QSettings settings(Multiuso::appDirPath() + "/ini/messagerie.ini");
				m_pseudo->setText(settings.value("pseudo").toString());
				m_password->setText(settings.value("password").toString());

				if (settings.value("remember_me").toBool())
					m_rememberMe->setCheckState(Qt::Checked);

				if (settings.value("remember_my_password").toBool())
					m_rememberMyPassword->setCheckState(Qt::Checked);
		}

		void saveInfos()
		{
			QString pseudo = m_pseudo->text();

			if (!m_rememberMe->isChecked())
				pseudo = "";

			QString password = m_password->text();
			
			if (!m_rememberMyPassword->isChecked())
				password = "";

			QSettings settings(Multiuso::appDirPath() + "/ini/messagerie.ini");
				settings.setValue("pseudo", pseudo);
				settings.setValue("password", password);
				settings.setValue("remember_me", m_rememberMe->isChecked());
				settings.setValue("remember_my_password", m_rememberMyPassword->isChecked());
		}

		QString pseudo()
		{
			return m_pseudo->text();
		}

		QString password()
		{
			return m_password->text();
		}

	public slots:
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
				return;
			}

			else if (linePseudo->text().isEmpty()
					|| linePseudo->text().length() > 25
					|| lineFirstName->text().isEmpty()
					|| lineLastName->text().isEmpty())
			{
				QMessageBox::critical(this, "Multiuso", "Vous devez remplir correctement tous les champs !");

				return;
			}

			PasswordDialog *pwdDialog = new PasswordDialog(this);

			if (pwdDialog->exec() == QDialog::Rejected)
				return;
			
			QString password = pwdDialog->getPassword();

			pwdDialog->deleteLater();

			suscribe(linePseudo->text(), password, lineFirstName->text(), lineLastName->text());
		}

		void suscribe(QString pseudo, QString pwd, QString firstName, QString lastName)
		{
			pwd = QCryptographicHash::hash(pwd.toAscii(), QCryptographicHash::Sha1);

			QNetworkRequest request(QCoreApplication::organizationDomain() + "messages.php?request=suscribe"
												"&pseudo=" + pseudo +
												"&pwd=" + pwd +
												"&first_name=" + firstName +
												"&last_name=" + lastName);

			QNetworkAccessManager *manager = new QNetworkAccessManager(this);

			suscribeReply = manager->get(request);
				connect(suscribeReply, SIGNAL(finished()), this, SLOT(getSuscribeReply()));
				connect(suscribeReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(getSuscribeReply(QNetworkReply::NetworkError)));
		}

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
							case 0: QMessageBox::information(this, "Multiuso", "Inscription réussie !<br />Vous pouvez maintenant vous connecter !"); break;
							case 2: QMessageBox::critical(this, "Multiuso", "Ce pseudo est déjà utilisé !"); break;
							default: QMessageBox::critical(this, "Multiuso", "Erreur iconnue !"); break;
						}
					}
				}

			reply.close();
			reply.remove();
		}

		void getSuscribeReply(QNetworkReply::NetworkError)
		{
			QMessageBox::critical(this, "Multiuso", "Impossible d'accéder à la page d'inscription, réessayez plus tard.");
		}

	signals:
		void connectRequest();

	private:
		QLineEdit *m_pseudo;
		QLineEdit *m_password;

		QCheckBox *m_rememberMe;
		QCheckBox *m_rememberMyPassword;

		QNetworkReply *suscribeReply;
};

struct Message
{
	QString id;
	QString from;
	QString date;
	QString message;
};

struct Contact
{
	QString id;
	QString pseudo;
	QString firstName;
	QString lastName;
};

class MessagesWidget : public QMainWindow
{
	Q_OBJECT

	public:
		MessagesWidget()
		{
			QLabel *connectedAs = new QLabel(" Vous êtes connecté en tant que ");
				m_pseudo = new QLabel("");
			QLabel *openParenthesis = new QLabel(" (");
				m_firstName = new QLabel("");
			QLabel *space = new QLabel(" ");
				m_lastName = new QLabel("");
			QLabel *closeParenthesis = new QLabel(")");

			QToolBar *labelToolBar = addToolBar("Informations");
				labelToolBar->setMovable(false);
				labelToolBar->addWidget(connectedAs);
				labelToolBar->addWidget(m_pseudo);
				labelToolBar->addWidget(openParenthesis);
				labelToolBar->addWidget(m_firstName);
				labelToolBar->addWidget(space);
				labelToolBar->addWidget(m_lastName);
				labelToolBar->addWidget(closeParenthesis);

			addToolBarBreak();
			
			QAction *actionLogOut = new QAction("Se déconnecter", this);
				actionLogOut->setIcon(QIcon(":/icones/messagerie/logout.png"));
				actionLogOut->setToolTip("Se déconnecter");
				connect(actionLogOut, SIGNAL(triggered()), this, SLOT(slotDisconnect()));

			QAction *actionReload = new QAction("Recharger", this);
				actionReload->setIcon(QIcon(":/icones/messagerie/reload.png"));
				actionReload->setToolTip("Recharger");
				connect(actionReload, SIGNAL(triggered()), this, SLOT(reload()));

			QAction *actionAddContact = new QAction("Ajouter un contact", this);
				actionAddContact->setIcon(QIcon(":/icones/messagerie/add_contact.png"));
				actionAddContact->setToolTip("Ajouter un contact");
				connect(actionAddContact, SIGNAL(triggered()), this, SLOT(slotAddContact()));

			m_contactsList = new QComboBox;
			
			QToolBar *actionsToolBar = addToolBar("Actions");
				actionsToolBar->setMovable(false);
				actionsToolBar->addAction(actionLogOut);
				actionsToolBar->addSeparator();
				actionsToolBar->addAction(actionAddContact);
				actionsToolBar->addWidget(m_contactsList);
	
			QStringList headerLabels;
				headerLabels << "#" << "-" << "De" << "Date" << "Message";

			mainTable = new QTableWidget(0, headerLabels.size());
				mainTable->setShowGrid(false);
				mainTable->setHorizontalHeaderLabels(headerLabels);
				mainTable->verticalHeader()->hide();
				mainTable->setSelectionBehavior(QAbstractItemView::SelectRows);
				mainTable->setSelectionMode(QAbstractItemView::SingleSelection);
				mainTable->resizeColumnsToContents();
				mainTable->horizontalHeader()->setStretchLastSection(true);
				connect(mainTable, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotShowMessage(QTableWidgetItem *)));
			
			QVBoxLayout *mainLayout = new QVBoxLayout;
				mainLayout->addWidget(mainTable);
				mainLayout->setContentsMargins(0, 0, 0, 0);

			QWidget *mainWidget = new QWidget;
				mainWidget->setLayout(mainLayout);

			setCentralWidget(mainWidget);
		}

		void setPseudo(QString pseudo)
		{
			m_pseudo->setText("<strong>" + pseudo + "</strong>");
		}

		void setFirstName(QString firstName)
		{
			m_firstName->setText(firstName);
		}

		void setLastName(QString lastName)
		{
			m_lastName->setText(lastName);
		}

		void setMessages(QList<Message> messages)
		{
			QStringList headerLabels;
				headerLabels << "#" << "-" << "De" << "Date" << "Message";

			mainTable->clear();
			mainTable->setRowCount(0);
			mainTable->setHorizontalHeaderLabels(headerLabels);

			m_messages = messages;

			pairs.clear();

			foreach (Message message, m_messages)
			{
				QString msgToShow = message.message;
					
				if (msgToShow.length() > 40)
					msgToShow = msgToShow.left(37) + "...";

				int newRowCount = mainTable->rowCount() + 1;

				Pair pair;
					pair.first = newRowCount - 1;
					pair.second = message.id.toInt();

				pairs << pair;	

				QTableWidgetItem *itemNumber = new QTableWidgetItem(QString::number(newRowCount));
					itemNumber->setFlags(itemNumber->flags() & ~Qt::ItemIsEditable);

				QPushButton *deleteMsg = new QPushButton;
					deleteMsg->setIcon(QIcon(":/icones/messagerie/delete_msg.png"));
					deleteMsg->setFixedSize(20, 20);
					deleteMsg->setObjectName(QString::number(newRowCount - 1));

				QVBoxLayout *deleteMsgLayout = new QVBoxLayout;
					deleteMsgLayout->addWidget(deleteMsg);

				QWidget *deleteMsgWidget = new QWidget;
					deleteMsgWidget->setLayout(deleteMsgLayout);

				QTableWidgetItem *itemFrom = new QTableWidgetItem(message.from);
					itemFrom->setFlags(itemFrom->flags() & ~Qt::ItemIsEditable);
			
				QTableWidgetItem *itemDate = new QTableWidgetItem(message.date);
					itemDate->setFlags(itemDate->flags() & ~Qt::ItemIsEditable);
			
				QTableWidgetItem *itemMessage = new QTableWidgetItem(msgToShow);
					itemMessage->setFlags(itemMessage->flags() & ~Qt::ItemIsEditable);

				mainTable->setRowCount(newRowCount);
					mainTable->setItem(newRowCount - 1, 0, itemNumber);
					mainTable->setCellWidget(newRowCount - 1, 1, deleteMsgWidget);
					mainTable->setItem(newRowCount - 1, 2, itemFrom);
					mainTable->setItem(newRowCount - 1, 3, itemDate);
					mainTable->setItem(newRowCount - 1, 4, itemMessage);
			}
			
			mainTable->resizeColumnsToContents();
			mainTable->horizontalHeader()->setStretchLastSection(true);
		}

		void setContacts(QList<Contact> contacts)
		{
			m_contacts = contacts;

			foreach (Contact contact, m_contacts)
				m_contactsList->addItem(contact.pseudo + " (" + contact.firstName + " " + contact.lastName + ")");
		}

	public slots:
		void slotShowMessage(QTableWidgetItem *item)
		{	
			Message message = m_messages.value(item->row());

				QDialog *dialog = new QDialog(this);
					dialog->setWindowTitle("De " + message.from
							+ " le " + message.date);
					dialog->resize(Multiuso::screenWidth() / 2, Multiuso::screenHeight() / 2);

				QTextBrowser *text = new QTextBrowser;
					text->setPlainText(message.message);

				QVBoxLayout *layout = new QVBoxLayout(dialog);
					layout->addWidget(text);
					layout->addWidget(Multiuso::closeButton(dialog));

				dialog->exec();
		}

		void slotDisconnect()
		{
			m_pseudo->setText("");
			m_firstName->setText("");
			m_lastName->setText("");
			mainTable->clear();
			m_messages.clear();
			m_contacts.clear();
			pairs.clear();

			emit disconnected();
		}

		void slotAddContact()
		{
			bool ok;

			QString pseudo = QInputDialog::getText(this, "Ajouter un contact", "Veuillez saisir le pseudo du contact :",
					QLineEdit::Normal, QString(), &ok);

			if (!ok || pseudo.isEmpty())
				return;

			emit addContactRequested(pseudo);
		}

		void reload()
		{
			m_pseudo->setText("");
			m_firstName->setText("");
			m_lastName->setText("");
			mainTable->clear();
			m_messages.clear();
			m_contacts.clear();
			pairs.clear();

			emit reloadRequested();
		}

	signals:
		void disconnected();
		void addContactRequested(QString pseudo);
		void reloadRequested();

	private:
		QLabel *m_pseudo;
		QLabel *m_firstName;
		QLabel *m_lastName;
		QComboBox *m_contactsList;
		QTableWidget *mainTable;
		QList<Message> m_messages;
		QList<Pair> pairs;
		QList<Contact> m_contacts;
};

class Messagerie : public QDialog
{
	Q_OBJECT

	public:
		Messagerie(QWidget *parent);

		void updateMessagesWidget();

	public slots:
		void connectPeople();
		void getConnectionReply();
		void getConnectionReply(QNetworkReply::NetworkError);
		void slotDisconnected();

		void addContact(QString pseudo);
		void getContactReply();
		void getContactReply(QNetworkReply::NetworkError);

	private:
		ConnectionWidget *connectionWidget;
		MessagesWidget *messagesWidget;

		QStackedWidget *mainWidget;

		QVBoxLayout *mainLayout;

		QString currentPseudo;
		QString currentPassword;
		QString currentFirstName;
		QString currentLastName;
	
		QList<Message> messages;
		QList<Contact> contacts;

		QNetworkReply *replyConnection;
		QNetworkReply *replyContacts;
};

#endif
