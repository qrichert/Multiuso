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

#include "Agenda.h"

Agenda::Agenda(QWidget *parent, QDate date) : QDialog(parent)
{
	QString dateChoisie = date.toString();

	groupName = dateChoisie;
		groupName.replace(" ", "_");

		QString jour = QString::number(date.dayOfWeek());

			if (jour == "1") { jour = "Lundi"; }
			else if (jour == "2") { jour = "Mardi"; }
			else if (jour == "3") { jour = "Mercredi"; }
			else if (jour == "4") { jour = "Jeudi"; }
			else if (jour == "5") { jour = "Vendredi"; }
			else if (jour == "6") { jour = "Samedi"; }
			else if (jour == "7") { jour = "Dimanche"; }

		QString mois = QString::number(date.month());

			if (mois == "1") { mois = "janvier"; }
			else if (mois == "2") { mois = "février"; }
			else if (mois == "3") { mois = "mars"; }
			else if (mois == "4") { mois = "avril"; }
			else if (mois == "5") { mois = "mai"; }
			else if (mois == "6") { mois = "juin"; }
			else if (mois == "7") { mois = "juillet"; }
			else if (mois == "8") { mois = "août"; }
			else if (mois == "9") { mois = "septembre"; }
			else if (mois == "10") { mois = "octobre"; }
			else if (mois == "11") { mois = "novembre"; }
			else if (mois == "12") { mois = "décembre"; }

		QString jourDuMois = QString::number(date.day());
		QString annee = QString::number(date.year());

	QString dateComplete = jour + ", " + jourDuMois + " " + mois + " " + annee;

	setWindowTitle("Multiuso - Agenda : " + dateComplete);
	setWindowIcon(QIcon(":/icones/widgets/agenda.png"));

	resize(Multiuso::screenWidth() / 2, Multiuso::screenHeight() / 2);

	QSettings settings(Multiuso::appDirPath() + "/ini/agenda.ini", QSettings::IniFormat);
		settings.setValue(groupName + "/date", date);	

	QString texte = settings.value(groupName + "/content").toString();

	champDeSaisie = new QTextEdit;
		champDeSaisie->setPlainText(texte);
		connect(champDeSaisie, SIGNAL(textChanged()), this, SLOT(slotChangementDeTexte()));

	QPushButton *deleteButton = new QPushButton("Supprimer");
		connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteNote()));

	QHBoxLayout *layoutButtons = new QHBoxLayout;
		layoutButtons->addWidget(deleteButton);
		layoutButtons->addWidget(Multiuso::closeButton(this));
		layoutButtons->setAlignment(Qt::AlignRight);

	QVBoxLayout *layoutAgenda = new QVBoxLayout(this);
		layoutAgenda->addWidget(champDeSaisie);
		layoutAgenda->addLayout(layoutButtons);
}

void Agenda::slotChangementDeTexte()
{
	QSettings settings(Multiuso::appDirPath() + "/ini/agenda.ini", QSettings::IniFormat);
		settings.setValue(groupName + "/content", champDeSaisie->toPlainText());	
}

void Agenda::deleteNote()
{
	champDeSaisie->clear();

	accept();
}
