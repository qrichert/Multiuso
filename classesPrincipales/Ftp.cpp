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

#include "Ftp.h"

Ftp::Ftp(QWidget *parent = 0) : QMainWindow(parent), ftp(0), fileToPut(0), fileToGet(0)
{
	QSettings settings(Multiuso::appDirPath() + "/reglages/ftp.ini", QSettings::IniFormat);
	
	createActions();
	createWidgets();
	createMenus();
	createToolBars();
	createViews();

	labelStatus = new QLabel("File d'attente : 0 fichier");

	labelLights = new QLabel;

	labelLightsMovie = new QMovie(":/icones/ftp/queue_active.gif");

	displayStatus = statusBar();
		displayStatus->addPermanentWidget(labelStatus);
		displayStatus->addPermanentWidget(labelLights);

	// Main splitter (vertical)
		
	QSplitter *mainSplitter = new QSplitter(Qt::Vertical);
		mainSplitter->addWidget(textOutput);
		mainSplitter->addWidget(splitterViews);
		mainSplitter->addWidget(tableDownloads);

	setCentralWidget(mainSplitter);
	
	currentComputerPath = "";
	currentServerPath = "";
	fakeServerPath = "";

	listComputerFiles(QDir::homePath());

	downloadIsInProgress = false;

	restoreState(settings.value("etat_fenetre").toByteArray());
}

void Ftp::createActions()
{
	actionConnect = new QAction("Connexion", this);
		actionConnect->setIcon(QIcon(":/icones/ftp/connexion.png"));
		connect(actionConnect, SIGNAL(triggered()), this, SLOT(connectOrDisconnect()));

	actionDisconnect = new QAction("Déconnexion", this);
		actionDisconnect->setIcon(QIcon(":/icones/ftp/deconnexion.png"));
		actionDisconnect->setVisible(false);
		connect(actionDisconnect, SIGNAL(triggered()), this, SLOT(connectOrDisconnect()));

	loadHistory();

	actionRemoveHistory = new QAction("Effacer l'historique", this);
		actionRemoveHistory->setIcon(QIcon(":/icones/ftp/effacer_historique.png"));
		connect(actionRemoveHistory, SIGNAL(triggered()), this, SLOT(removeHistory()));
}

void Ftp::createWidgets()
{
	lineHost = new QLineEdit;

	lineLogin = new QLineEdit;

	linePassword = new QLineEdit;
		linePassword->setEchoMode(QLineEdit::Password);

	linePort = new QLineEdit;
		linePort->setText("21");

	buttonActions = new QPushButton("Actions...");

	textOutput = new QTextEdit;
		textOutput->setReadOnly(true);
		textOutput->setFixedHeight(125);
}

void Ftp::createMenus()
{
	menuActions = new QMenu("Actions...");
		menuActions->addAction(actionConnect);
		menuActions->addAction(actionDisconnect);
		menuActions->addSeparator();

			foreach (FtpAction *action, history)
				menuActions->addAction(action);

		menuActions->addSeparator();
		menuActions->addAction(actionRemoveHistory);

	buttonActions->setMenu(menuActions);
}

void Ftp::createToolBars()
{
	toolBarConnection = addToolBar("Connexion");
		toolBarConnection->setObjectName("Connexion");
		toolBarConnection->addWidget(new QLabel(" Hôte : "));
		toolBarConnection->addWidget(lineHost);
		toolBarConnection->addWidget(new QLabel(" Identifiant : "));
		toolBarConnection->addWidget(lineLogin);
		toolBarConnection->addWidget(new QLabel(" Mot de passe : "));
		toolBarConnection->addWidget(linePassword);
		toolBarConnection->addWidget(new QLabel(" Port : "));
		toolBarConnection->addWidget(linePort);
		toolBarConnection->addWidget(new QLabel(" "));
		toolBarConnection->addWidget(buttonActions);
}

