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

#ifndef HEADER_CHRONOMETRE
#define HEADER_CHRONOMETRE

#include "../CurrentIncludes.h"

class Chronometre : public QWidget
{
	Q_OBJECT

	public:
		Chronometre(QWidget *parent);

	public slots:
		void slotDemarrer();
		void slotActualiser();
		void chronometreChangementEtat(bool etat);
		void minuterieChangementEtat(bool etat);
		void slotReinitialiser();
		void slotAfficherMaximumCourant(int valeur);

	private:
		QTimer *temps;
		int tempsEcoule;
		int tempsFin;
		int tempsRestant;
		QSpinBox *heures;
		QSpinBox *minutes;
		QSpinBox *secondes;
		//QSpinBox *millisecondes;
		QHBoxLayout *hms;
		QDial *representationTemps;
		QRadioButton *chronometre;
		QRadioButton *minuterie;
		QFormLayout *layoutType;
		QCheckBox *ouvrirBoiteDeDialogue;
		QFormLayout *layoutOptions;
		QPushButton *demarrer;
		QGridLayout *layoutPrincipal;
		bool premierPassage;
};

#endif
