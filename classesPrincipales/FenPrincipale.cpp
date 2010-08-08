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

#include "FenPrincipale.h"
#include "classesMenuEdition/Preferences.h"
#include "classesMenuAutresFonctionnalites/BlocNotes.h"
#include "classesMenuAutresFonctionnalites/Organisateur.h"
#include "classesMenuAutresFonctionnalites/Messagerie.h"
#include "classesMenuAutresFonctionnalites/CompteurDeLignes.h"
#include "classesMenuAutresFonctionnalites/CompresseurDeCode.h"
#include "classesMenuAutresFonctionnalites/GenerateurPageWeb.h"
#include "classesMenuAutresFonctionnalites/GenerateurClassesCpp.h"
#include "classesMenuAutresFonctionnalites/CreerFichier.h"
#include "classesMenuAutresFonctionnalites/FeedTarsiers/FeedTarsiers.h"
#include "classesMenuAutresFonctionnalites/PlusOuMoins.h"
#include "classesMenuAutresFonctionnalites/CreerReadme.h"
#include "classesMenuAide/APropos.h"
#include "autresClasses/SplashScreen.h"
#include "autresClasses/Astuces.h"
#include "autresClasses/MdiSubWindow.h"

FenPrincipale::FenPrincipale()
{
	loaded = false;

	QSettings mainSettings(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);
		int useSplashScreen = mainSettings.value("splash_screen/utiliser").toBool();

	SplashScreen splash;

	if (useSplashScreen)
		splash.show();

	qApp->processEvents();

	creerReglages();
	creerBarreMenus();
	creerActions();
	creerMenus();
	creerBarreEtat();


	if (useSplashScreen)
		splash.setSplashPicture(5);


	systemTray = new QSystemTrayIcon(this);
		systemTray->setIcon(QIcon(":/icones/application/iconeApplication.png"));
		systemTray->setContextMenu(menuSystemTray);
		systemTray->setToolTip("Multiuso v" + QCoreApplication::applicationVersion()[0] + " (bêta)");
		systemTray->hide();
		connect(systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(slotActivationSystemTray(QSystemTrayIcon::ActivationReason)));

	telechargements = new TelechargerFichier(this);
		telechargements->setModal(false);
		telechargements->hide();
		connect(telechargements, SIGNAL(DesactiverIcone()), this, SLOT(slotDesactiverIconeTelechargements()));
		connect(telechargements, SIGNAL(MajEtat()), this, SLOT(slotTelechargements()));

	/* Création du TabWidget qui contiendra les fonctionnalités principales du programme. */

	accueil = new Accueil(this);

		QVBoxLayout *layoutAccueil = new QVBoxLayout;
			layoutAccueil->addWidget(accueil);
			layoutAccueil->setContentsMargins(0, 0, 0, 0);

		tabAccueil = new QWidget;
			tabAccueil->setLayout(layoutAccueil);


	if (useSplashScreen)
		splash.setSplashPicture(10);


	navFichiers = new NavFichiers(this);

		QVBoxLayout *layoutNavFichiers = new QVBoxLayout;
			layoutNavFichiers->addWidget(navFichiers);
			layoutNavFichiers->setContentsMargins(0, 0, 0, 0);

		tabNavFichiers = new QWidget;
			tabNavFichiers->setLayout(layoutNavFichiers);


	if (useSplashScreen)
		splash.setSplashPicture(20);


	widgets = new Widgets(this);
		connect(widgets, SIGNAL(subWindowWasClosed(QString)), this, SLOT(updateSubWindowsMenu(QString)));

		QVBoxLayout *layoutWidgets = new QVBoxLayout;
			layoutWidgets->addWidget(widgets);
			layoutWidgets->setContentsMargins(0, 0, 0, 0);

		tabWidgets = new QWidget;
			tabWidgets->setLayout(layoutWidgets);


	if (useSplashScreen)
		splash.setSplashPicture(30);


	editeurDeTexte = new EditeurDeTexte(this);

		QVBoxLayout *layoutEditeurDeTexte = new QVBoxLayout;
			layoutEditeurDeTexte->addWidget(editeurDeTexte);
			layoutEditeurDeTexte->setContentsMargins(0, 0, 0, 0);

		tabEditeurDeTexte = new QWidget;
			tabEditeurDeTexte->setLayout(layoutEditeurDeTexte);


	if (useSplashScreen)
		splash.setSplashPicture(40);


	editeurDeCode = new EditeurDeCode(this);

		QVBoxLayout *layoutEditeurDeCode = new QVBoxLayout;
			layoutEditeurDeCode->addWidget(editeurDeCode);
			layoutEditeurDeCode->setContentsMargins(0, 0, 0, 0);

		tabEditeurDeCode = new QWidget;
			tabEditeurDeCode->setLayout(layoutEditeurDeCode);


	if (useSplashScreen)
		splash.setSplashPicture(50);


	ftp = new Ftp(this);

		QVBoxLayout *layoutFtp = new QVBoxLayout;
			layoutFtp->addWidget(ftp);
			layoutFtp->setContentsMargins(0, 0, 0, 0);

		tabFtp = new QWidget;
			tabFtp->setLayout(layoutFtp);


	if (useSplashScreen)
		splash.setSplashPicture(60);


	navigateur = new NavigateurWeb(this, telechargements, editeurDeCode);

		QVBoxLayout *layoutNavigateur = new QVBoxLayout;
			layoutNavigateur->addWidget(navigateur);
			layoutNavigateur->setContentsMargins(0, 0, 0, 0);

		tabNavigateur = new QWidget;
			tabNavigateur->setLayout(layoutNavigateur);


	if (useSplashScreen)
		splash.setSplashPicture(70);


	visionneurImages = new VisionneurImages(this);

		QVBoxLayout *layoutVisionneurImages = new QVBoxLayout;
			layoutVisionneurImages->addWidget(visionneurImages);
			layoutVisionneurImages->setContentsMargins(0, 0, 0, 0);

		tabVisionneurImages = new QWidget;
			tabVisionneurImages->setLayout(layoutVisionneurImages);


	navFichiers->setNavigateurWeb(navigateur);
	editeurDeCode->setWebBrowser(navigateur);


	if (useSplashScreen)
		splash.setSplashPicture(80);


	QStringList lastOpenedTabs = mainSettings.value("ouverture/onglets").value<QStringList>();


	pagesDuProgramme = new QTabWidget;
		pagesDuProgramme->setMovable(true);
		pagesDuProgramme->setDocumentMode(true);

			accueil->restoreOpenedTabs();

		pagesDuProgramme->insertTab(lastOpenedTabs.indexOf("Accueil"), tabAccueil, "Accueil");
		pagesDuProgramme->setCurrentIndex(mainSettings.value("pagesDuProgramme/dernierePage").toInt());

	layoutCentral = new QVBoxLayout;
		layoutCentral->addWidget(pagesDuProgramme);
		layoutCentral->setContentsMargins(4, 4, 4, 4);

	widgetCentral = new QWidget;
		widgetCentral->setLayout(layoutCentral);

	setCentralWidget(widgetCentral);

	disableEnableWidgetsMenuActions();

	if (useSplashScreen)
		splash.setSplashPicture(90);

	noms.clear();

	chargerPlugins();

	QStringList arguments = QCoreApplication::arguments();

	if (arguments.count() > 1)
	{
		for (int i = 1; i < arguments.count(); i++)
		{
			QString arg = arguments.value(i);

			if (arg == "-h" || arg == "--home") // Accueil
			{
				setTabIndex(Multiuso::tabIndexOf("Accueil", pagesDuProgramme));
			}

			else if (arg == "-fb" || arg == "--filebrowser") // Navigateur de fichiers
			{
				if (!Multiuso::openTabsList(pagesDuProgramme).contains("Navigateur de fichiers"))
					accueil->openTab("Navigateur de fichiers");

				setTabIndex(Multiuso::tabIndexOf("Navigateur de fichiers", pagesDuProgramme));
			}

			else if (arg == "-w" || arg == "--widgets") // Widgets
			{
				if (!Multiuso::openTabsList(pagesDuProgramme).contains("Widgets"))
					accueil->openTab("Widgets");

				setTabIndex(Multiuso::tabIndexOf("Widgets", pagesDuProgramme));
			}

			else if (arg == "-pw" || arg == "--pictureviewer") // Visionneur d'images
			{
				if (!Multiuso::openTabsList(pagesDuProgramme).contains("Visionneur d'images"))
					accueil->openTab("Visionneur d'images");

				setTabIndex(Multiuso::tabIndexOf("Visionneur d'images", pagesDuProgramme));
			}

			else if (arg == "-te" || arg == "--textedit") // Éditeur de texte
			{
				if (!Multiuso::openTabsList(pagesDuProgramme).contains("Éditeur de texte"))
					accueil->openTab("Éditeur de texte");

				setTabIndex(Multiuso::tabIndexOf("Éditeur de texte", pagesDuProgramme));
			}

			else if (arg == "-ce" || arg == "--codeedit") // Éditeur de code
			{
				if (!Multiuso::openTabsList(pagesDuProgramme).contains("Éditeur de code"))
					accueil->openTab("Éditeur de code");

				setTabIndex(Multiuso::tabIndexOf("Éditeur de code", pagesDuProgramme));
			}

			else if (arg == "-ftp" || arg == "--ftp") // Client FTP
			{
				if (!Multiuso::openTabsList(pagesDuProgramme).contains("Client FTP"))
					accueil->openTab("Client FTP");

				setTabIndex(Multiuso::tabIndexOf("Client FTP", pagesDuProgramme));
			}

			else if (arg == "-wb" || arg == "--webbrowser") // Navigateur Web
			{
				if (!Multiuso::openTabsList(pagesDuProgramme).contains("Navigateur Web"))
					accueil->openTab("Navigateur Web");

				setTabIndex(Multiuso::tabIndexOf("Navigateur Web", pagesDuProgramme));
			}

			else if (arg.contains(QRegExp("^(.+)://")))
			{
				navigateur->slotNouvelOnglet();
				navigateur->slotOuvrirUrl(arg);

				if (!Multiuso::openTabsList(pagesDuProgramme).contains("Navigateur Web"))
					accueil->openTab("Navigateur Web");

				setTabIndex(Multiuso::tabIndexOf("Navigateur Web", pagesDuProgramme));
			}

			else if (arg == "-sti" || arg == "--systemtrayicon") // System tray (see 'main.cpp')
			{
				systemTray->show();
				hide();

				if (mainSettings.value("systemTray/afficher").toBool())
				{
					systemTray->showMessage("Multiuso", "Multiuso a été minimisé dans le system tray.\n"
						"Cliquez sur l'icône pour ré-ouvrir le programme.", QSystemTrayIcon::Information, 5000);

					mainSettings.setValue("systemTray/afficher", false);
				}
			}

			else if (arg == "-st" || arg == "--showtips") // Astuces (see 'main.cpp')
			{
			}

			else
			{
				ouvrirFichier(arguments.value(i));
			}
		}
	}


	if (useSplashScreen)
		splash.setSplashPicture(100);


	confirmationVerificationMaj = false;

	verifierMAJ();

	qApp->processEvents();

	widgets->openTodayNotes();

	if (useSplashScreen)
		splash.finish(this);

	loaded = true;
}

