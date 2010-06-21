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

#ifndef HEADER_EDITEUR_DE_CODE
#define HEADER_EDITEUR_DE_CODE

#include "../CurrentIncludes.h"
#include "autresClasses/CodeEdit.h"

class EditeurDeCode : public QMainWindow
{
	Q_OBJECT

	public:
		EditeurDeCode(QWidget *parent = 0);

		void highlighterFor(QString extension);
		int highlighterIndex(QString highlighterName);
		void openContent(QString content, QString title);
		void setWebBrowser(QMainWindow *browser);

	public slots:
		void creerOptionsTexte();
		void slotChangementTexte();
		void slotChangementOnglet(int onglet);
		void slotFermerOnglet(int onglet);

		void changementHighlighter(int highlighter);

		void sauvegarderEtat();
		bool tousLesDocumentsEnregistres();

	public slots:
		QWidget *nouvelOnglet();
		CodeEdit *pageActuelle();
		QString titreTabCourrant();

	public slots:
		void slotAMRedo(bool available);
		void slotAMUndo(bool available);
		void slotRemettreValeurs();

	public slots:
		void slotSelectionMajuscule();
		void slotSelectionMinuscule();

		void slotNouveau();
		void setTextActuel(QString texte);
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

		void slotRepeterTexte();
		void slotRepeterTexte2();
		void slotRepeter(QString texte, int nombreDeFois);

		void slotOpenInWebBrowser();

	private:
		QToolBar *toolBarOptions;

	private:
		QTabWidget *onglets;

	private:
		QString s_motARechercher;
		QString s_motARemplacerPar;

	private:
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

		QAction *repeterTexte;

		QAction *selectionMajuscule;
		QAction *selectionMinuscule;

		QComboBox *colorisation;

		QAction *openInWebBrowser;

	private:
		QLineEdit *l_rechercher;
		QLineEdit *l_remplacer;

	private:
		QTextEdit *texteARepeter;
		QLineEdit *nombreDeFois;

	private:
		QWidget *pointeurSurParent;
		QMainWindow *webBrowserPointer;
};

#endif
