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

#include "Calendrier.h"
#include "autresClasses/Agenda.h"

Calendrier::Calendrier(QWidget *parent = 0) : QCalendarWidget(parent)
{
	setGridVisible(true);

	connect(this, SIGNAL(activated(QDate)), this, SLOT(slotChosenDate(QDate)));

	updateDates();
}

void Calendrier::updateDates()
{
	currentDateUsed = false;

	QSettings settings(Multiuso::appDirPath() + "/ini/agenda.ini", QSettings::IniFormat);

	foreach (QString group, settings.childGroups())
	{
		QDate date = settings.value(group + "/date").toDate();

		QBrush brush(Qt::darkCyan);

		QTextCharFormat textCharFormat = dateTextFormat(date);
			textCharFormat.setBackground(brush);

		setDateTextFormat(date, textCharFormat);

		if (date == QDate::currentDate())
			currentDateUsed = true;
	}	
}

void Calendrier::openTodayNotes()
{
	if (currentDateUsed)
		slotChosenDate(QDate::currentDate());
}

void Calendrier::slotChosenDate(QDate date)
{
	Agenda *agenda = new Agenda(this, date);

	QBrush brush(Qt::white);
		
		if (agenda->exec() == QDialog::Accepted)
			brush.setColor(Qt::darkCyan);
	
	agenda->deleteLater();	

	QTextCharFormat textCharFormat = dateTextFormat(date);
		textCharFormat.setBackground(brush);

	setDateTextFormat(date, textCharFormat);
}
