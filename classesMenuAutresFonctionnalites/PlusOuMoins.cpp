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

#include "PlusOuMoins.h"
#include "time.h"

PlusOuMoins::PlusOuMoins(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Plus ou Moins");
	setWindowIcon(QIcon(":/icones/actions/actionPlusOuMoins.png"));

	nombreATrouver = 0;
	nombreDeCoups = 0;

	minimum = new QSpinBox;
		minimum->setMinimum(0);
		minimum->setMaximum(5);
		minimum->setValue(0);

	maximum = new QSpinBox;
		maximum->setMinimum(10);
		maximum->setMaximum(1000000);
		maximum->setValue(100);

	nombreEntre = new QSpinBox;
		nombreEntre->setMinimum(0);
		nombreEntre->setMaximum(1000000);
		nombreEntre->setDisabled(true);

	QFormLayout *layoutMinMax = new QFormLayout;
		layoutMinMax->addRow("Minimum :", minimum);
		layoutMinMax->addRow("Maximum :", maximum);

	jouer = new QPushButton("&Jouer");
		connect(jouer, SIGNAL(clicked()), this, SLOT(jouerCoup()));
		jouer->setDisabled(true);

	demarrer = new QPushButton("&Démarrer une partie");
		connect(demarrer, SIGNAL(clicked()), this, SLOT(demarrerPartie()));

	quitter = new QPushButton("&Quitter la partie");
		connect(quitter, SIGNAL(clicked()), this, SLOT(quitterPartie()));
		quitter->setDisabled(true);

	statut = new QLabel;

	QGridLayout *layoutJeu = new QGridLayout;
		layoutJeu->addLayout(layoutMinMax, 0, 0, 1, 1);
		layoutJeu->addWidget(statut, 1, 0, 1, 2);
		layoutJeu->addWidget(nombreEntre, 2, 0, 1, 2);
		layoutJeu->addWidget(jouer, 3, 0, 1, 2);
		layoutJeu->addWidget(demarrer, 4, 0, 1, 1);
		layoutJeu->addWidget(quitter, 4, 1, 1, 1);

	QWidget *pageJeu = new QWidget;
		pageJeu->setLayout(layoutJeu);

	scores = new QTextEdit;
		scores->setReadOnly(true);

	QFile fichier(Multiuso::appDirPath() + "/textes/plus_ou_moins/scores.mltsscores");

	if (fichier.open(QIODevice::ReadOnly | QIODevice::Text))
		scores->setText(fichier.readAll());

	fichier.close();


	QPushButton *effacer = new QPushButton("&Effacer les scores");
		connect(effacer, SIGNAL(clicked()), this, SLOT(effacerScores()));

	QVBoxLayout *layoutScores = new QVBoxLayout;
		layoutScores->addWidget(scores);
		layoutScores->addWidget(effacer);

	QWidget *pageScores = new QWidget;
		pageScores->setLayout(layoutScores);

	QTabWidget *tabs = new QTabWidget;
		tabs->addTab(pageJeu, "Partie");
		tabs->addTab(pageScores, "Scores");

	QVBoxLayout *layout = new QVBoxLayout(this);
		layout->addWidget(tabs);
}

void PlusOuMoins::jouerCoup()
{
	nombreDeCoups++;

	int nombre = nombreEntre->value();

	if (nombre < nombreATrouver)
	{
		statut->setText("C'est plus que " + QString::number(nombre) + " !");
	}

	else if (nombre > nombreATrouver)
	{
		statut->setText("C'est moins que " + QString::number(nombre) + " !");
	}

	else
	{
		QString coups = "coups";

		if (nombreDeCoups == 1)
			coups = "coup";

		scores->textCursor().insertText(QString::number(nombreDeCoups) + " " + coups +
				" pour un nombre entre " + QString::number(minimum->value()) + " et " + QString::number(maximum->value()) + "\n");

		sauverScores();

		QMessageBox::information(this, "Multiuso", "Vous avez trouvé le nombre en " + QString::number(nombreDeCoups) + " " + coups + " !");

		statut->setText("");

		quitterPartie();
	}
}

void PlusOuMoins::demarrerPartie()
{
	nombreATrouver = 0;
	nombreDeCoups = 0;

	minimum->setDisabled(true);
	maximum->setDisabled(true);
	nombreEntre->setDisabled(false);

	const int MIN = minimum->value(), MAX = maximum->value();

	srand(time(NULL));
	nombreATrouver = (rand() % (MAX - MIN + 1)) + MIN;

	jouer->setDisabled(false);
	demarrer->setDisabled(true);
	quitter->setDisabled(false);
}

void PlusOuMoins::quitterPartie()
{
	nombreATrouver = 0;
	nombreDeCoups = 0;

	minimum->setDisabled(false);
	maximum->setDisabled(false);
	nombreEntre->setDisabled(true);

	jouer->setDisabled(true);
	demarrer->setDisabled(false);
	quitter->setDisabled(true);
}

void PlusOuMoins::effacerScores()
{
	scores->clear();

	sauverScores();
}

void PlusOuMoins::sauverScores()
{
	QFile fichier(Multiuso::appDirPath() + "/textes/plus_ou_moins/scores.mltsscores");

	if (fichier.open(QIODevice::WriteOnly | QIODevice::Text))
		fichier.write(scores->toPlainText().toAscii());

	fichier.close();
}
