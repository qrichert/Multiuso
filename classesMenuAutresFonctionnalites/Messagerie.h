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
#include "autresClasses/SuscribeDialog.h"

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
		ConnectionWidget(QDialog *parent)
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
				buttonConnect->setIcon(QIcon(":/icones/actions/actionMessagerie.png"));
				connect(buttonConnect, SIGNAL(clicked()), this, SIGNAL(connectRequest()));

			QHBoxLayout *buttonsLayout = new QHBoxLayout;
				buttonsLayout->addWidget(Multiuso::closeButton(parent));
			
			QVBoxLayout *mainLayout = new QVBoxLayout(this);
				mainLayout->addWidget(groupConnection);
				mainLayout->addLayout(layoutInfos);
				mainLayout->addWidget(labelSuscribe);
				mainLayout->addWidget(buttonConnect);
				mainLayout->addLayout(buttonsLayout);
				mainLayout->setAlignment(Qt::AlignCenter);


			setInfos();
		}

		void setInfos()
		{
			QSettings settings(Multiuso::appDirPath() + "/ini/user.ini", QSettings::IniFormat);
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

			QSettings settings(Multiuso::appDirPath() + "/ini/user.ini", QSettings::IniFormat);
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

		void setPassword(QString password)
		{
			m_password->setText(password);
		}

	public slots:
		void suscribe(SuscribeDialog *suscribeDialog = 0)
		{
			if (suscribeDialog)
			{
				suscribeDialog->deleteLater();

				return;
			}

			SuscribeDialog *dialog = new SuscribeDialog(this);
				connect(dialog, SIGNAL(deleteRequested(SuscribeDialog *)), this, SLOT(suscribe(SuscribeDialog *)));
		}
			
	signals:
		void connectRequest();

	private:
		QLineEdit *m_pseudo;
		QLineEdit *m_password;

		QCheckBox *m_rememberMe;
		QCheckBox *m_rememberMyPassword;
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

class BlockDialog : public QDialog
{
	Q_OBJECT

	public:
		BlockDialog(QList<Contact> contacts, QStringList blocked, QWidget *parent) : QDialog(parent)
		{
			setWindowTitle("Contacts bloqués");
			setWindowIcon(QIcon(":/icones/messagerie/blocked_contacts.png"));

			foreach(Contact contact, contacts)
				m_contacts << contact.pseudo;

			m_blocked = blocked;
				m_blocked.removeOne("BLOCKED_CONTACT:");

			foreach (QString blocked, m_blocked)
				m_contacts.removeOne(blocked);

			contactsList = new QListWidget;

			foreach (QString contact, m_contacts)
			{
				QListWidgetItem *item = new QListWidgetItem(contact);
					item->setFlags(item->flags() & ~Qt::ItemIsEditable);

				contactsList->addItem(item);
			}

			blockedList = new QListWidget;
			
			foreach (QString blocked, m_blocked)
			{
				QListWidgetItem *item = new QListWidgetItem(blocked);
					item->setFlags(item->flags() & ~Qt::ItemIsEditable);

				blockedList->addItem(item);
			}

			QPushButton *buttonBlock = new QPushButton(">>");
				connect(buttonBlock, SIGNAL(clicked()), this, SLOT(slotBlock()));

			QPushButton *buttonUnblock = new QPushButton("<<");
				connect(buttonUnblock, SIGNAL(clicked()), this, SLOT(slotUnblock()));

			QVBoxLayout *buttonsLayout = new QVBoxLayout;
				buttonsLayout->addWidget(buttonBlock);
				buttonsLayout->addWidget(buttonUnblock);

			QGridLayout *layout = new QGridLayout(this);
				layout->addWidget(new QLabel("Contacts"), 0, 0, 1, 1);
				layout->addWidget(contactsList, 1, 0, 2, 2);
				layout->addLayout(buttonsLayout, 1, 2, 2, 1);
				layout->addWidget(new QLabel("Bloqués"), 0, 3, 1, 1);
				layout->addWidget(blockedList, 1, 3, 2, 2);
				layout->addLayout(Multiuso::dialogButtons(this, "Annuler", "OK"), 3, 4, 1, 1);
		}

		QStringList blockedContacts()
		{
			QStringList str;

			for (int i = 0; i < blockedList->count(); i++)
				str << blockedList->item(i)->text();

			return str;
		}

	public slots:
		void slotBlock()
		{
			QListWidgetItem *item = contactsList->takeItem(contactsList->currentRow());

			if (item == 0)
				return;

			blockedList->addItem(item);
		}

		void slotUnblock()
		{
			QListWidgetItem *item = blockedList->takeItem(blockedList->currentRow());

			if (item == 0)
				return;

			contactsList->addItem(item);
		}

	private:
		QStringList m_contacts;
		QStringList m_blocked;

		QListWidget *contactsList;
		QListWidget *blockedList;
};

class ReplyButton : public QPushButton
{
	Q_OBJECT

	public:
		ReplyButton(QString text) : QPushButton(text), m_receiver("")
		{
			setIcon(QIcon(":/icones/messagerie/new_message.png"));
		}

		void setReceiver(QString receiver)
		{
			m_receiver = receiver;
		}

	protected:
		void mouseReleaseEvent(QMouseEvent *)
		{
			emit b_clicked(m_receiver);
		}

	signals:
		void b_clicked(QString receiver);

	private:
		QString m_receiver;
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
				
			QAction *actionNewMsg = new QAction("Nouveau message", this);
				actionNewMsg->setIcon(QIcon(":/icones/messagerie/new_message.png"));
				actionNewMsg->setToolTip("Nouveau message");
				connect(actionNewMsg, SIGNAL(triggered()), this, SLOT(slotNewMsg()));

			QAction *actionAddContact = new QAction("Ajouter un contact", this);
				actionAddContact->setIcon(QIcon(":/icones/messagerie/add_contact.png"));
				actionAddContact->setToolTip("Ajouter un contact");
				connect(actionAddContact, SIGNAL(triggered()), this, SLOT(slotAddContact()));

			QAction *actionRemoveContact = new QAction("Supprimer un contact", this);
				actionRemoveContact->setIcon(QIcon(":/icones/messagerie/remove_contact.png"));
				actionRemoveContact->setToolTip("Supprimer un contact");
				connect(actionRemoveContact, SIGNAL(triggered()), this, SLOT(slotRemoveContact()));

			m_contactsList = new QComboBox;

			QAction *actionModifyPwd = new QAction("Modifier mon mot de passe", this);
				actionModifyPwd->setIcon(QIcon(":/icones/messagerie/modify_mdp.png"));
				actionModifyPwd->setToolTip("Modifier mon mot de passe");
				connect(actionModifyPwd, SIGNAL(triggered()), this, SLOT(slotModifyPwd()));

			QAction *actionBlockedContacts = new QAction("Contacts bloqués", this);
				actionBlockedContacts->setIcon(QIcon(":/icones/messagerie/blocked_contacts.png"));
				actionBlockedContacts->setToolTip("Contacts bloqués");
				connect(actionBlockedContacts, SIGNAL(triggered()), this, SLOT(slotBlockedContacts()));

			QMenu *actionsMenu = new QMenu;
				actionsMenu->addAction(actionAddContact);
				actionsMenu->addAction(actionRemoveContact);
				actionsMenu->addAction(actionBlockedContacts);
				actionsMenu->addSeparator();
				actionsMenu->addAction(actionModifyPwd);

			QAction *actionMenu = new QAction(this);
				actionMenu->setIcon(QIcon(":/icones/messagerie/action_menu.png"));
				actionMenu->setMenu(actionsMenu);

			QToolBar *actionsToolBar = addToolBar("Actions");
				actionsToolBar->setMovable(false);
				actionsToolBar->addAction(actionLogOut);
				actionsToolBar->addAction(actionReload);
				actionsToolBar->addSeparator();
				actionsToolBar->addAction(actionNewMsg);
				actionsToolBar->addWidget(new QLabel(" "));
				actionsToolBar->addWidget(m_contactsList);
				actionsToolBar->addSeparator();
				actionsToolBar->addAction(actionMenu);
	
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
					connect(deleteMsg, SIGNAL(clicked()), this, SLOT(slotDeleteMessage()));

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

		void setBlocked(QStringList blocked)
		{
			m_blocked = blocked;
		}
		
		void setMainTableDisabled(bool disabled)
		{
			mainTable->setDisabled(disabled);
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

				ReplyButton *reply = new ReplyButton("Répondre");
					reply->setReceiver(message.from);
					connect(reply, SIGNAL(b_clicked(QString)), this, SLOT(slotNewMsg(QString)));

				QHBoxLayout *buttonsLayout = new QHBoxLayout;
					buttonsLayout->addWidget(Multiuso::closeButton(dialog));
					buttonsLayout->addWidget(reply);
					buttonsLayout->setAlignment(Qt::AlignRight);

				QVBoxLayout *layout = new QVBoxLayout(dialog);
					layout->addWidget(text);
					layout->addLayout(buttonsLayout);

				dialog->exec();
		}

		void slotDisconnect()
		{
			m_pseudo->setText("");
			m_firstName->setText("");
			m_lastName->setText("");
			mainTable->clear();
			m_messages.clear();
			m_contactsList->clear();
			m_contacts.clear();
			pairs.clear();
		
			setMainTableDisabled(true);

			emit disconnected();
		}
		
		void reload()
		{
			m_pseudo->setText("");
			m_firstName->setText("");
			m_lastName->setText("");
			mainTable->clear();
			m_messages.clear();
			m_contactsList->clear();
			m_contacts.clear();
			pairs.clear();
			
			setMainTableDisabled(true);

			emit reloadRequested();
		}

		void slotNewMsg(QString receiver = QString())
		{
			if (m_contactsList->count() == 0 && receiver.isEmpty())
			{
				QMessageBox::information(this, "Multiuso", "Votre liste de contacts est vide !");

				return;
			}

			if (!receiver.isEmpty())
				receiver.replace(QRegExp("^(.+) \\((.+)\\)$"), "\\1");

			Contact contact = m_contacts.value(m_contactsList->currentIndex());

			if (receiver.isEmpty())
				receiver = contact.pseudo;

			QDialog *dialog = new QDialog(this);
				dialog->setWindowTitle("Nouveau message");
				dialog->resize(Multiuso::screenWidth() / 2, Multiuso::screenHeight() / 2);

				QLabel *labelInfos = new QLabel("De " + m_pseudo->text() +
						" à <strong>" + receiver + "</strong>");

				QPlainTextEdit *textEdit = new QPlainTextEdit;

				QPushButton *reject = new QPushButton("Annuler");
					connect(reject, SIGNAL(clicked()), dialog, SLOT(reject()));

				QPushButton *accept = new QPushButton("Envoyer");
					accept->setIcon(QIcon(":/icones/messagerie/new_message.png"));
					connect(accept, SIGNAL(clicked()), dialog, SLOT(accept()));

				QHBoxLayout *buttonsLayout = new QHBoxLayout;
					buttonsLayout->addWidget(reject);
					buttonsLayout->addWidget(accept);
					buttonsLayout->setAlignment(Qt::AlignRight);

				QVBoxLayout *dialogLayout = new QVBoxLayout(dialog);
					dialogLayout->addWidget(labelInfos);
					dialogLayout->addWidget(textEdit);
					dialogLayout->addLayout(buttonsLayout);

			if (dialog->exec() == QDialog::Accepted)
			{
				QString msg = textEdit->toPlainText();
					msg.replace("&", "|0088amp;|");
					msg.replace("<", "|0088lt;|");
					msg.replace("\n", "<br />");
			
				setMainTableDisabled(true);

				emit sendMessageRequested(receiver, msg);
			}

			dialog->deleteLater();
		}

		void slotAddContact()
		{
			bool ok;

			QString pseudo = QInputDialog::getText(this, "Ajouter un contact", "Veuillez saisir le pseudo du contact :",
					QLineEdit::Normal, QString(), &ok);

			if (!ok || pseudo.isEmpty())
				return;
			
			setMainTableDisabled(true);

			emit addContactRequested(pseudo);
		}

		void slotRemoveContact()
		{
			QStringList contacts;

			foreach (Contact contact, m_contacts)
				contacts << contact.pseudo;

			if (contacts.isEmpty())
			{
				QMessageBox::information(this, "Multiuso", "Votre liste de contacts est vide !");

				return;
			}

			bool ok;

			QString pseudo = QInputDialog::getItem(this, "Multiuso", "Choisissez le contact à supprimer :", contacts, 0, false, &ok);

			if (!ok)
				return;

			bool b_continue = true;

			foreach (Contact contact, m_contacts)
			{
				if (b_continue == false)
					continue;

				if (contact.pseudo == pseudo)
				{
					b_continue = false;
			
					setMainTableDisabled(true);

					emit removeContactRequested(contact.id);
				}
			}
		}

		void slotDeleteMessage()
		{
			QPushButton *button = qobject_cast<QPushButton *>(sender());

			if (button == 0)
				return;

			bool b_continue = true;

			foreach (Pair pair, pairs)
			{
				if (b_continue == false)
					continue;

				if (pair.first == button->objectName().toInt())
				{
					setMainTableDisabled(true);
					
					emit removeMessageRequested(QString::number(pair.second));

					b_continue = false;
				}
			}
		}

		void slotModifyPwd()
		{
			PasswordDialog *pwdDialog = new PasswordDialog(this);
				pwdDialog->setWindowTitle("Nouveau mot de passe");

			if (pwdDialog->exec() == QDialog::Rejected)
				return;
			
			setMainTableDisabled(true);

			emit modifyPasswordRequested(pwdDialog->getPassword());

			pwdDialog->deleteLater();
		}

		void slotBlockedContacts()
		{
			QStringList contacts;

			foreach (Contact contact, m_contacts)
				contacts << contact.pseudo;

			if (contacts.isEmpty())
			{
				QMessageBox::information(this, "Multiuso", "Votre liste de contacts est vide !");

				return;
			}

			BlockDialog *dialog = new BlockDialog(m_contacts, m_blocked, this);
			
			if (dialog->exec() == QDialog::Accepted)
			{
				setMainTableDisabled(true);

				emit blockRequested(dialog->blockedContacts());
			}

			dialog->deleteLater();
		}

	signals:
		void disconnected();
		void addContactRequested(QString pseudo);
		void reloadRequested();
		void removeContactRequested(QString id);
		void sendMessageRequested(QString pseudo, QString message);
		void removeMessageRequested(QString id);
		void modifyPasswordRequested(QString password);
		void blockRequested(QStringList contacts);

	private:
		QLabel *m_pseudo;
		QLabel *m_firstName;
		QLabel *m_lastName;
		QComboBox *m_contactsList;
		QTableWidget *mainTable;
		QList<Message> m_messages;
		QList<Pair> pairs;
		QList<Contact> m_contacts;
		QStringList m_blocked;
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

		void removeContact(QString id);
		void getRContactReply();
		void getRContactReply(QNetworkReply::NetworkError);

		void sendMessage(QString pseudo, QString message);
		void getSendMessageReply();
		void getSendMessageReply(QNetworkReply::NetworkError);

		void removeMessage(QString id);
		void getRemoveMessageReply();
		void getRemoveMessageReply(QNetworkReply::NetworkError);
		
		void modifyPassword(QString password);
		void getModifyPasswordReply();
		void getModifyPasswordReply(QNetworkReply::NetworkError);

		void block(QStringList contacts);
		void getBlockReply();
		void getBlockReply(QNetworkReply::NetworkError);

	private:
		ConnectionWidget *connectionWidget;
		MessagesWidget *messagesWidget;

		QStackedWidget *mainWidget;

		QVBoxLayout *mainLayout;

		QString currentPseudo;
		QString currentPassword;
		QString currentFirstName;
		QString currentLastName;

		QString tempPassword;
	
		QList<Message> messages;
		QList<Contact> contacts;
		QStringList blocked;

		QNetworkReply *replyConnection;
		QNetworkReply *replyContacts;
		QNetworkReply *replyRContacts;
		QNetworkReply *replyMessages;
		QNetworkReply *replyRMessages;
		QNetworkReply *replyModifyPassword;
		QNetworkReply *replyBlock;
};

#endif
