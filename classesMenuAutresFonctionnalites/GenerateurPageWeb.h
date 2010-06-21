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

#ifndef HEADER_GENERATEUR_PAGE_WEB
#define HEADER_GENERATEUR_PAGE_WEB

#include "../CurrentIncludes.h"

class GenerateurPageWeb : public QDialog
{
	Q_OBJECT

	public:
		GenerateurPageWeb(QWidget *parent);

	public slots:
		void slotGenerer();
		void slotOuvrirFichier(QString texte);

	signals:
		void ouvrirFichierIntermediaire(QString texte);

	private:
		QLineEdit *titre;
		QFormLayout *formulaireTitre;
		QGroupBox *groupeTitre;

		QRadioButton *transitional;
		QRadioButton *strict;
		QFormLayout *formulaireDoctype;
		QGroupBox *groupeDoctype;

		QRadioButton *utf8;
		QRadioButton *iso88591;
		QFormLayout *formulaireEncodage;
		QGroupBox *groupeEncodage;

		QLineEdit *nomDuDesign;
		QLineEdit *emplacementDuDesign;
		QFormLayout *formulaireDesign;
		QGroupBox *groupeDesign;

		QLineEdit *emplacementFavicon;
		QFormLayout *formulaireFavicon;
		QGroupBox *groupeFavicon;

		QTextEdit *commentaires;
		QVBoxLayout *layoutCommentaires;
		QGroupBox *groupeCommentaires;

		QPushButton *generer;
};

#endif
