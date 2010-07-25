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

#include "Widgets.h"
#include "autresClasses/Calendrier.h"
#include "autresClasses/Calculatrice.h"
#include "autresClasses/Chronometre.h"

Widgets::Widgets(QWidget *parent = 0) : QWidget(parent)
{
	creerFenetreCalendrier();
	creerFenetreCalculatrice();
	creerFenetreChronometre();

	aireCentrale = new QMdiArea;
		aireCentrale->addSubWindow(fenetreCalendrier);
		aireCentrale->addSubWindow(fenetreCalculatrice);
		aireCentrale->addSubWindow(fenetreChronometre);
		aireCentrale->cascadeSubWindows();

	QPushButton *vueCascade = new QPushButton;
		vueCascade->setIcon(QIcon(":/icones/widgets/vueCascade.png"));
		vueCascade->setToolTip("Afficher les sous-fenêtres en cascade");
		connect(vueCascade, SIGNAL(clicked()), aireCentrale, SLOT(cascadeSubWindows()));

	QPushButton *vueTuile = new QPushButton;
		vueTuile->setIcon(QIcon(":/icones/widgets/vueTuiles.png"));
		vueTuile->setToolTip("Afficher les sous-fenêtres en tuile");
		connect(vueTuile, SIGNAL(clicked()), aireCentrale, SLOT(tileSubWindows()));

	QVBoxLayout *layoutOptions = new QVBoxLayout;
		layoutOptions->addWidget(vueCascade);
		layoutOptions->addWidget(vueTuile);
		layoutOptions->setContentsMargins(0, 0, 0, 0);

	QWidget *widgetOptions = new QWidget(aireCentrale);
		widgetOptions->setLayout(layoutOptions);
		widgetOptions->move(10, 10);

	QVBoxLayout *layoutCentral = new QVBoxLayout(this);
		layoutCentral->addWidget(aireCentrale);
		layoutCentral->setContentsMargins(0, 0, 0, 0);
}

void Widgets::newSubWindow(MdiSubWindow *newSubWindow)
{
	aireCentrale->addSubWindow(newSubWindow);
	aireCentrale->cascadeSubWindows();
}

void Widgets::creerFenetreCalendrier()
{
	Calendrier *calendrier = new Calendrier(this);

	fenetreCalendrier = new MdiSubWindow;
		fenetreCalendrier->setWidget(calendrier);
		fenetreCalendrier->setWindowTitle("Calendrier/Agenda");
		fenetreCalendrier->setWindowIcon(QIcon(":/icones/widgets/calendrier.png"));
		fenetreCalendrier->resize(0, 0);
		connect(fenetreCalendrier, SIGNAL(hasBeenClosed()), this, SLOT(subWindowClosed()));
		fenetreCalendrier->hide();
}

void Widgets::creerFenetreCalculatrice()
{
	Calculatrice *calculatrice = new Calculatrice(this);

	fenetreCalculatrice = new MdiSubWindow;
		fenetreCalculatrice->setWidget(calculatrice);
		fenetreCalculatrice->setWindowTitle("Calculatrice");
		fenetreCalculatrice->setWindowIcon(QIcon(":/icones/widgets/calculatrice.png"));
		fenetreCalculatrice->resize(0, 0);
		connect(fenetreCalculatrice, SIGNAL(hasBeenClosed()), this, SLOT(subWindowClosed()));
		fenetreCalculatrice->hide();
}

void Widgets::creerFenetreChronometre()
{
	Chronometre *chronometre = new Chronometre(this);

	fenetreChronometre = new MdiSubWindow;
		fenetreChronometre->setWidget(chronometre);
		fenetreChronometre->setWindowTitle("Chronomètre/Minuterie");
		fenetreChronometre->setWindowIcon(QIcon(":/icones/widgets/chronometre.png"));
		fenetreChronometre->resize(0, 0);
		connect(fenetreChronometre, SIGNAL(hasBeenClosed()), this, SLOT(subWindowClosed()));
		fenetreChronometre->hide();
}

void Widgets::subWindowClosed()
{
	MdiSubWindow *closedWindow = qobject_cast<MdiSubWindow *>(sender());

	if (closedWindow == 0)
		return;

	emit subWindowWasClosed(closedWindow->windowTitle());
}

void Widgets::showWindow(QString title)
{
	if (title == "Calendrier/Agenda")
		fenetreCalendrier->show();

	else if (title == "Calculatrice")
		fenetreCalculatrice->show();

	else if (title == "Chronomètre/Minuterie")
		fenetreChronometre->show();
}

void Widgets::openTodayNotes()
{
	fenetreCalendrier->findChild<Calendrier *>()->openTodayNotes();
}