void Ftp::createViews()
{
	QStringList filesViewsHeaderLabels;
		filesViewsHeaderLabels << "Nom" << "Taille" << "Type" << "Dernières modifications";

	// Computer

	lineComputerPath = new QLineEdit;
		connect(lineComputerPath, SIGNAL(returnPressed()), this, SLOT(listComputerFiles()));

	tableComputer = new QTableWidget(0, filesViewsHeaderLabels.size());
		tableComputer->setHorizontalHeaderLabels(filesViewsHeaderLabels);
		tableComputer->setShowGrid(false);
		tableComputer->setContextMenuPolicy(Qt::CustomContextMenu);
		tableComputer->verticalHeader()->hide();
		tableComputer->horizontalHeader()->setStretchLastSection(true);
		tableComputer->setSelectionBehavior(QAbstractItemView::SelectRows);
		tableComputer->setSelectionMode(QAbstractItemView::SingleSelection);
		tableComputer->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(tableComputer, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(listComputerFiles(QTableWidgetItem *)));
		connect(tableComputer, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(openComputerMenu(QPoint)));

	QVBoxLayout *computerLayout = new QVBoxLayout;
		computerLayout->addWidget(lineComputerPath);
		computerLayout->addWidget(tableComputer);
		computerLayout->setContentsMargins(0, 0, 0, 0);

	QWidget *computerWidget = new QWidget;
		computerWidget->setLayout(computerLayout);

	// Server

	lineServerPath = new QLineEdit;
		connect(lineServerPath, SIGNAL(returnPressed()), this, SLOT(listFtp()));

	tableServer = new QTableWidget(0, filesViewsHeaderLabels.size());
		tableServer->setHorizontalHeaderLabels(filesViewsHeaderLabels);
		tableServer->setShowGrid(false);
		tableServer->setContextMenuPolicy(Qt::CustomContextMenu);
		tableServer->verticalHeader()->hide();
		tableServer->horizontalHeader()->setStretchLastSection(true);
		tableServer->setSelectionBehavior(QAbstractItemView::SelectRows);
		tableServer->setSelectionMode(QAbstractItemView::SingleSelection);
		tableServer->setDisabled(true);
		tableServer->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(tableServer, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(listFtp(QTableWidgetItem *)));
		connect(tableServer, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(openFtpMenu(QPoint)));
	
	QVBoxLayout *serverLayout = new QVBoxLayout;
		serverLayout->addWidget(lineServerPath);
		serverLayout->addWidget(tableServer);
		serverLayout->setContentsMargins(0, 0, 0, 0);
		
	QWidget *serverWidget = new QWidget;
		serverWidget->setLayout(serverLayout);

	// Splitter
	
	splitterViews = new QSplitter(Qt::Horizontal);
		splitterViews->addWidget(computerWidget);
		splitterViews->addWidget(serverWidget);

	// Downloads

	QStringList downloadsHeaderLabels;
		downloadsHeaderLabels << "Nom" << "Direction" << "Progression" << "Statut";

	tableDownloads = new QTableWidget(0, downloadsHeaderLabels.size());
		tableDownloads->setHorizontalHeaderLabels(downloadsHeaderLabels);
		tableDownloads->setShowGrid(false);
		tableDownloads->verticalHeader()->hide();
		tableDownloads->horizontalHeader()->setStretchLastSection(true);
		tableDownloads->setSelectionBehavior(QAbstractItemView::SelectRows);
		tableDownloads->setSelectionMode(QAbstractItemView::SingleSelection);
}

void Ftp::loadHistory()
{
	history.clear();

	QSettings settings(Multiuso::appDirPath() + "/reglages/ftp.ini", QSettings::IniFormat);

	for (int i = 1; i <= 5; i++)
	{
		QStringList infos = settings.value("Historique/historique" + QString::number(i)).value<QStringList>();

		// 0 : Hôte
		// 1 : Identifiant
		// 2 : Mot de passe
		// 3 : Port

		FtpAction *newAction = new FtpAction(infos.value(1) + "@" + infos.value(0), this);
			newAction->setObjectName(QString::number(i - 1));
			newAction->setHost(infos.value(0));
			newAction->setLogin(infos.value(1));
			newAction->setPassword(infos.value(2));
			newAction->setPort(infos.value(3));
			connect(newAction, SIGNAL(triggered()), this, SLOT(openHistory()));

		if (newAction->getHost() == "(vide)")
		{
			newAction->setText("(vide)");
			newAction->setDisabled(true);
		}

		history << newAction;
	}
}

void Ftp::addError(QString msg)
{
	textOutput->append("<span style='color:red;'>" + msg + "</span>");
}

void Ftp::addSuccessful(QString msg)
{
	textOutput->append("<span style='color:green;'>" + msg + "</span>");
}

void Ftp::addInformation(QString msg)
{
	textOutput->append("<span style='color:blue;'>" + msg + "</span>");
}

void Ftp::sauvegarderEtat()
{
	QSettings enregistrer(Multiuso::appDirPath() + "/reglages/ftp.ini", QSettings::IniFormat);
		enregistrer.setValue("etat_fenetre", saveState());
}

