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

#include "NavFichiers.h"
#include "FenPrincipale.h"

NavFichiers::NavFichiers(QWidget *parent) : QMainWindow(parent)
{
	creerActions();

	QToolButton *buttonNewTab = new QToolButton;
		buttonNewTab->setDefaultAction(actionNouvelOnglet);
		buttonNewTab->setAutoRaise(true);

	QToolButton *buttonCloseTab = new QToolButton;
		buttonCloseTab->setDefaultAction(actionFermerOnglet);
		buttonCloseTab->setAutoRaise(true);

	onglets = new QTabWidget;
		onglets->setMovable(true);
		onglets->setDocumentMode(true);
		onglets->setCornerWidget(buttonNewTab, Qt::BottomLeftCorner);
		onglets->setCornerWidget(buttonCloseTab, Qt::BottomRightCorner);
		connect(onglets, SIGNAL(tabCloseRequested(int)), this, SLOT(slotFermerOnglet(int)));
		connect(onglets, SIGNAL(currentChanged(int)), this, SLOT(slotOngletChange(int)));
		ajouterOnglet();
		onglets->setTabsClosable(false);

	setCentralWidget(onglets);

	QSettings reglages(Multiuso::appDirPath() + "/ini/nav_fichiers.ini", QSettings::IniFormat);
		restoreState(reglages.value("etat_fenetre").toByteArray());

	QShortcut *shortcutSwitchTab = new QShortcut(QKeySequence("Ctrl+Tab"), this);
		connect(shortcutSwitchTab, SIGNAL(activated()), this, SLOT(slotSwitchTab()));

	p_fenPrincipale = parent;
	p_navigateurWeb = NULL;
}

void NavFichiers::creerActions()
{
	QToolBar *toolBar = addToolBar("Raccourcis");
		toolBar->setObjectName("Raccourcis");
		toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	actionPrecedent = new QAction("Précédent", this);
		actionPrecedent->setIcon(QIcon(":/icones/nav_fichiers/actionPrecedent.png"));
		connect(actionPrecedent, SIGNAL(triggered()), this, SLOT(slotPrecedent()));
			toolBar->addAction(actionPrecedent);

	actionSuivant = new QAction("Suivant", this);
		actionSuivant->setIcon(QIcon(":/icones/nav_fichiers/actionSuivant.png"));
		connect(actionSuivant, SIGNAL(triggered()), this, SLOT(slotSuivant()));
			toolBar->addAction(actionSuivant);

	actionHaut = new QAction("Haut", this);
		actionHaut->setIcon(QIcon(":/icones/nav_fichiers/actionHaut.png"));
		connect(actionHaut, SIGNAL(triggered()), this, SLOT(slotHaut()));
			toolBar->addAction(actionHaut);

	actionActualiser = new QAction("Actualiser", this);
		actionActualiser->setIcon(QIcon(":/icones/nav_fichiers/actionActualiser.png"));
		connect(actionActualiser, SIGNAL(triggered()), this, SLOT(slotActualiser()));
			toolBar->addAction(actionActualiser);


		toolBar->addSeparator();


	actionDossierPersonnel = new QAction("Dossier personnel", this);
		actionDossierPersonnel->setIcon(QIcon(":/icones/nav_fichiers/actionDossierPersonnel.png"));
		connect(actionDossierPersonnel, SIGNAL(triggered()), this, SLOT(slotDossierPersonnel()));
			toolBar->addAction(actionDossierPersonnel);

	actionDossierTelechargements = new QAction("Téléchargements", this);
		actionDossierTelechargements->setIcon(QIcon(":/icones/nav_fichiers/actionDossierTelechargements.png"));
		connect(actionDossierTelechargements, SIGNAL(triggered()), this, SLOT(slotDossierTelechargements()));
			toolBar->addAction(actionDossierTelechargements);

	actionDossiersCaches = new QAction("Éléments cachés", this);
		actionDossiersCaches->setIcon(QIcon(":/icones/nav_fichiers/actionDossiersCaches.png"));
		connect(actionDossiersCaches, SIGNAL(triggered()), this, SLOT(slotDossiersCaches()));
			toolBar->addAction(actionDossiersCaches);

	actionDossierRacine = new QAction("Racine", this);
		actionDossierRacine->setIcon(QIcon(":/icones/nav_fichiers/actionDossierRacine.png"));
		connect(actionDossierRacine, SIGNAL(triggered()), this, SLOT(slotDossierRacine()));
			toolBar->addAction(actionDossierRacine);

	actionDossierTmp = new QAction("Dossier temporaire", this);
		actionDossierTmp->setIcon(QIcon(":/icones/nav_fichiers/actionDossierTmp.png"));
		connect(actionDossierTmp, SIGNAL(triggered()), this, SLOT(slotDossierTmp()));
			toolBar->addAction(actionDossierTmp);


	addToolBarBreak();


	QToolBar *toolBar2 = addToolBar("Adresse");
		toolBar2->setObjectName("Adresse");

	afficherCheminActuel = new LineEdit;
		connect(afficherCheminActuel, SIGNAL(returnPressed()), this, SLOT(slotGo()));
		toolBar2->addWidget(afficherCheminActuel);

	actionGo = new QAction("Go", this);
		actionGo->setIcon(QIcon(":/icones/nav_fichiers/actionGo.png"));
		connect(actionGo, SIGNAL(triggered()), this, SLOT(slotGo()));
			toolBar2->addAction(actionGo);

	actionNouvelOnglet = new QAction("Nouvel onglet", this);
		actionNouvelOnglet->setIcon(QIcon(":/icones/nav_fichiers/actionNouvelOnglet.png"));
		actionNouvelOnglet->setShortcut(QKeySequence("Ctrl+T"));
		connect(actionNouvelOnglet, SIGNAL(triggered()), this, SLOT(ajouterOnglet()));

	actionFermerOnglet = new QAction("Fermer l'onglet", this);
		actionFermerOnglet->setIcon(QIcon(":/icones/nav_fichiers/actionFermerOnglet.png"));
		actionFermerOnglet->setShortcut(QKeySequence("Ctrl+W"));
		actionFermerOnglet->setEnabled(false);
		connect(actionFermerOnglet, SIGNAL(triggered()), this, SLOT(slotFermerOnglet()));
}