void FenPrincipale::creerReglages()
{
	setWindowTitle("Multiuso v" + QCoreApplication::applicationVersion()[0] + " (bêta)");
	setWindowIcon(QIcon(":/icones/application/iconeApplication.png"));
	setUnifiedTitleAndToolBarOnMac(true);

	QSettings ouverture(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);

	if (!ouverture.value("ouverture/agrandi").toBool())
	{
		QSettings reglagesFenetre(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);
			resize(reglagesFenetre.value("dimensions/largeur").toInt(), reglagesFenetre.value("dimensions/hauteur").toInt());
	}

	else
	{
		setWindowState(Qt::WindowMaximized);
	}

	setMinimumSize(700, 450);
}

void FenPrincipale::creerBarreMenus()
{
	menuMultiuso = menuBar()->addMenu("&Multiuso");

	menuEdition = menuBar()->addMenu("É&dition");

	menuAffichage = menuBar()->addMenu("&Affichage");

	menuAutresFonctionnalites = menuBar()->addMenu("A&utres fonctionnalités");
		menuAutresFonctionnalites->setTearOffEnabled(true);

	menuAide = menuBar()->addMenu("Aid&e");

	menuSystemTray = new QMenu;
}

void FenPrincipale::creerActions()
{
	QSettings reglagesActions(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);

	actionTelechargements = new QAction("&Téléchargements", this);
		actionTelechargements->setIcon(QIcon(":/icones/actions/actionTelechargements.png"));
		actionTelechargements->setStatusTip("Télécharger un fichier provenant d'un serveur web");
		connect(actionTelechargements, SIGNAL(triggered()), this, SLOT(slotTelechargements()));

	actionVerifierMaj = new QAction("&Vérifier les mises à jour", this);
		actionVerifierMaj->setIcon(QIcon(":/icones/actions/actionVerifierMaj.png"));
		actionVerifierMaj->setStatusTip("Vérifier si une nouvelle version de Multiuso est disponible");
		connect(actionVerifierMaj, SIGNAL(triggered()), this, SLOT(verifierMAJ()));

	actionAssistantMaj = new QAction("&Assistant de mise à jour", this);
		actionAssistantMaj->setIcon(QIcon(":/icones/actions/actionAssistantMaj.png"));
		actionAssistantMaj->setStatusTip("Lancer l'assistant de mise à jour");
		connect(actionAssistantMaj, SIGNAL(triggered()), this, SLOT(assistantDeMiseAJour()));

	actionGestionnaireDesPlugins = new QAction("Gestionnaire des &plugins", this);
		actionGestionnaireDesPlugins->setIcon(QIcon(":/icones/actions/actionGestionnaireDesPlugins.png"));
		actionGestionnaireDesPlugins->setStatusTip("Organiser les plugins de Multiuso");
		connect(actionGestionnaireDesPlugins, SIGNAL(triggered()), this, SLOT(slotGestionnaireDesPlugins()));

	actionQuitter = new QAction("&Quitter", this);
		actionQuitter->setIcon(QIcon(":/icones/actions/actionQuitter.png"));
		actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
		actionQuitter->setStatusTip("Quitter Multiuso");
		connect(actionQuitter, SIGNAL(triggered()), this, SLOT(slotQuitter()));

	actionGoToHome = new QAction("A&ccueil", this);
		actionGoToHome->setIcon(QIcon(":/icones/actions/actionGoToHome.png"));
		actionGoToHome->setShortcut(QKeySequence("Ctrl+Space"));
		actionGoToHome->setStatusTip("Aller à l'Accueil");
		connect(actionGoToHome, SIGNAL(triggered()), this, SLOT(slotGoToHome()));

	actionPreferences = new QAction("&Préférences", this);
		actionPreferences->setIcon(QIcon(":/icones/actions/actionPreferences.png"));
		actionPreferences->setStatusTip("Préférences de Multiuso");
		connect(actionPreferences, SIGNAL(triggered()), this, SLOT(slotPreferences()));

	actionBarreEtat = new QAction("&Afficher/Masquer la barre d'état", this);
		actionBarreEtat->setIcon(QIcon(":/icones/actions/actionBarreEtat.png"));
		actionBarreEtat->setStatusTip("Afficher ou masquer la barre d'état principale");
		actionBarreEtat->setCheckable(true);
		actionBarreEtat->setChecked(reglagesActions.value("statusBar/afficher").toBool());
		connect(actionBarreEtat, SIGNAL(triggered()), this, SLOT(slotBarreEtat()));

	actionTailleParDefaut = new QAction("&Taille par défaut", this);
		actionTailleParDefaut->setIcon(QIcon(":/icones/actions/actionTailleParDefaut.png"));
		actionTailleParDefaut->setStatusTip("Taille de la fenêtre par défaut");
		connect(actionTailleParDefaut, SIGNAL(triggered()), this, SLOT(slotTailleParDefaut()));

	actionPleinEcran = new QAction("&Plein Écran", this);
		actionPleinEcran->setCheckable(true);
		actionPleinEcran->setChecked(false);
		actionPleinEcran->setIcon(QIcon(":/icones/actions/actionPleinEcran.png"));
		actionPleinEcran->setStatusTip("Passer en plein écran");
		actionPleinEcran->setShortcut(QKeySequence("F11"));
		connect(actionPleinEcran, SIGNAL(triggered()), this, SLOT(slotPleinEcran()));

	actionSubCalendrier = new QAction("Calendrier/Agenda", this);
		actionSubCalendrier->setIcon(QIcon(":/icones/widgets/calendrier.png"));
		connect(actionSubCalendrier, SIGNAL(triggered()), this, SLOT(actionWidgetClicked()));

	actionSubCalculatrice = new QAction("Calculatrice", this);
		actionSubCalculatrice->setIcon(QIcon(":/icones/widgets/calculatrice.png"));
		connect(actionSubCalculatrice, SIGNAL(triggered()), this, SLOT(actionWidgetClicked()));

	actionSubChronometre = new QAction("Chronomètre/Minuterie", this);
		actionSubChronometre->setIcon(QIcon(":/icones/widgets/chronometre.png"));
		connect(actionSubChronometre, SIGNAL(triggered()), this, SLOT(actionWidgetClicked()));

	actionBlocNotes = new QAction("Bloc-&Notes", this);
		actionBlocNotes->setIcon(QIcon(":/icones/actions/actionBlocNotes.png"));
		actionBlocNotes->setStatusTip("Ouvrir le Bloc-Notes");
		connect(actionBlocNotes, SIGNAL(triggered()), this, SLOT(slotBlocNotes()));

	actionOrganisateur = new QAction("&Organisateur", this);
		actionOrganisateur->setIcon(QIcon(":/icones/actions/actionOrganisateur.png"));
		actionOrganisateur->setStatusTip("Gérer une liste des tâches à faire");
		connect(actionOrganisateur, SIGNAL(triggered()), this, SLOT(slotOrganisateur()));
		
	actionMessagerie = new QAction("&Messagerie", this);
		actionMessagerie->setIcon(QIcon(":/icones/actions/actionMessagerie.png"));
		actionMessagerie->setStatusTip("Envoyez des messages à vos amis");
		connect(actionMessagerie, SIGNAL(triggered()), this, SLOT(slotMessagerie()));

	actionCompteurDeLignes = new QAction("Compteur de &lignes", this);
		actionCompteurDeLignes->setIcon(QIcon(":/icones/actions/actionCompteurDeLignes.png"));
		actionCompteurDeLignes->setStatusTip("Compter le nombre de lignes d'un fichier");
		connect(actionCompteurDeLignes, SIGNAL(triggered()), this, SLOT(slotCompteurDeLignes()));

	actionCompresseurDeCode = new QAction("&Compresseur de code", this);
		actionCompresseurDeCode->setIcon(QIcon(":/icones/actions/actionCompresseurDeCode.png"));
		actionCompresseurDeCode->setStatusTip("Compresser du code en une seule ligne");
		connect(actionCompresseurDeCode, SIGNAL(triggered()), this, SLOT(slotCompresseurDeCode()));

	actionGenerateurPageWeb = new QAction("Générateur de pages &Web", this);
		actionGenerateurPageWeb->setIcon(QIcon(":/icones/actions/actionGenerateurPageWeb.png"));
		actionGenerateurPageWeb->setStatusTip("Générer le code de base d'une page Web (x)HTML");
		connect(actionGenerateurPageWeb, SIGNAL(triggered()), this, SLOT(slotGenerateurPageWeb()));

	actionGenerateurClassesCpp = new QAction("Générateur de c&lasses C++", this);
		actionGenerateurClassesCpp->setIcon(QIcon(":/icones/actions/actionGenerateurClassesCpp.png"));
		actionGenerateurClassesCpp->setStatusTip("Générer le code de base d'une classe C++");
		connect(actionGenerateurClassesCpp, SIGNAL(triggered()), this, SLOT(slotGenerateurClassesCpp()));

	actionCreerFichier = new QAction("Créer un fic&hier", this);
		actionCreerFichier->setIcon(QIcon(":/icones/actions/actionCreerFichier.png"));
		actionCreerFichier->setStatusTip("Créer un fichier de nom et d'extension libre");
		connect(actionCreerFichier, SIGNAL(triggered()), this, SLOT(slotCreerFichier()));

	actionFeedTarsiers = new QAction("Feed&Tarsiers", this);
		actionFeedTarsiers->setIcon(QIcon(":/icones/feed_tarsiers/tarsier_l_3.png"));
		actionFeedTarsiers->setStatusTip("Jouer à FeedTarsiers");
		connect(actionFeedTarsiers, SIGNAL(triggered()), this, SLOT(slotFeedTarsiers()));

	actionPlusOuMoins = new QAction("&Plus ou Moins", this);
		actionPlusOuMoins->setIcon(QIcon(":/icones/actions/actionPlusOuMoins.png"));
		actionPlusOuMoins->setStatusTip("Jouer au célèbre Plus + ou -");
		connect(actionPlusOuMoins, SIGNAL(triggered()), this, SLOT(slotPlusOuMoins()));

	actionCreerReadme = new QAction("Créer un &README", this);
		actionCreerReadme->setIcon(QIcon(":/icones/actions/actionCreerReadme.png"));
		actionCreerReadme->setStatusTip("Créer un README");
		connect(actionCreerReadme, SIGNAL(triggered()), this, SLOT(slotCreerReadme()));

	actionSurLeWeb = new QAction("Sur le &Web...", this);
		actionSurLeWeb->setIcon(QIcon(":/icones/actions/actionSurLeWeb.png"));
		actionSurLeWeb->setStatusTip("Site Web de Multiuso");
		connect(actionSurLeWeb, SIGNAL(triggered()), this, SLOT(slotSurLeWeb()));

	actionObtenirPlugins = new QAction("&Obtenir des nouveaux plugins", this);
		actionObtenirPlugins->setIcon(QIcon(":/icones/actions/actionObtenirPlugins.png"));
		actionObtenirPlugins->setStatusTip("Obtenir des nouveaux plugins sur le site Web de Multiuso");
		connect(actionObtenirPlugins, SIGNAL(triggered()), this, SLOT(slotObtenirPlugins()));

	actionAPropos = new QAction("À &propos...", this);
		actionAPropos->setIcon(QIcon(":/icones/actions/actionAPropos.png"));
		actionAPropos->setStatusTip("À propos de Multiuso");
		connect(actionAPropos, SIGNAL(triggered()), this, SLOT(slotAPropos()));

	actionAProposDeQt = new QAction("À propos de &Qt", this);
		actionAProposDeQt->setIcon(QIcon(":/icones/actions/actionAProposDeQt.png"));
		actionAProposDeQt->setStatusTip("À propos de Qt");
		connect(actionAProposDeQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	actionAstuces = new QAction("&Astuces", this);
		actionAstuces->setIcon(QIcon(":/icones/actions/actionAstuces.png"));
		actionAstuces->setShortcut(QKeySequence("F1"));
		actionAstuces->setStatusTip("Astuces de Multiuso");
		connect(actionAstuces, SIGNAL(triggered()), this, SLOT(slotAstuces()));
}

void FenPrincipale::creerMenus()
{
	menuMultiuso->addAction(actionTelechargements);
	menuMultiuso->addAction(actionVerifierMaj);
	menuMultiuso->addAction(actionAssistantMaj);
	menuMultiuso->addAction(actionGestionnaireDesPlugins);
	menuMultiuso->addSeparator();
	menuMultiuso->addAction(actionQuitter);

	menuEdition->addAction(actionGoToHome);
	menuEdition->addSeparator();
	menuEdition->addAction(actionPreferences);

	sousMenuWidgets = new QMenu("&Widgets");
		sousMenuWidgets->setIcon(QIcon(":/icones/widgets/widgets.png"));
		sousMenuWidgets->addAction(actionSubCalendrier);
		sousMenuWidgets->addAction(actionSubCalculatrice);
		sousMenuWidgets->addAction(actionSubChronometre);

	menuAffichage->addAction(actionBarreEtat);
	menuAffichage->addSeparator();
	menuAffichage->addAction(actionTailleParDefaut);
	menuAffichage->addAction(actionPleinEcran);
	menuAffichage->addSeparator();
	menuAffichage->addMenu(sousMenuWidgets);

	sousMenuProgrammation = new QMenu("Pro&grammation");
		sousMenuProgrammation->setIcon(QIcon(":/icones/menus/sousMenuProgrammation.png"));
		sousMenuProgrammation->addAction(actionCompresseurDeCode);
		sousMenuProgrammation->addAction(actionGenerateurPageWeb);
		sousMenuProgrammation->addAction(actionGenerateurClassesCpp);

	sousMenuMiniJeux = new QMenu("&Mini-Jeux");
		sousMenuMiniJeux->setIcon(QIcon(":/icones/menus/sousMenuMiniJeux.png"));
		sousMenuMiniJeux->addAction(actionFeedTarsiers);
		sousMenuMiniJeux->addAction(actionPlusOuMoins);

	menuAutresFonctionnalites->addMenu(sousMenuProgrammation);
	menuAutresFonctionnalites->addMenu(sousMenuMiniJeux);
	menuAutresFonctionnalites->addSeparator();
	menuAutresFonctionnalites->addAction(actionBlocNotes);
	menuAutresFonctionnalites->addAction(actionOrganisateur);
	menuAutresFonctionnalites->addAction(actionMessagerie);
	menuAutresFonctionnalites->addAction(actionCompteurDeLignes);
	menuAutresFonctionnalites->addAction(actionCreerFichier);
	menuAutresFonctionnalites->addAction(actionCreerReadme);

	menuAide->addAction(actionSurLeWeb);
	menuAide->addAction(actionObtenirPlugins);
	menuAide->addAction(actionAPropos);
	menuAide->addAction(actionAProposDeQt);
	menuAide->addSeparator();
	menuAide->addAction(actionAstuces);

	menuSystemTray->addAction(actionQuitter);
}

void FenPrincipale::creerBarreEtat()
{
	barreDEtat = statusBar();

	QSettings barreEtat(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);

	if (!barreEtat.value("statusBar/afficher").toBool())
		barreDEtat->hide();
}

void FenPrincipale::chargerPlugins()
{
	QStringList listePlugins = QDir(Multiuso::pluginsDirPath()).entryList();
		listePlugins.removeOne(".");
		listePlugins.removeOne("..");

	for (int i = 0; i < listePlugins.size(); i++)
	{
		QPluginLoader chargeur(Multiuso::pluginsDirPath() + listePlugins.value(i));

		if (QObject *nouveauPlugin = chargeur.instance())
		{
			QFileInfo fileInfo(Multiuso::pluginsDirPath() + listePlugins.value(i));

			QSettings settings(fileInfo.absolutePath() + "/" + fileInfo.baseName() +
					"/" + fileInfo.baseName() + ".ini", QSettings::IniFormat);

			int type = settings.value("type").toInt();

			switch (type)
			{
				case 1: peuplerMenus(qobject_cast<BaseAction *>(nouveauPlugin)); break;
				case 2: peuplerDocks(qobject_cast<BaseDockWidget *>(nouveauPlugin)); break;
				case 3: peuplerOnglets(qobject_cast<BaseOnglet *>(nouveauPlugin)); break;
				case 4: peuplerWidgets(qobject_cast<BaseWidget *>(nouveauPlugin)); break;
				default: continue; break;
			}
		}
	}
}

void FenPrincipale::peuplerMenus(BaseAction *plugin)
{
	if (plugin == NULL)
		return;

	if (noms.contains(plugin->nom()))
		return;

	noms << plugin->nom();

	QAction *actionPlugin = new QAction(plugin->nom(), this);
		actionPlugin->setIcon(QIcon(plugin->icone()));
		actionPlugin->setToolTip(plugin->aide());
		actionPlugin->setStatusTip(plugin->informations());
		connect(actionPlugin, SIGNAL(triggered()), plugin, SLOT(ouvrir()));

	double menu = plugin->menuParent();

		if (menu == 1)
			menuMultiuso->addAction(actionPlugin);

		else if (menu == 2)
			menuEdition->addAction(actionPlugin);

		else if (menu == 3)
			menuAffichage->addAction(actionPlugin);

		else if (menu == 4)
			menuAutresFonctionnalites->addAction(actionPlugin);

		else if (menu == 4.1)
			sousMenuProgrammation->addAction(actionPlugin);

		else if (menu == 4.2)
			sousMenuMiniJeux->addAction(actionPlugin);

		else if (menu == 5)
			menuAide->addAction(actionPlugin);
}

void FenPrincipale::peuplerDocks(BaseDockWidget *plugin)
{
	if (plugin == NULL)
		return;

	if (noms.contains(plugin->nom()))
		return;

	noms << plugin->nom();

	QDockWidget *dockWidget = new QDockWidget(plugin->nom(), this);
		dockWidget->setWidget(plugin->contenuDockWidget());

	switch (plugin->aireDockWidget())
	{
		case 1: addDockWidget(Qt::TopDockWidgetArea, dockWidget); break;
		case 2: addDockWidget(Qt::BottomDockWidgetArea, dockWidget); break;
		case 3: default: addDockWidget(Qt::LeftDockWidgetArea, dockWidget); break;
		case 4: addDockWidget(Qt::RightDockWidgetArea, dockWidget); break;
	}
}

void FenPrincipale::peuplerOnglets(BaseOnglet *plugin)
{
	if (plugin == NULL)
		return;

	pagesDuProgramme->addTab(plugin->contenuTab(), plugin->nom());
}

void FenPrincipale::peuplerWidgets(BaseWidget *plugin)
{
	if (plugin == NULL)
		return;

	MdiSubWindow *subWindow = new MdiSubWindow;
		subWindow->setWindowTitle(plugin->nom());
		subWindow->setWindowIcon(QIcon(plugin->icone()));
		subWindow->setWidget(plugin->contenuSubWindow());

	widgets->newSubWindow(subWindow);
}

bool FenPrincipale::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QEvent::FileOpen)
	{
		QFile fichier(((QFileOpenEvent *)event)->file());

		QFileInfo infosFichier(fichier);

		ouvrirFichier(infosFichier.filePath());

		return true;
	}

	else
	{
		return QObject::eventFilter(object, event);
	}
}