void Ftp::openHistory()
{
	FtpAction *action = qobject_cast<FtpAction *>(sender());

	if (action == 0)
		return;

	if (ftp)
	{
		int answer = QMessageBox::question(this, "Multiuso", "Une connexion est déjà en cours.<br />"
			"Voulez-vous interrompre cette connexion ?", QMessageBox::Yes | QMessageBox::No);

		if (answer == QMessageBox::Yes)
			connectOrDisconnect();

		else
			return;
	}

	lineHost->setText(action->getHost());
	lineLogin->setText(action->getLogin());
	linePassword->setText(action->getPassword());
	linePort->setText(action->getPort());

	connectOrDisconnect();
}

void Ftp::removeHistory()
{
	QSettings settings(Multiuso::appDirPath() + "/reglages/ftp.ini", QSettings::IniFormat);

	for (int i = 1; i <= 5; i++)
	{
		settings.setValue("Historique/historique" + QString::number(i),
			QStringList() << "(vide)" << "(vide)" << "(vide)" << "(vide)");
	}

	foreach (QAction *action, menuActions->actions())
		menuActions->removeAction(action);

	loadHistory();
		
	menuActions->addAction(actionConnect);
	menuActions->addAction(actionDisconnect);
	menuActions->addSeparator();

		foreach (FtpAction *action, history)
			menuActions->addAction(action);

	menuActions->addSeparator();
	menuActions->addAction(actionRemoveHistory);
}

void Ftp::listComputerFiles()
{
	QDir dir(lineComputerPath->text());

	if (dir.exists() || dir.path() == "..")
		listComputerFiles(dir);
}

void Ftp::listComputerFiles(QTableWidgetItem *item)
{
	QTableWidgetItem *indexItem = tableComputer->item(item->row(), 0);
		QString fileName = indexItem->data(Qt::DisplayRole).toString();
	
	QTableWidgetItem *typeItem = tableComputer->item(item->row(), 2);
		QString fileType = typeItem->data(Qt::DisplayRole).toString();


	if (fileName == "..")
	{
		listComputerFiles(QDir(fileName));
	}

	else if (fileType == "Fichier")
	{
		if (ftp)
		{
			Transfer newTransfer;
				newTransfer.firstFile = currentComputerPath + fileName;
				newTransfer.secondFile = currentServerPath + fileName;
				newTransfer.type = PUT_FILE;

			transferQueue.enqueue(newTransfer);

			continueDownloads();
			
			updateTransferLabel();
		}
	}

	else
	{
		QDir dir(currentComputerPath + fileName);
	
		if (dir.exists())
			listComputerFiles(dir);
	}
}

void Ftp::listComputerFiles(QDir dir)
{
	if (dir.path() == "..")
	{
		QDir newDir(currentComputerPath);
			newDir.cdUp();

		dir = newDir;
	}

	if (!dir.exists())
		return;

	lineComputerPath->setText(Multiuso::addSlash(dir.path()));
	currentComputerPath = Multiuso::addSlash(dir.path());

	QStringList filesViewsHeaderLabels;
		filesViewsHeaderLabels << "Nom" << "Taille" << "Type" << "Dernières modifications";

	tableComputer->clear();
	tableComputer->setRowCount(0);
	tableComputer->setHorizontalHeaderLabels(filesViewsHeaderLabels);

	QTableWidgetItem *itemCdUp = new QTableWidgetItem("..");
		itemCdUp->setFlags(itemCdUp->flags() & ~Qt::ItemIsEditable); // "&" → et | "~" → non/pas
		itemCdUp->setIcon(QIcon(":/icones/ftp/dossier_precedent.png"));
			
	QTableWidgetItem *itemFill1 = new QTableWidgetItem("");
		itemFill1->setFlags(itemFill1->flags() & ~Qt::ItemIsEditable);

	QTableWidgetItem *itemFill2 = new QTableWidgetItem("");
		itemFill2->setFlags(itemFill2->flags() & ~Qt::ItemIsEditable);

	QTableWidgetItem *itemFill3 = new QTableWidgetItem("");
		itemFill3->setFlags(itemFill3->flags() & ~Qt::ItemIsEditable);

	tableComputer->setRowCount(1);
		tableComputer->setItem(0, 0, itemCdUp);
		tableComputer->setItem(0, 1, itemFill1);
		tableComputer->setItem(0, 2, itemFill2);
		tableComputer->setItem(0, 3, itemFill3);

	foreach(QString file, dir.entryList(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot))
	{
		QFileInfo infos(Multiuso::addSlash(dir.path()) + file);

		QString fileSize = "";

		if (infos.isFile())
			fileSize = Multiuso::toSize(infos.size());

		QString fileType = "Dossier";

		if (infos.isFile())
			fileType = "Fichier";

		QTableWidgetItem *itemName = new QTableWidgetItem(infos.fileName());
			itemName->setFlags(itemName->flags() & ~Qt::ItemIsEditable);
			itemName->setIcon(Multiuso::iconForFile(infos.fileName(), fileType));

		QTableWidgetItem *itemSize = new QTableWidgetItem(fileSize);
			itemSize->setFlags(itemSize->flags() & ~Qt::ItemIsEditable);

		QTableWidgetItem *itemType = new QTableWidgetItem(fileType);
			itemType->setFlags(itemType->flags() & ~Qt::ItemIsEditable);

		QTableWidgetItem *itemLastModified = new QTableWidgetItem(infos.lastModified().toString());
			itemLastModified->setFlags(itemLastModified->flags() & ~Qt::ItemIsEditable);

		int newRowCount = tableComputer->rowCount();

		tableComputer->setRowCount(newRowCount + 1);
			tableComputer->setItem(newRowCount, 0, itemName);
			tableComputer->setItem(newRowCount, 1, itemSize);
			tableComputer->setItem(newRowCount, 2, itemType);
			tableComputer->setItem(newRowCount, 3, itemLastModified);
	}

	tableComputer->resizeColumnsToContents();
	tableComputer->horizontalHeader()->setStretchLastSection(true);
	tableComputer->selectRow(0);
}

