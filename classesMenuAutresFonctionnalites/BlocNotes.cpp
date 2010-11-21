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

#include "BlocNotes.h"
#include "autresClasses/LoginDialog.h"

BlocNotes::BlocNotes(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Bloc-Notes");
	setWindowIcon(QIcon(":/icones/actions/actionBlocNotes.png"));

	resize(Multiuso::screenWidth() / 2, Multiuso::screenHeight() / 2);
	
	QAction *actionAddTab = new QAction(this);
		actionAddTab->setIcon(QIcon(":/icones/bloc_notes/actionAddTab.png"));
		connect(actionAddTab, SIGNAL(triggered()), this, SLOT(addTab()));

	QToolButton *buttonAddTab = new QToolButton;
		buttonAddTab->setDefaultAction(actionAddTab);
		buttonAddTab->setAutoRaise(true);

	m_tabWidget = new QTabWidget;
		m_tabWidget->setDocumentMode(true);
		//m_tabWidget->setTabPosition(QTabWidget::East);
		m_tabWidget->setCornerWidget(buttonAddTab, Qt::BottomLeftCorner);
		connect(m_tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(removeTab(int)));
		connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

	m_containerLayout = new QVBoxLayout;
		m_containerLayout->addWidget(m_tabWidget);
		m_containerLayout->setContentsMargins(0, 0, 0, 0);

	m_container = new QWidget;
		m_container->setLayout(m_containerLayout);

	QVBoxLayout *containerLayout = new QVBoxLayout;
		containerLayout->addWidget(m_container);
		containerLayout->setContentsMargins(0, 0, 0, 0);

	QVBoxLayout *mainLayout = new QVBoxLayout;
		mainLayout->addLayout(containerLayout);
		mainLayout->addWidget(Multiuso::closeButton(this));
		mainLayout->setContentsMargins(4, 4, 4, 4);

	QWidget *mainWidget = new QWidget;
		mainWidget->setLayout(mainLayout);

	QMainWindow *mainWindow = new QMainWindow;
		mainWindow->setCentralWidget(mainWidget);

	QVBoxLayout *mainWindowLayout = new QVBoxLayout(this);
		mainWindowLayout->addWidget(mainWindow);
		mainWindowLayout->setContentsMargins(0, 0, 0, 0);	
	
	QToolBar *toolBar = mainWindow->addToolBar("Options");
		toolBar->setMovable(false);
		toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

		QAction *actionPut = new QAction("Sauvegarder", this);
			actionPut->setIcon(QIcon(":/icones/bloc_notes/put.png"));
			connect(actionPut, SIGNAL(triggered()), this, SLOT(putSafeguard()));
				toolBar->addAction(actionPut);

		QAction *actionGet = new QAction("Rétablir à la dernière sauvegarde", this);
			actionGet->setIcon(QIcon(":/icones/bloc_notes/get.png"));
			connect(actionGet, SIGNAL(triggered()), this, SLOT(getSafeguard()));
				toolBar->addAction(actionGet);

	loadNotes();
}

void BlocNotes::loadNotes()
{	
	QSettings settings(Multiuso::appDirPath() + "/ini/bloc_notes.ini", QSettings::IniFormat);

	int last_index = settings.value("last_index").toInt();

	foreach (QString page, settings.childGroups())
	{
		addTab(false);

		currentTextEdit(m_tabWidget->count() - 1)->setPlainText(
			settings.value(page + "/content").toString());

		connect(currentTextEdit(m_tabWidget->count() - 1),
			SIGNAL(textChanged()), this, SLOT(saveText()));
	}

	if (m_tabWidget->count() == 0)
		addTab();

	updateView();

	m_tabWidget->setCurrentIndex(last_index);
}

void BlocNotes::updateView()
{
	if (m_tabWidget->count() == 1)
		m_tabWidget->setTabsClosable(false);

	else
		m_tabWidget->setTabsClosable(true);

	for (int i = 0; i < m_tabWidget->count(); i++)
	{
		QString text = currentTextEdit(i)->toPlainText();

		if (text.isEmpty())
			text = "(vide)";

		else
			text = text.left(text.indexOf("\n"));

		if (text.size() > 17)
			text = text.left(14) + "...";

		text.replace("&", "&&");

		m_tabWidget->setTabText(i, text);
	}
}