void FenPrincipale::closeEvent(QCloseEvent *event)
{
	QSettings quitter(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);

	if (!quitter.value("fermeture/systemTray").toBool())
	{
		slotQuitter();
	}

	else
	{
		event->ignore();
		systemTray->show();
		hide();

		QSettings systemTraySettings(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);

		if (systemTraySettings.value("systemTray/afficher").toBool())
		{
			systemTray->showMessage("Multiuso", "Multiuso a été minimisé dans le system tray.\n"
				"Cliquez sur l'icône pour ré-ouvrir le programme.", QSystemTrayIcon::Information, 5000);

			systemTraySettings.setValue("systemTray/afficher", false);
		}
	}

	event->ignore();
}

void FenPrincipale::resizeEvent(QResizeEvent *event)
{
	event->accept();

	QSettings reglagesFenetre(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);
		reglagesFenetre.setValue("dimensions/largeur", width());
		reglagesFenetre.setValue("dimensions/hauteur", height());
}

void FenPrincipale::moveEvent(QMoveEvent *event)
{
	QSettings config(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);

 	if (config.value("deplacements/effets").toBool())
	{
		if (QCursor::pos().y() <= 0)
		{
			showMaximized();
		}

		else if (QCursor::pos().x() <= 0)
		{
			systemTray->show();
			hide();

			if (config.value("systemTray/afficher").toBool())
			{
				systemTray->showMessage("Multiuso", "Multiuso a été minimisé dans le system tray.\n"
					"Cliquez sur l'icône pour ré-ouvrir le programme.", QSystemTrayIcon::Information, 5000);

				config.setValue("systemTray/afficher", false);
			}
		}

		else if (QCursor::pos().x() >= Multiuso::screenWidth() - 1) // -1 because the cursor can't be at pos screenWidth()
		{
			move((Multiuso::screenWidth() / 2) - (width() / 2), (Multiuso::screenHeight() / 2) - (height() / 2));
			actionPleinEcran->setChecked(true);
			slotPleinEcran();
		}

		else if (QCursor::pos().y() >= Multiuso::screenHeight() - 1) // Idem
		{
			slotQuitter();
		}
	}

	QMainWindow::moveEvent(event);
}

