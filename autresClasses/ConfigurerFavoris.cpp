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

#include "ConfigurerFavoris.h"

ConfigurerFavoris::ConfigurerFavoris(NavigateurWeb *parent = 0)
{
	setWindowTitle("Navigateur Web - Configurer les favoris");
	setWindowIcon(QIcon(":/icones/navigateur_web/ajouterFavori.png"));

	pointeurSurParent = parent;

	listeFavoris = new QListWidget;

	QAction *actionRenommer = new QAction("Renommer", this);
		actionRenommer->setIcon(QIcon(":/icones/navigateur_web/renommer.png"));
		connect(actionRenommer, SIGNAL(triggered()), this, SLOT(slotRenommerFavori()));

	QAction *actionModifier = new QAction("Modifier", this);
		actionModifier->setIcon(QIcon(":/icones/navigateur_web/modifier.png"));
		connect(actionModifier, SIGNAL(triggered()), this, SLOT(slotModifierFavori()));

	QAction *actionSupprimer = new QAction("Supprimer", this);
		actionSupprimer->setIcon(QIcon(":/icones/navigateur_web/supprimer.png"));
		connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(slotSupprimerFavori()));

	QToolBar *toolBar = new QToolBar("Actions");
		toolBar->setMovable(false);
		toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		toolBar->addAction(actionRenommer);
		toolBar->addAction(actionModifier);
		toolBar->addAction(actionSupprimer);

	listerFavoris();

	QMainWindow *widgetCentral = new QMainWindow;
		widgetCentral->setCentralWidget(listeFavoris);
		widgetCentral->addToolBar(Qt::RightToolBarArea, toolBar);

	QGridLayout *layout = new QGridLayout(this);
		layout->addWidget(widgetCentral, 0, 0, 1, 1);
		layout->addWidget(Multiuso::closeButton(this), 1, 0, 1, 1);
}

void ConfigurerFavoris::listerFavoris()
{
	listeFavoris->clear();
	
	QDir dir(Multiuso::appDirPath() + "/navigateurWeb/favoris");

	QStringList favoris = dir.entryList(QDir::Files);

	for (int i = 0; i <= favoris.size(); i++)
	{
		QFile favori(Multiuso::appDirPath() + "/navigateurWeb/favoris/" + favoris.value(i));

		if (favori.exists() && favori.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QFileInfo infos(favori);

			QString nom = infos.baseName();
			QString nomCourt = nom;
			QString lien = favori.readLine();

			if (nom.size() > 20)
				nomCourt = nom.left(17) + "...";

			nom.replace("&", "&&");
			nomCourt.replace("&", "&&");

			QIcon favicon = pointeurSurParent->faviconUrl(lien);

			if (favicon.isNull())
				favicon.addFile(":/icones/autre/iconeVide.png");

			QListWidgetItem *item = new QListWidgetItem(nomCourt);
				item->setIcon(favicon);
				item->setStatusTip(lien);
				item->setToolTip(nom);

			listeFavoris->addItem(item);
		}

		favori.close();
	}
}

void ConfigurerFavoris::slotRenommerFavori()
{
	QListWidgetItem *item = listeFavoris->currentItem();
	
	QString titre = item->toolTip();

	QString newName = QInputDialog::getText(this, "Multiuso", "Veuillez saisir le nouveau nom de « " +
				Multiuso::htmlspecialchars(titre) + " » :",
				QLineEdit::Normal, titre);

	if (!newName.isEmpty())
	{
		QFile file(Multiuso::appDirPath() + "/navigateurWeb/favoris/" + titre + ".mltsbookmark");
			file.rename(Multiuso::appDirPath() + "/navigateurWeb/favoris/" + newName + ".mltsbookmark");

		listerFavoris();
		pointeurSurParent->actualiserFavoris();
	}
}

void ConfigurerFavoris::slotModifierFavori()
{
	QListWidgetItem *item = listeFavoris->currentItem();
	
	QString titre = item->toolTip();
	QString link = item->statusTip();

	QString newLink = QInputDialog::getText(this, "Multiuso", "Veuillez saisir le nouveau lien pour « " +
				titre.replace("&&", "&") + " » :",
				QLineEdit::Normal, link);

	if (!newLink.isEmpty())
	{
		QFile file(Multiuso::appDirPath() + "/navigateurWeb/favoris/" + titre + ".mltsbookmark");
			file.open(QIODevice::WriteOnly | QIODevice::Text);
			file.write(newLink.toAscii());
			file.close();

		listerFavoris();
		pointeurSurParent->actualiserFavoris();
	}
}

void ConfigurerFavoris::slotSupprimerFavori()
{
	QListWidgetItem *item = listeFavoris->currentItem();

	QString titre = item->toolTip();
	
	int reponse = QMessageBox::question(this, "Multiuso", "Voulez-vous supprimer ce favori ?<br />"
				"<em>« " + titre  + " »</em>", QMessageBox::Yes | QMessageBox::No);

	if (reponse == QMessageBox::Yes)
	{
		QFile::remove(Multiuso::appDirPath() + "/navigateurWeb/favoris/" + titre + ".mltsbookmark");

		listerFavoris();
		pointeurSurParent->actualiserFavoris();
	}
}

