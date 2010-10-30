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

#include "VueDossier.h"
#include "classesPrincipales/FenPrincipale.h"

VueDossier::VueDossier(NavFichiers *parent) : m_parent(parent)
{
	m_vue = new ListWidget;	
		connect(m_vue, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(ouvrir(QListWidgetItem *)));
		connect(m_vue, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ouvrirMenu(QPoint)));
		connect(m_vue, SIGNAL(moveRequested(QString, QPoint)), this, SLOT(moveFile(QString, QPoint)));

	QVBoxLayout *layout = new QVBoxLayout(this);
		layout->addWidget(m_vue);
		layout->setContentsMargins(0, 0, 0, 0);

	loadProgress = new QProgressBar(this);
		loadProgress->setGeometry(10, 10, 175, 25);
		loadProgress->hide();

	m_chemin = "";
	modifierPosition = true;
	afficherDossiersCaches = false;

	copyCutObject = new CopyCutObject;

	QShortcut *shortcutRename = new QShortcut(QKeySequence("F2"), this);
		connect(shortcutRename, SIGNAL(activated()), this, SLOT(menuRenommer()));

	QShortcut *shortcutDelete = new QShortcut(QKeySequence("Suppr"), this);
		connect(shortcutDelete, SIGNAL(activated()), this, SLOT(menuSupprimer()));

	QShortcut *shortcutNewFolder = new QShortcut(QKeySequence("Ctrl+N"), this);
		connect(shortcutNewFolder, SIGNAL(activated()), this, SLOT(menuCreerDossier()));

	QShortcut *shortcutNewFile = new QShortcut(QKeySequence("Ctrl+Shift+N"), this);
		connect(shortcutNewFile, SIGNAL(activated()), this, SLOT(menuCreerFichier()));

	QShortcut *shortcutProperties = new QShortcut(QKeySequence("Alt+Return"), this);
		connect(shortcutProperties, SIGNAL(activated()), this, SLOT(menuProperties()));
}

void VueDossier::lister()
{
	emit debutChargement();

	m_vue->clear();
	m_vue->setDisabled(true);
	m_vue->setCursor(Qt::WaitCursor);

	allItems.clear();

	loadProgress->show(); // Showing progress bar

	QStringList fichiers;

	if (afficherDossiersCaches)
       		fichiers = QDir(m_chemin).entryList(QDir::Dirs | QDir::Files | QDir::Hidden);

	else
       		fichiers = QDir(m_chemin).entryList(QDir::Dirs | QDir::Files);

		fichiers.removeOne(".");
		fichiers.removeOne("..");

	loadProgress->setRange(0, fichiers.size()); // Set the maximum to the files count

	for (int i = 0; i < fichiers.size(); i++)
	{
		QCoreApplication::processEvents();

		loadProgress->setValue(i); // Updating progress bar

		QFileInfo infosFichier(chemin() + fichiers.value(i));

		QString type = "Dossier";

		if (infosFichier.isFile())
			type = "Fichier " + infosFichier.suffix().toUpper();

		QBrush brush(Qt::black);

		if (infosFichier.isHidden())
			brush.setColor(Qt::darkGray);
	
		QString taille = "";

		if (type != "Dossier")
		       	taille = Multiuso::toSize(infosFichier.size());

		else
			taille = Multiuso::toSize(folderSize(QDir(infosFichier.canonicalFilePath())));

		QString name = infosFichier.fileName();

		if (name.length() > 45)
			name = name.left(42) + "...";
		
		for (int i = 0; i < name.length(); i++)
		{
			if (i == 15 || i == 30)
			{
				name.insert(i, "\n");
				i++;
			}
		}

		ListWidgetItem *newItem = new ListWidgetItem(name);
			newItem->setTextAlignment(Qt::AlignCenter);
			newItem->setForeground(brush);
			newItem->setName(infosFichier.fileName());
			newItem->setIcon(Multiuso::iconForFile(infosFichier.fileName(), type));
			newItem->setSize(taille);
			newItem->setType(type);
			newItem->setPath(chemin());
			newItem->setLastModified(infosFichier.lastModified().toString());

			newItem->setToolTip("Nom : " + infosFichier.fileName() + "\n"
					+ "Type : " + type + "\n"
					+ "Taille : " + taille + "\n"
					+ "Dernières modifications : " + infosFichier.lastModified().toString());

		allItems << newItem;
	}

	foreach (ListWidgetItem *item, allItems)
	{
		QCoreApplication::processEvents();

		if (item->type().startsWith("Dossier"))
			m_vue->addItem(item);
	}

	foreach (ListWidgetItem *item, allItems)
	{
		QCoreApplication::processEvents();

		if (!item->type().startsWith("Dossier"))
			m_vue->addItem(item);
	}
	
	m_vue->setDisabled(false);
	m_vue->setCursor(Qt::ArrowCursor);

	loadProgress->hide(); // Hidding progress bar

	emit finChargement();

	m_historique << chemin();

	if (modifierPosition)
		position = m_historique.size() - 1;
}

