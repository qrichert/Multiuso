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

#include <QCryptographicHash>
#include "Preferences.h"
#include "autresClasses/EditerStyle.h"

Preferences::Preferences(FenPrincipale *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Préférences");
	setWindowIcon(QIcon(":/icones/actions/actionPreferences.png"));
	setFixedSize(760, 400);

	choixFermerFenetre = new QCheckBox;
	choixOuvertureFenetre = new QCheckBox;
	choixEffetsDeDeplacement = new QCheckBox;
	choixUtiliserSplashScreen = new QCheckBox;
	choixRedemarrerApresRemiseAZero = new QCheckBox;

	choixUtiliserMdp = new QCheckBox;
		connect(choixUtiliserMdp, SIGNAL(toggled(bool)), this, SLOT(checkUsePassword(bool)));

	QPushButton *parcourirDossierDL = new QPushButton("Parcourir...");
		connect(parcourirDossierDL, SIGNAL(clicked()), this, SLOT(slotParcourirDossierDL()));

	dossierTelechargements = new QLineEdit;
		dossierTelechargements->setDisabled(true);

	QHBoxLayout *layoutParcourirDossierDL = new QHBoxLayout;
		layoutParcourirDossierDL->addWidget(parcourirDossierDL);
		layoutParcourirDossierDL->addWidget(dossierTelechargements);

	layoutGeneral = new QFormLayout;
		layoutGeneral->addRow("Mettre le programme dans le system tray :", choixFermerFenetre);
		layoutGeneral->addRow("Ouvrir le programme en mode \"agrandi\" :", choixOuvertureFenetre);
		layoutGeneral->addRow("Utiliser les effets de déplacement :", choixEffetsDeDeplacement);
		layoutGeneral->addRow("Utiliser le splash screen :", choixUtiliserSplashScreen);
		layoutGeneral->addRow("Redémarrer Multiuso après une remise à zéro :", choixRedemarrerApresRemiseAZero);
		layoutGeneral->addRow("\"Multiuso - Téléchargements\" se trouve dans :", layoutParcourirDossierDL);
		layoutGeneral->addRow("Protéger vos données avec un mot de passe :", choixUtiliserMdp);

	fondScreenshot = new QCheckBox;

	layoutAccueil = new QFormLayout;
		layoutAccueil->addRow("Utiliser une capture d'écran comme fond pour l'Accueil :", fondScreenshot);

	welcomeFolder = new QLineEdit;
		welcomeFolder->setDisabled(true);

	QPushButton *browseWelcomeFolder = new QPushButton("Parcourir...");
		connect(browseWelcomeFolder, SIGNAL(clicked()), this, SLOT(slotParcourirDossierAccueil()));

	QHBoxLayout *layoutBrowseFolder = new QHBoxLayout;
		layoutBrowseFolder->addWidget(welcomeFolder);
		layoutBrowseFolder->addWidget(browseWelcomeFolder);
		layoutBrowseFolder->setContentsMargins(0, 0, 0, 0);

	QWidget *widgetBrowseFolder = new QWidget;
		widgetBrowseFolder->setLayout(layoutBrowseFolder);

	layoutNavFichiers = new QFormLayout;
		layoutNavFichiers->addRow("Dossier d'accueil :", widgetBrowseFolder);

	enregistrementAutomatique = new QCheckBox;

	layoutEditeurDeTexte = new QFormLayout;
		layoutEditeurDeTexte->addRow("Enregistrer les modifications de façon automatique :", enregistrementAutomatique);

	enregistrementAutomatiqueCode = new QCheckBox;
	indentationAutomatiqueCode = new QCheckBox;

	layoutEditeurDeCode = new QFormLayout;
		layoutEditeurDeCode->addRow("Enregistrer les modifications de façon automatique :", enregistrementAutomatiqueCode);
		layoutEditeurDeCode->addRow("Indentation automatique :", indentationAutomatiqueCode);

	styleDeBase = new QRadioButton;
		connect(styleDeBase, SIGNAL(toggled(bool)), this, SLOT(changementEtatStyle(bool)));

	stylePersonnalise = new QRadioButton;
		connect(stylePersonnalise, SIGNAL(toggled(bool)), this, SLOT(changementEtatStyle(bool)));

		editerLeStyle = new QPushButton("É&diter le style");
			connect(editerLeStyle, SIGNAL(clicked()), this, SLOT(slotEditerStyle()));

	styleExterne = new QRadioButton;
		connect(styleExterne, SIGNAL(toggled(bool)), this, SLOT(changementEtatStyle(bool)));

		parcourirStyleExterne = new QPushButton("&Parcourir...");
			connect(parcourirStyleExterne, SIGNAL(clicked()), this, SLOT(slotParcourirStyle()));

		afficherStyleExterne = new QLineEdit;
			afficherStyleExterne->setDisabled(true);

		layoutStyleExterne = new QHBoxLayout;
			layoutStyleExterne->addWidget(parcourirStyleExterne);
			layoutStyleExterne->addWidget(afficherStyleExterne);

	stylesQt = new QRadioButton;
		connect(stylesQt, SIGNAL(toggled(bool)), this, SLOT(changementEtatStyle(bool)));

	listeStylesQt = new QComboBox;
		listeStylesQt->addItems(QStyleFactory::keys());

	layoutStyle = new QFormLayout;
		layoutStyle->addRow("Utiliser le style de base :", styleDeBase);
		layoutStyle->addRow("Utiliser un style personnalisé :", stylePersonnalise);
		layoutStyle->addRow("", editerLeStyle);
		layoutStyle->addRow("Utiliser un style externe :", styleExterne);
		layoutStyle->addRow("", layoutStyleExterne);
		layoutStyle->addRow("Styles de Qt :", stylesQt);
		layoutStyle->addRow("", listeStylesQt);

	pageAccueil = new QLineEdit;
	contenuEditable = new QCheckBox;
	activerJavascript = new QCheckBox;
	activerJava = new QCheckBox;
	activerFlash = new QCheckBox;
	zoomTextOnly = new QCheckBox;
	imprimerElementsFond = new QCheckBox;
	utiliserHistorique = new QCheckBox;
	restaurerOnglets = new QCheckBox;

	QStringList listeDesMoteursDeRecherche;
		listeDesMoteursDeRecherche << "AltaVista" << "Bing" << "Google" << "Yahoo!" << "Yauba" << "Personnalisé";

	moteurDeRecherche = new QComboBox;
		moteurDeRecherche->addItems(listeDesMoteursDeRecherche);
		connect(moteurDeRecherche, SIGNAL(currentIndexChanged(QString)), this, SLOT(changementMoteurDeRecherche(QString)));

	moteurPersonnalise = new QLineEdit;

	nouvelOngletMoteurRecherche = new QCheckBox;

	QHBoxLayout *layoutMoteurRecherche = new QHBoxLayout;
		layoutMoteurRecherche->addWidget(new QLabel("Recherches "));
		layoutMoteurRecherche->addWidget(moteurDeRecherche);
		layoutMoteurRecherche->addWidget(moteurPersonnalise);
		layoutMoteurRecherche->addWidget(new QLabel("dans un nouvel onglet :"));
		layoutMoteurRecherche->addWidget(nouvelOngletMoteurRecherche);
		layoutMoteurRecherche->setAlignment(Qt::AlignLeft);

	QPushButton *viderCache = new QPushButton("Vider le cache");
		connect(viderCache, SIGNAL(clicked()), this, SLOT(slotViderCache()));

	QPushButton *effacerCookies = new QPushButton("Effacer les cookies");
		connect(effacerCookies, SIGNAL(clicked()), this, SLOT(slotEffacerCookies()));

	QFormLayout *layoutNavigateurWeb1 = new QFormLayout;
		layoutNavigateurWeb1->addRow("Page d'accueil :", pageAccueil);
		layoutNavigateurWeb1->addRow("Contenu de la page éditable :", contenuEditable);
		layoutNavigateurWeb1->addRow("Activer JavaScript :", activerJavascript);
		layoutNavigateurWeb1->addRow("Activer Java :", activerJava);
		layoutNavigateurWeb1->addRow("Activer Flash :", activerFlash);
		layoutNavigateurWeb1->addRow("Zoomer seulement sur le texte :", zoomTextOnly);
		layoutNavigateurWeb1->addRow("Imprimer les éléments de fond :", imprimerElementsFond);
		layoutNavigateurWeb1->addRow("Utiliser l'historique de navigation :", utiliserHistorique);
		layoutNavigateurWeb1->addRow("Restaurer les onglets ouverts à la fermeture :", restaurerOnglets);

	QGridLayout *layoutNavigateurWeb2 = new QGridLayout;
		layoutNavigateurWeb2->addWidget(effacerCookies, 0, 0, 1, 1);
		layoutNavigateurWeb2->addWidget(viderCache, 0, 1, 1, 1);

	layoutNavigateurWeb = new QVBoxLayout;
		layoutNavigateurWeb->addLayout(layoutMoteurRecherche);
		layoutNavigateurWeb->addLayout(layoutNavigateurWeb1);
		layoutNavigateurWeb->addLayout(layoutNavigateurWeb2);

	// <Stalker (www.siteduzero.com)>
		adresseProxyHTTP = new QLineEdit;
		connect(adresseProxyHTTP,SIGNAL(textChanged(QString)),this,SLOT(changementAdresseProxyHTTP(QString)));
		adresseProxyFTP = new QLineEdit;
		adresseProxySSL = new QLineEdit;
		adresseHoteSOCKS = new QLineEdit;

		portProxyHTTP = new QSpinBox;
		portProxyHTTP->setRange(0,65535);
		connect(portProxyHTTP,SIGNAL(valueChanged(int)),this,SLOT(changementPortProxyHTTP(int)));
		portProxyFTP = new QSpinBox;
		portProxyFTP->setRange(0,65535);
		portProxySSL = new QSpinBox;
		portProxySSL->setRange(0,65535);
		portHoteSOCKS = new QSpinBox;
		portHoteSOCKS->setRange(0,65535);

		pasProxyPour = new QLineEdit;

		layoutProxy = new QVBoxLayout;
		groupboxParametresProxy = new QGroupBox("Activer proxy");
		groupboxParametresProxy->setCheckable(true);
		groupboxParametresProxy->setChecked(false);
		activerParametresIdentiquesProxy = new QCheckBox("Utiliser la configuration du proxy HTTP pour tous les autres protocoles");
		connect(activerParametresIdentiquesProxy,SIGNAL(stateChanged(int)),this,SLOT(parametresProxysIdentiques(int)));
		layoutParametresProxy = new QGridLayout;
		layoutParametresProxyText = new QFormLayout;
		layoutParametresProxyText->addRow("Adresse proxy HTTP :", adresseProxyHTTP);
		layoutParametresProxyText->addRow("Adresse proxy FTP :", adresseProxyFTP);
		layoutParametresProxyText->addRow("Adresse proxy SSL :", adresseProxySSL);
		layoutParametresProxyText->addRow("Hôte SOCKS :", adresseHoteSOCKS);
		layoutParametresProxyText->addRow("Pas de proxy pour :", pasProxyPour);
		layoutParametresProxyNombre = new QFormLayout;
		layoutParametresProxyNombre->addRow("Port :",portProxyHTTP);
		layoutParametresProxyNombre->addRow("Port :",portProxyFTP);
		layoutParametresProxyNombre->addRow("Port :",portProxySSL);
		layoutParametresProxyNombre->addRow("Port :",portHoteSOCKS);
		layoutParametresProxy->addWidget(activerParametresIdentiquesProxy,0,0);
		layoutParametresProxy->addLayout(layoutParametresProxyText,1,0);
		layoutParametresProxy->addLayout(layoutParametresProxyNombre,1,1);
		groupboxParametresProxy->setLayout(layoutParametresProxy);
		layoutProxy->addWidget(groupboxParametresProxy);
	// </Stalker (www.siteduzero.com)>

	QListWidgetItem *itemGeneral = new QListWidgetItem("Général");
		itemGeneral->setIcon(QIcon(":/icones/preferences/general.png"));

	QListWidgetItem *itemStyle = new QListWidgetItem("Style");
		itemStyle->setIcon(QIcon(":/icones/preferences/style.png"));

	// <Stalker (www.siteduzero.com)>
		QListWidgetItem *itemProxy = new QListWidgetItem("Proxy");
			itemProxy->setIcon(QIcon(":/icones/preferences/proxy.png"));
	// </Stalker (www.siteduzero.com)>

	QListWidgetItem *itemAccueil = new QListWidgetItem("Accueil");
		itemAccueil->setIcon(QIcon(":/icones/preferences/accueil.png"));

	QListWidgetItem *itemNavigateurFichiers = new QListWidgetItem("Navigateur de fichiers");
		itemNavigateurFichiers->setIcon(QIcon(":/icones/preferences/nav_fichiers.png"));

	QListWidgetItem *itemEditeurDeTexte = new QListWidgetItem("Éditeur de texte");
		itemEditeurDeTexte->setIcon(QIcon(":/icones/preferences/editeur_de_texte.png"));

	QListWidgetItem *itemEditeurDeCode = new QListWidgetItem("Éditeur de code");
		itemEditeurDeCode->setIcon(QIcon(":/icones/preferences/editeur_de_code.png"));

	QListWidgetItem *itemNavigateurWeb = new QListWidgetItem("Navigateur Web");
		itemNavigateurWeb->setIcon(QIcon(":/icones/preferences/navigateur_web.png"));

	QSettings itemActuel(Multiuso::appDirPath() + "/ini/preferences.ini", QSettings::IniFormat);

	listeOptions = new QListWidget;
		listeOptions->addItem(itemGeneral);
		listeOptions->addItem(itemStyle);
		// <Stalker (www.siteduzero.com)>
			listeOptions->addItem(itemProxy);
		// </Stalker (www.siteduzero.com)>
		listeOptions->addItem(itemAccueil);
		listeOptions->addItem(itemNavigateurFichiers);
		listeOptions->addItem(itemEditeurDeTexte);
		listeOptions->addItem(itemEditeurDeCode);
		listeOptions->addItem(itemNavigateurWeb);
		listeOptions->setCurrentRow(itemActuel.value("dernier_item").toInt());
		listeOptions->setFixedWidth(200);
		connect(listeOptions, SIGNAL(currentRowChanged(int)), this, SLOT(slotChangementItem(int)));

	widgetLayoutGeneral = new QWidget;
		widgetLayoutGeneral->setLayout(layoutGeneral);

	widgetLayoutAccueil = new QWidget;
		widgetLayoutAccueil->setLayout(layoutAccueil);

	widgetLayoutNavigateurFichiers = new QWidget;
		widgetLayoutNavigateurFichiers->setLayout(layoutNavFichiers);

	widgetLayoutEditeurDeTexte = new QWidget;
		widgetLayoutEditeurDeTexte->setLayout(layoutEditeurDeTexte);

	widgetLayoutEditeurDeCode = new QWidget;
		widgetLayoutEditeurDeCode->setLayout(layoutEditeurDeCode);

	widgetLayoutStyle = new QWidget;
		widgetLayoutStyle->setLayout(layoutStyle);

	widgetLayoutNavigateurWeb = new QWidget;
		widgetLayoutNavigateurWeb->setLayout(layoutNavigateurWeb);

	// <Stalker (www.siteduzero.com)>
		widgetLayoutProxy = new QWidget;
			widgetLayoutProxy->setLayout(layoutProxy);
	// </Stalker (www.siteduzero.com)>

	QStackedWidget *widgetWidgetsContenu = new QStackedWidget;
		widgetWidgetsContenu->addWidget(widgetLayoutGeneral);
		widgetWidgetsContenu->addWidget(widgetLayoutStyle);
		// <Stalker (www.siteduzero.com)>
			widgetWidgetsContenu->addWidget(widgetLayoutProxy);
		// </Stalker (www.siteduzero.com)>
		widgetWidgetsContenu->addWidget(widgetLayoutAccueil);
		widgetWidgetsContenu->addWidget(widgetLayoutNavigateurFichiers);
		widgetWidgetsContenu->addWidget(widgetLayoutEditeurDeTexte);
		widgetWidgetsContenu->addWidget(widgetLayoutEditeurDeCode);
		widgetWidgetsContenu->addWidget(widgetLayoutNavigateurWeb);

	connect(listeOptions, SIGNAL(currentRowChanged(int)), widgetWidgetsContenu, SLOT(setCurrentIndex(int)));

	QVBoxLayout *layoutWidgetsContenu = new QVBoxLayout;
		layoutWidgetsContenu->addWidget(widgetWidgetsContenu);

	widgetConteneur = new QWidget;
		widgetConteneur->setLayout(layoutWidgetsContenu);
		widgetConteneur->setFixedWidth(530);

	layoutContenu = new QHBoxLayout;
		layoutContenu->addWidget(listeOptions);
		layoutContenu->addWidget(widgetConteneur);

	remettreAZero = new QPushButton("Remettre à &Zéro");
		connect(remettreAZero, SIGNAL(clicked()), this, SLOT(slotRemettreAZero()));

	layoutGauche = new QHBoxLayout;
		layoutGauche->addWidget(remettreAZero);
		layoutGauche->setAlignment(Qt::AlignLeft);

	enregistrer = new QPushButton("&Enregistrer");
		connect(enregistrer, SIGNAL(clicked()), this, SLOT(enregistrerPreferences()));

	parDefaut = new QPushButton("&Réglages par défaut");
		connect(parDefaut, SIGNAL(clicked()), this, SLOT(reglagesParDefaut()));

	annuler = new QPushButton("&Annuler");
		connect(annuler, SIGNAL(clicked()), this, SLOT(accept()));

	layoutDroite = new QHBoxLayout;
		layoutDroite->addWidget(enregistrer);
		layoutDroite->addWidget(parDefaut);
		layoutDroite->addWidget(annuler);
		layoutDroite->setAlignment(Qt::AlignRight);

	layoutBoutons = new QHBoxLayout;
		layoutBoutons->addLayout(layoutGauche);
		layoutBoutons->addLayout(layoutDroite);

	layoutPreferences = new QVBoxLayout(this);
		layoutPreferences->addLayout(layoutContenu);
		layoutPreferences->addLayout(layoutBoutons);

	afficherPreferences();

	widgetWidgetsContenu->setCurrentIndex(itemActuel.value("dernier_item").toInt());

	pointeurSurParent = parent;
}

