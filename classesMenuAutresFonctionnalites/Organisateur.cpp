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

#include "Organisateur.h"
#include "autresClasses/LoginDialog.h"

Organisateur::Organisateur(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Organisteur");
	setWindowIcon(QIcon(":/icones/actions/actionOrganisteur.png"));
	resize(Multiuso::screenWidth() / 2, (Multiuso::screenHeight() / 2) + 20);

	m_sortBy = new QComboBox;
		m_sortBy->addItems(QStringList() << "Tout" << "Priorité très élevée" << "Priorité élevée"
				<< "Priorité normale" << "Priorité faible" << "Priorité très faible");
		connect(m_sortBy, SIGNAL(currentIndexChanged(QString)), this, SLOT(initializeTasks(QString)));

	QPushButton *addTask = new QPushButton("Ajouter une tâche");
		connect(addTask, SIGNAL(clicked()), this, SLOT(slotAddTask()));

	QStringList headerLabels;
		headerLabels << "#" << "-" << "Titre" << "Contenu" << "Priorité";

	mainTable = new QTableWidget(0, headerLabels.size());
		mainTable->setShowGrid(false);
		mainTable->setHorizontalHeaderLabels(headerLabels);
		mainTable->verticalHeader()->hide();
		mainTable->setSelectionBehavior(QAbstractItemView::SelectRows);
		mainTable->setSelectionMode(QAbstractItemView::SingleSelection);
		mainTable->resizeColumnsToContents();
		mainTable->horizontalHeader()->setStretchLastSection(true);
		connect(mainTable, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotShowTask(QTableWidgetItem *)));

	QGridLayout *mainLayout = new QGridLayout;
		mainLayout->addWidget(new QLabel("Double-cliquez sur une tâche pour l'afficher"), 0, 0, 1, 2);
		mainLayout->addWidget(m_sortBy, 0, 2, 1, 1);
		mainLayout->addWidget(addTask, 0, 3, 1, 1);
		mainLayout->addWidget(mainTable, 1, 0, 1, 4);
		mainLayout->addWidget(Multiuso::closeButton(this), 2, 3, 1, 1);

	QWidget *centralWidget = new QWidget;
		centralWidget->setLayout(mainLayout);

	QMainWindow *mainWindow = new QMainWindow;
		mainWindow->setCentralWidget(centralWidget);

	QVBoxLayout *centralLayout = new QVBoxLayout(this);
		centralLayout->addWidget(mainWindow);
		centralLayout->setContentsMargins(0, 0, 0, 0);
	
	QToolBar *toolBar = mainWindow->addToolBar("Options");
		toolBar->setMovable(false);
		toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

		QAction *actionPut = new QAction("Sauvegarder", this);
			actionPut->setIcon(QIcon(":/icones/organisateur/put.png"));
			connect(actionPut, SIGNAL(triggered()), this, SLOT(putSafeguard()));
				toolBar->addAction(actionPut);

		QAction *actionGet = new QAction("Rétablir à la dernière sauvegarde", this);
			actionGet->setIcon(QIcon(":/icones/organisateur/get.png"));
			connect(actionGet, SIGNAL(triggered()), this, SLOT(getSafeguard()));
				toolBar->addAction(actionGet);

	m_sortBy->setCurrentIndex(m_sortBy->findText("Tout"));
	initializeTasks();
}

void Organisateur::addTasksToTable(QList<QStringList> tasks)
{
	foreach (QStringList line, tasks)
	{
		int m_id = line.value(0).toInt();

		QString m_title = line.value(1);

			if (m_title.size() > 15)
				m_title = m_title.left(12) + "...";

		QString m_content = line.value(2);

			if (m_content.size() > 40)
				m_content = m_content.left(37) + "...";

		QString m_priority = line.value(3);

			QBrush brush(Qt::black);

			if (m_priority == "Très élevée")
				brush.setColor(Qt::red);

			else if (m_priority == "Élevée")
				brush.setColor(QColor(255, 108, 0));

			else if (m_priority == "Normale")
				brush.setColor(Qt::darkGreen);

			else if (m_priority == "Faible")
				brush.setColor(QColor(11, 58, 234));

			else if (m_priority == "Très faible")
				brush.setColor(Qt::darkBlue);

		int newRowCount = mainTable->rowCount() + 1;

		Pair newPair;
			newPair.first = newRowCount - 1;
			newPair.second = m_id;

		pairs << newPair;

		QTableWidgetItem *itemNumber = new QTableWidgetItem(QString::number(newRowCount));
			itemNumber->setFlags(itemNumber->flags() & ~Qt::ItemIsEditable);

		EditWidget *editTaskWidget = new EditWidget;
			editTaskWidget->setRow(newRowCount - 1);
			connect(editTaskWidget, SIGNAL(editRequest()), this, SLOT(slotEditTask()));
			connect(editTaskWidget, SIGNAL(deleteRequest()), this, SLOT(slotDeleteTask()));

		QTableWidgetItem *itemTitle = new QTableWidgetItem(m_title);
			itemTitle->setFlags(itemTitle->flags() & ~Qt::ItemIsEditable);

		QTableWidgetItem *itemContent = new QTableWidgetItem(m_content);
			itemContent->setFlags(itemContent->flags() & ~Qt::ItemIsEditable);

		QTableWidgetItem *itemPriority = new QTableWidgetItem(m_priority);
			itemPriority->setFlags(itemPriority->flags() & ~Qt::ItemIsEditable);
			itemPriority->setForeground(brush);

		mainTable->setRowCount(newRowCount);
			mainTable->setItem(newRowCount - 1, 0, itemNumber);
			mainTable->setCellWidget(newRowCount - 1, 1, editTaskWidget);
			mainTable->setItem(newRowCount - 1, 2, itemTitle);
			mainTable->setItem(newRowCount - 1, 3, itemContent);
			mainTable->setItem(newRowCount - 1, 4, itemPriority);
	}
}