QPlainTextEdit *BlocNotes::currentTextEdit(int index)
{
	if (index == -1)
		return m_tabWidget->currentWidget()->findChild<QPlainTextEdit *>();

	else
		return m_tabWidget->widget(index)->findChild<QPlainTextEdit *>();
}

void BlocNotes::addTab(bool connect)
{
	QPlainTextEdit *textEdit = new QPlainTextEdit;

	if (connect)
		QObject::connect(textEdit, SIGNAL(textChanged()), this, SLOT(saveText()));

	QVBoxLayout *textEditLayout = new QVBoxLayout;
		textEditLayout->addWidget(textEdit);
		textEditLayout->setContentsMargins(0, 0, 0, 0);

	QWidget *container = new QWidget;
		container->setLayout(textEditLayout);

	m_tabWidget->addTab(container, "(vide)");

	updateView();

	m_tabWidget->setCurrentIndex(m_tabWidget->count() - 1);
}

void BlocNotes::removeTab(int index)
{
	m_tabWidget->widget(index)->deleteLater();
	m_tabWidget->removeTab(index);

	QFile::remove(Multiuso::appDirPath() + "/ini/bloc_notes.ini");

	QSettings settings(Multiuso::appDirPath() + "/ini/bloc_notes.ini", QSettings::IniFormat);

	for (int i = 0; i < m_tabWidget->count(); i++)
		settings.setValue(QString::number(i) + "/content", currentTextEdit(i)->toPlainText());

	updateView();

	tabChanged(m_tabWidget->currentIndex());
}

void BlocNotes::tabChanged(int tab)
{
	QSettings settings(Multiuso::appDirPath() + "/ini/bloc_notes.ini", QSettings::IniFormat);
		settings.setValue("last_index", tab);
}

void BlocNotes::saveText()
{
	QPlainTextEdit *textEdit = qobject_cast<QPlainTextEdit *>(sender());

	if (textEdit == 0)
		return;

	QSettings settings(Multiuso::appDirPath() + "/ini/bloc_notes.ini", QSettings::IniFormat);
		settings.setValue(QString::number(m_tabWidget->currentIndex()) + "/content", textEdit->toPlainText());

	updateView();
}

void BlocNotes::putSafeguard()
{
	if (!Multiuso::checkIfConnected(this))
		return;

	QSettings settings(Multiuso::appDirPath() + "/ini/user.ini", QSettings::IniFormat);

	QString pseudo;
	QString password;
	bool login = false;
	
	if (!settings.value("pseudo").toString().isEmpty()
		&& !settings.value("password").toString().isEmpty())
	{
		pseudo = settings.value("pseudo").toString();
		password = settings.value("password").toString();
		login = true;
	}

	if (!login)
	{
		LoginDialog *dialog = new LoginDialog(windowIcon(), this);
		
		if (dialog->exec() == QDialog::Rejected)
		{
			dialog->deleteLater();

			return;
		}

		pseudo = dialog->getPseudo();
		password = dialog->getPassword();

		dialog->deleteLater();
	}
	
	password = QCryptographicHash::hash(password.toAscii(), QCryptographicHash::Sha1).toHex();
	
	QFile file(Multiuso::appDirPath() + "/ini/bloc_notes.ini");
		file.open(QIODevice::ReadOnly | QIODevice::Text);
			QString text = file.readAll();
		file.close();

	text.replace("&", "|0088amp;|");

	QNetworkRequest request(QCoreApplication::organizationDomain() + "notepad.php?request=put"
								"&pseudo=" + pseudo +
								"&pwd=" + password +
								"&text=" + text);

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	r_put = manager->get(request);
		connect(r_put, SIGNAL(finished()), this, SLOT(putSafeguardReply()));
		connect(r_put, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(putSafeguardReply(QNetworkReply::NetworkError)));

	setDisabled(true);
}