void Ftp::connectOrDisconnect()
{
	if (ftp)
	{
		ftp->abort();
		ftp->deleteLater();
		ftp = 0;
		
		actionConnect->setVisible(true);
		actionDisconnect->setVisible(false);
		
		lineServerPath->setText("");
		currentServerPath = "";

		QStringList filesViewsHeaderLabels;
			filesViewsHeaderLabels << "Nom" << "Taille" << "Type" << "Dernières modifications";

		tableServer->clear();
		tableServer->setRowCount(0);
		tableServer->setHorizontalHeaderLabels(filesViewsHeaderLabels);
		tableServer->setDisabled(true);

		transferQueue.clear();
		downloadIsInProgress = false;
		labelStatus->setText("File d'attente : 0 fichier");
	
		labelLightsMovie->stop();
		labelLights->setMovie(new QMovie);
		
		QStringList downloadsHeaderLabels;
			downloadsHeaderLabels << "Nom" << "Direction" << "Progression" << "Statut";

		tableDownloads->clear();
		tableDownloads->setRowCount(0);
		tableDownloads->setHorizontalHeaderLabels(downloadsHeaderLabels);

		addSuccessful("Déconnexion réussie !");
	}

	else
	{
		if (lineHost->text().isEmpty())
		{
			addError("Veuillez entrer un hôte !");

			return;
		}
		
		if (linePort->text().isEmpty())
		{
			addError("Veuillez entrer le numéro du port !");

			return;
		}

		ftp = new QFtp(this);
			connect(ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(ftpCommandFinished(int, bool)));
			connect(ftp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(addToList(QUrlInfo)));
			connect(ftp, SIGNAL(dataTransferProgress(qint64, qint64)), this, SLOT(ftpUpdateTransferProgress(qint64, qint64)));	
		
		ftp->connectToHost(lineHost->text(), (qint64) linePort->text().toInt());

		if (lineLogin->text().isEmpty())
			ftp->login();

		else
			ftp->login(lineLogin->text(), linePassword->text());

		actionConnect->setVisible(false);
		actionDisconnect->setVisible(true);

		addInformation("Tentative de connexion au serveur...");

		QSettings settings(Multiuso::appDirPath() + "/reglages/ftp.ini", QSettings::IniFormat);

		QString currentEntry = lineHost->text() + lineLogin->text() +
				linePassword->text() + linePort->text();

		QStringList lastEntries;

		for (int i = 1; i <= 5; i++)
		{
			QStringList entries = settings.value("Historique/historique" + QString::number(i)).value<QStringList>();;
			
			if (entries.value(0) == "(vide)")
			{
				if (lastEntries.contains(currentEntry))
				{
					i = 6;

					continue;
				}

				settings.setValue("Historique/historique" + QString::number(i),
						QStringList() << lineHost->text() << lineLogin->text() <<
						linePassword->text() << linePort->text());

				i = 6;
			}

			else
			{
				lastEntries << entries.value(0) + entries.value(1) +
					entries.value(2) + entries.value(3);
			}
		}
		
		foreach (QAction *action, menuActions->actions())
			menuActions->removeAction(action);

		loadHistory();
		
		menuActions->addAction(actionConnect);
		menuActions->addAction(actionDisconnect);
		menuActions->addSeparator();

			foreach (FtpAction *action, history)
				menuActions->addAction(action);

		menuActions->addSeparator();
		menuActions->addAction(actionRemoveHistory);
	}
}