int VueDossier::folderSize(QDir dir)
{
	int folderFilesSize = 0;

	foreach (QFileInfo info, dir.entryInfoList())
	{
		if (!info.isDir())
			folderFilesSize += info.size();
	}

	return folderFilesSize;
}

QString VueDossier::precedent()
{
	if (position > 0)
		position--;

	return m_historique.value(position);
}

QString VueDossier::suivant()
{
	if (position < m_historique.size() - 1)
		position++;

	return m_historique.value(position);
}

void VueDossier::setModifierPosition(bool choix)
{
	modifierPosition = choix;
}

void VueDossier::setAfficherDossiersCaches(bool choix)
{
	afficherDossiersCaches = choix;
}

bool VueDossier::isAfficherDossiersCaches()
{
	return afficherDossiersCaches;
}

void VueDossier::ouvrir(QListWidgetItem *item)
{
	ListWidgetItem *clickedItem = static_cast<ListWidgetItem *>(item);

	if (clickedItem == 0)
		return;

	QString nom = clickedItem->name();
	QString type = clickedItem->type();

	if (type == "Dossier" || type == "Disque")
	{
		setChemin(chemin() + nom);
		lister();
		emit demandeUpdate();
	}

	else if (type.startsWith("Fichier"))
	{
		emit openFileRequested(chemin() + nom);
	}
}

void VueDossier::ouvrirMenu(QPoint)
{
	ListWidgetItem *item = static_cast<ListWidgetItem *>(m_vue->currentItem());

	if (item != 0)
	{
		QString type = item->type();

		if (type == "Disque")
			return;
	}

	QMenu menu(this);

	if (item != 0)
	{
		QAction *supprimer = new QAction("Supprimer", this);
			supprimer->setIcon(QIcon(":/icones/nav_fichiers/supprimer.png"));
			connect(supprimer, SIGNAL(triggered()), this, SLOT(menuSupprimer()));
			menu.addAction(supprimer);

		QAction *renommer = new QAction("Renommer", this);
			renommer->setIcon(QIcon(":/icones/nav_fichiers/renommer.png"));
			connect(renommer, SIGNAL(triggered()), this, SLOT(menuRenommer()));
			menu.addAction(renommer);


		menu.addSeparator();


		QAction *copy = new QAction("Copier", this);
			connect(copy, SIGNAL(triggered()), this, SLOT(menuCopier()));
			menu.addAction(copy);

		QAction *cut = new QAction("Couper", this);
			connect(cut, SIGNAL(triggered()), this, SLOT(menuCouper()));
			menu.addAction(cut);
	}
		QAction *paste = new QAction("Coller", this);
			connect(paste, SIGNAL(triggered()), this, SLOT(menuColler()));
			menu.addAction(paste);
	

		menu.addSeparator();


		QAction *creerDossier = new QAction("Créer un dossier", this);
			creerDossier->setIcon(QIcon(":/icones/nav_fichiers/creerDossier.png"));
			connect(creerDossier, SIGNAL(triggered()), this, SLOT(menuCreerDossier()));
			menu.addAction(creerDossier);

		QAction *creerFichier = new QAction("Créer un fichier", this);
			creerFichier->setIcon(QIcon(":/icones/nav_fichiers/creerFichier.png"));
			connect(creerFichier, SIGNAL(triggered()), this, SLOT(menuCreerFichier()));
			menu.addAction(creerFichier);

	if (item != 0)
	{
		menu.addSeparator();


		QAction *compresserFichier = new QAction("Compresser...", this);
			connect(compresserFichier, SIGNAL(triggered()), this, SLOT(menuCompresserFichier()));
			menu.addAction(compresserFichier);

		if (item->name().endsWith(".zip"))
		{
			QAction *extraireFichier = new QAction("Extraire ici", this);
			connect(extraireFichier, SIGNAL(triggered()), this, SLOT(menuExtraireFichier()));
			menu.addAction(extraireFichier);
		}


		menu.addSeparator();


		QAction *properties = new QAction("Propriétés", this);
			properties->setIcon(QIcon(":/icones/nav_fichiers/properties.png"));
			connect(properties, SIGNAL(triggered()), this, SLOT(menuProperties()));
			menu.addAction(properties);
	}

	menu.exec(QCursor::pos());
}

