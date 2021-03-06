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

#ifndef HEADER_WIDGETS
#define HEADER_WIDGETS

#include "../CurrentIncludes.h"
#include "autresClasses/MdiSubWindow.h"

class Widgets : public QWidget
{
	Q_OBJECT

	public:
		Widgets(QWidget *parent);

		void newSubWindow(MdiSubWindow *newSubWindow);
		void showWindow(QString title);
		void openTodayNotes();

	public slots:
		void creerFenetreCalendrier();
		void creerFenetreCalculatrice();
		void creerFenetreChronometre();
		void subWindowClosed();

	signals:
		void subWindowWasClosed(QString title);

	private:
		QCalendarWidget *calendrier;
		MdiSubWindow *fenetreCalendrier;
		MdiSubWindow *fenetreCalculatrice;
		MdiSubWindow *fenetreChronometre;

	private:
		QMdiArea *aireCentrale;
};

#endif
