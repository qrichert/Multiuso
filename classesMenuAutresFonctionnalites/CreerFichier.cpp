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

#include "CreerFichier.h"

CreerFichier::CreerFichier(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Créer un fichier");
	setWindowIcon(QIcon(":/icones/actions/actionCreerFichier"));
	resize(500, 100);

	nomDuFichier = new QLineEdit;
		connect(nomDuFichier, SIGNAL(textChanged(QString)), this, SLOT(slotChangementTexte(QString)));

	extensionDuFichier = new QLineEdit;
		connect(extensionDuFichier, SIGNAL(textChanged(QString)), this, SLOT(slotChangementTexte(QString)));

	QFormLayout *layoutLineEdit = new QFormLayout;
		layoutLineEdit->addRow("Nom du fichier :", nomDuFichier);
		layoutLineEdit->addRow("Extension du fichier :", extensionDuFichier);

	enregistrer = new QPushButton("&Enregistrer :");
		connect(enregistrer, SIGNAL(clicked()), this, SLOT(slotEnregistrer()));

	QVBoxLayout *layout = new QVBoxLayout(this);
		layout->addLayout(layoutLineEdit);
		layout->addWidget(enregistrer);
}

void CreerFichier::slotChangementTexte(QString)
{
	if (extensionDuFichier->text().isEmpty())
		enregistrer->setText("&Enregistrer : " + nomDuFichier->text());

	else
		enregistrer->setText("&Enregistrer : " + nomDuFichier->text() +  "." + extensionDuFichier->text());
}

void CreerFichier::slotEnregistrer()
{
	QString nom = nomDuFichier->text();

	if (nomDuFichier->text().isEmpty() && extensionDuFichier->text().isEmpty())
		nom = "indéfini";

	QString chemin = QFileDialog::getExistingDirectory(this, "Multiuso", Multiuso::lastPath());
	
	Multiuso::setLastPath(chemin);

	if (extensionDuFichier->text().isEmpty())
		chemin += "/" + nom;

	else
		chemin += "/" + nom +  "." + extensionDuFichier->text();

	QFile fichier(chemin);

		if (fichier.open(QIODevice::WriteOnly | QIODevice::Text))
			fichier.write(QString("").toAscii());

	fichier.close();
}