void FenPrincipale::slotActivationSystemTray(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
		case QSystemTrayIcon::Unknown:
		case QSystemTrayIcon::DoubleClick:
		case QSystemTrayIcon::Trigger:
		case QSystemTrayIcon::MiddleClick:
			systemTray->hide();
			this->show();
		break;

		case QSystemTrayIcon::Context:
		break;
	}
}

void FenPrincipale::slotGestionnaireDesPlugins()
{
	int answer = QMessageBox::question(this, "Multiuso", "Voulez-vous fermer Multiuso et lancer le gestionnaire des plugins ?",
			QMessageBox::Yes | QMessageBox::No);

	if (answer == QMessageBox::Yes)
	{
		QProcess::startDetached(Multiuso::openCommand() + Multiuso::appDirPath() + "/GPlugins" + Multiuso::currentSuffix());

		Multiuso::quit();
	}
}

void FenPrincipale::slotQuitter()
{
	if (!loaded)
		return;

	if (!editeurDeTexte->isEverythingSaved() || !editeurDeCode->tousLesDocumentsEnregistres())
	{
		int reponse = QMessageBox::warning(this, "Multiuso", "Un ou plusieurs documents texte ne sont pas enregitrés,<br />"
			"voulez-vous quitter Multiuso malgré tout ?", QMessageBox::Yes | QMessageBox::No);

		if (reponse == QMessageBox::No)
			return;
	}

	navFichiers->sauvegarderEtat();
	visionneurImages->sauvegarderEtat();
	editeurDeTexte->sauvegarderEtat();
	editeurDeCode->sauvegarderEtat();
	ftp->sauvegarderEtat();
	navigateur->sauvegarderEtat();

	QStringList tabs = Multiuso::openTabsList(pagesDuProgramme);

	for (int i = 0; i < tabs.count(); i++)
	{
		if (!Multiuso::tabsList().contains(tabs.value(i)))
			tabs.removeAt(i);
	}

	if (tabs.isEmpty())
		tabs << "";

	QSettings reglagesFenetre(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);
		reglagesFenetre.setValue("dimensions/largeur", width());
		reglagesFenetre.setValue("dimensions/hauteur", height());
		reglagesFenetre.setValue("ouverture/crash", false);
		reglagesFenetre.setValue("ouverture/onglets", tabs);
		reglagesFenetre.setValue("ouverture/page", accueil->openedPage());
		reglagesFenetre.setValue("pagesDuProgramme/dernierePage", pagesDuProgramme->currentIndex());

	Multiuso::quit();
}

