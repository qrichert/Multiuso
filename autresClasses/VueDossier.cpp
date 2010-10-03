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

VueDossier::VueDossier()
{
	m_vue = new QListWidget;
		m_vue->setContextMenuPolicy(Qt::CustomContextMenu);
		m_vue->setViewMode(QListView::IconMode);
		m_vue->setIconSize(QSize(50, 50));
		m_vue->setResizeMode(QListView::Adjust);
		m_vue->setMovement(QListView::Snap);
		m_vue->setGridSize(QSize(135, 100));
		connect(m_vue, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(ouvrir(QListWidgetItem *)));
		connect(m_vue, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ouvrirMenu(QPoint)));

	QVBoxLayout *layout = new QVBoxLayout(this);
		layout->addWidget(m_vue);
		layout->setContentsMargins(0, 0, 0, 0);

	m_chemin = "";
	modifierPosition = true;
	afficherDossiersCaches = false;

	QShortcut *shortcutRename = new QShortcut(QKeySequence("F2"), this);
		connect(shortcutRename, SIGNAL(activated()), this, SLOT(menuRenommer()));

	QShortcut *shortcutDelete = new QShortcut(QKeySequence("Suppr"), this);
		connect(shortcutDelete, SIGNAL(activated()), this, SLOT(menuSupprimer()));

	QShortcut *shortcutNewFolder = new QShortcut(QKeySequence("Ctrl+N"), this);
		connect(shortcutNewFolder, SIGNAL(activated()), this, SLOT(menuCreerDossier()));

	QShortcut *shortcutNewFile = new QShortcut(QKeySequence("Ctrl+Shift+N"), this);
		connect(shortcutNewFile, SIGNAL(activated()), this, SLOT(menuCreerFichier()));
}

void VueDossier::lister()
{
	emit debutChargement();

	m_vue->clear();

	QStringList fichiers;

	if (afficherDossiersCaches)
       		fichiers = QDir(m_chemin).entryList(QDir::Dirs | QDir::Files | QDir::Hidden);

	else
       		fichiers = QDir(m_chemin).entryList(QDir::Dirs | QDir::Files);

		fichiers.removeOne(".");
		fichiers.removeOne("..");

	for (int i = 0; i < fichiers.size(); i++)
	{
		QCoreApplication::processEvents();

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
			newItem->setName(infosFichier.fileName());
			newItem->setIcon(Multiuso::iconForFile(infosFichier.fileName(), type));
			newItem->setSize(taille);
			newItem->setType(type);
			newItem->setLastModified(infosFichier.lastModified().toString());

			newItem->setToolTip("Nom : " + infosFichier.fileName() + "\n"
					+ "Type : " + type + "\n"
					+ "Taille : " + taille + "\n"
					+ "Dernières modifications : " + infosFichier.lastModified().toString());

		m_vue->addItem(newItem);
	}

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

	if (item == 0)
		return;

	QString type = item->type();

	if (type == "Disque")
		return;

	QMenu menu(this);

	QAction *supprimer = new QAction("Supprimer", this);
		supprimer->setIcon(QIcon(":/icones/nav_fichiers/supprimer.png"));
		connect(supprimer, SIGNAL(triggered()), this, SLOT(menuSupprimer()));
		menu.addAction(supprimer);

	QAction *renommer = new QAction("Renommer", this);
		renommer->setIcon(QIcon(":/icones/nav_fichiers/renommer.png"));
		connect(renommer, SIGNAL(triggered()), this, SLOT(menuRenommer()));
		menu.addAction(renommer);


	menu.addSeparator();


	QAction *creerDossier = new QAction("Créer un dossier", this);
		creerDossier->setIcon(QIcon(":/icones/nav_fichiers/creerDossier.png"));
		connect(creerDossier, SIGNAL(triggered()), this, SLOT(menuCreerDossier()));
		menu.addAction(creerDossier);

	QAction *creerFichier = new QAction("Créer un fichier", this);
		creerFichier->setIcon(QIcon(":/icones/nav_fichiers/creerFichier.png"));
		connect(creerFichier, SIGNAL(triggered()), this, SLOT(menuCreerFichier()));
		menu.addAction(creerFichier);

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

void VueDossier::setChemin(QString chemin)
{
	m_chemin = chemin;

	if (!m_chemin.endsWith("/"))
		m_chemin += "/";
}

QString VueDossier::chemin()
{
	return m_chemin;
}