void VueDossier::menuSupprimer()
{
	ListWidgetItem *item = static_cast<ListWidgetItem *>(m_vue->currentItem());

	if (item == 0)
		return;

	QFile fichier(chemin() + item->name());

	if (QFileInfo(fichier).fileName().isEmpty())
		return;

	int reponse = QMessageBox::warning(this, "Multiuso", "Souhaitez-vous vraiment supprimer définitivement « " +
		QFileInfo(fichier).fileName()  + " » ?", QMessageBox::Yes | QMessageBox::No);

	if (reponse == QMessageBox::No)
		return;

	if (QFileInfo(fichier).isFile())
		fichier.remove();

	else if (QFileInfo(fichier).isDir())
		Multiuso::removeDirectory(QFileInfo(fichier).absoluteFilePath());

	lister();
}

void VueDossier::menuRenommer()
{
	ListWidgetItem *item = static_cast<ListWidgetItem *>(m_vue->currentItem());

	if (item == 0)
		return;

	QString nouveauNom = QInputDialog::getText(this, "Multiuso", "Veuillez saisir le nouveau nom de « " +
			Multiuso::htmlspecialchars(item->name()) + " » :",
			QLineEdit::Normal, item->name());

	if (!nouveauNom.isEmpty())
	{
		QFile::rename(chemin() + item->name(), chemin() + nouveauNom);

		lister();
	}
}

void VueDossier::menuCouper()
{
	ListWidgetItem *item = static_cast<ListWidgetItem *>(m_vue->currentItem());

	if (item == 0)
		return;

	copyCutObject->setCurrentAction(CUT);
	copyCutObject->setFileLink(Multiuso::addSlash(item->path()) + item->name());
	copyCutObject->setFileType(item->type());
	
	m_parent->setCopyCutObject(copyCutObject);
}

void VueDossier::menuCopier()
{
	ListWidgetItem *item = static_cast<ListWidgetItem *>(m_vue->currentItem());

	if (item == 0)
		return;

	copyCutObject->setCurrentAction(COPY);
	copyCutObject->setFileLink(Multiuso::addSlash(item->path()) + item->name());
	copyCutObject->setFileType(item->type());
	
	m_parent->setCopyCutObject(copyCutObject);
}