void Preferences::slotRemettreAZero()
{
	QString futureAction = "fermé";

	if (choixRedemarrerApresRemiseAZero->isChecked())
		futureAction = "redémarré";

	int reponse = QMessageBox::warning(this, "Multiuso", "Cette action réinitialisera Multiuso.<br />"
			"Toutes les données enregistrées seront définitivement perdues.<br />"
			"<em>Multiuso sera " + futureAction + " automatiquement.</em><br />"
			"Continuer ?", QMessageBox::Yes | QMessageBox::Cancel);

	if (reponse == QMessageBox::Yes)
	{
		Multiuso::removeDirectory(Multiuso::appDirPath() + "/extensions");
		Multiuso::removeDirectory(Multiuso::appDirPath() + "/ini");
		Multiuso::removeDirectory(Multiuso::appDirPath() + "/navigateurWeb");

		if (choixRedemarrerApresRemiseAZero->isChecked())
			QProcess::startDetached(Multiuso::openCommand() + Multiuso::appDirPath() + "/Multiuso" + Multiuso::currentSuffix());

		Multiuso::quit();
	}
}

void Preferences::afficherPreferences()
{
	QSettings reglagesFenetre(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);

		if (reglagesFenetre.value("fermeture/systemTray").toBool())
			choixFermerFenetre->setCheckState(Qt::Checked);

		if (reglagesFenetre.value("ouverture/agrandi").toBool())
			choixOuvertureFenetre->setCheckState(Qt::Checked);

		if (reglagesFenetre.value("deplacements/effets").toBool())
			choixEffetsDeDeplacement->setCheckState(Qt::Checked);

		dossierTelechargements->setText(reglagesFenetre.value("telechargements/dossier").toString());

		if (reglagesFenetre.value("accueil/fond_screenshot").toBool())
			fondScreenshot->setCheckState(Qt::Checked);

		if (reglagesFenetre.value("splash_screen/utiliser").toBool())
			choixUtiliserSplashScreen->setCheckState(Qt::Checked);

		if (reglagesFenetre.value("remise_a_zero/restart").toBool())
			choixRedemarrerApresRemiseAZero->setCheckState(Qt::Checked);
		
		if (reglagesFenetre.value("mot_de_passe").toBool())
			choixUtiliserMdp->setCheckState(Qt::Checked);

	QSettings reglagesNavigateurFichiers(Multiuso::appDirPath() + "/ini/nav_fichiers.ini", QSettings::IniFormat);
		welcomeFolder->setText(reglagesNavigateurFichiers.value("dossier_accueil").toString());

	QSettings reglagesEditeurDeTexte(Multiuso::appDirPath() + "/ini/editeur_de_texte.ini", QSettings::IniFormat);

		if (reglagesEditeurDeTexte.value("enregistrement/enregistrement_automatique").toBool())
			enregistrementAutomatique->setCheckState(Qt::Checked);

	QSettings reglagesEditeurDeCode(Multiuso::appDirPath() + "/ini/editeur_de_code.ini", QSettings::IniFormat);

		if (reglagesEditeurDeCode.value("enregistrement/enregistrement_automatique").toBool())
			enregistrementAutomatiqueCode->setCheckState(Qt::Checked);

		if (reglagesEditeurDeCode.value("saisie/indentation_automatique").toBool())
			indentationAutomatiqueCode->setCheckState(Qt::Checked);

	QSettings apparenceFenetre(Multiuso::appDirPath() + "/ini/apparence.ini", QSettings::IniFormat);
		styleDeBase->setChecked(apparenceFenetre.value("style/style_de_base").toBool());
		stylePersonnalise->setChecked(apparenceFenetre.value("style/style_personnalise").toBool());
		styleExterne->setChecked(apparenceFenetre.value("style/style_externe").toBool());
		afficherStyleExterne->setText(apparenceFenetre.value("style/nom_style_externe").toString());
		stylesQt->setChecked(apparenceFenetre.value("style/style_qt").toBool());
		listeStylesQt->setCurrentIndex(listeStylesQt->findText(apparenceFenetre.value("style/style_qt_utilise").toString()));

	QSettings reglagesNavigateur(Multiuso::appDirPath() + "/ini/navigateur.ini", QSettings::IniFormat);
		pageAccueil->setText(reglagesNavigateur.value("page_accueil").toString());

		if (reglagesNavigateur.value("nouvel_onglet_recherche_moteur_recherche").toBool())
			nouvelOngletMoteurRecherche->setCheckState(Qt::Checked);

		if (reglagesNavigateur.value("contenu_editable").toBool())
			contenuEditable->setCheckState(Qt::Checked);

		if (reglagesNavigateur.value("settings/javascript").toBool())
			activerJavascript->setCheckState(Qt::Checked);

		if (reglagesNavigateur.value("settings/java").toBool())
			activerJava->setCheckState(Qt::Checked);

		if (reglagesNavigateur.value("settings/flash").toBool())
			activerFlash->setCheckState(Qt::Checked);

		if (reglagesNavigateur.value("settings/zoom_text_only").toBool())
			zoomTextOnly->setCheckState(Qt::Checked);

		if (reglagesNavigateur.value("settings/imprimer_elements_fond").toBool())
			imprimerElementsFond->setCheckState(Qt::Checked);

		if (reglagesNavigateur.value("utiliser_historique").toBool())
			utiliserHistorique->setCheckState(Qt::Checked);

		if (reglagesNavigateur.value("restaurer_onglets").toBool())
			restaurerOnglets->setCheckState(Qt::Checked);

		moteurDeRecherche->setCurrentIndex(moteurDeRecherche->findText(reglagesNavigateur.value("moteur_de_recherche").toString()));

		if (reglagesNavigateur.value("moteur_de_recherche").toString() != "Personnalisé")
			moteurPersonnalise->setDisabled(true);

		moteurPersonnalise->setText(reglagesNavigateur.value("moteur_personnalise").toString());

	// <Stalker (www.siteduzero.com)>
		QSettings reglagesProxy(Multiuso::appDirPath() + "/ini/proxy.ini", QSettings::IniFormat);

		if (reglagesProxy.value("activer",false).toBool())
			groupboxParametresProxy->setChecked(true);

		if (reglagesProxy.value("parametres_identiques",false).toBool())
			activerParametresIdentiquesProxy->setCheckState(Qt::Checked);

		adresseProxyHTTP->setText(reglagesProxy.value("adresse_http","").toString());
		portProxyHTTP->setValue(reglagesProxy.value("port_http",0).toInt());
		adresseProxyFTP->setText(reglagesProxy.value("adresse_ftp","").toString());
		portProxyFTP->setValue(reglagesProxy.value("port_ftp",0).toInt());
		adresseProxySSL->setText(reglagesProxy.value("adresse_ssl","").toString());
		portProxySSL->setValue(reglagesProxy.value("port_ssl",0).toInt());
		adresseHoteSOCKS->setText(reglagesProxy.value("adresse_socks","").toString());
		portHoteSOCKS->setValue(reglagesProxy.value("port_socks",0).toInt());
		pasProxyPour->setText(reglagesProxy.value("pas_proxy","").toString());
	// </Stalker (www.siteduzero.com)>
}