void Organisateur::initializeTasks(QString sortBy)
{
	pairs.clear();

	priorityVeryHigh.clear();
	priorityHigh.clear();
	priorityNormal.clear();
	priorityLow.clear();
	priorityVeryLow.clear();

	QStringList headerLabels;
		headerLabels << "#" << "-" << "Titre" << "Contenu" << "Priorité";

	mainTable->clear();
	mainTable->setRowCount(0);
	mainTable->setHorizontalHeaderLabels(headerLabels);

	QSettings settings(Multiuso::appDirPath() + "/ini/task_manager.ini", QSettings::IniFormat);

	foreach (QString group, settings.childGroups())
	{
		QStringList task = settings.value(group + "/content").value<QStringList>();

		if (task.value(3) == "Très élevée")
			priorityVeryHigh << task;

		else if (task.value(3) == "Élevée")
			priorityHigh << task;

		else if (task.value(3) == "Normale")
			priorityNormal << task;

		else if (task.value(3) == "Faible")
			priorityLow << task;

		else if (task.value(3) == "Très faible")
			priorityVeryLow << task;
	}

	if (sortBy == "Tout" || sortBy == "")
	{
		addTasksToTable(priorityVeryHigh);
		addTasksToTable(priorityHigh);
		addTasksToTable(priorityNormal);
		addTasksToTable(priorityLow);
		addTasksToTable(priorityVeryLow);
	}

	else if (sortBy == "Priorité très élevée")
	{
		addTasksToTable(priorityVeryHigh);
	}

	else if (sortBy == "Priorité élevée")
	{
		addTasksToTable(priorityHigh);
	}

	else if (sortBy == "Priorité normale")
	{
		addTasksToTable(priorityNormal);
	}

	else if (sortBy == "Priorité faible")
	{
		addTasksToTable(priorityLow);
	}

	else if (sortBy == "Priorité très faible")
	{
		addTasksToTable(priorityVeryLow);
	}

	mainTable->resizeColumnsToContents();
	mainTable->horizontalHeader()->setStretchLastSection(true);
}

void Organisateur::slotAddTask(bool edition, QStringList values)
{
	QDialog *dialog = new QDialog(this);

		if (edition)
			dialog->setWindowTitle("Ajouter une tâche");

		else
			dialog->setWindowTitle("Éditer une tâche");

		dialog->setWindowIcon(QIcon(":/icones/actions/actionOrganisteur.png"));

			QLineEdit *taskTitle = new QLineEdit;

				if (edition)
					taskTitle->setText(values.value(1));

			QTextEdit *taskContent = new QTextEdit;

				if (edition)
					taskContent->setText(values.value(2));

			QComboBox *taskPriority = new QComboBox;
				taskPriority->addItems(QStringList() << "Très élevée" << "Élevée" << "Normale"
						<< "Faible" << "Très faible");

				if (edition)
					taskPriority->setCurrentIndex(taskPriority->findText(values.value(3)));

				else
					taskPriority->setCurrentIndex(taskPriority->findText("Normale"));

			QPushButton *acceptDialog = new QPushButton("OK");
				connect(acceptDialog, SIGNAL(clicked()), dialog, SLOT(accept()));

			QPushButton *rejectDialog = new QPushButton("Annuler");
				connect(rejectDialog, SIGNAL(clicked()), dialog, SLOT(reject()));

			QFormLayout *dialogContent = new QFormLayout;
				dialogContent->addRow("Titre : ", taskTitle);
				dialogContent->addRow("Tâche : ", taskContent);
				dialogContent->addRow("Priorité : ", taskPriority);

			QHBoxLayout *buttonsLayout = new QHBoxLayout;
				buttonsLayout->addWidget(acceptDialog);
				buttonsLayout->addWidget(rejectDialog);
				buttonsLayout->setAlignment(Qt::AlignRight);

			QVBoxLayout *dialogMainLayout = new QVBoxLayout(dialog);
				dialogMainLayout->addLayout(dialogContent);
				dialogMainLayout->addLayout(buttonsLayout);

	if (!dialog->exec() == QDialog::Accepted)
		return;

	QString m_taskTitle = taskTitle->text();
	QString m_taskContent = taskContent->toPlainText();
	QString m_taskPriority = taskPriority->currentText();

	// Saving infos
		QSettings settings(Multiuso::appDirPath() + "/ini/task_manager.ini", QSettings::IniFormat);

			int group;

			if (edition)
			{
				group = values.value(0).toInt();
			}

			else
			{
				int nextID = settings.value("nextID").toInt() + 1;

				settings.setValue("nextID", nextID);

				group = nextID;
			}

			settings.setValue(QString::number(group) + "/content", QStringList() << QString::number(group)
					<< m_taskTitle << m_taskContent << m_taskPriority);

	initializeTasks(m_sortBy->currentText());
}