void VueDossier::menuColler()
{
	copyCutObject->setCurrentAction(m_parent->copyCutObject()->currentAction());
	copyCutObject->setFileLink(m_parent->copyCutObject()->fileLink());
	copyCutObject->setFileType(m_parent->copyCutObject()->fileType());

	if (copyCutObject->currentAction() == COPY)
	{
		if (copyCutObject->fileType() == "Dossier")
		{
			QFile file(copyCutObject->fileLink());

			QFile tmpFile(Multiuso::addSlash(chemin()) + QFileInfo(file).fileName());
			
			if (QFileInfo(file).absoluteFilePath() == QFileInfo(tmpFile).absoluteFilePath())
				return;

			if (!tmpFile.exists())
			{
				if (!Multiuso::copyDirectory(copyCutObject->fileLink(), chemin()))
					QMessageBox::critical(this, "Multiuso", "Erreur lors de la copie !");
			}

			else
			{
				int answer = QMessageBox::warning(this, "Multiuso", "Le dossier « " + QFileInfo(file).fileName() + " » existe déjà."
						+ "<br />Voulez-vous le remplaçer ?", QMessageBox::Yes | QMessageBox::No);

				if (answer == QMessageBox::Yes)
				{
					Multiuso::removeDirectory(Multiuso::addSlash(chemin()) + QFileInfo(file).fileName());

					if (!Multiuso::copyDirectory(copyCutObject->fileLink(), chemin()))
						QMessageBox::critical(this, "Multiuso", "Erreur lors de la copie !");
				}
			}
		}

		else
		{
			QFile file(copyCutObject->fileLink());

			QFile tmpFile(Multiuso::addSlash(chemin()) + QFileInfo(file).fileName());

			if (QFileInfo(file).absoluteFilePath() == QFileInfo(tmpFile).absoluteFilePath())
				return;

			if (!tmpFile.exists())
			{
				if (!file.copy(Multiuso::addSlash(chemin()) + QFileInfo(file).fileName()))
					QMessageBox::critical(this, "Multiuso", "Erreur lors de la copie !");
			}

			else
			{
				int answer = QMessageBox::warning(this, "Multiuso", "Le fichier « " + QFileInfo(file).fileName() + " » existe déjà."
						+ "<br />Voulez-vous le remplaçer ?", QMessageBox::Yes | QMessageBox::No);

				if (answer == QMessageBox::Yes)
				{
					tmpFile.remove();

					if (!file.copy(Multiuso::addSlash(chemin()) + QFileInfo(file).fileName()))
						QMessageBox::critical(this, "Multiuso", "Erreur lors de la copie !");
				}
			}
		}
	}

	else if (copyCutObject->currentAction() == CUT)
	{
		if (copyCutObject->fileType() == "Dossier")
		{
			QFile file(copyCutObject->fileLink());

			QFile tmpFile(Multiuso::addSlash(chemin()) + QFileInfo(file).fileName());
			
			if (QFileInfo(file).absoluteFilePath() == QFileInfo(tmpFile).absoluteFilePath())
				return;

			if (!tmpFile.exists())
			{
				if (!Multiuso::copyDirectory(copyCutObject->fileLink(), chemin()))
					QMessageBox::critical(this, "Multiuso", "Erreur lors de la copie !");
	
				else
					Multiuso::removeDirectory(copyCutObject->fileLink());
			}

			else
			{
				int answer = QMessageBox::warning(this, "Multiuso", "Le dossier « " + QFileInfo(file).fileName() + " » existe déjà."
						+ "<br />Voulez-vous le remplaçer ?", QMessageBox::Yes | QMessageBox::No);

				if (answer == QMessageBox::Yes)
				{
					Multiuso::removeDirectory(Multiuso::addSlash(chemin()) + QFileInfo(file).fileName());

					if (!Multiuso::copyDirectory(copyCutObject->fileLink(), chemin()))
						QMessageBox::critical(this, "Multiuso", "Erreur lors de la copie !");

					else
						Multiuso::removeDirectory(copyCutObject->fileLink());
				}
			}
		}

		else
		{
			QFile file(copyCutObject->fileLink());

			QFile tmpFile(Multiuso::addSlash(chemin()) + QFileInfo(file).fileName());

			if (QFileInfo(file).absoluteFilePath() == QFileInfo(tmpFile).absoluteFilePath())
				return;

			if (!tmpFile.exists())
			{
				if (!file.copy(Multiuso::addSlash(chemin()) + QFileInfo(file).fileName()))
					QMessageBox::critical(this, "Multiuso", "Erreur lors du déplacement !");

				else
					file.remove();
			}

			else
			{
				int answer = QMessageBox::warning(this, "Multiuso", "Le fichier « " + QFileInfo(file).fileName() + " » existe déjà."
						+ "<br />Voulez-vous le remplaçer ?", QMessageBox::Yes | QMessageBox::No);

				if (answer == QMessageBox::Yes)
				{
					tmpFile.remove();

					if (!file.copy(Multiuso::addSlash(chemin()) + QFileInfo(file).fileName()))
						QMessageBox::critical(this, "Multiuso", "Erreur lors du déplacement !");

					else
						file.remove();
				}
			}

		}
	}

	else
	{
		return;
	}

	lister();
}

