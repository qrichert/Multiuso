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

#include "Astuces.h"
#include "time.h"

Astuces::Astuces(QWidget *parent) : QDialog(parent)
{
	setWindowTitle("Multiuso - Astuces");
	setWindowIcon(QIcon(":/icones/actions/actionAstuces.png"));
	setFixedSize(510, 390);

	QLabel *logo = new QLabel("<img src=':/icones/actions/actionAstuces.png'/>");

	QLabel *label = new QLabel("Le saviez-vous ?");

	page = new QLabel("Page 0/0");

	astuce = new QTextEdit;
		astuce->setReadOnly(true);

	precedent = new QPushButton("&Précédent");
		connect(precedent, SIGNAL(clicked()), this, SLOT(slotPrecedent()));

	suivant = new QPushButton("&Suivant");
		connect(suivant, SIGNAL(clicked()), this, SLOT(slotSuivant()));

	QSettings reglages(Multiuso::appDirPath() + "/reglages/config.ini", QSettings::IniFormat);

	afficherAuDemarrage = new QCheckBox("Afficher les astuces au démarrage de Multiuso");
	
		if (reglages.value("ouverture/astuces").toBool())
			afficherAuDemarrage->setCheckState(Qt::Checked);
		
	QPushButton *closeTips = new QPushButton;
		closeTips->setIcon(QIcon(":/icones/astuces/close.png"));
		closeTips->setFlat(true);
		closeTips->setFixedWidth(34);
		connect(closeTips, SIGNAL(clicked()), this, SLOT(slotCloseTips()));

	QGridLayout *entete = new QGridLayout;
		entete->addWidget(logo, 0, 0, 2, 1);
		entete->addWidget(label, 0, 1, 1, 1);
		entete->addWidget(page, 1, 1, 1, 1);

	QHBoxLayout *bottom = new QHBoxLayout;
		bottom->addWidget(afficherAuDemarrage);
		bottom->addStretch();
		bottom->addWidget(closeTips);
		bottom->setContentsMargins(0, 0, 0, 0);

	QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	QHBoxLayout *navigation = new QHBoxLayout;
		navigation->addWidget(precedent);
		navigation->addWidget(suivant);

	QGridLayout *layoutAstuces = new QGridLayout(this);
		layoutAstuces->addLayout(entete, 0, 0, 1, 1);
		layoutAstuces->addItem(spacer, 0, 1, 1, 1);
		layoutAstuces->addWidget(astuce, 1, 0, 1, 2);
		layoutAstuces->addLayout(navigation, 2, 0, 1, 2);
		layoutAstuces->addLayout(bottom, 3, 0, 1, 2);

	selectionnerAstuce();

	QShortcut *shortcutClose = new QShortcut(Qt::Key_Escape, this);
		connect(shortcutClose, SIGNAL(activated()), this, SLOT(slotCloseTips()));
}

void Astuces::selectionnerAstuce()
{
	QDir dir = Multiuso::appDirPath() + "/textes/astuces/";

	const int MIN = 1, MAX = dir.count() - 2;

	nombreDeFichiers = dir.count() - 2;

	srand(time(NULL));
	astuceActuelle = (rand() % (MAX - MIN + 1)) + MIN;

	QFile fichier(Multiuso::appDirPath() + "/textes/astuces/astuce" + QString::number(astuceActuelle) + ".mltsastuce");

	if (fichier.open(QIODevice::ReadOnly | QIODevice::Text))
		astuce->setText(fichier.readAll());

	fichier.close();

	if (astuceActuelle == MIN)
		precedent->setDisabled(true);

	if (astuceActuelle == MAX)
		suivant->setDisabled(true);

	page->setText("Page " + QString::number(astuceActuelle) + "/" + QString::number(nombreDeFichiers));
}

void Astuces::slotPrecedent()
{
	if (astuceActuelle > 1)
	{
		astuceActuelle--;

		if (astuceActuelle == 1)
			precedent->setDisabled(true);

		else
			precedent->setDisabled(false);

		if (astuceActuelle == nombreDeFichiers)
			suivant->setDisabled(true);

		else
			suivant->setDisabled(false);

		QFile fichier(Multiuso::appDirPath() + "/textes/astuces/astuce" + QString::number(astuceActuelle) + ".mltsastuce");

		if (fichier.open(QIODevice::ReadOnly | QIODevice::Text))
			astuce->setText(fichier.readAll());

		fichier.close();
	}

	page->setText("Page " + QString::number(astuceActuelle) + "/" + QString::number(nombreDeFichiers));
}

void Astuces::slotSuivant()
{
	if (astuceActuelle < nombreDeFichiers)
	{
		astuceActuelle++;

		if (astuceActuelle == nombreDeFichiers)
			suivant->setDisabled(true);

		else
			suivant->setDisabled(false);

		if (astuceActuelle == 1)
			precedent->setDisabled(true);

		else
			precedent->setDisabled(false);


		QFile fichier(Multiuso::appDirPath() + "/textes/astuces/astuce" + QString::number(astuceActuelle) + ".mltsastuce");

		if (fichier.open(QIODevice::ReadOnly | QIODevice::Text))
			astuce->setText(fichier.readAll());

		fichier.close();
	}

	page->setText("Page " + QString::number(astuceActuelle) + "/" + QString::number(nombreDeFichiers));
}

void Astuces::slotCloseTips()
{
	QSettings reglages(Multiuso::appDirPath() + "/reglages/config.ini", QSettings::IniFormat);
		reglages.setValue("ouverture/astuces", afficherAuDemarrage->isChecked());

	accept();
}

void Astuces::closeEvent(QCloseEvent *event)
{
	QSettings reglages(Multiuso::appDirPath() + "/reglages/config.ini", QSettings::IniFormat);
		reglages.setValue("ouverture/astuces", afficherAuDemarrage->isChecked());

	event->accept();
}
