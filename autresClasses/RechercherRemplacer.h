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

#ifndef HEADER_RECHERCHER_REMPLACER
#define HEADER_RECHERCHER_REMPLACER

#include "../CurrentIncludes.h"

class RechercherRemplacer : public QDialog
{
	Q_OBJECT

	public:
		RechercherRemplacer(QWidget *parent, QTextEdit *texte, const int onglet, const QString selection);

	public slots:
		void slotRechercher();
		void slotRechercher2(QTextEdit *texteFantome);
		void slotRemplacer();
		QString slotRemplacer2(QString &texteComplet);

	private:
		QTextEdit *texteFantome;
		QString texteComplet;

		QVBoxLayout *layoutPrincipal;

		QLineEdit *motARechercher;
		QLineEdit *motARemplacer;

		QFormLayout *layoutMots;

		QCheckBox *rechercheArriere;
		QCheckBox *rechercheSensibleCasse;
		QCheckBox *rechercheSeulementMotsComplets;

		QFormLayout *layoutOptions;

		QPushButton *annuler;
		QPushButton *remplacer;
		QPushButton *rechercher;

		QHBoxLayout *layoutBoutons;
};

#endif
