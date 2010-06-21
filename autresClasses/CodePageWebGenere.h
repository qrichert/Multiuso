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

#ifndef HEADER_CODE_PAGE_WEB_GENERE
#define HEADER_CODE_PAGE_WEB_GENERE

#include "../CurrentIncludes.h"

class CodePageWebGenere : public QDialog
{
	Q_OBJECT

	public:
		CodePageWebGenere(QString &codeGenere, QString &titrePage, QWidget *parent);

	public slots:
		void slotEnregistrerCode();
		void erreur(QString erreur);
		void slotOuvrirCode();

	signals:
		void ouvrirFichier(QString texte);

	private:
		QTextEdit *code;

		QPushButton *enregistrer;
		QLineEdit *nom;
		QLineEdit *extension;

		QHBoxLayout *layoutOptions;

		QVBoxLayout *layoutPrincipal;
};

#endif
