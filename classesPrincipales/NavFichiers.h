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

#ifndef HEADER_NAVFICHIERS
#define HEADER_NAVFICHIERS

#include "../CurrentIncludes.h"
#include "autresClasses/VueDossier.h"
#include "autresClasses/LineEdit.h"
#include "classesPrincipales/EditeurDeTexte.h"
#include "classesPrincipales/EditeurDeCode.h"
#include "classesPrincipales/NavigateurWeb.h"
#include "classesPrincipales/VisionneurImages.h"


class NavFichiers : public QMainWindow
{
	Q_OBJECT

	public:
		NavFichiers(QWidget *parent = 0);

		void creerActions();
		VueDossier *pageActuelle();

		void setVisionneurImages(VisionneurImages *widget);
		void setEditeurDeTexte(EditeurDeTexte *widget);
		void setEditeurDeCode(EditeurDeCode *widget);
		void setNavigateurWeb(NavigateurWeb *widget);

	public slots:
		void ajouterOnglet();
		QWidget *nouvelOnglet();
		void slotFermerOnglet();
		void slotFermerOnglet(int onglet);
		void slotOngletChange(int);
		void slotDebutChargement();
		void slotFinChargement();
		void slotUpdateAffichage();
		void slotPrecedent();
		void slotSuivant();
		void slotHaut();
		void slotGo();
		void slotActualiser();
		void sauvegarderEtat();
		void slotOpenFile(QString file);

	public slots:
		void slotDossierPersonnel();
		void slotDossierTelechargements();
		void slotDossiersCaches();
		void slotDossierRacine();
		void slotDossierTmp();
		void slotSwitchTab();

	private:
		QWidget *p_fenPrincipale;
		VisionneurImages *p_visionneurImages;
		EditeurDeTexte *p_editeurDeTexte;
		EditeurDeCode *p_editeurDeCode;
		NavigateurWeb *p_navigateurWeb;

	private:
		QTabWidget *onglets;

	private:
		QAction *actionPrecedent;
		QAction *actionSuivant;
		QAction *actionHaut;
		QAction *actionActualiser;
		QAction *actionDossierPersonnel;
		QAction *actionDossierTelechargements;
		QAction *actionDossiersCaches;
		QAction *actionDossierRacine;
		QAction *actionDossierTmp;
		LineEdit *afficherCheminActuel;
		QAction *actionGo;
		QAction *actionNouvelOnglet;
		QAction *actionFermerOnglet;
};

#endif
