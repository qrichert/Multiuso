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

#ifndef HEADER_GENERATEUR_CLASSES_CPP
#define HEADER_GENERATEUR_CLASSES_CPP

#include "../CurrentIncludes.h"

class GenerateurClassesCpp : public QDialog
{
	Q_OBJECT

	public:
		GenerateurClassesCpp(QWidget *parent);

	public slots:
		void slotGenerer();
		void slotOuvrirFichier(QString fichier);

	signals:
		void ouvrirFichierIntermediaire(QString texte);

	private:
		QLineEdit *nomDeLaClasse;
		QLineEdit *classeMere;
		QFormLayout *layoutDefinition;
		QGroupBox *groupeDefinition;

		QCheckBox *inclusionsMultiples;
		QCheckBox *genererDestructeur;
		QFormLayout *layoutOptions;
		QGroupBox *groupeOptions;

		QLineEdit *auteur;
		QLineEdit *nomDuProgramme;
		QCheckBox *gnuGpl;
		QTextEdit *commentaires;
		QFormLayout *layoutCommentaires;
		QGroupBox *groupeCommentaires;

		QPushButton *generer;
};

#endif