void Preferences::enregistrerPreferences()
{
	QSettings reglagesFenetre(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);
		reglagesFenetre.setValue("fermeture/systemTray", choixFermerFenetre->isChecked());
		reglagesFenetre.setValue("ouverture/agrandi", choixOuvertureFenetre->isChecked());
		reglagesFenetre.setValue("deplacements/effets", choixEffetsDeDeplacement->isChecked());
		reglagesFenetre.setValue("telechargements/dossier", dossierTelechargements->text());
		reglagesFenetre.setValue("accueil/fond_screenshot", fondScreenshot->isChecked());
		reglagesFenetre.setValue("splash_screen/utiliser", choixUtiliserSplashScreen->isChecked());
		reglagesFenetre.setValue("remise_a_zero/restart", choixRedemarrerApresRemiseAZero->isChecked());

	QSettings reglagesNavigateurFichiers(Multiuso::appDirPath() + "/ini/nav_fichiers.ini", QSettings::IniFormat);
		reglagesNavigateurFichiers.setValue("dossier_accueil", welcomeFolder->text());

	QSettings reglagesEditeurDeTexte(Multiuso::appDirPath() + "/ini/editeur_de_texte.ini", QSettings::IniFormat);
		reglagesEditeurDeTexte.setValue("enregistrement/enregistrement_automatique", enregistrementAutomatique->isChecked());

	QSettings reglagesEditeurDeCode(Multiuso::appDirPath() + "/ini/editeur_de_code.ini", QSettings::IniFormat);
		reglagesEditeurDeCode.setValue("enregistrement/enregistrement_automatique", enregistrementAutomatiqueCode->isChecked());
		reglagesEditeurDeCode.setValue("saisie/indentation_automatique", indentationAutomatiqueCode->isChecked());

	QSettings apparenceFenetre(Multiuso::appDirPath() + "/ini/apparence.ini", QSettings::IniFormat);
		apparenceFenetre.setValue("style/style_de_base", styleDeBase->isChecked());
		apparenceFenetre.setValue("style/style_personnalise", stylePersonnalise->isChecked());
		apparenceFenetre.setValue("style/style_externe", styleExterne->isChecked());
		apparenceFenetre.setValue("style/nom_style_externe", afficherStyleExterne->text());
		apparenceFenetre.setValue("style/style_qt", stylesQt->isChecked());
		apparenceFenetre.setValue("style/style_qt_utilise", listeStylesQt->currentText());

	QSettings reglagesNavigateur(Multiuso::appDirPath() + "/ini/navigateur.ini", QSettings::IniFormat);
		reglagesNavigateur.setValue("page_accueil", pageAccueil->text());
		reglagesNavigateur.setValue("nouvel_onglet_recherche_moteur_recherche", nouvelOngletMoteurRecherche->isChecked());
		reglagesNavigateur.setValue("contenu_editable", contenuEditable->isChecked());
		reglagesNavigateur.setValue("settings/javascript", activerJavascript->isChecked());
		reglagesNavigateur.setValue("settings/java", activerJava->isChecked());
		reglagesNavigateur.setValue("settings/flash", activerFlash->isChecked());
		reglagesNavigateur.setValue("settings/zoom_text_only", zoomTextOnly->isChecked());
		reglagesNavigateur.setValue("settings/imprimer_elements_fond", imprimerElementsFond->isChecked());
		reglagesNavigateur.setValue("utiliser_historique", utiliserHistorique->isChecked());
		reglagesNavigateur.setValue("restaurer_onglets", restaurerOnglets->isChecked());
		reglagesNavigateur.setValue("moteur_de_recherche", moteurDeRecherche->currentText());
		reglagesNavigateur.setValue("moteur_personnalise", moteurPersonnalise->text());

	// <Stalker (www.siteduzero.com)>
		QSettings reglagesProxy(Multiuso::appDirPath() + "/ini/proxy.ini", QSettings::IniFormat);
			reglagesProxy.setValue("activer",groupboxParametresProxy->isChecked());
			reglagesProxy.setValue("parametres_identiques",activerParametresIdentiquesProxy->isChecked());
			reglagesProxy.setValue("adresse_http",adresseProxyHTTP->text());
			reglagesProxy.setValue("port_http",portProxyHTTP->value());
			reglagesProxy.setValue("adresse_ftp",adresseProxyFTP->text());
			reglagesProxy.setValue("port_ftp",portProxyFTP->value());
			reglagesProxy.setValue("adresse_ssl",adresseProxySSL->text());
			reglagesProxy.setValue("port_ssl",portProxySSL->value());
			reglagesProxy.setValue("adresse_socks",adresseHoteSOCKS->text());
			reglagesProxy.setValue("port_socks",portHoteSOCKS->value());
			reglagesProxy.setValue("pas_proxy",pasProxyPour->text());
	// </Stalker (www.siteduzero.com)>

	pointeurSurParent->appliquerReglages();

	this->accept();
}

