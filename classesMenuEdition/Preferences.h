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

#ifndef HEADER_PREFERENCES
#define HEADER_PREFERENCES

#include "../CurrentIncludes.h"
#include "autresClasses/PasswordDialog.h"
#include "classesPrincipales/FenPrincipale.h"

class Preferences : public QDialog
{
	Q_OBJECT

	public:
		Preferences(FenPrincipale *parent);

	public slots:
		void slotRemettreAZero();
		void afficherPreferences();
		void enregistrerPreferences();
		void reglagesParDefaut();

		void slotParcourirDossierDL();
		void slotEditerStyle();
		void slotParcourirStyle();
		void slotParcourirDossierAccueil();
		void slotChangementItem(int item);

		void slotViderCache();
		void slotEffacerCookies();

		void changementMoteurDeRecherche(QString newOne);
		void changementEtatStyle(bool);
		void checkUsePassword(bool toggled);

		void randomBackgroundStateChanged(int);
		void screenshotBackgroundStateChanged(int);
		
	// <Stalker (www.siteduzero.com)>
		private slots:
			void parametresProxysIdentiques(int etat);
			void changementAdresseProxyHTTP(QString adresse);
			void changementPortProxyHTTP(int port);
	// </Stalker (www.siteduzero.com)>

	private:
	// Général
		QCheckBox *choixFermerFenetre;
		QCheckBox *choixOuvertureFenetre;
		QCheckBox *choixEffetsDeDeplacement;
		QCheckBox *choixUtiliserSplashScreen;
		QCheckBox *choixRedemarrerApresRemiseAZero;
		QCheckBox *choixUtiliserMdp;

		QLineEdit *dossierTelechargements;

		QFormLayout *layoutGeneral;

	// Accueil
		QCheckBox *randomBackground;
		QCheckBox *fondScreenshot;

		QFormLayout *layoutAccueil;

	// Navigateur de fichiers
		QLineEdit *welcomeFolder;

		QFormLayout *layoutNavFichiers;

	// Éditeur de texte
		QCheckBox *enregistrementAutomatique;

		QFormLayout *layoutEditeurDeTexte;

	// Éditeur de code
		QCheckBox *enregistrementAutomatiqueCode;
		QCheckBox *indentationAutomatiqueCode;

		QFormLayout *layoutEditeurDeCode;

	// Navigateur Web
		QLineEdit *pageAccueil;
		QComboBox *moteurDeRecherche;
		QLineEdit *moteurPersonnalise;
		QCheckBox *nouvelOngletMoteurRecherche;
		QCheckBox *contenuEditable;
		QCheckBox *activerJavascript;
		QCheckBox *activerJava;
		QCheckBox *activerFlash;
		QCheckBox *zoomTextOnly;
		QCheckBox *imprimerElementsFond;
		QCheckBox *utiliserHistorique;
		QCheckBox *restaurerOnglets;

		QVBoxLayout *layoutNavigateurWeb;
		
	// <Stalker (www.siteduzero.com)>
		// Réseau
			QVBoxLayout *layoutProxy;
			QGridLayout *layoutParametresProxy;
			QCheckBox *activerParametresIdentiquesProxy;
			QFormLayout *layoutParametresProxyText;
			QFormLayout *layoutParametresProxyNombre;
			QGroupBox *groupboxParametresProxy;
			QLineEdit *adresseProxyHTTP;
			QLineEdit *adresseProxyFTP;
			QLineEdit *adresseProxySSL;
			QLineEdit *adresseHoteSOCKS;
			QSpinBox *portProxyHTTP;
			QSpinBox *portProxyFTP;
			QSpinBox *portProxySSL;
			QSpinBox *portHoteSOCKS;
			QLineEdit *pasProxyPour;
	// </Stalker (www.siteduzero.com)>

	// Style
		QRadioButton *styleDeBase;
		QRadioButton *stylePersonnalise;
			QPushButton *editerLeStyle;
		QRadioButton *styleExterne;
			QPushButton *parcourirStyleExterne;
			QLineEdit *afficherStyleExterne;
			QHBoxLayout *layoutStyleExterne;
		QRadioButton *stylesQt;
			QComboBox *listeStylesQt;

		QFormLayout *layoutStyle;

	// Et le reste...
		QListWidget *listeOptions;

		QWidget *widgetLayoutGeneral;
		QWidget *widgetLayoutStyle;
		QWidget *widgetLayoutAccueil;
		QWidget *widgetLayoutNavigateurFichiers;
		QWidget *widgetLayoutEditeurDeTexte;
		QWidget *widgetLayoutEditeurDeCode;
		QWidget *widgetLayoutNavigateurWeb;
		// <Stalker (www.siteduzero.com)>
			QWidget *widgetLayoutProxy;
		// </Stalker (www.siteduzero.com)>

		QStackedWidget *widgetWidgetsContenu;

		QWidget *widgetConteneur;
		QHBoxLayout *layoutContenu;

		QPushButton *remettreAZero;
			QHBoxLayout *layoutGauche;

		QPushButton *enregistrer;
		QPushButton *parDefaut;
		QPushButton *annuler;
			QHBoxLayout *layoutDroite;

		QHBoxLayout *layoutBoutons;

		QVBoxLayout *layoutPreferences;

		FenPrincipale *pointeurSurParent; // Pour accéder à la fenêtre parent
		
		bool loaded;
};

#endif