void FenPrincipale::slotGoToHome()
{
	pagesDuProgramme->setCurrentIndex(Multiuso::tabIndexOf("Accueil", pagesDuProgramme));
}

void FenPrincipale::actionWidgetClicked()
{
	QAction *action = qobject_cast<QAction *>(sender());

	if (action == 0)
		return;

	QSettings settings(Multiuso::appDirPath() + "/ini/widgets.ini", QSettings::IniFormat);
		settings.setValue(Multiuso::cleanStr(action->text()) + "/afficher", true);

	disableEnableWidgetsMenuActions();
}

void FenPrincipale::updateSubWindowsMenu(QString windowTitle)
{
	QSettings settings(Multiuso::appDirPath() + "/ini/widgets.ini", QSettings::IniFormat);
		settings.setValue(Multiuso::cleanStr(windowTitle) + "/afficher", false);

	disableEnableWidgetsMenuActions();
}

void FenPrincipale::disableEnableWidgetsMenuActions()
{
	QSettings settings(Multiuso::appDirPath() + "/ini/widgets.ini", QSettings::IniFormat);
		bool first = settings.value("first").toBool();
		settings.setValue("first", false);

	QStringList subWindows;
		subWindows << "Calendrier/Agenda" << "Calculatrice" << "Chronomètre/Minuterie";

	for (int i = 0; i < subWindows.size(); i++)
	{
		if (first)
			settings.setValue(Multiuso::cleanStr(subWindows.value(i)) + "/afficher", true);

		if (settings.value(Multiuso::cleanStr(subWindows.value(i)) + "/afficher").toBool())
			widgets->showWindow(subWindows.value(i));

		if (subWindows.value(i) == "Calendrier/Agenda")
			actionSubCalendrier->setDisabled(settings.value(Multiuso::cleanStr(subWindows.value(i)) + "/afficher").toBool());

		else if (subWindows.value(i) == "Calculatrice")
			actionSubCalculatrice->setDisabled(settings.value(Multiuso::cleanStr(subWindows.value(i)) + "/afficher").toBool());

		else if (subWindows.value(i) == "Chronomètre/Minuterie")
			actionSubChronometre->setDisabled(settings.value(Multiuso::cleanStr(subWindows.value(i)) + "/afficher").toBool());
	}

	if (pagesDuProgramme != NULL && loaded)
		pagesDuProgramme->setCurrentIndex(Multiuso::tabIndexOf("Widgets", pagesDuProgramme));
}

