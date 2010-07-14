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

#include "CodePageWebGenere.h"
#include "HighlighterHTML.h"

CodePageWebGenere::CodePageWebGenere(QString &codeGenere, QString &titrePage, QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Générateur de pages Web - Page générée");
	setWindowIcon(QIcon(":/icones/actions/actionGenerateurPageWeb.png"));

	resize(900, 450);

	titrePage = titrePage.toLower();
	titrePage.replace(QRegExp("[^a-zA-Z0-9-]"), "_");
	titrePage.replace(QRegExp("_{1,}"), "_");

	code = new QTextEdit;
		code->setPlainText(codeGenere);
		code->setLineWrapMode(QTextEdit::NoWrap);

	HighlighterHTML *highlighter = new HighlighterHTML(new QTextDocument);
		highlighter->setDocument(code->document());

	enregistrer = new QPushButton("&Enregistrer");
		connect(enregistrer, SIGNAL(clicked()), this, SLOT(slotEnregistrerCode()));

	nom = new QLineEdit(titrePage);
	QLabel *point = new QLabel(".");
	extension = new QLineEdit("html");

	QPushButton *ouvrir = new QPushButton("Ouvrir dans l'éditeur de code");
		connect(ouvrir, SIGNAL(clicked()), this, SLOT(slotOuvrirCode()));

	layoutOptions = new QHBoxLayout;
		layoutOptions->addWidget(enregistrer);
		layoutOptions->addWidget(nom);
		layoutOptions->addWidget(point);
		layoutOptions->addWidget(extension);

	QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);
		layoutPrincipal->addWidget(code);
		layoutPrincipal->addLayout(layoutOptions);
		layoutPrincipal->addWidget(ouvrir);
		layoutPrincipal->addWidget(Multiuso::closeButton(this));
}

void CodePageWebGenere::slotEnregistrerCode()
{
	if (code->toPlainText().isEmpty())
	{
		erreur("Le champ de saisie est vide !");

		return;
	}

	QString nomFichier;

	if (!nom->text().isEmpty())
	{
		nomFichier = nom->text();
	}

	else
	{
		erreur("Vous devez entrer un nom pour le fichier !");

		return;
	}

	QString extensionFichier;

	if (!extension->text().isEmpty())
	{
		extensionFichier = extension->text();
	}

	else
	{
		erreur("Vous devez entrer une extension pour le fichier !");

		return;
	}

	QString nomComplet = nomFichier + "." + extensionFichier;

	QString cheminEnregistrement = QFileDialog::getSaveFileName(this, "Enregistrer", Multiuso::lastPath() + "/" + nomComplet, "Document texte (*)");
	
	Multiuso::setLastPath(cheminEnregistrement);

	QFile fichier(cheminEnregistrement);

	if (fichier.open(QIODevice::WriteOnly | QIODevice::Text))
		fichier.write(code->toPlainText().toAscii());

	if (fichier.isOpen())
		fichier.close();
}

void CodePageWebGenere::erreur(QString erreur)
{
	QMessageBox::critical(this, "Multiuso", erreur);
}

void CodePageWebGenere::slotOuvrirCode()
{
	emit ouvrirFichier(code->toPlainText());
}