void Preferences::reglagesParDefaut()
{
	choixFermerFenetre->setCheckState(Qt::Unchecked);
	choixOuvertureFenetre->setCheckState(Qt::Unchecked);
	choixEffetsDeDeplacement->setCheckState(Qt::Unchecked);
	choixUtiliserSplashScreen->setCheckState(Qt::Checked);
	choixRedemarrerApresRemiseAZero->setCheckState(Qt::Unchecked);
	choixUtiliserMdp->setCheckState(Qt::Unchecked);
	dossierTelechargements->setText(QDir::homePath());

	styleDeBase->setChecked(true);

	fondScreenshot->setCheckState(Qt::Unchecked);

	welcomeFolder->setText(QDir::homePath());

	enregistrementAutomatique->setCheckState(Qt::Unchecked);

	enregistrementAutomatiqueCode->setCheckState(Qt::Unchecked);
	indentationAutomatiqueCode->setCheckState(Qt::Checked);

	pageAccueil->setText(QCoreApplication::organizationDomain());
	nouvelOngletMoteurRecherche->setCheckState(Qt::Checked);
	moteurDeRecherche->setCurrentIndex(moteurDeRecherche->findText("Google"));
	moteurPersonnalise->setText("");
	contenuEditable->setCheckState(Qt::Unchecked);
	activerJavascript->setCheckState(Qt::Checked);
	activerJava->setCheckState(Qt::Checked);
	activerFlash->setCheckState(Qt::Checked);
	zoomTextOnly->setCheckState(Qt::Unchecked);
	imprimerElementsFond->setCheckState(Qt::Unchecked);
	utiliserHistorique->setCheckState(Qt::Checked);
	restaurerOnglets->setCheckState(Qt::Checked);

	// <Stalker (www.siteduzero.com)>
		groupboxParametresProxy->setChecked(false);
		activerParametresIdentiquesProxy->setCheckState(Qt::Unchecked);
		adresseProxyHTTP->setText("");
		adresseProxyFTP->setText("");
		adresseProxySSL->setText("");
		adresseHoteSOCKS->setText("");
		portProxyHTTP->setValue(0);
		portProxyFTP->setValue(0);
		portProxySSL->setValue(0);
		portHoteSOCKS->setValue(0);
		pasProxyPour->setText("");
	// </Stalker (www.siteduzero.com)>
}

