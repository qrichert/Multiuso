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

#include "Chronometre.h"

Chronometre::Chronometre(QWidget *parent = 0) : QWidget(parent)
{
	QSettings reglagesChronometre(Multiuso::appDirPath() + "/reglages/chronometre.ini", QSettings::IniFormat);

	temps = new QTimer(this);
		connect(temps, SIGNAL(timeout()), this, SLOT(slotActualiser()));

	tempsEcoule = 0;
	tempsFin = 0;
	tempsRestant = 0;
	premierPassage = true;

	heures = new QSpinBox;
		heures->setMinimum(0);
		heures->setMaximum(8800);
		heures->setSuffix(" h");
		connect(heures, SIGNAL(valueChanged(int)), this, SLOT(slotAfficherMaximumCourant(int)));

	minutes = new QSpinBox;
		minutes->setMinimum(0);
		minutes->setMaximum(59);
		minutes->setSuffix(" min");
		connect(minutes, SIGNAL(valueChanged(int)), this, SLOT(slotAfficherMaximumCourant(int)));


	secondes = new QSpinBox;
		secondes->setMinimum(0);
		secondes->setMaximum(59);
		secondes->setSuffix(" s");
		connect(secondes, SIGNAL(valueChanged(int)), this, SLOT(slotAfficherMaximumCourant(int)));


	//millisecondes = new QSpinBox;
		//millisecondes->setMinimum(0);
		//millisecondes->setMaximum(999);
		//millisecondes->setSuffix(" ms");
		//millisecondes->setDisabled(reglagesChronometre.value("chronometre/millisecondes_disabled").toBool());

	hms = new QHBoxLayout;
		hms->addWidget(heures);
		hms->addWidget(minutes);
		hms->addWidget(secondes);
		//hms->addWidget(millisecondes);

	representationTemps = new QDial;
		representationTemps->setMaximum(0);
		representationTemps->setNotchesVisible(true);

	chronometre = new QRadioButton;
		chronometre->setChecked(reglagesChronometre.value("chronometre/afficher").toBool());
		connect(chronometre, SIGNAL(toggled(bool)), this, SLOT(chronometreChangementEtat(bool)));

	minuterie = new QRadioButton;
		minuterie->setChecked(reglagesChronometre.value("minuterie/afficher").toBool());
		connect(minuterie, SIGNAL(toggled(bool)), this, SLOT(minuterieChangementEtat(bool)));

	layoutType = new QFormLayout;
		layoutType->addRow("Chronomètre :", chronometre);
		layoutType->addRow("Minuterie :", minuterie);

	ouvrirBoiteDeDialogue = new QCheckBox;
		ouvrirBoiteDeDialogue->setChecked(reglagesChronometre.value("minuterie/boite_de_dialogue").toBool());
		ouvrirBoiteDeDialogue->setDisabled(reglagesChronometre.value("minuterie/boite_de_dialogue_disabled").toBool());

	layoutOptions = new QFormLayout;
		layoutOptions->addRow("Afficher un message à la fin du compte à rebours :", ouvrirBoiteDeDialogue);

	demarrer = new QPushButton("Démarrer");
		connect(demarrer, SIGNAL(clicked()), this, SLOT(slotDemarrer()));

	QPushButton *reinitialiserChrono = new QPushButton("Réinitialiser");
		connect(reinitialiserChrono, SIGNAL(clicked()), this, SLOT(slotReinitialiser()));

	layoutPrincipal = new QGridLayout(this);
		layoutPrincipal->addLayout(hms, 1, 0, 1, 2);
		layoutPrincipal->addWidget(representationTemps, 0, 1);
		layoutPrincipal->addLayout(layoutType, 0, 0);
		layoutPrincipal->addLayout(layoutOptions, 2, 0, 1, 2);
		layoutPrincipal->addWidget(demarrer, 3, 0);
		layoutPrincipal->addWidget(reinitialiserChrono, 3, 1);
}