void FenPrincipale::slotPreferences()
{
	Preferences *fenPreferences = new Preferences(this);
		fenPreferences->exec();
		fenPreferences->deleteLater();
}

void FenPrincipale::slotBarreEtat()
{
	QSettings barreEtat(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);
		barreEtat.setValue("statusBar/afficher", actionBarreEtat->isChecked());

	if (actionBarreEtat->isChecked())
		barreDEtat->show();

	else
		barreDEtat->hide();
}

void FenPrincipale::slotTailleParDefaut()
{
	resize(985, 660);
}

void FenPrincipale::slotPleinEcran()
{
	if (actionPleinEcran->isChecked())
	{
		showFullScreen();

		actionPleinEcran->setText("&Normal");
		actionPleinEcran->setIcon(QIcon(":/icones/actions/actionEcranNormal.png"));
		actionPleinEcran->setStatusTip("Passer en affichage normal");
	}

	else
	{
		showNormal();

		actionPleinEcran->setText("&Plein Écran");
		actionPleinEcran->setIcon(QIcon(":/icones/actions/actionPleinEcran.png"));
		actionPleinEcran->setStatusTip("Passer en plein écran");
	}
}

void FenPrincipale::slotBlocNotes()
{
	BlocNotes *fenBlocNotes = new BlocNotes(this);
		fenBlocNotes->exec();
		fenBlocNotes->deleteLater();
}

void FenPrincipale::slotOrganisateur()
{
	Organisateur *fenOrganisateur = new Organisateur(this);
		fenOrganisateur->exec();
		fenOrganisateur->deleteLater();
}

void FenPrincipale::slotMessagerie()
{
	Messagerie *fenMessagerie = new Messagerie(this);
		fenMessagerie->exec();
		fenMessagerie->deleteLater();
}

void FenPrincipale::slotCompteurDeLignes()
{
	CompteurDeLignes *fenCompteurDeLignes = new CompteurDeLignes(this);
		fenCompteurDeLignes->exec();
		fenCompteurDeLignes->deleteLater();
}

void FenPrincipale::slotCompresseurDeCode()
{
	CompresseurDeCode *fenCompresseurDeCode = new CompresseurDeCode(this);
		fenCompresseurDeCode->exec();
		fenCompresseurDeCode->deleteLater();
}

void FenPrincipale::slotGenerateurPageWeb()
{
	GenerateurPageWeb *fenGenerateurPageWeb = new GenerateurPageWeb(this);
		connect(fenGenerateurPageWeb, SIGNAL(ouvrirFichierIntermediaire(QString)), this, SLOT(ouvrirFichierGenere(QString)));
		fenGenerateurPageWeb->exec();
		fenGenerateurPageWeb->deleteLater();
}


void FenPrincipale::slotGenerateurClassesCpp()
{
	GenerateurClassesCpp *fenGenerateurClassesCpp = new GenerateurClassesCpp(this);
		connect(fenGenerateurClassesCpp, SIGNAL(ouvrirFichierIntermediaire(QString)), this, SLOT(ouvrirFichierGenere(QString)));
		fenGenerateurClassesCpp->exec();
		fenGenerateurClassesCpp->deleteLater();
}

void FenPrincipale::slotCreerFichier()
{
	CreerFichier *fenCreerFichier = new CreerFichier(this);
		fenCreerFichier->exec();
		fenCreerFichier->deleteLater();
}

void FenPrincipale::slotFeedTarsiers()
{
	FeedTarsiers *fenFeedTarsiers = new FeedTarsiers(this);
		fenFeedTarsiers->exec();
		fenFeedTarsiers->deleteLater();
}

void FenPrincipale::slotPlusOuMoins()
{
	PlusOuMoins *plusOuMoins = new PlusOuMoins(this);
		plusOuMoins->exec();
		plusOuMoins->deleteLater();
}

