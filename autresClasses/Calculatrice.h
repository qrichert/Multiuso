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

#ifndef HEADER_CALCULATRICE
#define HEADER_CALCULATRICE

#include "../CurrentIncludes.h"

class Calculatrice : public QWidget
{
	Q_OBJECT

	public:
		Calculatrice(QWidget *parent);

	public slots:
		void creerBoutons();

	public slots:
		void ajouterPlus();
		void ajouterMoins();
		void ajouterDivise();
		void ajouterMultiplie();
		void ajouterPuissance();
		void ajouterCarre();
		void ajouterCube();
		void ajouterRacineCarree();
		void ajouterEgal();
		void ajouterEffacerCaractere();
		void ajouterViderLigne();
		void ajouterRemettreZero();
		void ajouterVirgule();
		void ajouterAns();

		void ajouterZero();
		void ajouterUn();
		void ajouterDeux();
		void ajouterTrois();
		void ajouterQuatre();
		void ajouterCinq();
		void ajouterSix();
		void ajouterSept();
		void ajouterHuit();
		void ajouterNeuf();

	private:
		QLineEdit *operation;

		QPushButton *plus;
		QPushButton *moins;
		QPushButton *divise;
		QPushButton *multiplie;
		QPushButton *puissance;
		QPushButton *carre;
		QPushButton *cube;
		QPushButton *racineCarree;
		QPushButton *egal;
		QPushButton *effacerCaractere;
		QPushButton *viderLigne;
		QPushButton *remettreZero;
		QPushButton *virgule;
		QPushButton *ans;

		QPushButton *zero;
		QPushButton *un;
		QPushButton *deux;
		QPushButton *trois;
		QPushButton *quatre;
		QPushButton *cinq;
		QPushButton *six;
		QPushButton *sept;
		QPushButton *huit;
		QPushButton *neuf;

	private:
		QGridLayout *layoutCalculatrice;
		QHBoxLayout *layoutOperation;

	private:
		double premierNombre;
		double secondNombre;
		double resultat;
		double nombreAns;
		QString operateur;
		bool currentResult;
};

#endif
