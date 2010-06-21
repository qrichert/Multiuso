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

#include "Notes.h"

Notes::Notes(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Notes");
	setWindowIcon(QIcon(":/icones/actions/actionNotes.png"));

	resize(Multiuso::screenWidth() / 2, Multiuso::screenHeight() / 2);

	notes = new QDirModel;

	vueNotes = new QTreeView;
		vueNotes->setModel(notes);
		vueNotes->hideColumn(1);
		vueNotes->hideColumn(2);
		vueNotes->hideColumn(3);
		vueNotes->hideColumn(4);
		vueNotes->setRootIndex(notes->index(Multiuso::appDirPath() + "/textes/notes"));
		connect(vueNotes, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slotChangementItem(const QModelIndex &)));

	noteActuelle = new QTextEdit;
		noteActuelle->setDisabled(true);
		connect(noteActuelle, SIGNAL(textChanged()), this, SLOT(slotChangementDeTexte()));

	QSplitter *splitter = new QSplitter(Qt::Horizontal);
		splitter->addWidget(vueNotes);
		splitter->addWidget(noteActuelle);

	QHBoxLayout *layoutNotes = new QHBoxLayout;
		layoutNotes->addWidget(splitter);
		layoutNotes->setContentsMargins(0, 0, 0, 0);

	QWidget *widgetNotes = new QWidget;
		widgetNotes->setLayout(layoutNotes);

	contenu = new QMainWindow;
		contenu->setCentralWidget(widgetNotes);

	QAction *actionNouvelleNote = new QAction("Nouvelle note", this);
		actionNouvelleNote->setIcon(QIcon(":/icones/notes/actionNouvelleNote.png"));
		connect(actionNouvelleNote, SIGNAL(triggered()), this, SLOT(slotNouvelleNote()));

	QAction *actionRenommer = new QAction("Renommer", this);
		actionRenommer->setIcon(QIcon(":/icones/notes/actionRenommer.png"));
		connect(actionRenommer, SIGNAL(triggered()), this, SLOT(slotRenommer()));

	QAction *actionSupprimer = new QAction("Supprimer", this);
		actionSupprimer->setIcon(QIcon(":/icones/notes/actionSupprimer.png"));
		connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(slotSupprimer()));

	QToolBar *toolBarOptions = contenu->addToolBar("Options");
		toolBarOptions->addAction(actionNouvelleNote);
		toolBarOptions->addAction(actionRenommer);
		toolBarOptions->addAction(actionSupprimer);
		toolBarOptions->setMovable(false);
		toolBarOptions->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	fichierOuvert = new QLabel("Fichier ouvert : Pas de fichier ouvert");

	statut = contenu->statusBar();
		statut->addPermanentWidget(fichierOuvert);

	QVBoxLayout *layoutContenu = new QVBoxLayout(this);
		layoutContenu->addWidget(contenu);
		layoutContenu->setContentsMargins(0, 0, 0, 0);

	noteOuverte = "";
	premiereFois = true;
}

void Notes::slotChangementItem(const QModelIndex &index)
{
	QString itemSelectionne = index.data().toString();

	QFile fichierNote(Multiuso::appDirPath() + "/textes/notes/" + itemSelectionne);

	if (fichierNote.exists() && fichierNote.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		noteOuverte.clear();

		QString contenuDeLaNote = fichierNote.readAll();

		noteActuelle->setPlainText(contenuDeLaNote);

		QFileInfo infosNote(fichierNote);

		noteOuverte = infosNote.absoluteFilePath();

		premiereFois = true;

		fichierOuvert->setText("Fichier ouvert : " + infosNote.baseName());

		noteActuelle->setDisabled(false);
	}

	fichierNote.close();

	notes->refresh();
}

void Notes::slotChangementDeTexte()
{
	if (noteOuverte.isEmpty())
		return;

	QFile enregistrer(noteOuverte);

	if (enregistrer.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		enregistrer.write(noteActuelle->toPlainText().toAscii());
		premiereFois = false;
	}

	enregistrer.close();

	notes->refresh();
}

void Notes::slotNouvelleNote()
{
	QString titre = QInputDialog::getText(this, "Multiuso", "Entrez le titre de la nouvelle note :");

	if (!titre.isEmpty())
	{
		titre.replace(QRegExp("/{1,}"), "/");
		titre.replace("/", "_");

		QFile fichierNote(Multiuso::appDirPath() + "/textes/notes/" + titre + ".mltsnotes");

		if (!fichierNote.exists() && fichierNote.open(QIODevice::WriteOnly | QIODevice::Text))
			fichierNote.write(QString("").toAscii());

		else
			statut->showMessage("La note n'a pas pu être créée", 3000);

		fichierNote.close();
	}

	notes->refresh();
}

void Notes::slotRenommer()
{
	if (notes->filePath(vueNotes->currentIndex()).isEmpty())
		return;

	QFile fichier(notes->filePath(vueNotes->currentIndex()));

	QFileInfo infosFichier(fichier);

	QString nouveauNom = QInputDialog::getText(this, "Multiuso", "Entrez le nouveau nom de \"" + infosFichier.baseName() + "\" :");

	if (!nouveauNom.isEmpty())
	{
		nouveauNom.replace(QRegExp("/{1,}"), "/");
		nouveauNom.replace("/", "_");

		fichier.rename(Multiuso::appDirPath() + "/textes/notes/" + nouveauNom + ".mltsnotes");

		fichier.open(QIODevice::ReadOnly | QIODevice::Text);

			QString texte = fichier.readAll();

		fichier.close();

		QFileInfo nouvellesInfosFichier(fichier);

		noteOuverte = nouvellesInfosFichier.filePath();
		noteActuelle->setPlainText(texte);
		fichierOuvert->setText("Fichier ouvert : " + nouvellesInfosFichier.baseName());
	}

	notes->refresh();
}

void Notes::slotSupprimer()
{
	QString itemSelectionne = notes->filePath(vueNotes->currentIndex());

	if (itemSelectionne.isEmpty())
		return;

	QFile supprimer(itemSelectionne);
		supprimer.remove();

	fichierOuvert->setText("Fichier ouvert : Pas de fichier ouvert");

	noteOuverte.clear();
	noteActuelle->clear();

	notes->refresh();
}