void FenPrincipale::slotCreerReadme()
{
	CreerReadme *fenCreerReadme = new CreerReadme(this);
		fenCreerReadme->exec();
		fenCreerReadme->deleteLater();
}

void FenPrincipale::slotSurLeWeb()
{
	navigateur->slotNouvelOnglet();
	navigateur->slotOuvrirUrl(QCoreApplication::organizationDomain());

	if (!Multiuso::openTabsList(pagesDuProgramme).contains("Navigateur Web"))
		accueil->openTab("Navigateur Web");

	setTabIndex(Multiuso::tabIndexOf("Navigateur Web", pagesDuProgramme));
}

void FenPrincipale::slotObtenirPlugins()
{
	navigateur->slotNouvelOnglet();
	navigateur->slotOuvrirUrl(QCoreApplication::organizationDomain() + "plugins.php");

	if (!Multiuso::openTabsList(pagesDuProgramme).contains("Navigateur Web"))
		accueil->openTab("Navigateur Web");

	setTabIndex(Multiuso::tabIndexOf("Navigateur Web", pagesDuProgramme));
}

void FenPrincipale::slotAPropos()
{
	APropos *fenAPropos = new APropos(this);
		fenAPropos->exec();
		fenAPropos->deleteLater();
}

void FenPrincipale::slotAstuces()
{
	Astuces *astuces = new Astuces(this);
		astuces->exec();
		astuces->deleteLater();
}

void FenPrincipale::appliquerReglages()
{
	accueil->actualiserFond();

	QSettings apparenceFenetre(Multiuso::appDirPath() + "/ini/apparence.ini", QSettings::IniFormat);

	QString style = " ";

	if (apparenceFenetre.value("style/style_de_base").toBool())
	{
		setStyle(NULL);
	}

	else if (apparenceFenetre.value("style/style_personnalise").toBool())
	{
		QSettings settings(Multiuso::appDirPath() + "/ini/apparence.ini", QSettings::IniFormat);
			style = settings.value("style_perso/content").toString();
	}

	else if (apparenceFenetre.value("style/style_externe").toBool())
	{
		QString styleExterne = apparenceFenetre.value("style/nom_style_externe").toString();

		QFile fichierStyle(Multiuso::appDirPath() + "/extensions/styles/" + styleExterne);

		if (fichierStyle.open(QIODevice::ReadOnly | QIODevice::Text))
			style = fichierStyle.readAll();

		else
			QMessageBox::critical(this, "Multiuso", "Impossible d'ouvrir le fichier contenant le style.<br />"
					"Le style de base va être appliqué.");

		fichierStyle.close();
	}

	else if (apparenceFenetre.value("style/style_qt").toBool())
	{
		setStyle(QStyleFactory::create(apparenceFenetre.value("style/style_qt_utilise").toString()));
	}

	setStyleSheet(style);
}

void FenPrincipale::ouvrirFichier(QString fichier)
{
	QStringList textesFormates;
		textesFormates << "mltshtml";

	QStringList textes;
		textes << "mltstxt" << "txt" << "c" << "h" << "cpp" << "hpp" << "css" << "jar" << "js" << "tex" << "php" << "sql" << "xml";

	QStringList navigateurWeb;
		navigateurWeb << "html" << "htm";

	QStringList images;
		images << "png" << "jpg" << "jpeg" << "bmp" << "gif" << "pbm" << "pgm" << "ppm" << "xbm" << "xpm" << "svg";

	QFileInfo infos(fichier);

	QString suffixe = infos.suffix();

	if (textesFormates.contains(suffixe.toLower()))
	{
		editeurDeTexte->openFile(fichier);

		if (!Multiuso::openTabsList(pagesDuProgramme).contains("Éditeur de texte"))
			accueil->openTab("Éditeur de texte");

		setTabIndex(Multiuso::tabIndexOf("Éditeur de texte", pagesDuProgramme));
	}

	else if (textes.contains(suffixe.toLower()))
	{
		editeurDeCode->slotOuvrirFichier(fichier);
		editeurDeCode->highlighterFor(suffixe);

		if (!Multiuso::openTabsList(pagesDuProgramme).contains("Éditeur de code"))
			accueil->openTab("Éditeur de code");

		setTabIndex(Multiuso::tabIndexOf("Éditeur de code", pagesDuProgramme));
	}

	else if (navigateurWeb.contains(suffixe.toLower()))
	{
		navigateur->slotNouvelOnglet();
		navigateur->slotOuvrirFichier(fichier);

		if (!Multiuso::openTabsList(pagesDuProgramme).contains("Navigateur Web"))
			accueil->openTab("Navigateur Web");

		setTabIndex(Multiuso::tabIndexOf("Navigateur Web", pagesDuProgramme));
	}

	else if (images.contains(suffixe.toLower()))
	{
		if (visionneurImages->needNewTab())
			visionneurImages->slotNouvelOnglet();

		else
			visionneurImages->switchToLastIndex();

		visionneurImages->slotOuvrirFichier(fichier);

		if (!Multiuso::openTabsList(pagesDuProgramme).contains("Visionneur d'images"))
			accueil->openTab("Visionneur d'images");

		setTabIndex(Multiuso::tabIndexOf("Visionneur d'images", pagesDuProgramme));
	}

	else
	{
		int reponse = QMessageBox::question(this, "Multiuso", "Désolé, Multiuso ne gère pas ce type de fichier.\n"
				"Voulez-vous l'ouvrir avec l'application par défaut ?", QMessageBox::Yes | QMessageBox::No);

		if (reponse == QMessageBox::Yes)
			QDesktopServices::openUrl(QUrl::fromLocalFile(fichier));
	}
}

void FenPrincipale::ouvrirFichierGenere(QString fichier)
{
	editeurDeCode->slotNouveau();
	editeurDeCode->setTextActuel(fichier);

	if (!Multiuso::openTabsList(pagesDuProgramme).contains("Éditeur de code"))
			accueil->openTab("Éditeur de code");

	setTabIndex(Multiuso::tabIndexOf("Éditeur de code", pagesDuProgramme));
}

void FenPrincipale::slotTelechargements()
{
	if (telechargements->isHidden())
	{
		actionTelechargements->setDisabled(true);
		telechargements->show();
	}

	else
	{
		actionTelechargements->setDisabled(false);
		telechargements->hide();
	}
}

void FenPrincipale::slotDesactiverIconeTelechargements()
{
	actionTelechargements->setDisabled(true);
}

void FenPrincipale::verifierMAJ()
{
	QNetworkRequest requete(QUrl(QCoreApplication::organizationDomain() + "maj/maj.ini"));

	QNetworkAccessManager *manager = new QNetworkAccessManager;

	fichierMaj = manager->get(requete);

	verificationMaj = new QProgressDialog("Vérification des mises à jour en cours...", "Annuler", 0, 100, this);
		verificationMaj->setWindowTitle("Multiuso - Vérification des mises à jour");

	connect(fichierMaj, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(verificationMajProgresse(qint64, qint64)));
	connect(fichierMaj, SIGNAL(finished()), this, SLOT(comparerMaj()));
	connect(fichierMaj, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(erreurDL(QNetworkReply::NetworkError)));
	connect(verificationMaj, SIGNAL(canceled()), this, SLOT(annulerVerificationMaj()));
}

