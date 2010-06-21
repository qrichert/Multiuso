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

#include "CodeClasseCppGenere.h"
#include "classesHighlighters/HighlighterCPP.h"

CodeClasseCppGenere::CodeClasseCppGenere(QString &header, QString &constructeur, QString &nom, QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Générateur de classes C++ - Classe générée");
	setWindowIcon(QIcon(":/icones/actions/actionGenerateurClassesCpp.png"));

	resize(650, 500);

	nomClasse = nom;

	texteHeader = new QTextEdit;
		texteHeader->setPlainText(header);
		texteHeader->setLineWrapMode(QTextEdit::NoWrap);
	
	HighlighterCPP *highlighterHeader = new HighlighterCPP(new QTextDocument);
		highlighterHeader->setDocument(texteHeader->document());

	enregistrerHeader = new QPushButton("Enregistrer : " + nom + ".h");
		connect(enregistrerHeader, SIGNAL(clicked()), this, SLOT(slotEnregistrerHeader()));

	ouvrirHeader = new QPushButton("Ouvrir dans l'éditeur de code");
		connect(ouvrirHeader, SIGNAL(clicked()), this, SLOT(slotOuvrirHeader()));

	layoutHeader = new QVBoxLayout;
		layoutHeader->addWidget(texteHeader);
		layoutHeader->addWidget(enregistrerHeader);
		layoutHeader->addWidget(ouvrirHeader);
	
	QWidget *widgetHeader = new QWidget;
		widgetHeader->setLayout(layoutHeader);

	texteConstructeur = new QTextEdit;
		texteConstructeur->setPlainText(constructeur);
		texteConstructeur->setLineWrapMode(QTextEdit::NoWrap);
		
	HighlighterCPP *highlighterConstructeur = new HighlighterCPP(new QTextDocument);
		highlighterConstructeur->setDocument(texteConstructeur->document());

	enregistrerConstructeur = new QPushButton("Enregistrer : " + nom + ".cpp");
		connect(enregistrerConstructeur, SIGNAL(clicked()), this, SLOT(slotEnregistrerConstructeur()));

	ouvrirConstructeur = new QPushButton("Ouvrir dans l'éditeur de code");
		connect(ouvrirConstructeur, SIGNAL(clicked()), this, SLOT(slotOuvrirConstructeur()));

	layoutConstructeur = new QVBoxLayout;
		layoutConstructeur->addWidget(texteConstructeur);
		layoutConstructeur->addWidget(enregistrerConstructeur);
		layoutConstructeur->addWidget(ouvrirConstructeur);

	QWidget *widgetConstructeur = new QWidget;
		widgetConstructeur->setLayout(layoutConstructeur);

	QTabWidget *tabCode = new QTabWidget;
		tabCode->addTab(widgetHeader, nom + ".h");
		tabCode->addTab(widgetConstructeur, nom + ".cpp");

	QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);
		layoutPrincipal->addWidget(tabCode);
}

void CodeClasseCppGenere::slotEnregistrerHeader()
{
	QString cheminFichier = QFileDialog::getSaveFileName(this, "Multiuso", Multiuso::lastPath() + "/" + nomClasse + ".h", "*.h");
	
	Multiuso::setLastPath(cheminFichier);

	QFile fichier(cheminFichier);

	if (fichier.open(QIODevice::WriteOnly | QIODevice::Text))
		fichier.write(texteHeader->toPlainText().toAscii());

	fichier.close();
}

void CodeClasseCppGenere::slotOuvrirHeader()
{
	emit ouvrirFichier(texteHeader->toPlainText());
}

void CodeClasseCppGenere::slotEnregistrerConstructeur()
{
	QString cheminFichier = QFileDialog::getSaveFileName(this, "Multiuso", Multiuso::lastPath() + "/" + nomClasse + ".cpp", "*.cpp");
	
	Multiuso::setLastPath(cheminFichier);

	QFile fichier(cheminFichier);

	if (fichier.open(QIODevice::WriteOnly | QIODevice::Text))
		fichier.write(texteConstructeur->toPlainText().toAscii());

	fichier.close();
}

void CodeClasseCppGenere::slotOuvrirConstructeur()
{
	emit ouvrirFichier(texteConstructeur->toPlainText());
}