void Ftp::cdFtp(QString dirName)
{
	ftp->cd(dirName);
	fakeServerPath = dirName;

	addInformation("Tentative de changement de répertoire...");
}

void Ftp::listFtp()
{
	if (!ftp)
		return;

	cdFtp(Multiuso::addSlash(lineServerPath->text()));
}

void Ftp::listFtp(QTableWidgetItem *item)
{
	if (!ftp)
		return;
	
	QTableWidgetItem *indexItem = tableServer->item(item->row(), 0);
		QString fileName = indexItem->data(Qt::DisplayRole).toString();
	
	QTableWidgetItem *typeItem = tableServer->item(item->row(), 2);
		QString fileType = typeItem->data(Qt::DisplayRole).toString();


	if (fileName == "..")
	{
		cdFtp(Multiuso::cdUp(currentServerPath));
	}

	else if (fileType == "Fichier")
	{
		if (ftp)
		{
			Transfer newTransfer;
				newTransfer.firstFile = currentServerPath + fileName;
				newTransfer.secondFile = currentComputerPath + fileName;
				newTransfer.type = GET_FILE;
					
			transferQueue.enqueue(newTransfer);

			continueDownloads();

			updateTransferLabel();
		}
	}

	else
	{
		cdFtp(currentServerPath + fileName);
	}
}

void Ftp::listFtp(QString dirName)
{
	if (ftp)
	{
		lineServerPath->setText(Multiuso::addSlash(dirName));
		currentServerPath = Multiuso::addSlash(dirName);

		QStringList filesViewsHeaderLabels;
			filesViewsHeaderLabels << "Nom" << "Taille" << "Type" << "Dernières modifications";

		tableServer->clear();
		tableServer->setRowCount(0);
		tableServer->setHorizontalHeaderLabels(filesViewsHeaderLabels);
		tableServer->setDisabled(false);
		
		QTableWidgetItem *itemCdUp = new QTableWidgetItem("..");
			itemCdUp->setFlags(itemCdUp->flags() & ~Qt::ItemIsEditable); // "&" → et | "~" → non/pas
			itemCdUp->setIcon(QIcon(":/icones/ftp/dossier_precedent.png"));

		QTableWidgetItem *itemFill1 = new QTableWidgetItem("");
			itemFill1->setFlags(itemFill1->flags() & ~Qt::ItemIsEditable);

		QTableWidgetItem *itemFill2 = new QTableWidgetItem("");
			itemFill2->setFlags(itemFill2->flags() & ~Qt::ItemIsEditable);

		QTableWidgetItem *itemFill3 = new QTableWidgetItem("");
			itemFill3->setFlags(itemFill3->flags() & ~Qt::ItemIsEditable);

		tableServer->setRowCount(1);
			tableServer->setItem(0, 0, itemCdUp);
			tableServer->setItem(0, 1, itemFill1);
			tableServer->setItem(0, 2, itemFill2);
			tableServer->setItem(0, 3, itemFill3);

		tableServer->resizeColumnsToContents();
		tableServer->horizontalHeader()->setStretchLastSection(true);
		tableServer->selectRow(0);

		ftp->list(dirName);
		
		addInformation("Tentative de listage...");
	}
}

