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

#include "GenerateurPageWeb.h"
#include "autresClasses/CodePageWebGenere.h"

GenerateurPageWeb::GenerateurPageWeb(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Générateur de pages Web");
	setWindowIcon(QIcon(":/icones/actions/actionGenerateurPageWeb.png"));

	titre = new QLineEdit;

	formulaireTitre = new QFormLayout;
		formulaireTitre->addRow("Titre :", titre);

	groupeTitre = new QGroupBox("Titre");
		groupeTitre->setLayout(formulaireTitre);

	transitional = new QRadioButton;
		transitional->setChecked(true);

	strict = new QRadioButton;

	formulaireDoctype = new QFormLayout;
		formulaireDoctype->addRow("Transitional :", transitional);
		formulaireDoctype->addRow("Strict :", strict);

	groupeDoctype = new QGroupBox("Doctype");
		groupeDoctype->setLayout(formulaireDoctype);

	utf8 = new QRadioButton;
		utf8->setChecked(true);

	iso88591 = new QRadioButton;

	formulaireEncodage = new QFormLayout;
		formulaireEncodage->addRow("UTF-8 :", utf8);
		formulaireEncodage->addRow("ISO-8859-1 :", iso88591);

	groupeEncodage = new QGroupBox("Encodage");
		groupeEncodage->setLayout(formulaireEncodage);

	nomDuDesign = new QLineEdit;
	emplacementDuDesign = new QLineEdit;

	formulaireDesign = new QFormLayout;
		formulaireDesign->addRow("Nom du design :", nomDuDesign);
		formulaireDesign->addRow("Emplacement du design :", emplacementDuDesign);

	groupeDesign = new QGroupBox("Design");
		groupeDesign->setCheckable(true);
		groupeDesign->setChecked(false);
		groupeDesign->setLayout(formulaireDesign);

	emplacementFavicon = new QLineEdit;

	formulaireFavicon = new QFormLayout;
		formulaireFavicon->addRow("Emplacement de la favicon :", emplacementFavicon);

	groupeFavicon = new QGroupBox("Favicon");
		groupeFavicon->setCheckable(true);
		groupeFavicon->setChecked(false);
		groupeFavicon->setLayout(formulaireFavicon);

	commentaires = new QTextEdit;

	layoutCommentaires = new QVBoxLayout;
		layoutCommentaires->addWidget(commentaires);

	groupeCommentaires = new QGroupBox("Commentaires");
		groupeCommentaires->setCheckable(true);
		groupeCommentaires->setChecked(false);
		groupeCommentaires->setLayout(layoutCommentaires);

	QSplitter *separateur1 = new QSplitter;
		separateur1->setOrientation(Qt::Horizontal);
		separateur1->addWidget(groupeTitre);
		separateur1->addWidget(groupeDoctype);
		separateur1->addWidget(groupeEncodage);

	QSplitter *separateur2 = new QSplitter;
		separateur2->setOrientation(Qt::Horizontal);
		separateur2->addWidget(groupeDesign);
		separateur2->addWidget(groupeFavicon);

	QSplitter *separateur3 = new QSplitter;
		separateur3->setOrientation(Qt::Horizontal);
		separateur3->addWidget(groupeCommentaires);

	QSplitter *separateur4 = new QSplitter;
		separateur4->setOrientation(Qt::Vertical);
		separateur4->addWidget(separateur1);
		separateur4->addWidget(separateur2);
		separateur4->addWidget(separateur3);

	generer = new QPushButton("&Générer");
		connect(generer, SIGNAL(clicked()), this, SLOT(slotGenerer()));

	QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);
		layoutPrincipal->addWidget(separateur4);
		layoutPrincipal->addWidget(generer);
}

void GenerateurPageWeb::slotGenerer()
{
	QString contenuPageWeb;

	QString g_titre = titre->text();

	if (titre->text().isEmpty())
		g_titre = "indéfini";

	QString g_doctype = "Transitional";
	QString g_min_doctype = "transitional";

	if (strict->isChecked())
	{
		g_doctype = "Strict";
		g_min_doctype = "strict";
	}

	QString g_encodage = "UTF-8";

	if (iso88591->isChecked())
		g_encodage = "ISO-8859-1";

	contenuPageWeb = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 " + g_doctype + "//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-" + g_min_doctype +  ".dtd\">\n";
	contenuPageWeb += "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"fr\">\n";
	contenuPageWeb += "\t<head>\n";
	contenuPageWeb += "\t\t<title>" + g_titre + "</title>\n";
	contenuPageWeb += "\t\t<meta http-equiv=\"Content-Type\" content=\"text/html; charset=" + g_encodage + "\"/>\n";

	if (groupeDesign->isChecked())
	{
		QString g_titreDesign = nomDuDesign->text();

		if (nomDuDesign->text().isEmpty())
			g_titreDesign = "indéfini";

		QString g_emplacementDesign = emplacementDuDesign->text();

		if (emplacementDuDesign->text().isEmpty())
			g_emplacementDesign = "indéfini";

		contenuPageWeb += "\t\t<link rel=\"stylesheet\" media=\"screen\" type=\"text/css\" title=\"" + g_titreDesign + "\" href=\"" + g_emplacementDesign + "\"/>\n";
	}

	if (groupeFavicon->isChecked())
	{
		QString g_emplacementFavicon = emplacementFavicon->text();

		if (emplacementFavicon->text().isEmpty())
			g_emplacementFavicon = "indéfini";

		contenuPageWeb += "\t\t<link rel=\"shortcut icon\" href=\"" + g_emplacementFavicon + "\"/>\n";
	}

	contenuPageWeb += "\t</head>\n";
	contenuPageWeb += "\t<body>\n";
	contenuPageWeb += "\t</body>\n";
	contenuPageWeb += "</html>";

	if (groupeCommentaires->isChecked())
		contenuPageWeb = "<!-- " + commentaires->toPlainText() + " -->\n\n" + contenuPageWeb;

	CodePageWebGenere *fenCodePageWebGenere = new CodePageWebGenere(contenuPageWeb, g_titre, this);
		connect(fenCodePageWebGenere, SIGNAL(ouvrirFichier(QString)), this, SLOT(slotOuvrirFichier(QString)));
		fenCodePageWebGenere->exec();

	generer->setText("Régénérer");
}

void GenerateurPageWeb::slotOuvrirFichier(QString texte)
{
	emit ouvrirFichierIntermediaire(texte);
}