void Preferences::slotParcourirDossierDL()
{
	QString dossier = QFileDialog::getExistingDirectory(this, "Multiuso", dossierTelechargements->text(),
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	if (!dossier.isEmpty())
		dossierTelechargements->setText(dossier);
}

void Preferences::slotEditerStyle()
{
	EditerStyle *editerStyle = new EditerStyle(this);
		editerStyle->exec();
		editerStyle->deleteLater();
}

void Preferences::slotParcourirStyle()
{
	QString style = QFileDialog::getOpenFileName(this, "Multiuso",
			Multiuso::appDirPath() + "/extensions/styles",
			"Style de Multiuso (*.mltsstyle)");

	if (!style.isEmpty())
	{
		QFileInfo infosFichierStyle(style);
		afficherStyleExterne->setText(infosFichierStyle.fileName());
	}
}

void Preferences::slotParcourirDossierAccueil()
{
	QString folder = QFileDialog::getExistingDirectory(this, "Multiuso",
		welcomeFolder->text());

	if (!folder.isEmpty())
		welcomeFolder->setText(folder);
}

void Preferences::slotChangementItem(int item)
{
	QSettings itemActuel(Multiuso::appDirPath() + "/ini/preferences.ini", QSettings::IniFormat);
		itemActuel.setValue("dernier_item", item);
}

void Preferences::changementMoteurDeRecherche(QString newOne)
{
	if (newOne == "Personnalisé")
		moteurPersonnalise->setDisabled(false);

	else
		moteurPersonnalise->setDisabled(true);
}

void Preferences::changementEtatStyle(bool)
{
	if (stylePersonnalise->isChecked())
	{
		editerLeStyle->setDisabled(false);
		parcourirStyleExterne->setDisabled(true);
		listeStylesQt->setDisabled(true);
	}

	else if (styleExterne->isChecked())
	{
		editerLeStyle->setDisabled(true);
	moteurDeRecherche->setCurrentIndex(moteurDeRecherche->findText("Google"));
		parcourirStyleExterne->setDisabled(false);
		listeStylesQt->setDisabled(true);
	}

	else if (stylesQt->isChecked())
	{
		editerLeStyle->setDisabled(true);
		parcourirStyleExterne->setDisabled(true);
		listeStylesQt->setDisabled(false);
	}

	else
	{
		editerLeStyle->setDisabled(true);
		parcourirStyleExterne->setDisabled(true);
		listeStylesQt->setDisabled(true);
	}
}

void Preferences::slotViderCache()
{
	QDir dir(Multiuso::appDirPath() + "/navigateurWeb/cache/");

	Multiuso::removeDirectory(dir);
	dir.mkpath(Multiuso::appDirPath() + "/navigateurWeb/cache/");

	QMessageBox::information(this, "Multiuso", "Le cache a été vidé !");
}

void Preferences::slotEffacerCookies()
{
	QFile::remove(Multiuso::appDirPath() + "/navigateurWeb/autre/cookies.mltscookie");

	QMessageBox::information(this, "Multiuso", "Les cookies on été effacés !");
}

void Preferences::checkUsePassword(bool toogled)
{
	QSettings reglages(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);
		reglages.setValue("mot_de_passe", toogled);
		
	if (toogled)
	{
		PasswordDialog *pwd = new PasswordDialog(this);

			if (pwd->exec() == QDialog::Accepted)
			{
				QString password = "ér97&_Èhz" + pwd->getPassword() + "~odE987sDe!";
					
				QByteArray ba = password.toAscii();
					ba = QCryptographicHash::hash(ba, QCryptographicHash::Sha1);

				QSettings reglagesPassword(Multiuso::appDirPath() + "/ini/PWD.ini", QSettings::IniFormat);
					reglagesPassword.setValue("pwd", ba);
			}

			pwd->deleteLater();
	}

	else
	{
		QFile::remove(Multiuso::appDirPath() + "/ini/PWD.ini");
	}
}

// <Stalker (www.siteduzero.com)>
	void Preferences::parametresProxysIdentiques(int etat)
	{
		if (etat == Qt::Checked)
		{
			adresseProxyFTP->setEnabled(false);
			adresseProxyFTP->setText(adresseProxyHTTP->text());
			adresseProxySSL->setEnabled(false);
			adresseProxySSL->setText(adresseProxyHTTP->text());
			adresseHoteSOCKS->setEnabled(false);
			adresseHoteSOCKS->setText(adresseProxyHTTP->text());
			portProxyFTP->setEnabled(false);
			portProxyFTP->setValue(portProxyHTTP->value());
			portProxySSL->setEnabled(false);
			portProxySSL->setValue(portProxyHTTP->value());
			portHoteSOCKS->setEnabled(false);
			portHoteSOCKS->setValue(portProxyHTTP->value());
		}

		else
		{
			adresseProxyFTP->setEnabled(true);
			adresseProxySSL->setEnabled(true);
			adresseHoteSOCKS->setEnabled(true);
			portProxyFTP->setEnabled(true);
			portProxySSL->setEnabled(true);
			portHoteSOCKS->setEnabled(true);
		}
	}

	void Preferences::changementAdresseProxyHTTP(QString adresse)
	{
		if (activerParametresIdentiquesProxy->checkState() == Qt::Checked)
		{
			adresseProxyFTP->setText(adresse);
			adresseProxySSL->setText(adresse);
			adresseHoteSOCKS->setText(adresse);
		}
	}

	void Preferences::changementPortProxyHTTP(int port)
	{
		if (activerParametresIdentiquesProxy->checkState() == Qt::Checked)
		{
			portProxyFTP->setValue(port);
			portProxySSL->setValue(port);
			portHoteSOCKS->setValue(port);
		}
	}
// </Stalker (www.siteduzero.com)>
