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

#include "GenerateurClassesCpp.h"
#include "autresClasses/CodeClasseCppGenere.h"

GenerateurClassesCpp::GenerateurClassesCpp(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Générateur de classes C++");
	setWindowIcon(QIcon(":/icones/actions/actionGenerateurClassesCpp.png"));
	resize(500, 450);

	QSettings reglagesCpp(Multiuso::appDirPath() + "/reglages/generer.ini", QSettings::IniFormat);

	nomDeLaClasse = new QLineEdit;
	classeMere = new QLineEdit;

	layoutDefinition = new QFormLayout;
		layoutDefinition->addRow("Nom de la classe :", nomDeLaClasse);
		layoutDefinition->addRow("Classe mère :", classeMere);

	groupeDefinition = new QGroupBox("Définition de la classe");
		groupeDefinition->setLayout(layoutDefinition);

	inclusionsMultiples = new QCheckBox;
		inclusionsMultiples->setChecked(true);

	genererDestructeur = new QCheckBox;

	layoutOptions = new QFormLayout;
		layoutOptions->addRow("Éviter les inclusions multiples :", inclusionsMultiples);
		layoutOptions->addRow("Générer un destructeur :", genererDestructeur);

	groupeOptions = new QGroupBox("Options");
		groupeOptions->setLayout(layoutOptions);

	auteur = new QLineEdit(reglagesCpp.value("cpp/auteur").toString());
	nomDuProgramme = new QLineEdit(reglagesCpp.value("cpp/nom_du_programme").toString());

	gnuGpl = new QCheckBox;
		gnuGpl->setChecked(reglagesCpp.value("cpp/gnu_gpl").toBool());

	commentaires = new QTextEdit;
		commentaires->setLineWrapMode(QTextEdit::NoWrap);

	layoutCommentaires = new QFormLayout;
		layoutCommentaires->addRow("Auteur :", auteur);
		layoutCommentaires->addRow("Nom du programme :", nomDuProgramme);
		layoutCommentaires->addRow("Licence GNU/GPL :", gnuGpl);
		layoutCommentaires->addRow("Commentaires :", commentaires);

	groupeCommentaires = new QGroupBox("Commentaires");
		groupeCommentaires->setLayout(layoutCommentaires);
		groupeCommentaires->setCheckable(true);
		groupeCommentaires->setChecked(reglagesCpp.value("cpp/commentaires").toBool());

	generer = new QPushButton("&Générer");
		connect(generer, SIGNAL(clicked()), this, SLOT(slotGenerer()));

	QGridLayout *layoutPrincipal = new QGridLayout(this);
		layoutPrincipal->addWidget(groupeDefinition, 0, 0);
		layoutPrincipal->addWidget(groupeOptions, 1, 0);
		layoutPrincipal->addWidget(groupeCommentaires, 2, 0);
		layoutPrincipal->addWidget(generer, 3, 0, Qt::AlignRight);
}

void GenerateurClassesCpp::slotGenerer()
{
	QSettings reglages(Multiuso::appDirPath() + "/reglages/generer.ini", QSettings::IniFormat);
		reglages.setValue("cpp/auteur", auteur->text());
		reglages.setValue("cpp/commentaires", groupeCommentaires->isChecked());
		reglages.setValue("cpp/nom_du_programme", nomDuProgramme->text());
		reglages.setValue("cpp/gnu_gpl", gnuGpl->isChecked());

	QString nom = nomDeLaClasse->text();

	if (nomDeLaClasse->text().isEmpty())
		nom = "Indéfini";

	QString s_classeMere = classeMere->text();

	if (classeMere->text().isEmpty())
		s_classeMere = "Indéfini";

	QString auteurDuProgramme = auteur->text();

	if (auteur->text().isEmpty())
		auteurDuProgramme = "Indéfini";

	QString nomDuProg = nomDuProgramme->text();

	if (nomDuProgramme->text().isEmpty())
		nomDuProg = "Indéfini";

	QString comms = commentaires->toPlainText();

	if (commentaires->toPlainText().isEmpty())
		comms = "Généré automatiquement via Multiuso.";

		QString licenceGnuGpl = "Copyright © " + QString::number(QDate::currentDate().year()) + " " + auteurDuProgramme + "\n\n"
			+ nomDuProg + " is free software: you can redistribute it and/or modify\n"
			"it under the terms of the GNU General Public License as published by\n"
			"the Free Software Foundation, either version 3 of the License, or\n"
			"(at your option) any later version.\n"
			"\n"
			+ nomDuProg + " is distributed in the hope that it will be useful,\n"
			"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
			"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
			"GNU General Public License for more details.\n"
			"\n"
			"You should have received a copy of the GNU General Public License\n"
			"along with " + nomDuProg + ".  If not, see <http://www.gnu.org/licenses/>.\n\n";

	QString header = "";

	if (groupeCommentaires->isChecked())
	{
		header += "/*\n\n";
		header += "Par : " + auteurDuProgramme + "\n\n";

		if (gnuGpl->isChecked())
			header += licenceGnuGpl;

		header += "Commentaires de l'auteur :\n\n";
		header += comms + "\n\n";
		header += "*/\n\n";
	}

	if (inclusionsMultiples->isChecked())
	{
		header += "#ifndef HEADER_" + nom.toUpper().replace(" ", "_") +"\n";
		header += "#define HEADER_" + nom.toUpper().replace(" ", "_") +"\n\n";
	}

	header += "#include <QtGui>\n\n";
	header += "class " + nom + " : public " + s_classeMere + "\n";
	header += "{\n";
	header += "\tQ_OBJECT\n\n";
	header += "\tpublic:\n";
	header += "\t\t" + nom + "();\n";

	if (genererDestructeur->isChecked())
		header += "\t\t~" + nom + "();\n";

	header += "\n\tpublic slots:\n\n";
	header += "\tsignals:\n\n";
	header += "\tprivate:\n\n";
	header += "};\n";

	if (inclusionsMultiples->isChecked())
		header += "\n#endif\n";

	QString constructeur = "";

	if (groupeCommentaires->isChecked() && gnuGpl->isChecked())
		constructeur += "/*\n\n" + licenceGnuGpl + "*/\n\n";

	constructeur += "#include \"" + nom + ".h\"\n\n";
	constructeur += nom + "::" + nom + "()\n";
	constructeur += "{\n";
	constructeur += "}\n";

	if (genererDestructeur->isChecked())
	{
		constructeur += "\n~" + nom + "::" + nom + "()\n";
		constructeur += "{\n";
		constructeur += "}\n";
	}

	generer->setText("Régénérer");

	CodeClasseCppGenere *fenCodeClasseCppGenere = new CodeClasseCppGenere(header, constructeur, nom, this);
		connect(fenCodeClasseCppGenere, SIGNAL(ouvrirFichier(QString)), this, SLOT(slotOuvrirFichier(QString)));
		fenCodeClasseCppGenere->exec();
}

void GenerateurClassesCpp::slotOuvrirFichier(QString texte)
{
	emit ouvrirFichierIntermediaire(texte);
}
