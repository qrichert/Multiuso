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

#include "RechercherRemplacer.h"

RechercherRemplacer::RechercherRemplacer(QWidget *parent = 0, QTextEdit *texte = 0, const int onglet = 0, const QString selection = 0) : QDialog(parent)
{
	texteFantome = texte;
	texteComplet = texte->toPlainText();

	if (texte->toPlainText().isEmpty())
		QMessageBox::warning(this, "Multiuso", "Le champ de texte est vide !");

	setWindowIcon(QIcon(":/icones/editeur_de_texte/editeur_de_texte.png"));
	setFixedSize(500, 225);

	if (onglet == 0)
		setWindowTitle("Multiuso - Rechercher");

	else
		setWindowTitle("Multiuso - Rechercher/Remplacer");

	QLineEdit *motARechercher = new QLineEdit;
		motARechercher->setText(selection);

	QLineEdit *motARemplacer = new QLineEdit;

	layoutMots = new QFormLayout;
		layoutMots->addRow("Mot à rechercher :", motARechercher);

		if (onglet == 1)
			layoutMots->addRow("Remplacer le mot par :", motARemplacer);

	rechercheArriere = new QCheckBox;
	rechercheSensibleCasse = new QCheckBox;
	rechercheSeulementMotsComplets = new QCheckBox;

	layoutOptions = new QFormLayout;
		layoutOptions->addRow("Recherche en arrière :", rechercheArriere);
		layoutOptions->addRow("Recherche sensible à la casse :", rechercheSensibleCasse);
		layoutOptions->addRow("Rechercher seulement les mots complets :", rechercheSeulementMotsComplets);

	annuler = new QPushButton("Annuler");
		connect(annuler, SIGNAL(clicked()), this, SLOT(accept()));

	remplacer = new QPushButton("Remplacer");
		connect(remplacer, SIGNAL(clicked()), this, SLOT(slotRemplacer()));

	rechercher = new QPushButton("Rechercher");
		connect(rechercher, SIGNAL(clicked()), this, SLOT(slotRechercher()));

	layoutBoutons = new QHBoxLayout;
		layoutBoutons->addWidget(annuler);

		if (onglet == 1)
			layoutBoutons->addWidget(remplacer);

		layoutBoutons->addWidget(rechercher);
		layoutBoutons->setAlignment(Qt::AlignRight);

	layoutPrincipal = new QVBoxLayout(this);
		layoutPrincipal->addLayout(layoutMots);
		layoutPrincipal->addLayout(layoutOptions);
		layoutPrincipal->addLayout(layoutBoutons);
}

void RechercherRemplacer::slotRechercher()
{
	//slotRechercher2(texteFantome);
}

void RechercherRemplacer::slotRechercher2(QTextEdit *texte)
{
	QString texteTemporaire = texte->toPlainText();
		texteTemporaire = "";

	/*if (!motARechercher->text().isEmpty())
	{
		QTextDocument::FindFlags options;

		if (rechercheArriere->isChecked())
		{
			options << QTextDocument::FindBackward;
		}

		if (rechercheSensibleCasse->isChecked())
		{
			options << QTextDocument::FindCaseSensitively;
		}

		if (rechercheSeulementMotsComplets->isChecked())
		{
			options << QTextDocument::FindWholeWords;
		}

		texte->find(motARechercher->text(), options);
	}

	else
	{
		QMessageBox::critical(this, "Multiuso", "Il faut entrer un mot à rechercher !");
	}*/
}


void RechercherRemplacer::slotRemplacer()
{
	/*slotRemplacer2(texteComplet);
	texteFantome->setPlainText(texteComplet);*/
}

QString RechercherRemplacer::slotRemplacer2(QString &texteComplet)
{
	/*if (!motARechercher->text().isEmpty() && !motARemplacer->text().isEmpty())
	{
		QString rechercher = motARechercher->text();
		QString remplacer = motARechercher->text();

		while (texteComplet.contains(rechercher))
		{
			texteComplet.replace(rechercher, remplacer);
		}
	}

	else
	{
		QMessageBox::critical(this, "Multiuso", "Il faut entrer un mot à rechercher et à remplacer !");
	}*/

	return texteComplet;
}
