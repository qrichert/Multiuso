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
	QStringList entetes;
		entetes << "Nom" << "Taille" << "Type" << "Dernières modifications";

	m_modele = new QStandardItemModel(0, entetes.size());
		m_modele->setHorizontalHeaderLabels(entetes);

	m_vue = new QTableView;
		m_vue->setModel(m_modele);
		m_vue->setShowGrid(false);
		m_vue->setContextMenuPolicy(Qt::CustomContextMenu);
		m_vue->verticalHeader()->hide();
		m_vue->horizontalHeader()->setStretchLastSection(true);
		m_vue->setSelectionBehavior(QAbstractItemView::SelectRows);
		m_vue->setSelectionMode(QAbstractItemView::SingleSelection);
		connect(m_vue, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ouvrir(QModelIndex)));
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

	m_modele->removeRows(0, m_modele->rowCount());

	QStringList fichiers;

	if (afficherDossiersCaches)
       		fichiers = QDir(m_chemin).entryList(QDir::Dirs | QDir::Files | QDir::Hidden);

	else
       		fichiers = QDir(m_chemin).entryList(QDir::Dirs | QDir::Files);

		fichiers.removeOne(".");
		fichiers.removeOne("..");

	for (int i = 0; i < fichiers.size(); i++)
	{
		QFileInfo infosFichier(chemin() + fichiers.value(i));

		QString type = "Dossier";

		if (infosFichier.isFile())
			type = "Fichier " + infosFichier.suffix().toUpper();

		QBrush brush(Qt::black);

		if (infosFichier.isHidden())
			brush.setColor(Qt::darkGray);

		QStandardItem *itemNom = new QStandardItem(infosFichier.fileName());
			itemNom->setIcon(Multiuso::iconForFile(infosFichier.fileName(), type));
			itemNom->setEditable(false);
			itemNom->setForeground(brush);

		QString taille = "";

		if (type != "Dossier")
		       	taille = QString::number(infosFichier.size()) + " octets";

		else
			taille = QString::number(folderSize(QDir(infosFichier.canonicalFilePath()))) + " octets";

		QStandardItem *itemTaille = new QStandardItem(taille);
			itemTaille->setEditable(false);
			itemTaille->setForeground(brush);

		QStandardItem *itemType = new QStandardItem(type);
			itemType->setEditable(false);
			itemType->setForeground(brush);

		QStandardItem *itemDernieresModifications = new QStandardItem(infosFichier.lastModified().toString());
			itemDernieresModifications->setEditable(false);
			itemDernieresModifications->setForeground(brush);

		QList<QStandardItem *> nouveauxItems;
			nouveauxItems << itemNom << itemTaille << itemType << itemDernieresModifications;

		m_modele->appendRow(nouveauxItems);

		m_vue->resizeColumnsToContents();
		m_vue->horizontalHeader()->setStretchLastSection(true);
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

void VueDossier::vider()
{
	m_modele->removeRows(0, m_modele->rowCount());
}

void VueDossier::ajouterListe(QList<QStandardItem *> items)
{
	m_modele->appendRow(items);

	m_vue->resizeColumnsToContents();
	m_vue->horizontalHeader()->setStretchLastSection(true);
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

void VueDossier::ouvrir(QModelIndex)
{
	QModelIndex index;
		index = m_modele->sibling(m_vue->currentIndex().row(), 2, index);

	QString type = index.data().toString();

		index = m_modele->sibling(m_vue->currentIndex().row(), 0, index);

	QString nom = index.data().toString();

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
	QModelIndex index;
		index = m_modele->sibling(m_vue->currentIndex().row(), 2, index);

	QString type = index.data().toString();

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
	QModelIndex index;
		index = m_modele->sibling(m_vue->currentIndex().row(), 0, index);

	QFile fichier(chemin() + index.data().toString());

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
	QModelIndex index;
		index = m_modele->sibling(m_vue->currentIndex().row(), 0, index);

	if (index.data().toString().isEmpty())
		return;

	QString nouveauNom = QInputDialog::getText(this, "Multiuso", "Veuillez saisir le nouveau nom de « " +
			Multiuso::htmlspecialchars(index.data().toString()) + " » :",
			QLineEdit::Normal, index.data().toString());

	if (!nouveauNom.isEmpty())
	{
		QFile::rename(chemin() + index.data().toString(), chemin() + nouveauNom);

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
