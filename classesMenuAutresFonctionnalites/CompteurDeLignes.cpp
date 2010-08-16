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

#include "CompteurDeLignes.h"

CompteurDeLignes::CompteurDeLignes(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Compteur de lignes");
	setWindowIcon(QIcon(":/icones/actions/actionCompteurDeLignes.png"));

	resize(425, 390);

	QStringList entetesVue;
		entetesVue << "Nom" << "Taille" << "Lignes";

	modele = new QStandardItemModel(0, 3);
		modele->setHorizontalHeaderLabels(entetesVue);

	vue = new QTableView;
		vue->setModel(modele);
		vue->horizontalHeader()->setStretchLastSection(true);
		vue->setSelectionBehavior(QAbstractItemView::SelectRows);

	QLabel *indication = new QLabel("Fichier(s) à traiter :");

	QPushButton *parcourir = new QPushButton("Parcourir...");
		connect(parcourir, SIGNAL(clicked()), this, SLOT(selectionFichiers()));

	totalDeLignes = 0;

	resultat = new QLabel;
		resultat->setText("<strong>0</strong> lignes pour <strong>0</strong> fichiers.");

	doNotCount = new QSpinBox;
		doNotCount->setRange(0, 10000);
		doNotCount->setValue(0);

	QHBoxLayout *doNotCountLayout = new QHBoxLayout;
		doNotCountLayout->addWidget(new QLabel("Retirer "));
		doNotCountLayout->addWidget(doNotCount);
		doNotCountLayout->addWidget(new QLabel("ligne(s) par fichier."));
		doNotCountLayout->setContentsMargins(0, 0, 0, 0);

	QPushButton *clearButton = new QPushButton("Tout effacer");
		connect(clearButton, SIGNAL(clicked()), this, SLOT(clearAll()));

	QGridLayout *layoutPrincipal = new QGridLayout(this);
		layoutPrincipal->addWidget(vue, 0, 0, 1, 2);
		layoutPrincipal->addWidget(indication, 1, 0, 1, 1);
		layoutPrincipal->addWidget(parcourir, 1, 1, 1, 1);
		layoutPrincipal->addWidget(resultat, 2, 0, 1, 1);
		layoutPrincipal->addLayout(doNotCountLayout, 2, 1, 1, 1);
		layoutPrincipal->addWidget(clearButton, 3, 0, 1, 1);
		layoutPrincipal->addWidget(Multiuso::closeButton(this), 3, 1, 1, 1);
}

void CompteurDeLignes::selectionFichiers()
{
	setCursor(Qt::WaitCursor);

	QStringList fichiers = QFileDialog::getOpenFileNames(this, "Multiuso", Multiuso::lastPath() , "Tous les fichiers (*)");
	
	Multiuso::setLastPath(fichiers.value(0));

	if (!fichiers.isEmpty())
	{
		for (int i = 0; i <= fichiers.size(); i++)
		{
			QFile fichier(fichiers.value(i));

			if (fichier.exists())
			{
				if (fichier.open(QIODevice::ReadOnly | QIODevice::Text))
				{
					int nombreDeLignes = 0;

					QTextStream fluxFichier(&fichier);
						fluxFichier.setCodec("UTF-8");

					while (!fluxFichier.atEnd())
					{
						QCoreApplication::processEvents();

						QString nouvelleLigne = fluxFichier.readLine();
						nombreDeLignes++;
					}

					nombreDeLignes -= doNotCount->value();

					if (nombreDeLignes < 0)
						nombreDeLignes = 0;

					QFileInfo infosFichier(fichier);

					QStandardItem *n_nom = new QStandardItem(infosFichier.fileName());
						n_nom->setEditable(false);

					QStandardItem *n_taille = new QStandardItem(Multiuso::toSize(infosFichier.size()));
						n_taille->setEditable(false);

					QStandardItem *n_lignes = new QStandardItem(QString::number(nombreDeLignes));
						n_lignes->setEditable(false);

					QList<QStandardItem *> nouveauxItems;
						nouveauxItems << n_nom << n_taille << n_lignes;

					modele->appendRow(nouveauxItems);

					vue->resizeColumnsToContents();
					vue->horizontalHeader()->setStretchLastSection(true);

					totalDeLignes += nombreDeLignes;

					QString txtLignes = "lignes";

					if (totalDeLignes == 1)
						txtLignes = "ligne";

					QString txtFichiers = "fichiers";

					if (modele->rowCount() == 1)
						txtFichiers = "fichier";

					resultat->setText("<strong>" + QString::number(totalDeLignes) + "</strong> " + txtLignes + " "
						"pour <strong>" + QString::number(modele->rowCount()) + "</strong> " + txtFichiers + ".");
				}

				fichier.close();
			}
		}
	}

	setCursor(Qt::ArrowCursor);
}

void CompteurDeLignes::clearAll()
{
	totalDeLignes = 0;
	modele->removeRows(0, modele->rowCount());
	resultat->setText("<strong>0</strong> lignes pour <strong>0</strong> fichiers.");
}
