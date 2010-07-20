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
#include "classesPrincipales/FenPrincipale.h"

enum Level
{
	VERY_HIGH,
	HIGH,
	GOOD,
	LOW,
	VERY_LOW,
	NUL
};

class SecurityLevel : public QWidget
{
	Q_OBJECT

	public:
		SecurityLevel()
		{
			setFixedHeight(25);
			m_level = NUL;
		}

		void setLevel(Level level)
		{
			m_level = level;

			update();
		}

	protected:
		void paintEvent(QPaintEvent *)
		{
			float level = 0.0;
			int minus = 2;

			switch (m_level)
			{
				case VERY_HIGH : level = 1; minus = 0; break;
				case HIGH : level = 0.75; break;
				case GOOD : level = 0.5; break;
				case LOW : level = 0.25; break;
				case VERY_LOW: level = 0.1; break;
				case NUL : level = 0; break;
			}

			QLinearGradient gradient(0, 0, width(), 0);
				gradient.setColorAt(0, Qt::darkGreen);
				gradient.setColorAt(0.1, "#27c103");
				gradient.setColorAt(0.25, Qt::green);
				gradient.setColorAt(0.5, Qt::yellow);
				gradient.setColorAt(0.75, "#ff6c00");
				gradient.setColorAt(1, Qt::red);

			QPainter painter(this);
				painter.setPen("#303030");
				painter.setBrush(gradient);
					painter.drawRect(0, 0, width() - 1, height() - 1);

				painter.setPen(Qt::NoPen);
					painter.fillRect(QRectF(1 + (width() * level), 1, (width() - (width() * level)) - minus, 23), Qt::white);
		}

	private:
		Level m_level;
};

class PasswordDialog : public QDialog
{
	Q_OBJECT

	public:
		PasswordDialog(QWidget *parent) : QDialog(parent)
		{
			setWindowTitle("Définition du mot de passe");
			setWindowIcon(QIcon(":/icones/preferences/password.png"));

			password = new QLineEdit;
				password->setEchoMode(QLineEdit::Password);
				connect(password, SIGNAL(textChanged(QString)), this, SLOT(slotPasswordChanged(QString)));

			passwordCheck = new QLineEdit;
				passwordCheck->setEchoMode(QLineEdit::Password);
				connect(passwordCheck, SIGNAL(textChanged(QString)), this, SLOT(slotPasswordCheck(QString)));

			level = new SecurityLevel;

			QFormLayout *layout = new QFormLayout;
				layout->addRow("Mot de passe :", password);
				layout->addRow("Confirmation :", passwordCheck);
				layout->addRow("Niveau de sécurité :", level);

			QPushButton *rejectButton = new QPushButton("Annuler");
				connect(rejectButton, SIGNAL(clicked()), this, SLOT(reject()));

			QHBoxLayout *buttons = new QHBoxLayout;
				buttons->addWidget(rejectButton);
				buttons->addWidget(Multiuso::closeButton(this, "OK"));
				buttons->setAlignment(Qt::AlignRight);

			QVBoxLayout *mainLayout = new QVBoxLayout(this);
				mainLayout->addLayout(layout);
				mainLayout->addLayout(buttons);
		}

		QString getPassword()
		{
			return password->text();
		}

	public slots:
		void slotPasswordChanged(QString pwd)
		{
			QRegExp rx1 = QRegExp("^[0-9]{2, 20}$");
			QRegExp rx2 = QRegExp("^([a-z]{4, 20} | [A-Z]{2, 20})$");
			QRegExp rx3 = QRegExp("^[^0-9a-zA-Z]{2, 20}$");

			if (pwd.contains("1"))
				level->setLevel(VERY_LOW);

			else if (pwd.contains("2"))
				level->setLevel(LOW);

			else if (pwd.contains("3"))
				level->setLevel(GOOD);

			else if (pwd.contains("4") && pwd.contains("4"))
				level->setLevel(HIGH);

			else if (pwd.contains("5") && pwd.contains("5") && pwd.contains("5"))
				level->setLevel(VERY_HIGH);

			else
				level->setLevel(NUL);

			slotPasswordCheck(passwordCheck->text());
		}

		void slotPasswordCheck(QString pwd)
		{
			if (pwd == password->text())
				passwordCheck->setStyleSheet("background-color : #bbffbb");
				
			else
				passwordCheck->setStyleSheet("background-color : #ffb3b3");
		}

		void accept()
		{
			if (password->text() == passwordCheck->text())
			{
				if (!password->text().isEmpty())
					QDialog::accept();

				else
					QMessageBox::critical(this, "Multiuso", "Il faut entrer un mot de passe !");
			}

			else
			{
				QMessageBox::critical(this, "Multiuso", "Les mots de passe diffèrent !");
			}
		}

	private:
		QLineEdit *password;
		QLineEdit *passwordCheck;

		SecurityLevel *level;
};

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

	public slots:
		void slotParcourirDossierDL();
		void slotEditerStyle();
		void slotParcourirStyle();
		void slotParcourirDossierAccueil();
		void slotChangementItem(int item);

		void slotViderCache();
		void slotEffacerCookies();

	public slots:
		void changementMoteurDeRecherche(QString newOne);
		void changementEtatStyle(bool);
		void checkUsePassword(bool toggled);
		
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
};

#endif