void Ftp::ftpCommandFinished(int, bool error)
{
	switch (ftp->currentCommand())
	{
		case QFtp::ConnectToHost:
		{
			if (error)
			{
				addError("Impossible de se connecter au serveur !");
				connectOrDisconnect();
			}

			else
			{
				addSuccessful("Connexion au serveur réussie !");
			}
		}
		break;

		case QFtp::Login:
		{
			if (error)
			{
				addError("Identifiant ou mot de passe incorrect !");
				connectOrDisconnect();
			}

			else
			{
				addSuccessful("Identification réussie !");

				cdFtp("/");
			}
		}
		break;
		
		case QFtp::Cd:
		{
			if (error)
			{
				addError("Changement de répertoire échoué !");
			}

			else
			{
				addSuccessful("Changement de répertoire réussi!");

				currentServerPath = Multiuso::addSlash(fakeServerPath);

				listFtp(currentServerPath);
			}
		}
		break;
		
		case QFtp::Put:
		{
			QProgressBar *progress = qobject_cast<QProgressBar *>(tableDownloads->cellWidget(0, 2));

			if (progress)
				progress->setValue(progress->maximum());

			QTableWidgetItem *item = tableDownloads->item(0, 3);

			if (error)
			{
				if (item)
					item->setText("Erreur lors de l'envoi");

				addError("Erreur lors de l'envoi !");
			}

			else
			{
				if (item)
					item->setText("Envoi réussi");

				addSuccessful("Envoi réussi !");
			}

			delete fileToPut;
			fileToPut = NULL;

			cdFtp(currentServerPath);

			downloadIsInProgress = false;
			
			labelLightsMovie->stop();
			labelLights->setMovie(new QMovie);

			continueDownloads();
		}
		break;
		
		case QFtp::Get:
		{
			QProgressBar *progress = qobject_cast<QProgressBar *>(tableDownloads->cellWidget(0, 2));

			if (progress)
				progress->setValue(progress->maximum());

			QTableWidgetItem *item = tableDownloads->item(0, 3);

			if (error)
			{
				if (item)
					item->setText("Erreur lors du téléchargement");

				addError("Erreur lors du téléchargement !");
			}

			else
			{
				if (item)
					item->setText("Téléchargement réussi");

				addSuccessful("Téléchargement réussi !");
			}

			fileToGet->close();

			delete fileToGet;
			fileToGet = NULL;

			listComputerFiles(QDir(currentComputerPath));
			
			downloadIsInProgress = false;

			labelLightsMovie->stop();
			labelLights->setMovie(new QMovie);

			continueDownloads();
		}
		break;

		case QFtp::Mkdir:
		{
			if (error)
			{
				addError("Erreur lors de la création du répertoire !");
			}

			else
			{
				addSuccessful("Création du répertoire réussie !");
				cdFtp(currentServerPath);
			}
		}
		break;
		
		case QFtp::List:
		{
			if (error)
				addError("Erreur lors du listage des fichiers !");

			else
				addSuccessful("Listage réussi !");
		}
		break;

		case QFtp::Remove:
		case QFtp::Rmdir:
		{
			if (error)
			{
				addError("Erreur lors de la supression !");
			}

			else
			{
				addSuccessful("Supression réussie !");
				cdFtp(currentServerPath);
			}
		}
		break;
		
		case QFtp::Rename:
		{
			if (error)
			{
				addError("Erreur lors du renommage !");
			}

			else
			{
				addSuccessful("Renommage réussi !");
				cdFtp(currentServerPath);
			}
		}
		break;


		case QFtp::None:
		case QFtp::SetTransferMode:
		case QFtp::SetProxy:
		case QFtp::Close:
		case QFtp::RawCommand:
		break;
	}
}

void Ftp::addToList(QUrlInfo infos)
{
	if (infos.name() == "." || infos.name() == "..")
		return;

	QString fileSize = "";

	if (infos.isFile())
		fileSize = Multiuso::toSize(infos.size());

	QString fileType = "Dossier";

	if (infos.isFile())
		fileType = "Fichier";

	QTableWidgetItem *itemName = new QTableWidgetItem(infos.name());
		itemName->setFlags(itemName->flags() & ~Qt::ItemIsEditable);
		itemName->setIcon(Multiuso::iconForFile(infos.name(), fileType));

	QTableWidgetItem *itemSize = new QTableWidgetItem(fileSize);
		itemSize->setFlags(itemSize->flags() & ~Qt::ItemIsEditable);

	QTableWidgetItem *itemType = new QTableWidgetItem(fileType);
		itemType->setFlags(itemType->flags() & ~Qt::ItemIsEditable);

	QTableWidgetItem *itemLastModified = new QTableWidgetItem(infos.lastModified().toString());
		itemLastModified->setFlags(itemLastModified->flags() & ~Qt::ItemIsEditable);

	int newRowCount = tableServer->rowCount();

	tableServer->setRowCount(newRowCount + 1);
		tableServer->setItem(newRowCount, 0, itemName);
		tableServer->setItem(newRowCount, 1, itemSize);
		tableServer->setItem(newRowCount, 2, itemType);
		tableServer->setItem(newRowCount, 3, itemLastModified);

	tableServer->resizeColumnsToContents();
	tableServer->horizontalHeader()->setStretchLastSection(true);
}

void Ftp::updateTransferLabel(bool minus)
{
	int waitingFiles = transferQueue.count();

	if (minus)
		waitingFiles--;
	
	QString plural = "s";

	if (waitingFiles < 2)
		plural = "";

	labelStatus->setText("File d'attente : " + QString::number(waitingFiles) + " fichier" + plural);
}