void BlocNotes::putSafeguardReply()
{
	QFile reply(Multiuso::tempPath() + "/reply");
		reply.open(QIODevice::WriteOnly | QIODevice::Text);
			reply.write(r_put->readAll());
		reply.close();

		r_put->deleteLater();

		QTextStream stream(&reply);
			stream.setCodec("UTF-8");

		reply.open(QIODevice::ReadOnly | QIODevice::Text);

		while (!stream.atEnd())
		{
			QString line = stream.readLine();

			if (line.startsWith("ERROR:"))
			{
				int error = line.replace(QRegExp("ERROR:([0-9]+)"), "\\1").toInt();

				switch (error)
				{
					case 0: QMessageBox::information(this, "Multiuso", "Sauvegarde réussie !"); break;
					case 1: QMessageBox::critical(this, "Multiuso", "Pseudo ou mot de passe incorrect !"); break;
					default: QMessageBox::critical(this, "Multiuso", "Erreur inconnue !"); break;
				}
			}
		}

	reply.close();
	reply.remove();

	setDisabled(false);
}

void BlocNotes::putSafeguardReply(QNetworkReply::NetworkError)
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

	if (reply != 0)
	{
		reply->abort();
		reply->deleteLater();
	}

	QMessageBox::critical(this, "Multiuso", "Impossible d'accéder à la page de sauvegarde, réessayez plus tard.");
	
	setDisabled(false);
}

void BlocNotes::getSafeguard()
{
	if (!Multiuso::checkIfConnected(this))
		return;

	QSettings settings(Multiuso::appDirPath() + "/ini/user.ini", QSettings::IniFormat);

	QString pseudo;
	QString password;
	bool login = false;
	
	if (!settings.value("pseudo").toString().isEmpty()
		&& !settings.value("password").toString().isEmpty())
	{
		pseudo = settings.value("pseudo").toString();
		password = settings.value("password").toString();
		login = true;
	}

	if (!login)
	{
		LoginDialog *dialog = new LoginDialog(windowIcon(), this);
		
		if (dialog->exec() == QDialog::Rejected)
		{
			dialog->deleteLater();

			return;
		}

		pseudo = dialog->getPseudo();
		password = dialog->getPassword();

		dialog->deleteLater();
	}
	
	password = QCryptographicHash::hash(password.toAscii(), QCryptographicHash::Sha1).toHex();
	
	QNetworkRequest request(QCoreApplication::organizationDomain() + "notepad.php?request=get"
								"&pseudo=" + pseudo +
								"&pwd=" + password);

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	r_get = manager->get(request);
		connect(r_get, SIGNAL(finished()), this, SLOT(getSafeguardReply()));
		connect(r_get, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(getSafeguardReply(QNetworkReply::NetworkError)));
	
	setDisabled(true);
}

void BlocNotes::getSafeguardReply()
{
	QString text = r_get->readAll();
		r_get->deleteLater();

	QRegExp rx("^<head>(.+)</head>\n(.+)");
		rx.setMinimal(true);

	text.replace(rx, "\\2");
		
	if (text.startsWith("ERROR:1"))
	{
		QMessageBox::critical(this, "Multiuso", "Pseudo ou mot de passe incorrect !");

		setDisabled(false);

		return;
	}
	
	text.replace("|0088amp;|", "&");

	for (int i = m_tabWidget->count() - 1; i >= 0; i--) // Because index 1 would be index 0 (and index 1 will no longer exist).
		removeTab(i);

	QFile reply(Multiuso::appDirPath() + "/ini/bloc_notes.ini");
		reply.open(QIODevice::WriteOnly | QIODevice::Text);
			reply.write(text.toAscii());
		reply.close();
	
	loadNotes();
	
	setDisabled(false);
}

void BlocNotes::getSafeguardReply(QNetworkReply::NetworkError)
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

	if (reply != 0)
	{
		reply->abort();
		reply->deleteLater();
	}

	QMessageBox::critical(this, "Multiuso", "Impossible d'accéder à la page de sauvegarde, réessayez plus tard.");
	
	setDisabled(false);
}

