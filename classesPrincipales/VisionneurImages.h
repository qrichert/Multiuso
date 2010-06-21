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

#ifndef HEADER_VISIONNEURIMAGES
#define HEADER_VISIONNEURIMAGES

#include "../CurrentIncludes.h"

class VisionneurImages : public QMainWindow
{
	Q_OBJECT

	public:
		VisionneurImages(QWidget *parent);

	public slots:
		void slotOuvrir();
		void slotFermer();
		void slotZoomPlus();
		void slotZoomNormal();
		void slotZoomMoins();
		void slotDiaporama();

	public slots:
		void slotOuvrirFichier(QString fichier);
		void zoomer(double facteurZoom);
		void changerDiapo();
		void ajusterScrollBar(QScrollBar *scrollBar, double facteurZoom);
		void sauvegarderEtat();

	signals:

	private:
		QScrollArea *area;
		QLabel *labelImage;
		double zoom;
		QString dossierDiaporama;
		QStringList diapos;
		int diapoNumeroX;
		QTimer *timer;

	private:
		QAction *actionOuvrir;
		QAction *actionFermer;
		QAction *actionZoomPlus;
		QAction *actionZoomNormal;
		QAction *actionZoomMoins;
		QAction *actionDiaporama;
};

#endif