void Chronometre::slotDemarrer()
{
	if (demarrer->text() == "Pause")
	{
		temps->stop();
		demarrer->setText("Démarrer");

		return;
	}

	demarrer->setText("Pause");

	if (!chronometre->isChecked())
	{
		heures->setDisabled(true);
		minutes->setDisabled(true);
		secondes->setDisabled(true);
	}

	if (!minuterie->isChecked())
	{
		//millisecondes->setDisabled(false);
	}

	chronometre->setDisabled(true);
	minuterie->setDisabled(true);
	ouvrirBoiteDeDialogue->setDisabled(true);

	bool b_millisecondes = false;
	bool b_boite_de_dialogue = true;

	if (!chronometre->isChecked())
	{
		b_millisecondes = true;
		b_boite_de_dialogue = false;
	}

	QSettings enregistrerReglagesChronometre(Multiuso::appDirPath() + "/reglages/chronometre.ini", QSettings::IniFormat);
		enregistrerReglagesChronometre.setValue("chronometre/afficher", chronometre->isChecked());
		enregistrerReglagesChronometre.setValue("chronometre/millisecondes_disabled", b_millisecondes);
		enregistrerReglagesChronometre.setValue("minuterie/afficher", minuterie->isChecked());
		enregistrerReglagesChronometre.setValue("minuterie/boite_de_dialogue", ouvrirBoiteDeDialogue->isChecked());
		enregistrerReglagesChronometre.setValue("minuterie/boite_de_dialogue_disabled", b_boite_de_dialogue);

	if (minuterie->isChecked())
	{
		int t_heures = heures->value();
		int t_minutes = minutes->value();
		int t_secondes = secondes->value();

		tempsFin = (t_heures * 3600) + (t_minutes * 60) + t_secondes;

		if (premierPassage)
		{
			tempsRestant = tempsFin;

			if (tempsFin == 0)
			{
				QMessageBox::critical(this, "Multiuso", "Le temps défini doit être différent de 0 secondes !");
				slotReinitialiser();

				return;
			}
		}

		if (premierPassage)
		{
			representationTemps->setMaximum(tempsFin);

			premierPassage = false;
		}

		representationTemps->setValue(tempsRestant);
	}

	int delai = 1000; // Trouver un moyen de gérer les millisecondes -> mettre ce délai à 1.

	if (minuterie->isChecked())
		delai = 1000;

	temps->start(delai);
}

void Chronometre::slotActualiser()
{
	tempsEcoule++;
	tempsRestant--;

	if (minuterie->isChecked())
	{
		representationTemps->setValue(tempsRestant);

		if (tempsRestant == 0)
		{
			temps->stop();

			if (ouvrirBoiteDeDialogue->isChecked())
				QMessageBox::information(this, "Multiuso", "Le temps que vous avez défini est écoulé !");

			slotReinitialiser();
		}
	}

	else
	{
		int tempsEcouleActuel = tempsEcoule * 1000;
		int valeur = 1000 * 3600;

		int t_heures = tempsEcouleActuel / valeur;
		int reste = tempsEcouleActuel - t_heures * valeur;
		valeur /= 60;

		int t_minutes = reste / valeur;
		reste = reste - t_minutes * valeur;
		valeur /= 60;

		int t_secondes = reste / valeur;
		//int t_millisecondes = reste - t_secondes * valeur;

		heures->setValue(t_heures);
		minutes->setValue(t_minutes);
		secondes->setValue(t_secondes);
		//millisecondes->setValue(t_millisecondes);
	}
}

void Chronometre::slotReinitialiser()
{
	temps->stop();

	tempsEcoule = 0;
	tempsFin = 0;
	tempsRestant = 0;
	premierPassage = true;

	heures->setValue(0);
	minutes->setValue(0);
	secondes->setValue(0);
	//millisecondes->setValue(0);

	representationTemps->setValue(0);
	representationTemps->setMaximum(0);

	demarrer->setText("Démarrer");

	if (!chronometre->isChecked())
	{
		heures->setDisabled(false);
		minutes->setDisabled(false);
		secondes->setDisabled(false);
	}

	if (!minuterie->isChecked())
	{
		//millisecondes->setDisabled(false);
	}

	chronometre->setDisabled(false);
	minuterie->setDisabled(false);

	if (!chronometre->isChecked())
		ouvrirBoiteDeDialogue->setDisabled(false);

	else
		ouvrirBoiteDeDialogue->setDisabled(true);
}

void Chronometre::chronometreChangementEtat(bool etat)
{
	if (etat)
	{
		ouvrirBoiteDeDialogue->setDisabled(true);
		//millisecondes->setDisabled(false);
	}
}

void Chronometre::minuterieChangementEtat(bool etat)
{
	if (etat)
	{
		ouvrirBoiteDeDialogue->setDisabled(false);
		//millisecondes->setDisabled(true);
	}
}

void Chronometre::slotAfficherMaximumCourant(int valeur)
{
	valeur = (heures->value() * 3600) + (minutes->value() * 60) + secondes->value();

	if (minuterie->isChecked())
		representationTemps->setMaximum(valeur);
}
