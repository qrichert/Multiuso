/*

Copyright © 2009-2010 Quentin RICHERT
Copyright © 2010 Stalker (www.siteduzero.com)

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

#include "NavigateurWeb.h"
#include "autresClasses/Cookies.h"
#include "autresClasses/SpeedDial.h"
#include "autresClasses/ConfigurerFavoris.h"
#include "classesHighlighters/HighlighterHTML.h"
#include "classesPrincipales/FenPrincipale.h"
#include "autresClasses/LoginDialog.h"

NavigateurWeb::NavigateurWeb(QWidget *parent, TelechargerFichier *telechargements, EditeurDeCode *editeurDeCode) : QMainWindow(parent)
{
	QDir cacheDir(Multiuso::appDirPath() + "/navigateurWeb/cache"); // cache

	if (!cacheDir.exists())
		cacheDir.mkdir(Multiuso::appDirPath() + "/navigateurWeb/cache");

	QWebSettings::setIconDatabasePath(Multiuso::appDirPath() + "/navigateurWeb/cache/");

	QDir autreDir(Multiuso::appDirPath() + "/navigateurWeb/autre"); // autre

	if (!autreDir.exists())
		autreDir.mkdir(Multiuso::appDirPath() + "/navigateurWeb/autre");

	QDir favorisDir(Multiuso::appDirPath() + "/navigateurWeb/favoris"); // favoris

	if (!favorisDir.exists())
		favorisDir.mkdir(Multiuso::appDirPath() + "/navigateurWeb/favoris");

	creerActions();

	QFile completer(Multiuso::appDirPath() + "/navigateurWeb/autre/completer.mltscompleter");

		if (completer.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream in(&completer);
				in.setCodec("UTF-8");

			while (!in.atEnd())
				sitesVisites << in.readLine();
		}

	completer.close();

	QCompleter *completerBarreAdresse = new QCompleter(sitesVisites, this);
		completerBarreAdresse->setCaseSensitivity(Qt::CaseInsensitive);

	barreAdresse->setCompleter(completerBarreAdresse);

	lastLink = "";

	menuNavigation = new QMenu("Navigation", this);
		menuNavigation->addAction(actionNouveau);
		menuNavigation->addAction(actionFermer);
		menuNavigation->addAction(actionSpeedDial);
		menuNavigation->addAction(actionCodeSource);
		menuNavigation->addAction(actionOuvrirCode);
		menuNavigation->addAction(actionHistorique);
		menuNavigation->addSeparator();
		menuNavigation->addAction(actionOuvrir);
		menuNavigation->addAction(actionRechercher);
		menuNavigation->addSeparator();
		menuNavigation->addAction(actionZoomPlus);
		menuNavigation->addAction(actionZoomEgal);
		menuNavigation->addAction(actionZoomMoins);

	QAction *actionNavigation = new QAction("Navigation", this);
		actionNavigation->setIcon(QIcon(":/icones/navigateur_web/actionNavigation.png"));
		actionNavigation->setMenu(menuNavigation);
		connect(actionNavigation, SIGNAL(triggered()), this, SLOT(slotOuvrirMenuNavigation()));

	QToolBar *toolBarNavigation = addToolBar("Navigation");
		toolBarNavigation->addAction(actionNavigation);
		toolBarNavigation->addSeparator();
		toolBarNavigation->addAction(actionPrecedent);
		toolBarNavigation->addAction(actionSuivant);
		toolBarNavigation->addAction(actionRecharger);
		toolBarNavigation->addAction(actionStop);
		toolBarNavigation->addAction(actionAccueil);
		toolBarNavigation->addSeparator();
		toolBarNavigation->addWidget(barreAdresse);
		toolBarNavigation->addAction(actionGo);
		toolBarNavigation->addAction(ajouterFavori);
		toolBarNavigation->setObjectName("Navigation");

	actionCacherRecherche = new QAction("", this);
		actionCacherRecherche->setIcon(QIcon(":/icones/navigateur_web/actionCacherRecherche.png"));
		connect(actionCacherRecherche, SIGNAL(triggered()), this, SLOT(slotCacherRecherche()));

	QLabel *labelRecherche = new QLabel("Rechercher : ");

	motARechercher = new QLineEdit;
		motARechercher->setFixedWidth(200);
		connect(motARechercher, SIGNAL(returnPressed()), this, SLOT(slotMotSuivant()));

	actionMotPrecedent = new QAction("&Précédent", this);
		actionMotPrecedent->setIcon(QIcon(":/icones/navigateur_web/actionMotPrecedent.png"));
		connect(actionMotPrecedent, SIGNAL(triggered()), this, SLOT(slotMotPrecedent()));

	actionMotSuivant = new QAction("&Suivant", this);
		actionMotSuivant->setIcon(QIcon(":/icones/navigateur_web/actionMotSuivant.png"));
		connect(actionMotSuivant, SIGNAL(triggered()), this, SLOT(slotMotSuivant()));

	respecterCasse = new QCheckBox("&Respecter la casse");
		respecterCasse->setCheckState(Qt::Unchecked);

	toolBarRechercher = new QToolBar("Rechercher");
		toolBarRechercher->setMovable(false);
		toolBarRechercher->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		toolBarRechercher->addAction(actionCacherRecherche);
		toolBarRechercher->addSeparator();
		toolBarRechercher->addWidget(labelRecherche);
		toolBarRechercher->addWidget(motARechercher);
		toolBarRechercher->addAction(actionMotPrecedent);
		toolBarRechercher->addAction(actionMotSuivant);
		toolBarRechercher->addSeparator();
		toolBarRechercher->addWidget(respecterCasse);
		toolBarRechercher->setObjectName("Rechercher");
			addToolBar(Qt::BottomToolBarArea, toolBarRechercher);

	QFile historique(Multiuso::appDirPath() + "/navigateurWeb/autre/historique.mltshistory");

		if (historique.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream inH(&historique);
				inH.setCodec("UTF-8");

			while (!inH.atEnd())
				liensHistorique << inH.readLine();
		}

	historique.close();

	modeleHistorique = new QStringListModel(liensHistorique);

	vueHistorique = new QListView;
		vueHistorique->setModel(modeleHistorique);
		connect(vueHistorique, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotOuvrirHistorique(QModelIndex)));

	QPushButton *effacerHistorique = new QPushButton("Effacer l'historique");
		connect(effacerHistorique, SIGNAL(clicked()), this, SLOT(slotEffacerHistorique()));

	QVBoxLayout *layoutHistorique = new QVBoxLayout;
		layoutHistorique->addWidget(vueHistorique);
		layoutHistorique->addWidget(effacerHistorique);
		layoutHistorique->setContentsMargins(0, 0, 0, 0);

	QWidget *widgetHistorique = new QWidget;
		widgetHistorique->setLayout(layoutHistorique);

	dockHistorique = new QDockWidget("Historique", this);
		dockHistorique->setWidget(widgetHistorique);
		dockHistorique->setObjectName("Historique");
		addDockWidget(Qt::LeftDockWidgetArea, dockHistorique);
		dockHistorique->hide();

	QToolButton *buttonCloseTab = new QToolButton;
		buttonCloseTab->setDefaultAction(actionFermer);
		buttonCloseTab->setAutoRaise(true);

	QToolButton *buttonNewTab = new QToolButton;
		buttonNewTab->setDefaultAction(actionNouveau);
		buttonNewTab->setAutoRaise(true);

	onglets = new QTabWidget;
		onglets->setMovable(true);
		onglets->setDocumentMode(true);
		onglets->setCornerWidget(buttonNewTab, Qt::BottomLeftCorner);
		onglets->setCornerWidget(buttonCloseTab, Qt::BottomRightCorner);
		connect(onglets, SIGNAL(tabCloseRequested(int)), this, SLOT(slotFermerOnglet(int)));
		connect(onglets, SIGNAL(currentChanged(int)), this, SLOT(ongletChange(int)));
		slotNouvelOnglet();
		onglets->setTabsClosable(false);

	progression = new QProgressBar;
		progression->setMinimum(0);
		progression->setMaximum(100);
		progression->setFixedSize(250, 15);
		progression->hide();

	termine = new QLabel("Terminé ");

	statut = statusBar();
		statut->addPermanentWidget(progression);
		statut->addPermanentWidget(termine);

	QVBoxLayout *layoutNavigateur = new QVBoxLayout;
		layoutNavigateur->addWidget(onglets);
		layoutNavigateur->setContentsMargins(0, 0, 0, 0);

	QWidget *widgetCentral = new QWidget;
		widgetCentral->setLayout(layoutNavigateur);

	setCentralWidget(widgetCentral);

	QSettings restaurer(Multiuso::appDirPath() + "/ini/navigateur.ini", QSettings::IniFormat);
		restoreState(restaurer.value("etat_fenetre").toByteArray());

	actualiserFavoris();

	pointeurSurParent = parent;
	pointeurSurTelechargements = telechargements;
	pointeurSurEditeurDeCode = editeurDeCode;

	// <Stalker (www.siteduzero.com)>
		httpConnexion = new QHttp(this);
		proxy = new QNetworkProxy;
	// </Stalker (www.siteduzero.com)>

	// <Easter Egg>

		easterEggMatrix = new EasterEggMatrix(this);

	// </Easter Egg>

	if (restaurer.value("restaurer_onglets").toBool())
	{
		QStringList links = restaurer.value("derniers_onglets").value<QStringList>(); // QSettings::value() → QVariant::value()

		for (int i = 0; i < links.size(); i++)
		{
			if (i != 0)
				slotNouvelOnglet();

			slotOuvrirUrl(links.value(i));
		}

		onglets->setCurrentIndex(restaurer.value("index_dernier_onglet").toInt());
	}
}

void NavigateurWeb::creerActions()
{
	actionNouveau = new QAction("Nouvel onglet", this);
		actionNouveau->setIcon(QIcon(":/icones/navigateur_web/actionNouveau.png"));
		actionNouveau->setShortcut(QKeySequence("Ctrl+T"));
		connect(actionNouveau, SIGNAL(triggered()), this, SLOT(speedDial()));

	actionFermer = new QAction("Fermer l'onglet", this);
		actionFermer->setIcon(QIcon(":/icones/navigateur_web/actionFermer.png"));
		actionFermer->setShortcut(QKeySequence("Ctrl+W"));
		actionFermer->setEnabled(false);
		connect(actionFermer, SIGNAL(triggered()), this, SLOT(slotFermerOnglet()));

	actionSpeedDial = new QAction("Speed dial", this);
		actionSpeedDial->setIcon(QIcon(":/icones/navigateur_web/actionSpeedDial.png"));
		actionSpeedDial->setShortcut(QKeySequence("Ctrl+S"));
		connect(actionSpeedDial, SIGNAL(triggered()), this, SLOT(speedDial()));

	actionCodeSource = new QAction("Code source", this);
		actionCodeSource->setIcon(QIcon(":/icones/navigateur_web/actionCodeSource.png"));
		actionCodeSource->setShortcut(QKeySequence("Ctrl+U"));
		connect(actionCodeSource, SIGNAL(triggered()), this, SLOT(slotCodeSource()));

	actionOuvrirCode = new QAction("Ouvrir dans l'éditeur de code", this);
		actionOuvrirCode->setIcon(QIcon(":/icones/navigateur_web/actionOuvrirCode.png"));
		actionOuvrirCode->setShortcut(QKeySequence("Ctrl+Shift+O"));
		connect(actionOuvrirCode, SIGNAL(triggered()), this, SLOT(slotOuvrirCode()));

	actionHistorique = new QAction("Historique", this);
		actionHistorique->setIcon(QIcon(":/icones/navigateur_web/actionHistorique.png"));
		actionHistorique->setShortcut(QKeySequence("Ctrl+Shift+H"));
		connect(actionHistorique, SIGNAL(triggered()), this, SLOT(slotHistorique()));

	actionOuvrir = new QAction("Ouvrir", this);
		actionOuvrir->setIcon(QIcon(":/icones/navigateur_web/actionOuvrir.png"));
		actionOuvrir->setShortcut(QKeySequence("Ctrl+O"));
		connect(actionOuvrir, SIGNAL(triggered()), this, SLOT(slotSelectionFichierPourOuvrir()));

	actionRechercher = new QAction("Rechercher", this);
		actionRechercher->setIcon(QIcon(":/icones/navigateur_web/actionRechercher.png"));
		actionRechercher->setShortcut(QKeySequence("Ctrl+F"));
		connect(actionRechercher, SIGNAL(triggered()), this, SLOT(slotRechercher()));

	actionZoomPlus = new QAction("Zoom : +", this);
		actionZoomPlus->setIcon(QIcon(":/icones/navigateur_web/actionZoomPlus.png"));
		actionZoomPlus->setShortcut(QKeySequence("Ctrl++"));
		connect(actionZoomPlus, SIGNAL(triggered()), this, SLOT(slotZoomPlus()));

	actionZoomEgal = new QAction("Zoom : =", this);
		actionZoomEgal->setIcon(QIcon(":/icones/navigateur_web/actionZoomEgal.png"));
		actionZoomEgal->setShortcut(QKeySequence("Ctrl+0"));
		connect(actionZoomEgal, SIGNAL(triggered()), this, SLOT(slotZoomEgal()));

	actionZoomMoins = new QAction("Zoom : -", this);
		actionZoomMoins->setIcon(QIcon(":/icones/navigateur_web/actionZoomMoins.png"));
		actionZoomMoins->setShortcut(QKeySequence("Ctrl+-"));
		connect(actionZoomMoins, SIGNAL(triggered()), this, SLOT(slotZoomMoins()));

	actionPrecedent = new QAction("Précédent", this);
		actionPrecedent->setIcon(QIcon(":/icones/navigateur_web/actionPrecedent.png"));
		connect(actionPrecedent, SIGNAL(triggered()), this, SLOT(slotPrecedent()));

	actionSuivant = new QAction("Suivant", this);
		actionSuivant->setIcon(QIcon(":/icones/navigateur_web/actionSuivant.png"));
		connect(actionSuivant, SIGNAL(triggered()), this, SLOT(slotSuivant()));

	actionRecharger = new QAction("Recharger (Ctrl + R)", this);
		actionRecharger->setIcon(QIcon(":/icones/navigateur_web/actionRecharger.png"));
		actionRecharger->setShortcut(QKeySequence("Ctrl+R"));
		actionRecharger->setVisible(true);
		connect(actionRecharger, SIGNAL(triggered()), this, SLOT(slotRecharger()));

	actionStop = new QAction("Stop (Ctrl + X)", this);
		actionStop->setIcon(QIcon(":/icones/navigateur_web/actionStop.png"));
		actionStop->setShortcut(QKeySequence("Ctrl+X"));
		actionStop->setVisible(false);
		connect(actionStop, SIGNAL(triggered()), this, SLOT(slotStop()));

	actionAccueil = new QAction("Accueil", this);
		actionAccueil->setIcon(QIcon(":/icones/navigateur_web/actionAccueil.png"));
		connect(actionAccueil, SIGNAL(triggered()), this, SLOT(slotAccueil()));

	barreAdresse = new LineEdit("about:blank");
		barreAdresse->setPlaceholderText("Entrez l'adresse d'une page Web.");
		connect(barreAdresse, SIGNAL(returnPressed()), this, SLOT(slotChangerAdresse()));

	actionGo = new QAction("Go !", this);
		actionGo->setIcon(QIcon(":/icones/navigateur_web/actionGo.png"));
		connect(actionGo, SIGNAL(triggered()), this, SLOT(slotChangerAdresse()));

	screen = new WebPageScreenshot;
		connect(screen, SIGNAL(screenshotTaken(QString, QString, QPixmap)), this, SLOT(slotScreenshotTaken(QString, QString, QPixmap)));
		connect(screen, SIGNAL(screenshotNotTaken()), this, SLOT(slotScreenshotNotTaken()));

	actionTakeScreenshot = new QAction("Capturer la page Web...", this);
		actionTakeScreenshot->setIcon(QIcon(":/icones/navigateur_web/actionTakeScreenshot.png"));
		connect(actionTakeScreenshot, SIGNAL(triggered()), this, SLOT(slotTakeScreenshot()));

	menuFavoris = new QMenu(this);
		menuFavoris->setTearOffEnabled(true);

	ajouterFavori = new QAction("Ajouter aux favoris", this);
		ajouterFavori->setIcon(QIcon(":/icones/navigateur_web/ajouterFavori.png"));
		ajouterFavori->setShortcut(QKeySequence("Ctrl+D"));
		ajouterFavori->setMenu(menuFavoris);
		connect(ajouterFavori, SIGNAL(triggered()), this, SLOT(slotAjouterFavori()));
}

void NavigateurWeb::slotFermerOnglet(int onglet)
{
	if (onglets->count() > 1)
	{
		onglets->widget(onglet)->findChild<QWebView *>()->stop();
		onglets->widget(onglet)->deleteLater();
		onglets->removeTab(onglet);
	}

	if (onglets->count() <= 1)
	{
		onglets->setTabsClosable(false);
		actionFermer->setEnabled(false);
	}
}

void NavigateurWeb::speedDial()
{
	SpeedDial *pageWeb = new SpeedDial;
		pageWeb->setContextMenuPolicy(Qt::CustomContextMenu);
		pageWeb->page()->setContentEditable(false);
		pageWeb->page()->setForwardUnsupportedContent(false);

		pageWeb->page()->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
		pageWeb->page()->settings()->setAttribute(QWebSettings::ZoomTextOnly, false);
		pageWeb->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

		connect(pageWeb, SIGNAL(openUrlRequested(QString)), this, SLOT(slotOuvrirUrlSpeedDial(QString)));

		connect(pageWeb, SIGNAL(iconChanged()), this, SLOT(changementIcone()));
		connect(pageWeb, SIGNAL(loadFinished(bool)), this, SLOT(chargementFini(bool)));
		connect(pageWeb, SIGNAL(loadProgress(int)), this, SLOT(chargementContinue(int)));
		connect(pageWeb, SIGNAL(loadStarted()), this, SLOT(debutChargement()));
		connect(pageWeb, SIGNAL(titleChanged(QString)), this, SLOT(changementDeTitre(QString)));
		connect(pageWeb, SIGNAL(urlChanged(QUrl)), this, SLOT(changementUrl(QUrl)));
		connect(pageWeb, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotMenuPage(QPoint)));
		connect(pageWeb->page(), SIGNAL(printRequested(QWebFrame *)), this, SLOT(slotImprimer(QWebFrame *)));

	QShortcut *copier = new QShortcut(QKeySequence("Ctrl+C"), pageWeb);
		connect(copier, SIGNAL(activated()), this, SLOT(slotCopier()));

	QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(pageWeb);
		layout->setContentsMargins(0, 0, 0, 0);

	QWidget *page = new QWidget;
		page->setLayout(layout);


	onglets->addTab(page, "Speed dial");
	onglets->setCurrentIndex(onglets->count() - 1);
	onglets->setTabIcon(onglets->currentIndex(), QIcon(":/icones/navigateur_web/actionSpeedDial.png"));

	if (onglets->count() > 1)
	{
		onglets->setTabsClosable(true);
		actionFermer->setEnabled(true);
	}
}

void NavigateurWeb::slotOuvrirUrlSpeedDial(QString url)
{
	int index = onglets->currentIndex();

	onglets->insertTab(index + 1, nouvelOnglet(), "(Sans titre)");
	onglets->setCurrentIndex(index);
	onglets->setTabIcon(onglets->currentIndex(), faviconUrl(pageActuelle()->url().toString()));

	onglets->currentWidget()->deleteLater();
	onglets->removeTab(index);

	barreAdresse->setFocus();

	slotOuvrirUrl(url);
}

QWebView *NavigateurWeb::pageActuelle()
{
	return onglets->currentWidget()->findChild<QWebView *>();
}

QWidget *NavigateurWeb::nouvelOnglet()
{
	QSettings connecteReseau(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);

	QSettings reglages(Multiuso::appDirPath() + "/ini/navigateur.ini", QSettings::IniFormat);

	QWebView *pageWeb = new QWebView;
		pageWeb->setContextMenuPolicy(Qt::CustomContextMenu);

		if (!connecteReseau.value("reseau/internet").toBool())
			pageWeb->load(QUrl(Multiuso::appDirPath() + "/navigateurWeb/html/pas_de_connexion.html"));

		else
			pageWeb->load(QUrl(reglages.value("page_accueil").toString()));

		pageWeb->page()->setContentEditable(reglages.value("contenu_editable").toBool());
		pageWeb->page()->setForwardUnsupportedContent(true);
		pageWeb->page()->networkAccessManager()->setCookieJar(new Cookies(this));

		pageWeb->page()->settings()->setAttribute(QWebSettings::JavascriptEnabled, reglages.value("settings/javascript").toBool());
		pageWeb->page()->settings()->setAttribute(QWebSettings::JavaEnabled, reglages.value("settings/java").toBool());
		pageWeb->page()->settings()->setAttribute(QWebSettings::PluginsEnabled, reglages.value("settings/flash").toBool());
		pageWeb->page()->settings()->setAttribute(QWebSettings::ZoomTextOnly, reglages.value("settings/zoom_text_only").toBool());
		pageWeb->page()->settings()->setAttribute(QWebSettings::PrintElementBackgrounds, reglages.value("settings/imprimer_elements_fond").toBool());
		pageWeb->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

		connect(pageWeb, SIGNAL(iconChanged()), this, SLOT(changementIcone()));
		connect(pageWeb, SIGNAL(loadFinished(bool)), this, SLOT(chargementFini(bool)));
		connect(pageWeb, SIGNAL(loadProgress(int)), this, SLOT(chargementContinue(int)));
		connect(pageWeb, SIGNAL(loadStarted()), this, SLOT(debutChargement()));
		connect(pageWeb, SIGNAL(titleChanged(QString)), this, SLOT(changementDeTitre(QString)));
		connect(pageWeb, SIGNAL(urlChanged(QUrl)), this, SLOT(changementUrl(QUrl)));
		connect(pageWeb, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotMenuPage(QPoint)));
		connect(pageWeb, SIGNAL(linkClicked(QUrl)), this, SLOT(slotLinkClicked(QUrl)));
		connect(pageWeb->page(), SIGNAL(unsupportedContent(QNetworkReply *)), this, SLOT(telechargerFichierAuto(QNetworkReply *)));
		connect(pageWeb->page(), SIGNAL(downloadRequested(QNetworkRequest)), this, SLOT(telechargerFichier(QNetworkRequest)));
		connect(pageWeb->page(), SIGNAL(linkHovered(QString, QString, QString)), this, SLOT(survolLien(QString, QString, QString)));
		connect(pageWeb->page(), SIGNAL(printRequested(QWebFrame *)), this, SLOT(slotImprimer(QWebFrame *)));
		connect(pageWeb->page(), SIGNAL(databaseQuotaExceeded(QWebFrame *, QString)), this, SLOT(slotDatabaseQuotaExceeded(QWebFrame *, QString)));
		connect(pageWeb->page()->networkAccessManager(), SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)), this, SLOT(authenticationRequired(QNetworkReply *, QAuthenticator *)));
		connect(pageWeb->page()->networkAccessManager(), SIGNAL(proxyAuthenticationRequired(QNetworkProxy, QAuthenticator *)), this, SLOT(proxyAuthenticationRequired(QNetworkProxy, QAuthenticator *)));

		QUrl url = barreAdresse->text();

		if (QSettings(Multiuso::appDirPath() + "/ini/proxy.ini", QSettings::IniFormat).value("activer").toBool())
		{
			// <Stalker (www.siteduzero.com)>
				connect(pageWeb->page()->networkAccessManager(),SIGNAL(finished(QNetworkReply*)),this,SLOT(slotErrorPageCheck(QNetworkReply*)));
			// </Stalker (www.siteduzero.com)>
		}

	QNetworkDiskCache *cache = new QNetworkDiskCache(this);
		cache->setCacheDirectory(Multiuso::appDirPath() + "/navigateurWeb/cache/");

	pageWeb->page()->networkAccessManager()->setCache(cache);

	QShortcut *copier = new QShortcut(QKeySequence("Ctrl+C"), pageWeb);
		connect(copier, SIGNAL(activated()), this, SLOT(slotCopier()));

	QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(pageWeb);
		layout->setContentsMargins(0, 0, 0, 0);

	QWidget *page = new QWidget;
		page->setLayout(layout);

	return page;
}

QIcon NavigateurWeb::faviconUrl(QString url)
{
	return QWebSettings::iconForUrl(QUrl(url));
}

void NavigateurWeb::sauvegarderEtat()
{
	QSettings enregistrer(Multiuso::appDirPath() + "/ini/navigateur.ini", QSettings::IniFormat);
		enregistrer.setValue("etat_fenetre", saveState());

	QStringList lastTabs;

	for (int i = 0; i < onglets->count(); i++)
		lastTabs << onglets->widget(i)->findChild<QWebView *>()->url().toString();

		lastTabs.removeAll("about:blank");
		lastTabs.removeAll("speed-dial");

			if (lastTabs.isEmpty())
				lastTabs << enregistrer.value("page_accueil").toString();

		enregistrer.setValue("derniers_onglets", lastTabs);
		enregistrer.setValue("index_dernier_onglet", onglets->currentIndex());
}

void NavigateurWeb::slotEffacerHistorique()
{
	QFile::remove(Multiuso::appDirPath() + "/navigateurWeb/autre/historique.mltshistory");

	QFile::remove(Multiuso::appDirPath() + "/navigateurWeb/autre/completer.mltscompleter");

	actualiserHistoriqueEfface();

	QMessageBox::information(this, "Multiuso", "L'historique a été effacé !");
}

void NavigateurWeb::actualiserHistoriqueEfface()
{
	sitesVisites.clear();
	barreAdresse->completer()->deleteLater();
	barreAdresse->setCompleter(new QCompleter(barreAdresse));

	liensHistorique.clear();
	modeleHistorique->setStringList(liensHistorique);
}

void NavigateurWeb::actualiserFavoris()
{
	while (!menuFavoris->actions().isEmpty())
		menuFavoris->removeAction(menuFavoris->actions().first());

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

			QAction *action = new QAction(nomCourt, this);
				action->setIcon(faviconUrl(lien));
				action->setStatusTip(lien);
				connect(action, SIGNAL(triggered()), this, SLOT(slotOuvrirFavori()));

			menuFavoris->addAction(action);
		}

		favori.close();
	}

	menuFavoris->addSeparator();

	QAction *configurerFavoris = new QAction("&Configurer les favoris", this);
		configurerFavoris->setIcon(QIcon(":/icones/actions/actionPreferences.png"));
		connect(configurerFavoris, SIGNAL(triggered()), this, SLOT(configurerFavoris()));
			menuFavoris->addAction(configurerFavoris);

	menuFavoris->addSeparator();

	QAction *actionPut = new QAction("Sauvegarder", this);
		actionPut->setIcon(QIcon(":/icones/navigateur_web/put.png"));
		connect(actionPut, SIGNAL(triggered()), this, SLOT(putSafeguard()));
			menuFavoris->addAction(actionPut);

	QAction *actionGet = new QAction("Rétablir à la dernière sauvegarde", this);
		actionGet->setIcon(QIcon(":/icones/navigateur_web/get.png"));
		connect(actionGet, SIGNAL(triggered()), this, SLOT(getSafeguard()));
			menuFavoris->addAction(actionGet);
}

void NavigateurWeb::configurerFavoris()
{
	ConfigurerFavoris *configurerFavoris = new ConfigurerFavoris(this);
		configurerFavoris->exec();
		configurerFavoris->deleteLater();
}

QPixmap NavigateurWeb::capturerPage()
{
	return QPixmap::grabWidget(pageActuelle());
}

void NavigateurWeb::putSafeguard()
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

	QString text;

	foreach (QString bookmark_file, QDir(Multiuso::appDirPath() + "/navigateurWeb/favoris").entryList())
	{
		if (bookmark_file == "." || bookmark_file == "..")
			continue;

		QFile file(Multiuso::appDirPath() + "/navigateurWeb/favoris/" + QFileInfo(bookmark_file).fileName());
			file.open(QIODevice::ReadOnly | QIODevice::Text);
				text += QFileInfo(bookmark_file).fileName() + "/" + QString(file.readAll()) + "\n";
			file.close();
	}
		
	text.replace("&", "|0088amp;|");
	text.replace("#", "|0089n;|");

	QNetworkRequest request(QCoreApplication::organizationDomain() + "bookmarks.php?request=put"
								"&pseudo=" + pseudo +
								"&pwd=" + password +
								"&text=" + text);

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	r_put = manager->get(request);
		connect(r_put, SIGNAL(finished()), this, SLOT(putSafeguardReply()));
		connect(r_put, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(putSafeguardReply(QNetworkReply::NetworkError)));
}

void NavigateurWeb::putSafeguardReply()
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
}

void NavigateurWeb::putSafeguardReply(QNetworkReply::NetworkError)
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

	if (reply != 0)
	{
		reply->abort();
		reply->deleteLater();
	}

	QMessageBox::critical(this, "Multiuso", "Impossible d'accéder à la page de sauvegarde, réessayez plus tard.");
}

void NavigateurWeb::getSafeguard()
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
	
	QNetworkRequest request(QCoreApplication::organizationDomain() + "bookmarks.php?request=get"
								"&pseudo=" + pseudo +
								"&pwd=" + password);

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);

	r_get = manager->get(request);
		connect(r_get, SIGNAL(finished()), this, SLOT(getSafeguardReply()));
		connect(r_get, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(getSafeguardReply(QNetworkReply::NetworkError)));
}

void NavigateurWeb::getSafeguardReply()
{
	QString text = r_get->readAll();
		r_get->deleteLater();

	QRegExp rx("^<head>(.+)</head>\n(.+)");
		rx.setMinimal(true);

	text.replace(rx, "\\2");
		
	if (text.startsWith("ERROR:1"))
	{
		QMessageBox::critical(this, "Multiuso", "Pseudo ou mot de passe incorrect !");

		return;
	}
	
	text.replace("|0089n;|", "#");
	text.replace("|0088amp;|", "&");
	
	foreach (QString bookmark_file, QDir(Multiuso::appDirPath() + "/navigateurWeb/favoris").entryList())
	{
		if (bookmark_file == "." || bookmark_file == "..")
			continue;

		QFile::remove(Multiuso::appDirPath() + "/navigateurWeb/favoris/" + QFileInfo(bookmark_file).fileName());
	}

	foreach (QString line, text.split("\n"))
	{
		if (line.isEmpty())
			continue;
		
		QString name = line;
			name.replace(QRegExp("(.+)mltsbookmark(.+)"), "\\1mltsbookmark");

		QString url = line;
			url.replace(QRegExp("(.+)http(.+)"), "http\\2");

		QFile file(Multiuso::appDirPath() + "/navigateurWeb/favoris/" + name);
			file.open(QIODevice::WriteOnly | QIODevice::Text);

			QTextStream out(&file);
				out.setCodec("UTF-8");
				out << url;

			file.close();
	}

	actualiserFavoris();
}

void NavigateurWeb::getSafeguardReply(QNetworkReply::NetworkError)
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

	if (reply != 0)
	{
		reply->abort();
		reply->deleteLater();
	}

	QMessageBox::critical(this, "Multiuso", "Impossible d'accéder à la page de sauvegarde, réessayez plus tard.");
}

void NavigateurWeb::slotOuvrirFavori()
{
	QAction *action = qobject_cast<QAction *>(sender());

	QString url = action->statusTip();
		url.replace(QRegExp(" {1,}"), "");

	pageActuelle()->load(QUrl(url));
}

void NavigateurWeb::slotOuvrirUrl(QString url)
{
	if (url.isEmpty())
		return;

	url.replace(QRegExp(" {1,}"), "");

	if (!url.contains(QRegExp("^(.+)://")))
	{
		if (!url.contains(QRegExp("\\.")))
		{
			slotRechercherMoteurRecherche();

			return;
		}

		url = "http://" + url;
	}

	pageActuelle()->load(QUrl(url));
}

void NavigateurWeb::slotOpenLinkInNewTab()
{
	QAction *action = qobject_cast<QAction *>(sender());

	if (action == 0)
		return;

	slotNouvelOnglet();
	slotOuvrirUrl(action->objectName());
}

void NavigateurWeb::slotCopier()
{
	pageActuelle()->page()->triggerAction(QWebPage::Copy);
}

void NavigateurWeb::slotNouvelOnglet()
{
	onglets->addTab(nouvelOnglet(), "(Sans titre)");
	onglets->setCurrentIndex(onglets->count() - 1);
	onglets->setTabIcon(onglets->currentIndex(), faviconUrl(pageActuelle()->url().toString()));

	if (onglets->count() > 1)
	{
		onglets->setTabsClosable(true);
		actionFermer->setEnabled(true);
	}

	barreAdresse->setFocus();
}

void NavigateurWeb::slotFermerOnglet()
{
	slotFermerOnglet(onglets->currentIndex());
}

void NavigateurWeb::slotLinkClicked(QUrl url)
{
	int index = onglets->currentIndex();

	QFlags<Qt::KeyboardModifier> modifiers = qApp->keyboardModifiers();

	if (modifiers == Qt::ControlModifier || modifiers == (Qt::ControlModifier | Qt::AltModifier)) // Ctrl || Ctrl + Alt = New tab
		slotNouvelOnglet();

	pageActuelle()->load(url);

	if (modifiers == (Qt::ControlModifier | Qt::AltModifier)) // Ctrl + Alt = New tab but stay at sender's one
		onglets->setCurrentIndex(index);
}

void NavigateurWeb::slotCodeSource()
{
	QPlainTextEdit *codeSource = new QPlainTextEdit;
		codeSource->setPlainText(pageActuelle()->page()->mainFrame()->toHtml());
		codeSource->setReadOnly(true);
		codeSource->setLineWrapMode(QPlainTextEdit::NoWrap);

	HighlighterHTML *colorisationSyntaxiqueHTML = new HighlighterHTML(new QTextDocument);
		colorisationSyntaxiqueHTML->setDocument(codeSource->document());

	QVBoxLayout *layoutCodeSource = new QVBoxLayout;
		layoutCodeSource->addWidget(codeSource);
		layoutCodeSource->setContentsMargins(0, 0, 0, 0);

	QWidget *dialogueCodeSource = new QWidget;
		dialogueCodeSource->setWindowTitle("Code source de la page : " + pageActuelle()->url().toString());
		dialogueCodeSource->setWindowIcon(QIcon(":/icones/navigateur_web/actionCodeSource.png"));
		dialogueCodeSource->resize(600, 400);
		dialogueCodeSource->setLayout(layoutCodeSource);
		dialogueCodeSource->show();
}

void NavigateurWeb::slotHistorique()
{
	if (dockHistorique->isHidden())
		dockHistorique->show();

	else
		dockHistorique->hide();
}

void NavigateurWeb::slotOuvrirHistorique(QModelIndex index)
{
	pageActuelle()->load(QUrl(index.data().toString()));
}

void NavigateurWeb::slotOuvrirCode()
{
	pointeurSurEditeurDeCode->openWebPage(pageActuelle()->page()->mainFrame()->toHtml(), pageActuelle()->title());

	FenPrincipale *fen = qobject_cast<FenPrincipale *>(pointeurSurParent);

	if (!Multiuso::openTabsList(fen->tabWidget()).contains("Éditeur de code"))
		fen->homeTab()->openTab("Éditeur de code");

	fen->setTabIndex(Multiuso::tabIndexOf("Éditeur de code", fen->tabWidget()));
}

void NavigateurWeb::slotSelectionFichierPourOuvrir()
{
	QString fichier = QFileDialog::getOpenFileName(this, "Multiuso", Multiuso::lastPath(), "Tous les fichiers (*)");

	Multiuso::setLastPath(fichier);

	if (!fichier.isEmpty())
		slotOuvrirFichier(fichier);
}

void NavigateurWeb::slotOuvrirFichier(QString fichier)
{
	pageActuelle()->load(QUrl(fichier));
}

void NavigateurWeb::openCodeEditDocument(QString title, QString contents)
{
	pageActuelle()->setHtml(contents);
	onglets->setTabText(onglets->currentIndex(), title);
	onglets->setTabIcon(onglets->currentIndex(), QIcon());
}

void NavigateurWeb::slotRechercher()
{
	if (toolBarRechercher->isHidden())
		toolBarRechercher->show();

	else
		toolBarRechercher->hide();
}

void NavigateurWeb::slotCacherRecherche()
{
	toolBarRechercher->hide();
	actionRechercher->setDisabled(false);
}

void NavigateurWeb::slotMotPrecedent()
{
	QString mot = motARechercher->text();

	if (respecterCasse->isChecked())
	{
		if (pageActuelle()->findText(mot, QWebPage::FindBackward | QWebPage::FindCaseSensitively))
			motARechercher->setStyleSheet("background-color: #bbffbb;");

		else
			motARechercher->setStyleSheet("background-color: #ffb3b3;");
	}

	else
	{
		if (pageActuelle()->findText(mot, QWebPage::FindBackward))
			motARechercher->setStyleSheet("background-color: #bbffbb;");

		else
			motARechercher->setStyleSheet("background-color: #ffb3b3;");
	}
}

void NavigateurWeb::slotMotSuivant()
{
	QString mot = motARechercher->text();

	if (respecterCasse->isChecked())
	{
		if (pageActuelle()->findText(mot, QWebPage::FindCaseSensitively))
			motARechercher->setStyleSheet("background-color: #bbffbb;");

		else
			motARechercher->setStyleSheet("background-color: #ffb3b3;");
	}

	else
	{
		if (pageActuelle()->findText(mot))
			motARechercher->setStyleSheet("background-color: #bbffbb;");

		else
			motARechercher->setStyleSheet("background-color: #ffb3b3;");
	}
}


void NavigateurWeb::slotZoomPlus()
{
	pageActuelle()->setZoomFactor(pageActuelle()->zoomFactor() + 0.1);
}

void NavigateurWeb::slotZoomEgal()
{
	pageActuelle()->setZoomFactor(1);
}

void NavigateurWeb::slotZoomMoins()
{
	pageActuelle()->setZoomFactor(pageActuelle()->zoomFactor() - 0.1);
}

void NavigateurWeb::slotPrecedent()
{
	pageActuelle()->back();
}

void NavigateurWeb::slotSuivant()
{
	pageActuelle()->forward();
}

void NavigateurWeb::slotRecharger()
{
	pageActuelle()->reload();
}

void NavigateurWeb::slotStop()
{
	pageActuelle()->stop();
}

void NavigateurWeb::slotAccueil()
{
	QSettings reglages(Multiuso::appDirPath() + "/ini/navigateur.ini", QSettings::IniFormat);
		pageActuelle()->load(QUrl(reglages.value("page_accueil").toString()));
}

void NavigateurWeb::slotChangerAdresse()
{
	// <Stalker (www.siteduzero.com)>
		QSettings reglagesProxy(Multiuso::appDirPath() + "/ini/proxy.ini", QSettings::IniFormat);
		QUrl url = barreAdresse->text();
	// </Stalker (www.siteduzero.com)>

	if (barreAdresse->text() == "about:blank" || barreAdresse->text().isEmpty())
	{
		pageActuelle()->load(QUrl(Multiuso::appDirPath() + "/navigateurWeb/html/page_vierge.html"));
	}

	// <Easter Egg>

	else if (barreAdresse->text() == "about:matrix")
	{
		static bool showable = true;

		if (showable)
			easterEggMatrix->showText();

		showable = false;
	}

	// </Easter Egg>

	else if (barreAdresse->text() == "speed-dial")
	{
		speedDial();
	}

	else if (barreAdresse->text().left(11) == "javascript:")
	{
		QString js = barreAdresse->text().replace(QRegExp("javascript:(.+)"), "\\1");

		pageActuelle()->page()->mainFrame()->evaluateJavaScript(js);
	}

	// <Stalker (www.siteduzero.com)>
		else if (barreAdresse->text().left(7) == "http://")
		{
			if (reglagesProxy.value("activer",false).toBool())
			{
				proxy->setType(QNetworkProxy::HttpProxy);
				proxy->setHostName(reglagesProxy.value("adresse_http","").toString());
				proxy->setPort(reglagesProxy.value("port_http",80).toInt());
				QNetworkProxy::setApplicationProxy(*proxy);
			}

			else
			{
				proxy->setType(QNetworkProxy::NoProxy);
				QNetworkProxy::setApplicationProxy(*proxy);
			}

			httpConnexion = new QHttp(url.host(),QHttp::ConnectionModeHttp);

			slotOuvrirUrl(barreAdresse->text());
		}

		else if (barreAdresse->text().left(8) == "https://")
		{
			if (reglagesProxy.value("activer",false).toBool())
			{
				proxy->setType(QNetworkProxy::HttpProxy);
				proxy->setHostName(reglagesProxy.value("adresse_ssl","").toString());
				proxy->setPort(reglagesProxy.value("port_ssl",80).toInt());
				QNetworkProxy::setApplicationProxy(*proxy);
			}

			else
			{
				proxy->setType(QNetworkProxy::NoProxy);
				QNetworkProxy::setApplicationProxy(*proxy);
			}

			httpConnexion = new QHttp(url.host(),QHttp::ConnectionModeHttps);

			slotOuvrirUrl(barreAdresse->text());
		}

		else if (barreAdresse->text().left(6) == "ftp://")
		{
			if (reglagesProxy.value("activer",false).toBool())
			{
				proxy->setType(QNetworkProxy::FtpCachingProxy);
				proxy->setHostName(reglagesProxy.value("adresse_ftp","").toString());
				proxy->setPort(reglagesProxy.value("port_ftp",80).toInt());
				QNetworkProxy::setApplicationProxy(*proxy);
			}

			else
			{
				proxy->setType(QNetworkProxy::NoProxy);
				QNetworkProxy::setApplicationProxy(*proxy);
			}

			httpConnexion = new QHttp(url.host(),QHttp::ConnectionModeHttps);

			slotOuvrirUrl(barreAdresse->text());
		}
	// </Stalker (www.siteduzero.com)>

	else
	{
		slotOuvrirUrl(barreAdresse->text());
	}
}

void NavigateurWeb::slotRechercherMoteurRecherche()
{
	QSettings reglages(Multiuso::appDirPath() + "/ini/navigateur.ini", QSettings::IniFormat);

	QString moteur = reglages.value("moteur_de_recherche").toString();
	QString search = "";

	QString q = barreAdresse->text().replace(" ", "+");

	if (reglages.value("nouvel_onglet_recherche_moteur_recherche").toBool())
		slotNouvelOnglet();

	if (moteur == "AltaVista")
		search = "http://www.altavista.fr/web/results?q=" + q;

	else if (moteur == "Bing")
		search = "http://www.bing.com/search?q=" + q;

	else if (moteur == "Yahoo!")
		search = "http://fr.search.yahoo.com/search?p=" + q;

	else if (moteur == "Yauba")
		search = "http://fr.yauba.com/?q=" + q;

	else if (moteur == "Personnalisé")
		search = reglages.value("moteur_personnalise").toString() + q;

	else // We use Google ad default
		search = "http://www.google.fr/search?q=" + q;

	pageActuelle()->load(QUrl(search));
}

void NavigateurWeb::slotAjouterFavori()
{
	QString titre = QInputDialog::getText(this, "Multiuso", "Veuillez saisir le nom du favori :",
		QLineEdit::Normal, pageActuelle()->title());

	if (titre.isEmpty())
		return;

		titre.replace("/", " ");
		titre.replace(",", " ");
		titre.replace(".", " ");
		titre.replace(":", " ");

	if (titre.size() > 30)
		titre = titre.left(30);

	QDir dir(Multiuso::appDirPath() + "/navigateurWeb/favoris");

	QStringList listeFavoris = dir.entryList(QDir::Files);

	if (!listeFavoris.contains(titre + ".mltsbookmark"))
	{
		QString url = pageActuelle()->url().toString();

		QFile favori(Multiuso::appDirPath() + "/navigateurWeb/favoris/" + titre + ".mltsbookmark");

		if (!favori.exists() && favori.open(QIODevice::WriteOnly | QIODevice::Text))
			favori.write(url.toAscii());

		else
			QMessageBox::information(this, "Multiuso", "Ce favori existe déjà, aucun nouveau favori n'a été créé.");

		favori.close();

		actualiserFavoris();
	}

	else
	{
		QMessageBox::critical(this, "Multiuso", "Ce favori existe déjà !");
	}
}

void NavigateurWeb::telechargerFichierAuto(QNetworkReply *reponse)
{
	if (reponse->bytesAvailable())
	{
		QString lien = reponse->url().toString();

		if (lien.contains("ad_type=iframe"))
			return;

		pointeurSurTelechargements->nouveauTelechargement(lien);
	}

	else
	{
		pageActuelle()->stop();
		reponse->abort();
	}
}

void NavigateurWeb::telechargerFichier(QNetworkRequest requete)
{
	QString lien = requete.url().toString();

	if (lien.contains("ad_type=iframe"))
			return;

	pointeurSurTelechargements->nouveauTelechargement(lien);
}

void NavigateurWeb::slotTakeScreenshot()
{
	screen->takeScreenshotOf(pageActuelle()->page());
}

void NavigateurWeb::slotScreenshotTaken(QString title, QString url, QPixmap pixmap)
{
	if (title.size() > 18)
		title = title.left(15) + "...";

	url = Multiuso::addSlash(url);

	url = url.replace(QRegExp("https?://([^/]*)/(.+)?"), "\\1"); // http://www.foobar.com/index.html → www.foobar.com

	QString savePath = QFileDialog::getSaveFileName(this, "Multiuso", Multiuso::lastPath() + "/" + title + " (" + url + ").png",
			"Images (*.png, *.bmp, *.jpg, *.jpeg, *.ppm, *.xbm, *.xpm)");

	Multiuso::setLastPath(savePath);

	if (!savePath.isEmpty())
		pixmap.save(savePath);
}

void NavigateurWeb::slotScreenshotNotTaken()
{
	QMessageBox::critical(this, "Multiuso", "Une erreur est survenue lors de la capture de la page !");
}

void NavigateurWeb::survolLien(QString lien, QString titre, QString contenu)
{
	statut->showMessage(lien);
	lastLink = lien;

	titre = "";
	contenu = "";
}

void NavigateurWeb::changementIcone()
{
	QWebView *view = qobject_cast<QWebView *>(sender());

	if (view == 0)
		return;

	onglets->setTabIcon(onglets->indexOf(view->parentWidget()), faviconUrl(view->url().toString()));
}

void NavigateurWeb::chargementFini(bool ok)
{
	progression->hide();
	termine->show();
	actionRecharger->setVisible(true);
	actionStop->setVisible(false);

	if (ok)
	{
		QSettings reglages(Multiuso::appDirPath() + "/ini/navigateur.ini", QSettings::IniFormat);

		if (reglages.value("utiliser_historique").toBool())
		{
			QFile completer(Multiuso::appDirPath() + "/navigateurWeb/autre/completer.mltscompleter");

			if (completer.open(QIODevice::Append | QIODevice::Text))
			{
				if (!sitesVisites.contains(pageActuelle()->url().toString()))
				{
					completer.write(QString(pageActuelle()->url().toString() + "\n").toAscii());
					sitesVisites << pageActuelle()->url().toString();

					QCompleter *completerBarreAdresse = new QCompleter(sitesVisites, this);
						completerBarreAdresse->setCaseSensitivity(Qt::CaseInsensitive);

					barreAdresse->completer()->deleteLater();
					barreAdresse->setCompleter(completerBarreAdresse);
				}
			}

			completer.close();

			QFile historique(Multiuso::appDirPath() + "/navigateurWeb/autre/historique.mltshistory");

			if (historique.open(QIODevice::Append | QIODevice::Text))
			{
				historique.write(QString(pageActuelle()->url().toString() + "\n").toAscii());
				liensHistorique << pageActuelle()->url().toString();
				modeleHistorique->setStringList(liensHistorique);
			}

			historique.close();
		}
	}

	else
	{
		statut->showMessage("Le chargement ne s'est pas terminé correctement.", 3000);

		barreAdresse->setText("about:blank");
		slotChangerAdresse();
	}
}

void NavigateurWeb::chargementContinue(int progressionDuChargement)
{
	progression->setValue(progressionDuChargement);
}

void NavigateurWeb::debutChargement()
{
	QWebView *view = qobject_cast<QWebView *>(sender());

	if (view == 0)
		return;

	progression->setValue(0);
	progression->show();
	actionRecharger->setVisible(false);
	actionStop->setVisible(true);
	termine->hide();
	onglets->setTabText(onglets->indexOf(view->parentWidget()), "Chargement en cours...");
	onglets->setTabIcon(onglets->indexOf(view->parentWidget()), QIcon(":/icones/navigateur_web/chargement.png"));
}

void NavigateurWeb::changementDeTitre(QString titre)
{
	QWebView *view = qobject_cast<QWebView *>(sender());

	if (view == 0)
		return;

	if (titre.size() > 30)
		titre = titre.left(27) + "...";

	onglets->setTabText(onglets->indexOf(view->parentWidget()), titre.replace("&", "&&"));
}

void NavigateurWeb::changementUrl(QUrl url)
{
	if (pageActuelle()->url() != url)
		return;

	if (url.toString().contains(QRegExp("file(.+)" + Multiuso::appDirPath() + "/navigateurWeb/html/page_vierge.html")))
		barreAdresse->setText("about:blank");

	else if (url.toString().contains(QRegExp("file(.+)" + Multiuso::appDirPath() + "/navigateurWeb/speedDial/speedDial.html")))
		barreAdresse->setText("speed-dial");

	else
		barreAdresse->setText(url.toString());
}

void NavigateurWeb::ongletChange(int onglet)
{
	onglet = 0;

	changementUrl(pageActuelle()->url());
}

void NavigateurWeb::slotImprimer(QWebFrame *)
{
	QPrinter *imprimante = new QPrinter(QPrinter::ScreenResolution);
		imprimante->setDocName(pageActuelle()->title());
		imprimante->setPaperSize(QPrinter::A4);
		imprimante->setOrientation(QPrinter::Portrait);

	QPrintDialog *dialogueImpression = new QPrintDialog(imprimante, this);
		dialogueImpression->setWindowTitle("Navigateur Web - Imprimer");
		dialogueImpression->setWindowIcon(QIcon(":/icones/navigateur_web/navigateur_web.png"));

	connect(dialogueImpression, SIGNAL(accepted(QPrinter *)), this, SLOT(slotImprimerPage(QPrinter *)));

	dialogueImpression->exec();
}

void NavigateurWeb::slotImprimerPage(QPrinter *imprimante)
{
	pageActuelle()->page()->mainFrame()->print(imprimante);
}

void NavigateurWeb::slotDatabaseQuotaExceeded(QWebFrame *frame, QString name)
{
	frame->setHtml("Le site Web <strong>" + frame->title() + " (" + frame->url().toString() + ")</strong>"
			" demandant de la mémoire de la base de données « " + name + " » a dépassé son quota.");
}

void NavigateurWeb::slotOuvrirMenuNavigation()
{
	menuNavigation->exec(QCursor::pos());
}

void NavigateurWeb::slotMenuPage(QPoint)
{
	QWebHitTestResult hitTestResult = pageActuelle()->page()->mainFrame()->hitTestContent(QCursor::pos());

	QList<QAction *> menuActions = pageActuelle()->page()->createStandardContextMenu()->actions();
	QList<QAction *> newActions;

	foreach (QAction *action, menuActions)
	{
		action->setText(Multiuso::firstLetterToUpper(action->text()));

		// Actions
		if (action->text().toLower() == "recharger")
		{
			action->setIcon(QIcon(":/icones/navigateur_web/actionRecharger.png"));
			newActions << action;
		}

		else if (action->text().toLower() == "stop")
		{
			action->setIcon(QIcon(":/icones/navigateur_web/actionStop.png"));
			newActions << action;
		}

		else if (action->text().toLower() == "précédent")
		{
			action->setIcon(QIcon(":/icones/navigateur_web/actionPrecedent.png"));
			newActions << action;
		}

		else if (action->text().toLower() == "suivant")
		{
			action->setIcon(QIcon(":/icones/navigateur_web/actionSuivant.png"));
			newActions << action;
		}

		else if (action->text().toLower() == "copier")
		{
			newActions << action;
		}

		// Links
		else if (action->text().toLower() == "ouvrir le lien")
		{
			newActions << action;

			QString link = hitTestResult.linkUrl().toString();

			if (link.isEmpty())
				link = lastLink;

			QAction *openLinkInNewTab = new QAction("Ouvrir le lien dans un nouvel onglet", this);
				openLinkInNewTab->setObjectName(link);
				connect(openLinkInNewTab, SIGNAL(triggered()), this, SLOT(slotOpenLinkInNewTab()));
				newActions << openLinkInNewTab;
		}

		else if (action->text().toLower() == "sauvegarder le lien...")
		{
			newActions << action;
		}

		else if (action->text().toLower() == "copier le lien")
		{
			newActions << action;
		}

		// Pictures
		else if (action->text().toLower() == "ouvrir l'image")
		{
			newActions << action;
		}

		else if (action->text().toLower() == "sauvegarder l'image")
		{
			action->setText("Enregistrer l'image sous...");

			newActions << action;
		}

		else if (action->text().toLower() == "copier l'image")
		{
			newActions << action;
		}
	}

	QMenu *menu = new QMenu(this);
		menu->addActions(newActions);
		menu->addSeparator();
		menu->addAction(actionTakeScreenshot);
		menu->addMenu(menuNavigation);
		menu->addMenu(menuFavoris);
		menu->exec(QCursor::pos());
}

// <Stalker (www.siteduzero.com)>
	void NavigateurWeb::slotErrorPageCheck(QNetworkReply *netRep)
	{
		QNetworkReply::NetworkError erreur = netRep->error();

		if (erreur == QNetworkReply::NoError)
		{
			// Il n'y a pas d'erreur
		}

		else
		{
			switch (erreur)
			{
				// Le serveur refuse la connexion
				case QNetworkReply::ConnectionRefusedError:
					QMessageBox::warning(this, "Connexion refusée", "Le serveur a refusé la connexion !");
				break;
				case QNetworkReply::RemoteHostClosedError:
					QMessageBox::warning(this, "Connexion fermée", "Le serveur distant a fermé la connexion prématurément, avant que la réponse entière a été reçue et traitée !");
				break;
				// Erreur 404
				case QNetworkReply::ContentNotFoundError:
				   QMessageBox::warning(this, "Erreur 404", "Page Web non trouvée !");
				break;
				// Erreur 401, interdiction d'accès (exemple htaccess)
				case QNetworkReply::ContentAccessDenied:
				   QMessageBox::warning(this, "Erreur 401", "Vous n'avez pas le droit d'accéder à cette page !");
				break;
				// Proxy introuvable
				case QNetworkReply::ProxyNotFoundError:
					QMessageBox::warning(this, "Proxy", "Proxy introuvable !");
				break;
				// Hôte introuvable
				case QNetworkReply::HostNotFoundError:
					QMessageBox::warning(this, "Hôte introuvable", "Hôte introuvable !");
				break;
				default:
				break;
			}
		}
	}
// </Stalker (www.siteduzero.com)>

QDialog *NavigateurWeb::authenticationDialog(QString message, bool password, bool username, QString title)
{
	QDialog *dialog = new QDialog(this);
		dialog->setWindowTitle(title);
	
	QLineEdit *l_username = new QLineEdit;
		l_username->setObjectName("username");
	
	QLineEdit *l_password = new QLineEdit;
		l_password->setEchoMode(QLineEdit::Password);
		l_password->setObjectName("password");

	QFormLayout *layout = new QFormLayout;

		if (username)
			layout->addRow("Nom d'utilisateur :", l_username);

		if (password)
			layout->addRow("Mot de passe :", l_password);

	QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
		mainLayout->addWidget(new QLabel(message));
		mainLayout->addLayout(layout);
		mainLayout->addLayout(Multiuso::dialogButtons(dialog, "Annuler", "Connecter"));

	return dialog;
}

void NavigateurWeb::authenticationRequired(QNetworkReply *reply, QAuthenticator *auth)
{
	QString message = "Le site <strong>" + Qt::escape(reply->url().toString()) + "</strong> demande<br />"
		"un nom d'utilisateur et un mot de passe.<br />"
		"Le site indique « " + Qt::escape(auth->realm()) + " » :";

	QDialog *dialog = authenticationDialog(message, true, true);

	if (dialog->exec() == QDialog::Accepted)
	{
		auth->setUser(dialog->findChild<QLineEdit *>("username")->text());
		auth->setPassword(dialog->findChild<QLineEdit *>("password")->text());
	}

	dialog->deleteLater();
}

void NavigateurWeb::proxyAuthenticationRequired(QNetworkProxy proxy, QAuthenticator *auth)
{
	QString message = "Connexion au serveur proxy \"" + proxy.hostName() + "\" :";

	QDialog *dialog = authenticationDialog(message, true, true);
	
	if (dialog->exec() == QDialog::Accepted)
	{
		auth->setUser(dialog->findChild<QLineEdit *>("username")->text());
		auth->setPassword(dialog->findChild<QLineEdit *>("password")->text());
	}

	dialog->deleteLater();
}
