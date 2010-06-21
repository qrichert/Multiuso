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

#include "Calculatrice.h"

Calculatrice::Calculatrice(QWidget *parent = 0) : QWidget(parent)
{
	premierNombre = 0;
	secondNombre = 0;
	resultat = 0;
	nombreAns = 0;
	operateur = "";
	currentResult = false;

	creerBoutons();

	layoutOperation = new QHBoxLayout;
		layoutOperation->addWidget(operation);

	layoutCalculatrice = new QGridLayout;
		layoutCalculatrice->addWidget(plus, 5, 3);
		layoutCalculatrice->addWidget(moins, 4, 3);
		layoutCalculatrice->addWidget(divise, 2, 3);
		layoutCalculatrice->addWidget(multiplie, 3, 3);
		layoutCalculatrice->addWidget(puissance, 1, 0);
		layoutCalculatrice->addWidget(carre, 1, 1);
		layoutCalculatrice->addWidget(cube, 1, 2);
		layoutCalculatrice->addWidget(racineCarree, 1, 3);
		layoutCalculatrice->addWidget(egal, 5, 2);
		layoutCalculatrice->addWidget(effacerCaractere, 0, 0);
		layoutCalculatrice->addWidget(viderLigne, 0, 1);
		layoutCalculatrice->addWidget(remettreZero, 0, 2);
		layoutCalculatrice->addWidget(virgule, 5, 1);
		layoutCalculatrice->addWidget(ans, 0, 3);

		layoutCalculatrice->addWidget(zero, 5, 0);
		layoutCalculatrice->addWidget(un, 4, 0);
		layoutCalculatrice->addWidget(deux, 4, 1);
		layoutCalculatrice->addWidget(trois, 4, 2);
		layoutCalculatrice->addWidget(quatre, 3, 0);
		layoutCalculatrice->addWidget(cinq, 3, 1);
		layoutCalculatrice->addWidget(six, 3, 2);
		layoutCalculatrice->addWidget(sept, 2, 0);
		layoutCalculatrice->addWidget(huit, 2, 1);
		layoutCalculatrice->addWidget(neuf, 2, 2);

	QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);
		layoutPrincipal->addLayout(layoutOperation);
		layoutPrincipal->addLayout(layoutCalculatrice);
}

void Calculatrice::creerBoutons()
{
	operation = new QLineEdit;
		operation->setStyleSheet("border: 0; height: 30px;");

	plus = new QPushButton("+");
		connect(plus, SIGNAL(clicked()), this, SLOT(ajouterPlus()));

	moins = new QPushButton("-");
		connect(moins, SIGNAL(clicked()), this, SLOT(ajouterMoins()));

	divise = new QPushButton("÷");
		connect(divise, SIGNAL(clicked()), this, SLOT(ajouterDivise()));

	multiplie = new QPushButton("×");
		connect(multiplie, SIGNAL(clicked()), this, SLOT(ajouterMultiplie()));

	puissance = new QPushButton("xⁿ");
		connect(puissance, SIGNAL(clicked()), this, SLOT(ajouterPuissance()));

	carre = new QPushButton("x²");
		connect(carre, SIGNAL(clicked()), this, SLOT(ajouterCarre()));

	cube = new QPushButton("x³");
		connect(cube, SIGNAL(clicked()), this, SLOT(ajouterCube()));

	racineCarree = new QPushButton("√");
		connect(racineCarree, SIGNAL(clicked()), this, SLOT(ajouterRacineCarree()));

	egal = new QPushButton("=");
		connect(egal, SIGNAL(clicked()), this, SLOT(ajouterEgal()));

	effacerCaractere = new QPushButton("←");
		connect(effacerCaractere, SIGNAL(clicked()), this, SLOT(ajouterEffacerCaractere()));
		effacerCaractere->setToolTip("Effacer le caractère à droite du curseur");

	viderLigne = new QPushButton("CE");
		viderLigne->setToolTip("Vider la ligne");
		connect(viderLigne, SIGNAL(clicked()), this, SLOT(ajouterViderLigne()));

	remettreZero = new QPushButton("Clr");
		remettreZero->setToolTip("Remettre à zéro");
		connect(remettreZero, SIGNAL(clicked()), this, SLOT(ajouterRemettreZero()));

	virgule = new QPushButton(",");
		connect(virgule, SIGNAL(clicked()), this, SLOT(ajouterVirgule()));

	ans = new QPushButton("Ans");
		ans->setToolTip("Afficher le dernier résultat en mémoire");
		connect(ans, SIGNAL(clicked()), this, SLOT(ajouterAns()));

	zero = new QPushButton("0");
		connect(zero, SIGNAL(clicked()), this, SLOT(ajouterZero()));

	un = new QPushButton("1");
		connect(un, SIGNAL(clicked()), this, SLOT(ajouterUn()));

	deux = new QPushButton("2");
		connect(deux, SIGNAL(clicked()), this, SLOT(ajouterDeux()));

	trois = new QPushButton("3");
		connect(trois, SIGNAL(clicked()), this, SLOT(ajouterTrois()));

	quatre = new QPushButton("4");
		connect(quatre, SIGNAL(clicked()), this, SLOT(ajouterQuatre()));

	cinq = new QPushButton("5");
		connect(cinq, SIGNAL(clicked()), this, SLOT(ajouterCinq()));

	six = new QPushButton("6");
		connect(six, SIGNAL(clicked()), this, SLOT(ajouterSix()));

	sept = new QPushButton("7");
		connect(sept, SIGNAL(clicked()), this, SLOT(ajouterSept()));

	huit = new QPushButton("8");
		connect(huit, SIGNAL(clicked()), this, SLOT(ajouterHuit()));

	neuf = new QPushButton("9");
		connect(neuf, SIGNAL(clicked()), this, SLOT(ajouterNeuf()));
}