void FenPrincipale::verificationMajProgresse(qint64 recu, qint64 total)
{
	if (total != -1)
	{
		verificationMaj->setRange(0, total);
		verificationMaj->setValue(recu);
	}
}

void FenPrincipale::comparerMaj()
{
	QFile tmp(Multiuso::tempPath() + "/MAJ.ini");

		if (tmp.exists())
			tmp.remove();

		tmp.open(QIODevice::WriteOnly | QIODevice::Truncate);
		tmp.write(fichierMaj->readAll());
		tmp.close();

		QSettings derVersion(Multiuso::tempPath() + "/MAJ.ini", QSettings::IniFormat);
			derniereMaj = derVersion.value("version_actuelle").toString();

		tmp.remove();

	QString versionActuelle = QCoreApplication::applicationVersion();

	if (derniereMaj != versionActuelle)
	{
		if (!derniereMaj.isEmpty())
		{
			if (derniereMaj.left(1) == versionActuelle.left(1)) // versionActuelle.left(1) retourne la "tranche" de version, ex: |2|.0.0
				MajDisponible();
		}
	}

	else
	{
		if (confirmationVerificationMaj)
			QMessageBox::information(this, "Multiuso", "Aucune nouvelle version n'est disponible !");
	}

	confirmationVerificationMaj = true;
}

void FenPrincipale::annulerVerificationMaj()
{
	fichierMaj->abort();
	verificationMaj->close();
}

void FenPrincipale::MajDisponible()
{
	int reponse = QMessageBox::question(this, "Multiuso", "La version " + derniereMaj + " de Multiuso est disponible !\n"
			"Voulez-vous la télécharger maintenant ?", QMessageBox::Yes | QMessageBox::No);

	if (reponse == QMessageBox::Yes)
	{
		QString os = Multiuso::currentOS();

		QNetworkRequest requete(QUrl(QCoreApplication::organizationDomain() + "telechargements_" + os + "/Multiuso_" + derniereMaj.left(7) + ".zip"));

		QFile fichier(requete.url().toString());

		QFileInfo infosFichier(fichier);

		emplacementTelechargementNouvelleVersion = QFileDialog::getSaveFileName(this, "Multiuso",
				Multiuso::lastPath() + "/" + infosFichier.fileName(), "Fichier (*)");

		Multiuso::setLastPath(emplacementTelechargementNouvelleVersion);

		if (emplacementTelechargementNouvelleVersion.isEmpty())
			return;

		QNetworkAccessManager *manager = new QNetworkAccessManager;

		nouvelleVersion = manager->get(requete);

		telechargerNouvelleVersion = new QProgressDialog("Téléchargement de la nouvelle version en cours...", "Annuler", 0, 100, this);
			telechargerNouvelleVersion->setWindowTitle("Multiuso - Téléchargement de la nouvelle version");

		connect(nouvelleVersion, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(telechargementNouvelleVersionContinue(qint64, qint64)));
		connect(nouvelleVersion, SIGNAL(finished()), this, SLOT(telechargementNouvelleVersionFini()));
		connect(telechargerNouvelleVersion, SIGNAL(canceled()), this, SLOT(arreterTelechargementNouvelleVersion()));
	}
}

void FenPrincipale::erreurDL(QNetworkReply::NetworkError)
{
	QSettings connecteReseau(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);
		connecteReseau.setValue("reseau/internet", false);
}

void FenPrincipale::telechargementNouvelleVersionContinue(qint64 recu, qint64 total)
{
	if (total != -1)
	{
		telechargerNouvelleVersion->setRange(0, total);
		telechargerNouvelleVersion->setValue(recu);
	}
}

void FenPrincipale::telechargementNouvelleVersionFini()
{
	QFile fichier(emplacementTelechargementNouvelleVersion);

		if (fichier.exists())
			fichier.remove();

		fichier.open(QIODevice::WriteOnly | QIODevice::Truncate);
		fichier.write(nouvelleVersion->readAll());
		fichier.close();

	int answer = QMessageBox::question(this, "Multiuso", "La nouvelle version a été téléchargée avec succès !<br />"
			"Voulez-vous fermer Multiuso et lancer l'assistant de mise à jour ?",
			QMessageBox::Yes | QMessageBox::No);

	if (answer == QMessageBox::Yes)
	{
		QProcess::startDetached(Multiuso::openCommand() + Multiuso::appDirPath() + "/Updater" + Multiuso::currentSuffix());

		Multiuso::quit();
	}
}

void FenPrincipale::arreterTelechargementNouvelleVersion()
{
	nouvelleVersion->abort();
	telechargerNouvelleVersion->close();
}

void FenPrincipale::assistantDeMiseAJour()
{
	int answer = QMessageBox::question(this, "Multiuso", "Voulez-vous fermer Multiuso et lancer l'assistant de mise à jour ?",
			QMessageBox::Yes | QMessageBox::No);

	if (answer == QMessageBox::Yes)
	{
		QProcess::startDetached(Multiuso::openCommand() + Multiuso::appDirPath() + "/Updater" + Multiuso::currentSuffix());

		Multiuso::quit();
	}
}

QTabWidget *FenPrincipale::tabWidget()
{
	return pagesDuProgramme;
}

Accueil *FenPrincipale::homeTab()
{
	return accueil;
}

void FenPrincipale::setTabIndex(int index)
{
	pagesDuProgramme->setCurrentIndex(index);
}

void FenPrincipale::showTab(QString tab, bool show)
{
	QString tabName = Multiuso::getFullNameOf(tab);

	if (tabName == "")
		return;

	if (show)
	{
		if (tab == "filebrowser")
			pagesDuProgramme->addTab(tabNavFichiers, tabName);

		else if (tab == "widgets")
			pagesDuProgramme->addTab(tabWidgets, tabName);

		else if (tab == "textedit")
			pagesDuProgramme->addTab(tabEditeurDeTexte, tabName);

		else if (tab == "codeedit")
			pagesDuProgramme->addTab(tabEditeurDeCode, tabName);

		else if (tab == "ftp")
			pagesDuProgramme->addTab(tabFtp, tabName);

		else if (tab == "webbrowser")
			pagesDuProgramme->addTab(tabNavigateur, tabName);

		else if (tab == "pictureviewer")
			pagesDuProgramme->addTab(tabVisionneurImages, tabName);

		setTabIndex(Multiuso::tabIndexOf(tabName, pagesDuProgramme));
	}

	else
	{
		if (Multiuso::tabIndexOf(tabName, pagesDuProgramme) != -1)
			pagesDuProgramme->removeTab(Multiuso::tabIndexOf(tabName, pagesDuProgramme));
	}
}

void FenPrincipale::openTab(QString tab)
{
	accueil->openTab(tab);
}