void Organisateur::slotEditTask()
{
	EditWidget *editWidget = qobject_cast<EditWidget *>(sender());

	if (editWidget == 0)
		return;

	int row = editWidget->row();

	QSettings settings(Multiuso::appDirPath() + "/ini/task_manager.ini", QSettings::IniFormat);

	foreach (Pair pair, pairs)
	{
		if (pair.first == row)
		{
			slotAddTask(true, settings.value(QString::number(pair.second) + "/content").value<QStringList>());

			return;
		}
	}
}

void Organisateur::slotDeleteTask()
{
	EditWidget *editWidget = qobject_cast<EditWidget *>(sender());

	if (editWidget == 0)
		return;

	int row = editWidget->row();

	QSettings settings(Multiuso::appDirPath() + "/ini/task_manager.ini", QSettings::IniFormat);

	foreach (Pair pair, pairs)
	{
		if (pair.first == row)
		{
			settings.remove(QString::number(pair.second));

			initializeTasks(m_sortBy->currentText());

			return;
		}
	}
}

void Organisateur::slotShowTask(QTableWidgetItem *item)
{
	int row = item->row();

	QSettings settings(Multiuso::appDirPath() + "/ini/task_manager.ini", QSettings::IniFormat);

	foreach (Pair pair, pairs)
	{
		if (pair.first == row)
		{
			QStringList taskContent = settings.value(QString::number(pair.second)
					+ "/content").value<QStringList>();

			QDialog *dialog = new QDialog(this);
				dialog->setWindowTitle("#" + taskContent.value(0) + " - " + taskContent.value(1));
				dialog->setWindowIcon(QIcon(":/icones/actions/actionOrganisteur.png"));
				dialog->resize(Multiuso::screenWidth() / 2, Multiuso::screenHeight() / 2);

				QTextBrowser *text = new QTextBrowser;
					text->setHtml("<h2>" + taskContent.value(1) + "</h2>"
							+ taskContent.value(2).replace("\n", "<br />"));

				QVBoxLayout *dialogLayout = new QVBoxLayout(dialog);
					dialogLayout->addWidget(text);
					dialogLayout->addWidget(Multiuso::closeButton(dialog));

				dialog->exec();

			return;
		}
	}
}

void Organisateur::putSafeguard()
{
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
	
	QFile file(Multiuso::appDirPath() + "/ini/task_manager.ini");
		file.open(QIODevice::ReadOnly | QIODevice::Text);
			QString text = file.readAll();
		file.close();

	text.replace("&", "|0088amp;|");

	QNetworkRequest request(QCoreApplication::organizationDomain() + "organizer.php?request=put"
								"&pseudo=" + pseudo +
								"&pwd=" + password +
								"&text=" + text);

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	r_put = manager->get(request);
		connect(r_put, SIGNAL(finished()), this, SLOT(putSafeguardReply()));
		connect(r_put, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(putSafeguardReply(QNetworkReply::NetworkError)));

	setDisabled(true);
}

void Organisateur::putSafeguardReply()
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

void Organisateur::putSafeguardReply(QNetworkReply::NetworkError)
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

void Organisateur::getSafeguard()
{
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
	
	QNetworkRequest request(QCoreApplication::organizationDomain() + "organizer.php?request=get"
								"&pseudo=" + pseudo +
								"&pwd=" + password);

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	r_get = manager->get(request);
		connect(r_get, SIGNAL(finished()), this, SLOT(getSafeguardReply()));
		connect(r_get, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(getSafeguardReply(QNetworkReply::NetworkError)));
	
	setDisabled(true);
}

void Organisateur::getSafeguardReply()
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

	QFile reply(Multiuso::appDirPath() + "/ini/task_manager.ini");
		reply.open(QIODevice::WriteOnly | QIODevice::Text);
			reply.write(text.toAscii());
		reply.close();
	
	m_sortBy->setCurrentIndex(m_sortBy->findText("Tout"));
	initializeTasks();
	
	setDisabled(false);
}

void Organisateur::getSafeguardReply(QNetworkReply::NetworkError)
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