void Ftp::continueDownloads()
{
	if (downloadIsInProgress || transferQueue.isEmpty() || !ftp)
		return;

	Transfer transfer = transferQueue.dequeue();

	updateTransferLabel();

	QString firstFile = transfer.firstFile;
	QString secondFile = transfer.secondFile;
	TransferType type = transfer.type;

	QString direction = "";

	switch (type)
	{
		case PUT_FILE:
		{
			direction = "→";

			if (fileToPut)
			{
				delete fileToPut;
				fileToPut = NULL;
			}

			fileToPut = new QFile(firstFile);
			QString secondPutFile(secondFile);

			ftp->put(fileToPut, secondPutFile);
			
			addInformation("Envoi de « " + QFileInfo(firstFile).fileName() + " »");
		}
		break;

		case GET_FILE:
		{
			direction = "←";
			
			if (fileToGet)
			{
				delete fileToGet;
				fileToGet = NULL;
			}

			QString firstGetFile(firstFile);
			fileToGet = new QFile(secondFile);

			if (fileToGet->open(QIODevice::WriteOnly))
				ftp->get(firstGetFile, fileToGet);
			
			addInformation("Téléchargement de « " + QFileInfo(firstFile).fileName() + " »");
		}
		break;

		default:
		{
			continueDownloads();

			return;
		}
		break;
	}

	QTableWidgetItem *itemName = new QTableWidgetItem(QFileInfo(firstFile).fileName());
		itemName->setFlags(itemName->flags() & ~Qt::ItemIsEditable);
		itemName->setIcon(QIcon(Multiuso::iconForFile(QFileInfo(firstFile).fileName(), "")));
		
	QTableWidgetItem *itemDirection = new QTableWidgetItem(direction);
		itemDirection->setFlags(itemDirection->flags() & ~Qt::ItemIsEditable);
	
	QProgressBar *itemProgress = new QProgressBar;
		itemProgress->setValue(0);

	QTableWidgetItem *itemState = new QTableWidgetItem("En attente...");
		itemState->setFlags(itemState->flags() & ~Qt::ItemIsEditable);

	tableDownloads->insertRow(0);
	tableDownloads->setItem(0, 0, itemName);
	tableDownloads->setItem(0, 1, itemDirection);
	tableDownloads->setCellWidget(0, 2, itemProgress);
	tableDownloads->setItem(0, 3, itemState);
	
	tableDownloads->resizeColumnsToContents();
	tableDownloads->horizontalHeader()->setStretchLastSection(true);

	downloadIsInProgress = true;

	labelLights->setMovie(labelLightsMovie);
	labelLightsMovie->start();
}

void Ftp::ftpUpdateTransferProgress(qint64 downloaded, qint64 total)
{
	if (total != -1)
	{
		QProgressBar *progress = qobject_cast<QProgressBar *>(tableDownloads->cellWidget(0, 2));

		if (progress)
		{
			progress->setRange(0, total);
			progress->setValue(downloaded);
		}

		QTableWidgetItem *item = tableDownloads->item(0, 3);

		if (item)
			item->setText("Transfert en cours...");
	}
}

void Ftp::openComputerMenu(QPoint)
{
	QString type = tableComputer->item(tableComputer->currentRow(), 2)->text();

	QMenu menu;

	if (tableComputer->item(tableComputer->currentRow(), 0)->text() != "..")
	{
		QAction *remove = new QAction("Supprimer", this);
			remove->setIcon(QIcon(":/icones/ftp/supprimer.png"));
			connect(remove, SIGNAL(triggered()), this, SLOT(removeOnComputer()));
				menu.addAction(remove);
				
		QAction *rename = new QAction("Renommer", this);
			rename->setIcon(QIcon(":/icones/ftp/renommer.png"));
			connect(rename, SIGNAL(triggered()), this, SLOT(renameOnComputer()));
				menu.addAction(rename);
		
		menu.addSeparator();
	}

	QAction *addFolder = new QAction("Créer un dossier", this);
		addFolder->setIcon(QIcon(":/icones/ftp/creerDossier.png"));
		connect(addFolder, SIGNAL(triggered()), this, SLOT(addComputerFolder()));
			menu.addAction(addFolder);

	menu.exec(QCursor::pos());
}