VueDossier *NavFichiers::pageActuelle()
{
	return onglets->currentWidget()->findChild<VueDossier *>();
}

void NavFichiers::setNavigateurWeb(NavigateurWeb *widget)
{
	p_navigateurWeb = widget;
}

void NavFichiers::ajouterOnglet()
{
	onglets->addTab(nouvelOnglet(), QDir::home().dirName());
	onglets->setCurrentIndex(onglets->count() - 1);

	if (onglets->count() > 1)
	{
		onglets->setTabsClosable(true);
		actionFermerOnglet->setEnabled(true);
	}

	afficherCheminActuel->setText(pageActuelle()->chemin());
}

QWidget *NavFichiers::nouvelOnglet()
{
	QSettings settings(Multiuso::appDirPath() + "/ini/nav_fichiers.ini", QSettings::IniFormat);

	QString welcomeFolder = settings.value("dossier_accueil").toString();

	if (!QDir(welcomeFolder).exists())
		welcomeFolder = QDir::homePath();

	VueDossier *vue = new VueDossier;
		connect(vue, SIGNAL(debutChargement()), this, SLOT(slotDebutChargement()));
		connect(vue, SIGNAL(finChargement()), this, SLOT(slotFinChargement()));
		connect(vue, SIGNAL(demandeUpdate()), this, SLOT(slotUpdateAffichage()));
		connect(vue, SIGNAL(openFileRequested(QString)), this, SLOT(slotOpenFile(QString)));
		vue->setChemin(welcomeFolder);
		vue->lister();

	QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(vue);
		layout->setContentsMargins(0, 0, 0, 0);

	QWidget *page = new QWidget;
		page->setLayout(layout);

	return page;
}

void NavFichiers::slotFermerOnglet()
{
	slotFermerOnglet(onglets->currentIndex());
}

void NavFichiers::slotFermerOnglet(int onglet)
{
	if (onglets->count() > 1)
	{
		onglets->widget(onglet)->deleteLater();
		onglets->removeTab(onglet);
	}

	if (onglets->count() <= 1)
	{
		onglets->setTabsClosable(false);
		actionFermerOnglet->setEnabled(false);
	}
}

void NavFichiers::slotOngletChange(int)
{
	afficherCheminActuel->setText(pageActuelle()->chemin());
}

void NavFichiers::slotDebutChargement()
{
	actionActualiser->setEnabled(false);
}

void NavFichiers::slotFinChargement()
{
	actionActualiser->setEnabled(true);
}

void NavFichiers::slotUpdateAffichage()
{
	afficherCheminActuel->setText(pageActuelle()->chemin());
	onglets->setTabText(onglets->currentIndex(), QDir(pageActuelle()->chemin()).dirName());
}

void NavFichiers::slotPrecedent()
{
	pageActuelle()->setChemin(pageActuelle()->precedent());
	pageActuelle()->setModifierPosition(false);
	pageActuelle()->lister();
	pageActuelle()->setModifierPosition(true);
	afficherCheminActuel->setText(pageActuelle()->chemin());
	onglets->setTabText(onglets->currentIndex(), QDir(pageActuelle()->chemin()).dirName());
}

void NavFichiers::slotSuivant()
{
	pageActuelle()->setChemin(pageActuelle()->suivant());
	pageActuelle()->setModifierPosition(false);
	pageActuelle()->lister();
	pageActuelle()->setModifierPosition(true);
	afficherCheminActuel->setText(pageActuelle()->chemin());
	onglets->setTabText(onglets->currentIndex(), QDir(pageActuelle()->chemin()).dirName());
}

