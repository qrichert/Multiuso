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

#ifndef HEADER_FENPRINCIPALE
#define HEADER_FENPRINCIPALE

#include "../CurrentIncludes.h"
#include "classesPrincipales/TelechargerFichier.h"
#include "classesPrincipales/Accueil.h"
#include "classesPrincipales/Widgets.h"
#include "classesPrincipales/NavFichiers.h"
#include "classesPrincipales/EditeurDeTexte.h"
#include "classesPrincipales/EditeurDeCode.h"
#include "classesPrincipales/Ftp.h"
#include "classesPrincipales/NavigateurWeb.h"
#include "classesPrincipales/VisionneurImages.h"
#include "classesPlugins/BaseAction.h"
#include "classesPlugins/BaseDockWidget.h"
#include "classesPlugins/BaseOnglet.h"
#include "classesPlugins/BaseWidget.h"

class FenPrincipale : public QMainWindow
{
	Q_OBJECT

	public:
		FenPrincipale();

		QTabWidget *tabWidget();
		Accueil *homeTab();
		void setTabIndex(int index);
		void showTab(QString tab, bool show);

	protected: // Évènements de la fenêtre.
		bool eventFilter(QObject *object, QEvent *event);
		void closeEvent(QCloseEvent *event);
		void resizeEvent(QResizeEvent *event);
		void moveEvent(QMoveEvent *event);

	public slots: // Initilisation de la fenêtre.
		void creerReglages(); // Règlages de la fenêtre (titre, icône...).
		void creerBarreMenus(); // Barre des menus de la fenêtre (fichier, affichage...).
		void creerActions(); // Actions du programme (quitter, preferences...).
		void creerMenus(); // Menus de la fenêtre (fichier, affichage...).
		void creerBarreEtat();
		void chargerPlugins();
		void peuplerMenus(BaseAction *plugin);
		void peuplerDocks(BaseDockWidget *plugin);
		void peuplerOnglets(BaseOnglet *plugin);
		void peuplerWidgets(BaseWidget *plugin);
		void actionWidgetClicked();
		void updateSubWindowsMenu(QString windowTitle);
		void disableEnableWidgetsMenuActions();

	public slots: // Slots personnalisés.
		void slotActivationSystemTray(QSystemTrayIcon::ActivationReason reason);
		void slotTelechargements();
		void slotDesactiverIconeTelechargements();
		void slotGestionnaireDesPlugins();
		void slotQuitter();
		void slotGoToHome();
		void slotPreferences();
		void slotBarreEtat();
		void slotTailleParDefaut();
		void slotPleinEcran();
		void slotBlocNotes();
		void slotOrganisateur();
		void slotCompteurDeLignes();
		void slotCompresseurDeCode();
		void slotGenerateurPageWeb();
		void slotGenerateurClassesCpp();
		void slotCreerFichier();
		void slotFeedTarsiers();
		void slotPlusOuMoins();
		void slotCreerReadme();
		void slotSurLeWeb();
		void slotObtenirPlugins();
		void slotAPropos();
		void slotAstuces();

	public slots:
		void appliquerReglages();
		void ouvrirFichier(QString chemin);
		void ouvrirFichierGenere(QString chemin);

	public slots:
		void verifierMAJ();
		void verificationMajProgresse(qint64 recu, qint64 total);
		void comparerMaj();
		void annulerVerificationMaj();
		void MajDisponible();
		void erreurDL(QNetworkReply::NetworkError);
		void assistantDeMiseAJour();

	public slots:
		void telechargementNouvelleVersionContinue(qint64 recu, qint64 total);
		void telechargementNouvelleVersionFini();
		void arreterTelechargementNouvelleVersion();

	private: // Les menus.
		QMenu *menuMultiuso;
		QMenu *menuEdition;
		QMenu *menuAffichage;
		QMenu *sousMenuWidgets;
		QMenu *sousMenuProgrammation;
		QMenu *sousMenuMiniJeux;
		QMenu *menuAutresFonctionnalites;
		QMenu *menuAide;

		QMenu *menuSystemTray;

	private: // Éléments composant la fenêtre.
		QSystemTrayIcon *systemTray;
		QStatusBar *barreDEtat;
		QTabWidget *pagesDuProgramme;
		QVBoxLayout *layoutCentral;
		QWidget *widgetCentral;
		TelechargerFichier *telechargements;

	private: // Pages principales
		Accueil *accueil;
		NavFichiers *navFichiers;
		Widgets *widgets;
		EditeurDeTexte *editeurDeTexte;
		EditeurDeCode *editeurDeCode;
		Ftp *ftp;
		NavigateurWeb *navigateur;
		VisionneurImages *visionneurImages;

	private:
		QWidget *tabAccueil;
		QWidget *tabNavFichiers;
		QWidget *tabWidgets;
		QWidget *tabEditeurDeTexte; 
		QWidget *tabEditeurDeCode;
		QWidget *tabFtp;
		QWidget *tabNavigateur;
		QWidget *tabVisionneurImages;

	private: // Les actions.

			/* MENU FICHIER */
		QAction *actionTelechargements;
		QAction *actionVerifierMaj;
		QAction *actionAssistantMaj;
		QAction *actionGestionnaireDesPlugins;
		QAction *actionQuitter;

			/* MENU ÉDITION */
		QAction *actionGoToHome;
		QAction *actionPreferences;

			/* MENU AFFICHAGE */
		QAction *actionBarreEtat;
		QAction *actionTailleParDefaut;
		QAction *actionPleinEcran;
		QAction *actionSubCalendrier;
		QAction *actionSubCalculatrice;
		QAction *actionSubChronometre;

			/* MENU AUTRES FONCTIONNALITÉS	*/
		QAction *actionBlocNotes;
		QAction *actionOrganisateur;
		QAction *actionCompteurDeLignes;
		QAction *actionCompresseurDeCode;
		QAction *actionGenerateurPageWeb;
		QAction *actionGenerateurClassesCpp;
		QAction *actionCreerFichier;
		QAction *actionFeedTarsiers;
		QAction *actionPlusOuMoins;
		QAction *actionCreerReadme;

			/* MENU AIDE */
		QAction *actionSurLeWeb;
		QAction *actionObtenirPlugins;
		QAction *actionAPropos;
		QAction *actionAProposDeQt;
		QAction *actionAstuces;

	private:
		QProgressDialog *verificationMaj;
		QNetworkReply *fichierMaj;
		QString derniereMaj;
		bool confirmationVerificationMaj;
		QStringList noms;

	private:
		QProgressDialog *telechargerNouvelleVersion;
		QNetworkReply *nouvelleVersion;
		QString emplacementTelechargementNouvelleVersion;

	private:
		bool loaded;
};

#endif