void Ftp::openFtpMenu(QPoint)
{
	QString type = tableServer->item(tableServer->currentRow(), 2)->text();

	QMenu menu;

	if (tableServer->item(tableServer->currentRow(), 0)->text() != "..")
	{
		QAction *remove = new QAction("Supprimer", this);
			remove->setIcon(QIcon(":/icones/ftp/supprimer.png"));
			connect(remove, SIGNAL(triggered()), this, SLOT(removeOnServer()));
				menu.addAction(remove);
				
		QAction *rename = new QAction("Renommer", this);
			rename->setIcon(QIcon(":/icones/ftp/renommer.png"));
			connect(rename, SIGNAL(triggered()), this, SLOT(renameOnServer()));
				menu.addAction(rename);
	
		menu.addSeparator();
	}
	
	QAction *addFolder = new QAction("Créer un dossier", this);
		addFolder->setIcon(QIcon(":/icones/ftp/creerDossier.png"));
		connect(addFolder, SIGNAL(triggered()), this, SLOT(addServerFolder()));
			menu.addAction(addFolder);

	menu.exec(QCursor::pos());
}

void Ftp::removeOnComputer()
{
	QString type = tableComputer->item(tableComputer->currentRow(), 2)->text();
	
	if (type != "Fichier" && type != "Dossier")
		return;

	QString file = currentComputerPath + tableComputer->item(tableComputer->currentRow(), 0)->text();

	int answer = QMessageBox::question(this, "Multiuso", "Voulez-vous vraiment supprimer « " +
		QFileInfo(file).fileName() + " » de façon définitive ?",
			QMessageBox::Yes | QMessageBox::No);

	if (type == "Fichier")
	{
		if (answer == QMessageBox::Yes)
			QFile::remove(file);
	}

	else if (type == "Dossier")
	{
		if (answer == QMessageBox::Yes)
			Multiuso::removeDirectory(file);
	}

	if (answer == QMessageBox::Yes)
		listComputerFiles(QDir(currentComputerPath));
}

void Ftp::renameOnComputer()
{
	QString type = tableComputer->item(tableComputer->currentRow(), 2)->text();
	
	if (type != "Fichier" && type != "Dossier")
		return;

	QString file = currentComputerPath + tableComputer->item(tableComputer->currentRow(), 0)->text();

	QString newName = QInputDialog::getText(this, "Multiuso", "Entrez le nouveau nom de « " +
		QFileInfo(file).fileName() + " » :", QLineEdit::Normal, QFileInfo(file).fileName());

	if (newName.isEmpty())
		return;

	if (type == "Fichier")
		QFile::rename(file, currentComputerPath + newName);

	else if (type == "Dossier")
		QDir().rename(file, currentComputerPath + newName);

	listComputerFiles(QDir(currentComputerPath));
}

void Ftp::addComputerFolder()
{
	QString name = QInputDialog::getText(this, "Multiuso", "Entrez le nom du dossier :");

	if (name.isEmpty())
		return;

	if (QDir().mkdir(currentComputerPath + name))
		listComputerFiles(QDir(currentComputerPath));
}		

void Ftp::removeOnServer()
{
	if (!ftp)
		return;

	QString type = tableServer->item(tableServer->currentRow(), 2)->text();
	
	if (type != "Fichier" && type != "Dossier")
		return;

	QString file = currentServerPath + tableServer->item(tableServer->currentRow(), 0)->text();

	int answer = QMessageBox::question(this, "Multiuso", "Voulez-vous vraiment supprimer « " +
		QFileInfo(file).fileName() + " » de façon définitive ?",
			QMessageBox::Yes | QMessageBox::No);

	if (type == "Fichier")
	{
		if (answer == QMessageBox::Yes)
			ftp->remove(file);
	}

	else if (type == "Dossier")
	{
		if (answer == QMessageBox::Yes)
			ftp->rmdir(file);
	}

	if (answer == QMessageBox::Yes)
		addInformation("Tentative de suppression...");
}

void Ftp::renameOnServer()
{
	if (!ftp)
		return;
	
	QString type = tableServer->item(tableServer->currentRow(), 2)->text();
	
	if (type != "Fichier" && type != "Dossier")
		return;

	QString file = currentServerPath + tableServer->item(tableServer->currentRow(), 0)->text();

	QString newName = QInputDialog::getText(this, "Multiuso", "Entrez le nouveau nom de « " +
		QFileInfo(file).fileName() + " » :", QLineEdit::Normal, QFileInfo(file).fileName());

	if (newName.isEmpty())
		return;

	ftp->rename(file, currentServerPath + newName);

	addInformation("Tentative de renommage...");
}

void Ftp::addServerFolder()
{
	if (!ftp)
		return;

	QString name = QInputDialog::getText(this, "Multiuso", "Entrez le nom du dossier :");

	if (name.isEmpty())
		return;

	ftp->mkdir(currentServerPath + name);
	
	addInformation("Tentative de création d'un répertoire...");
}