void NavFichiers::slotHaut()
{
	QString chemin = afficherCheminActuel->text();

	QDir dir(chemin);

	if (dir.cdUp())
	{
		pageActuelle()->setChemin(dir.absolutePath());
		pageActuelle()->lister();
		afficherCheminActuel->setText(pageActuelle()->chemin());
		onglets->setTabText(onglets->currentIndex(), QDir(pageActuelle()->chemin()).dirName());
	}

	else
	{
		afficherCheminActuel->setText(pageActuelle()->chemin());
	}
}

void NavFichiers::slotGo()
{
	QString chemin = afficherCheminActuel->text();

	if (chemin.contains(QRegExp("^(http|http|ftp)")))
	{
		p_navigateurWeb->slotNouvelOnglet();
		p_navigateurWeb->slotOuvrirUrl(chemin);

		FenPrincipale *fen = qobject_cast<FenPrincipale *>(p_fenPrincipale);

		if (fen != 0)
		{
			if (!Multiuso::openTabsList(fen->tabWidget()).contains("Navigateur Web"))
				fen->openTab("Navigateur Web");

			fen->setTabIndex(Multiuso::tabIndexOf("Navigateur Web", fen->tabWidget()));
		}

		return;
	}

	QDir dir(chemin);

	if (dir.exists())
	{
		pageActuelle()->setChemin(chemin);
		pageActuelle()->lister();
		afficherCheminActuel->setText(pageActuelle()->chemin());
		onglets->setTabText(onglets->currentIndex(), QDir(pageActuelle()->chemin()).dirName());
	}

	else
	{
		afficherCheminActuel->setText(pageActuelle()->chemin());
	}
}

void NavFichiers::slotActualiser()
{
	pageActuelle()->lister();
}

void NavFichiers::slotDossierPersonnel()
{
	pageActuelle()->setChemin(QDir::homePath());
	pageActuelle()->lister();

	afficherCheminActuel->setText(pageActuelle()->chemin());
	onglets->setTabText(onglets->currentIndex(), QDir(pageActuelle()->chemin()).dirName());
}

void NavFichiers::slotDossierTelechargements()
{
	QSettings emplacementDossier(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);

	QDir dir;
		dir.mkpath(emplacementDossier.value("telechargements/dossier").toString() + "/Multiuso - Téléchargements");

	pageActuelle()->setChemin(emplacementDossier.value("telechargements/dossier").toString() + "/Multiuso - Téléchargements");
	pageActuelle()->lister();

	afficherCheminActuel->setText(pageActuelle()->chemin());
	onglets->setTabText(onglets->currentIndex(), QDir(pageActuelle()->chemin()).dirName());
}

void NavFichiers::slotDossiersCaches()
{
	pageActuelle()->setAfficherDossiersCaches(!pageActuelle()->isAfficherDossiersCaches());
	pageActuelle()->lister();
}

void NavFichiers::slotDossierRacine()
{
	pageActuelle()->setChemin(QDir::rootPath());
	pageActuelle()->lister();

	afficherCheminActuel->setText(pageActuelle()->chemin());
	onglets->setTabText(onglets->currentIndex(), QDir(pageActuelle()->chemin()).dirName());

	if (QDir(pageActuelle()->chemin()).dirName().isEmpty())
		onglets->setTabText(onglets->currentIndex(), "/");
}

void NavFichiers::slotDossierTmp()
{
	pageActuelle()->setChemin(QDir::tempPath());
	pageActuelle()->lister();

	afficherCheminActuel->setText(pageActuelle()->chemin());
	onglets->setTabText(onglets->currentIndex(), QDir(pageActuelle()->chemin()).dirName());
}

void NavFichiers::slotSwitchTab()
{
	int nextTab = onglets->currentIndex() + 1; // nextTab = current tab + 1

	if (onglets->currentIndex() == onglets->count() - 1) // If the current tab is te last tab (currentIndex starts at 0 | count starts at 1)
		nextTab = 0; // nextTab = 0 in order to start at the beginning

	onglets->setCurrentIndex(nextTab);
}

void NavFichiers::sauvegarderEtat()
{
	QSettings enregistrer(Multiuso::appDirPath() + "/ini/nav_fichiers.ini", QSettings::IniFormat);
		enregistrer.setValue("etat_fenetre", saveState());
}

void NavFichiers::slotOpenFile(QString file)
{
	FenPrincipale *fen = qobject_cast<FenPrincipale *>(p_fenPrincipale);

	if (fen == 0)
		QDesktopServices::openUrl(QUrl::fromLocalFile(file));

	else
		fen->ouvrirFichier(file); /* An open-file function already exists, that's the reason why we use
					ouvrirFichier(QString fichier) from FenPrincipale. */
}
