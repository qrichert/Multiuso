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

#include "CompresseurDeCode.h"

CompresseurDeCode::CompresseurDeCode(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Compresseur de code");
	setWindowIcon(QIcon(":/icones/actions/actionCompresseurDeCode.png"));

	resize((Multiuso::screenWidth() / 2.5) * 1.5, (Multiuso::screenHeight() / 2.5) * 1.5);

	texteEntier = new QTextEdit("Collez/saisissez ici le code d'origine ou ouvrez un fichier le contenant.");
		texteEntier->setLineWrapMode(QTextEdit::NoWrap);

	texteCompresse = new QTextEdit("Ici se trouvera le code une fois compressé.");
		texteCompresse->setLineWrapMode(QTextEdit::NoWrap);
		texteCompresse->resize(texteCompresse->width(), 100);

	javascript = new QCheckBox("Ajouter « javascript: » au début de la ligne.");

	QPushButton *compresser = new QPushButton("Compresser");
		connect(compresser, SIGNAL(clicked()), this, SLOT(slotCompresser()));

	QPushButton *ouvrir = new QPushButton("Ouvrir...");
		connect(ouvrir, SIGNAL(clicked()), this, SLOT(slotOuvrir()));

	copier = new QPushButton("Copier le code compressé");
		copier->setDisabled(true);
		connect(copier, SIGNAL(clicked()), this, SLOT(slotCopier()));

	enregistrer = new QPushButton("Enregistrer le code compressé");
		enregistrer->setDisabled(true);
		connect(enregistrer, SIGNAL(clicked()), this, SLOT(slotEnregistrer()));

	QHBoxLayout *layoutBoutons = new QHBoxLayout;
		layoutBoutons->addWidget(ouvrir);
		layoutBoutons->addWidget(enregistrer);
		layoutBoutons->addWidget(copier);

	QVBoxLayout *layoutCompresseurDeCode = new QVBoxLayout(this);
		layoutCompresseurDeCode->addWidget(texteEntier);
		layoutCompresseurDeCode->addLayout(layoutBoutons);
		layoutCompresseurDeCode->addWidget(texteCompresse);
		layoutCompresseurDeCode->addWidget(javascript);
		layoutCompresseurDeCode->addWidget(compresser);
}

void CompresseurDeCode::slotOuvrir()
{
	QString cheminDuFichier = QFileDialog::getOpenFileName(this, "Ouvrir...", Multiuso::lastPath() , "Document texte (*)");
	
	Multiuso::setLastPath(cheminDuFichier);

	QFile fichier(cheminDuFichier);

	if (fichier.open(QIODevice::ReadOnly | QIODevice::Text))
		texteEntier->setPlainText(fichier.readAll());

	else if (cheminDuFichier.isEmpty())
		return;

	else
		QMessageBox::critical(this, "Multiuso", "Impossible d'ouvrir le fichier !");

	fichier.close();
}

void CompresseurDeCode::slotCopier()
{
	texteCompresse->selectAll();
	texteCompresse->copy();
}

void CompresseurDeCode::slotEnregistrer()
{
	QString cheminDuFichier = QFileDialog::getSaveFileName(this, "Enregistrer le code compressé", Multiuso::lastPath() , "Document texte (*)");
	
	Multiuso::setLastPath(cheminDuFichier);

	QFile fichier(cheminDuFichier);

	if (fichier.open(QIODevice::WriteOnly | QIODevice::Text))
		fichier.write(texteCompresse->toPlainText().toAscii());

	else if (cheminDuFichier.isEmpty())
		return;

	else
		QMessageBox::critical(this, "Multiuso", "Impossible d'enregistrer le fichier !");

	fichier.close();
}

void CompresseurDeCode::slotCompresser()
{
	QString texte = texteEntier->toPlainText();
		texte.replace("\n", " ");
		texte.replace("\t", "");

	if (javascript->isChecked())
		texte = "javascript:" + texte;

	texteCompresse->setPlainText(texte);
	enregistrer->setDisabled(false);
	copier->setDisabled(false);
}
