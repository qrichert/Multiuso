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

#ifndef HEADER_EDITEUR_DE_TEXTE
#define HEADER_EDITEUR_DE_TEXTE

#include "../CurrentIncludes.h"
#include "autresClasses/TextEdit.h"

class EditeurDeTexte : public QMainWindow
{
	Q_OBJECT

	public:
		EditeurDeTexte(QWidget *parent);

	public slots:
		void creerOptionsTexte();
		void slotChangementTexte();
		void slotChangementOnglet(int onglet);
		void slotFermerOnglet(int onglet);

		void sauvegarderEtat();
		bool tousLesDocumentsEnregistres();

	public slots:
		QWidget *nouvelOnglet();
		TextEdit *pageActuelle();
		QString titreTabCourrant();

	public slots:
		void slotAMRedo(bool available);
		void slotAMUndo(bool available);
		void slotRemettreValeurs();

	public slots:
		void slotGras();
		void slotItalique();
		void slotSouligne();

		void slotGauche();
		void slotCentre();
		void slotDroite();
		void slotJustifie();

		void changerTaille(QString);

		void changerPolice(QFont);

		void changerCouleur();

		void slotSelectionMajuscule();
		void slotSelectionMinuscule();

		void slotNouveau();
		void slotEnregistrer();
		void slotEnregistrerSous();
		void slotOuvrir();
		void slotOuvrirFichier(QString cheminFichier);

		void slotAnnuler();
		void slotRetablir();

		void slotSupprimerSelection();

		void slotRechercher();
		void slotRechercher2();

		void slotRechercherRemplacer();
		void slotRechercherRemplacer2();

		void slotImprimer();
		void imprimerTexte(QPrinter *imprimante);

		void slotCopier();
		void slotCouper();
		void slotColler();
		void slotToutSelectionner();

		void slotInsererImage();

		void slotRepeterTexte();
		void slotRepeterTexte2();
		void slotRepeter(QString texte, int nombreDeFois);

	private:
		QToolBar *toolBarTexte;
		QToolBar *toolBarOptions;

	private:
		QTabWidget *onglets;

	private:
		QString s_motARechercher;
		QString s_motARemplacerPar;

	private:
		QAction *gras;
		QAction *italique;
		QAction *souligne;

		QAction *gauche;
		QAction *droite;
		QAction *centre;
		QAction *justifie;

		QComboBox *taille;
		QFontComboBox *police;
		QAction *couleur;

		QAction *selectionMajuscule;
		QAction *selectionMinuscule;

		QAction *nouveau;
		QAction *enregistrer;
		QAction *enregistrerSous;
		QAction *ouvrir;

		QAction *annuler;
		QAction *retablir;

		QAction *supprimerSelection;

		QAction *rechercher;
		QAction *rechercherRemplacer;

		QAction *imprimer;

		QAction *copier;
		QAction *couper;
		QAction *coller;
		QAction *toutSelectionner;

		QAction *insererImage;
		QAction *repeterTexte;

	private:
		QLineEdit *l_rechercher;
		QLineEdit *l_remplacer;

	private:
		QTextEdit *texteARepeter;
		QLineEdit *nombreDeFois;
};

#endif