void VueDossier::menuCreerDossier()
{
	QString dossier = QInputDialog::getText(this, "Multiuso", "Entrez le nom du nouveau dossier :");

	if (!dossier.isEmpty())
	{
		QDir dir;
			dir.mkpath(chemin() + dossier);

		lister();
	}
}

void VueDossier::menuCreerFichier()
{
	QString fichier = QInputDialog::getText(this, "Multiuso", "Entrez le nom du nouveau fichier (nom + extension) :");

	if (!fichier.isEmpty())
	{
		QFile newFile(chemin() + fichier);
			newFile.open(QIODevice::WriteOnly | QIODevice::Text);
			newFile.write(QString("").toAscii());
			newFile.close();

		lister();
	}
}

void VueDossier::menuCompresserFichier()
{
	ListWidgetItem *item = static_cast<ListWidgetItem *>(m_vue->currentItem());

	if (item == 0)
		return;

	QString name = QInputDialog::getText(this, "Multiuso", "Entrez le nom de l'archive :",
						QLineEdit::Normal, QFileInfo(item->name()).baseName());

	if (name.isEmpty())
		return;

	QString zippedFile = Multiuso::addSlash(item->path()) + name + ".zip";

	QStringList files;
		files << Multiuso::addSlash(item->path()) + item->name();

	if (!Multiuso::zip(zippedFile, files))
		QMessageBox::critical(this, "Multiuso", "Erreur lors de la compression !");

	lister();
}

void VueDossier::menuExtraireFichier()
{
	ListWidgetItem *item = static_cast<ListWidgetItem *>(m_vue->currentItem());

	if (item == 0)
		return;

	if (!Multiuso::unzip(Multiuso::addSlash(item->path()) + item->name(), chemin()))
	{
		QMessageBox::critical(this, "Multiuso", "Impossible d'extraire l'archive !");

		return;
	}

	lister();
}

void VueDossier::menuProperties()
{
	ListWidgetItem *item = static_cast<ListWidgetItem *>(m_vue->currentItem());

	if (item == 0)
		return;

	QIcon icon = Multiuso::iconForFile(Multiuso::addSlash(item->path()) + item->name(), item->type());

	QLabel *pixmap = new QLabel;
		pixmap->setPixmap(icon.pixmap(icon.actualSize(QSize(42, 42))));

	QLabel *label = new QLabel("Nom : " + item->name() + "\n"
					+ "Type : " + item->type() + "\n"
					+ "Taille : " + item->size() + "\n"
					+ "\n"
					+ "Emplacement : " + item->path() + "\n"
					+ "\n"
					+ "Dernières modifications : " + item->lastModified());

	QHBoxLayout *layout = new QHBoxLayout;
		layout->addWidget(pixmap);
		layout->addWidget(label);

	QDialog *dialog = new QDialog(this);
		dialog->setWindowTitle("Propriétés de « " + item->name() + " »");
		dialog->setWindowIcon(QIcon(":/icones/nav_fichiers/properties.png"));

			QVBoxLayout *mainLayout = new QVBoxLayout;
				mainLayout->addLayout(layout);
				mainLayout->addWidget(Multiuso::closeButton(dialog));

		dialog->setLayout(mainLayout);
		dialog->exec();
		dialog->deleteLater();
}

void VueDossier::setChemin(QString chemin)
{
	m_chemin = chemin;

	if (!m_chemin.endsWith("/"))
		m_chemin += "/";
}

void VueDossier::moveFile(QString file, QPoint pos)
{
	qDebug() << "jk";
	QListWidgetItem *itemAtPos = m_vue->itemAt(pos);

	if (!itemAtPos)
		return;

	m_vue->setCurrentItem(itemAtPos);

	ListWidgetItem *item = static_cast<ListWidgetItem *>(m_vue->currentItem());

	if (item == 0)
		return;

	if (item->type() != "Dossier")
		return;
}

QString VueDossier::chemin()
{
	return m_chemin;
}