void Calculatrice::ajouterPlus()
{
	premierNombre = operation->text().toDouble();

	operation->clear();

	operateur = "+";

	currentResult = false;
}

void Calculatrice::ajouterMoins()
{
	premierNombre = operation->text().toDouble();

	operation->clear();

	operateur = "-";
	
	currentResult = false;
}

void Calculatrice::ajouterDivise()
{
	premierNombre = operation->text().toDouble();

	operation->clear();

	operateur = "/";
	
	currentResult = false;
}

void Calculatrice::ajouterMultiplie()
{
	premierNombre = operation->text().toDouble();

	operation->clear();

	operateur = "*";
	
	currentResult = false;
}

void Calculatrice::ajouterPuissance()
{
	premierNombre = operation->text().toDouble();

	operation->clear();

	operateur = "^";
	
	currentResult = false;
}

void Calculatrice::ajouterCarre()
{
	premierNombre = operation->text().toDouble();

	operation->clear();

	operateur = "^2";

	ajouterEgal();
	
	currentResult = false;
}

void Calculatrice::ajouterCube()
{
	premierNombre = operation->text().toDouble();

	operation->clear();

	operateur = "^3";

	ajouterEgal();
	
	currentResult = false;
}

void Calculatrice::ajouterRacineCarree()
{
	premierNombre = operation->text().toDouble();

	operation->clear();

	operateur = "Sqrt";

	ajouterEgal();
	
	currentResult = false;
}

void Calculatrice::ajouterViderLigne()
{
	operation->clear();
	
	currentResult = false;
}

void Calculatrice::ajouterEffacerCaractere()
{
	operation->backspace();
	
	currentResult = false;
}

void Calculatrice::ajouterEgal()
{
	secondNombre = operation->text().toDouble();

	if (operateur == "+")
		resultat = premierNombre + secondNombre;

	else if (operateur == "-")
		resultat = premierNombre - secondNombre;

	else if (operateur == "/")
		resultat = premierNombre / secondNombre;

	else if (operateur == "*")
		resultat = premierNombre * secondNombre;

	else if (operateur == "^")
		resultat = pow(premierNombre, secondNombre);

	else if (operateur == "^2")
		resultat = pow(premierNombre, 2);

	else if (operateur == "^3")
		resultat = pow(premierNombre, 3);

	else if (operateur == "Sqrt")
		resultat = sqrt(premierNombre);

	else
		return;

	premierNombre = 0;
	secondNombre = 0;
	nombreAns = resultat;

	operation->clear();
	operation->setText(QString::number(resultat));

	currentResult = true;
}

void Calculatrice::ajouterRemettreZero()
{
	operation->clear();

	premierNombre = 0;
	secondNombre = 0;
	resultat = 0;
	nombreAns = 0;
	operateur.clear();
	
	currentResult = false;
}

void Calculatrice::ajouterVirgule()
{
	if (!operation->text().isEmpty())
	{
		if (!operation->text().contains(QRegExp("\\.")))
			operation->insert(".");
	}
	
	currentResult = false;
}

void Calculatrice::ajouterAns()
{
	operation->setText(QString::number(nombreAns));
	
	currentResult = false;
}

void Calculatrice::ajouterZero()
{
	if (currentResult)
	{
		operation->clear();
		currentResult = false;
	}

	operation->insert("0");
}


void Calculatrice::ajouterUn()
{
	if (currentResult)
	{
		operation->clear();
		currentResult = false;
	}

	operation->insert("1");
}

void Calculatrice::ajouterDeux()
{
	if (currentResult)
	{
		operation->clear();
		currentResult = false;
	}

	operation->insert("2");
}

void Calculatrice::ajouterTrois()
{
	if (currentResult)
	{
		operation->clear();
		currentResult = false;
	}

	operation->insert("3");
}

void Calculatrice::ajouterQuatre()
{
	if (currentResult)
	{
		operation->clear();
		currentResult = false;
	}

	operation->insert("4");
}

void Calculatrice::ajouterCinq()
{
	if (currentResult)
	{
		operation->clear();
		currentResult = false;
	}

	operation->insert("5");
}

void Calculatrice::ajouterSix()
{
	if (currentResult)
	{
		operation->clear();
		currentResult = false;
	}

	operation->insert("6");
}

void Calculatrice::ajouterSept()
{
	if (currentResult)
	{
		operation->clear();
		currentResult = false;
	}

	operation->insert("7");
}

void Calculatrice::ajouterHuit()
{
	if (currentResult)
	{
		operation->clear();
		currentResult = false;
	}

	operation->insert("8");
}

void Calculatrice::ajouterNeuf()
{
	if (currentResult)
	{
		operation->clear();
		currentResult = false;
	}

	operation->insert("9");
}
