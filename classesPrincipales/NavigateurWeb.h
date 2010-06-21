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

#ifndef HEADER_NAVIGATEURWEB
#define HEADER_NAVIGATEURWEB

#include "../CurrentIncludes.h"
#include <QtWebKit>
#include "classesPrincipales/TelechargerFichier.h"
#include "classesPrincipales/EditeurDeCode.h"
#include "autresClasses/LineEdit.h"
#include "autresClasses/CodeEdit.h"
#include "autresClasses/WebPageScreenshot.h"


class NavigateurWeb : public QMainWindow
{
	Q_OBJECT

	public:
		NavigateurWeb(QWidget *parent = 0, TelechargerFichier *telechargements = 0, EditeurDeCode *editeurDeCode = 0);

		void openCodeEditDocument(QString title, QString contents);

	public slots:
		void slotFermerOnglet(int onglet);
		void speedDial();
		void slotOuvrirUrlSpeedDial(QString url);
		QWebView *pageActuelle();
		QWidget *nouvelOnglet();
		QIcon faviconUrl(QString url);
		void sauvegarderEtat();
		void slotEffacerHistorique();
		void actualiserHistoriqueEfface();
		void actualiserFavoris();
		void configurerFavoris();
		QPixmap capturerPage();

	public slots:
		void slotNouvelOnglet();
		void slotFermerOnglet();
		void slotLinkClicked(QUrl url);
		void slotCodeSource();
		void slotHistorique();
		void slotOuvrirHistorique(QModelIndex index);
		void slotOuvrirCode();
		void slotSelectionFichierPourOuvrir();
		void slotOuvrirFichier(QString fichier);
		void slotRechercher();
		void slotCacherRecherche();
		void slotMotPrecedent();
		void slotMotSuivant();
		void slotZoomPlus();
		void slotZoomEgal();
		void slotZoomMoins();
		void slotPrecedent();
		void slotSuivant();
		void slotRecharger();
		void slotStop();
		void slotAccueil();
		void slotChangerAdresse();

		// <Easter Egg>

		void slotEasterEgg();

		// </Easter Egg>

		void slotRechercherMoteurRecherche();
		void slotAjouterFavori();
		void slotOuvrirFavori();
		void slotOuvrirUrl(QString url);
		void slotOpenLinkInNewTab();

		void slotCopier();

	public slots:
		void telechargerFichierAuto(QNetworkReply *reponse);
		void telechargerFichier(QNetworkRequest requete);
		void slotTakeScreenshot();
		void slotScreenshotTaken(QString title, QString url, QPixmap pixmap);
		void slotScreenshotNotTaken();

	public slots:
		void survolLien(QString lien, QString titre, QString contenu);
		void changementIcone();
		void chargementFini(bool ok);
		void chargementContinue(int progressionDuChargement);
		void debutChargement();
		void changementDeTitre(QString titre);
		void changementUrl(QUrl url);
		void ongletChange(int onglet);
		void slotImprimer(QWebFrame *);
		void slotImprimerPage(QPrinter *imprimante);
		void slotDatabaseQuotaExceeded(QWebFrame *frame, QString name);
		void slotOuvrirMenuNavigation();
		void slotMenuPage(QPoint);
		// <Stalker (www.siteduzero.com)>
			void slotErrorPageCheck(QNetworkReply *netRep);
		// </Stalker (www.siteduzero.com)>

	private:
		QTabWidget *onglets;
		QStatusBar *statut;
		QProgressBar *progression;
		QLabel *termine;
		QWidget *pointeurSurParent;
		TelechargerFichier *pointeurSurTelechargements;
		EditeurDeCode *pointeurSurEditeurDeCode;

	private:
		QAction *actionNouveau;
		QAction *actionFermer;
		QAction *actionSpeedDial;
		QAction *actionCodeSource;
		QAction *actionHistorique;
		QAction *actionOuvrirCode;
		QAction *actionOuvrir;
		QAction *actionRechercher;
		QAction *actionZoomPlus;
		QAction *actionZoomEgal;
		QAction *actionZoomMoins;
		QAction *actionPrecedent;
		QAction *actionSuivant;
		QAction *actionRecharger;
		QAction *actionStop;
		QAction *actionAccueil;
		LineEdit *barreAdresse;
		QAction *actionGo;
		WebPageScreenshot *screen;
		QAction *actionTakeScreenshot;

	private:
		QToolBar *toolBarNavigation;
		QMenu *menuNavigation;
		QMenu *menuFavoris;
		QAction *ajouterFavori;

	private:
		QStringList liensHistorique;
		QStringList sitesVisites;
		QString lastLink;

	private:
		QToolBar *toolBarRechercher;
		QAction *actionCacherRecherche;
		QLineEdit *motARechercher;
		QAction *actionMotPrecedent;
		QAction *actionMotSuivant;
		QCheckBox *respecterCasse;

	private:
		QDialog *widgetSpeedDial;

	private:
		QDockWidget *dockHistorique;
		QStringListModel *modeleHistorique;
		QListView *vueHistorique;
		
	// <Stalker (www.siteduzero.com)>
		private:
 		       QHttp *httpConnexion;
 		       QNetworkProxy *proxy;
	// </Stalker (www.siteduzero.com)>

	private slots:
		void creerActions();
};

#endif
