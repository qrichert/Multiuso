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

#include "CreerReadme.h"

CreerReadme::CreerReadme(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Créer un README");
	setWindowIcon(QIcon(":/icones/actions/actionCreerReadme.png"));
	resize(700, 370);

	QSettings infos(Multiuso::appDirPath() + "/ini/creer_readme.ini", QSettings::IniFormat);

		nomDuProgramme = new QLineEdit(infos.value("programme/nom_du_programme").toString());
		version = new QLineEdit(infos.value("programme/version").toString());
		siteWebProgramme = new QLineEdit(infos.value("programme/site_web").toString());
		nomDeLAuteur = new QLineEdit(infos.value("auteur/auteur").toString());
		siteWebAuteur = new QLineEdit(infos.value("auteur/site_web").toString());

		commentaires = new QTextEdit;
			commentaires->setLineWrapMode(QTextEdit::NoWrap);

	QPushButton *boutonGenerer = new QPushButton("&Générer");
		connect(boutonGenerer, SIGNAL(clicked()), this, SLOT(genererReadme()));

	QFormLayout *layoutInfos = new QFormLayout(this);
		layoutInfos->addRow("Nom :", nomDuProgramme);
		layoutInfos->addRow("Version :", version);
		layoutInfos->addRow("Site Web :", siteWebProgramme);
		layoutInfos->addRow("Auteur :", nomDeLAuteur);
		layoutInfos->addRow("Site web de l'auteur :", siteWebAuteur);
		layoutInfos->addRow("Commentaires :", commentaires);
		layoutInfos->addWidget(boutonGenerer);
		layoutInfos->addWidget(Multiuso::closeButton(this));
}

void CreerReadme::genererReadme()
{
	QSettings infos(Multiuso::appDirPath() + "/ini/creer_readme.ini", QSettings::IniFormat);
		infos.setValue("programme/nom_du_programme", nomDuProgramme->text());
		infos.setValue("programme/version", version->text());
		infos.setValue("programme/site_web", siteWebProgramme->text());
		infos.setValue("auteur/auteur", nomDeLAuteur->text());
		infos.setValue("auteur/site_web", siteWebAuteur->text());

	QString readme;

	QString s_nomDuProgramme = nomDuProgramme->text();

		if (s_nomDuProgramme.isEmpty())
			s_nomDuProgramme = "Indéfini";

	readme += s_nomDuProgramme + " ";

	QString s_version = version->text();

		if (s_version.isEmpty())
			s_version = "Indéfini";

	readme += s_version;

	QString s_siteWebProgramme = siteWebProgramme->text();

		if (!s_siteWebProgramme.isEmpty())
			readme += " (" + s_siteWebProgramme + ")";

	readme += "\n\n--------------------------------------------------------------------------------------------------------\n\n";

	QString s_nomDeLAuteur = nomDeLAuteur->text();

		if (s_nomDeLAuteur.isEmpty())
			s_nomDeLAuteur = "Indéfini";

	readme += "Ce programme a été écrit par " + s_nomDeLAuteur;


	QString s_siteWebAuteur = siteWebAuteur->text();

		if (!s_siteWebAuteur.isEmpty())
			readme += " (" + s_siteWebAuteur + ")";

	QString s_commentaires = commentaires->toPlainText();

		if (!s_commentaires.isEmpty())
		{
			readme += "\n\n--------------------------------------------------------------------------------------------------------\n\n";
			readme += s_commentaires;
		}

	QDialog *readmeGenere = new QDialog(this);
		readmeGenere->setWindowTitle("Multiuso - Créer un README - README généré");
		readmeGenere->setWindowIcon(QIcon(":/icones/actions/actionCreerReadme.png"));
		readmeGenere->resize(700, 370);

	// Construction du contenu du QDialog

	contenuReadme = new QTextEdit;
		contenuReadme->setLineWrapMode(QTextEdit::NoWrap);
		contenuReadme->setPlainText(readme);

	QPushButton *enregistrer = new QPushButton("&Enregistrer");
		connect(enregistrer, SIGNAL(clicked()), this, SLOT(enregistrerReadme()));

	QVBoxLayout *layoutReadme = new QVBoxLayout(readmeGenere);
		layoutReadme->addWidget(contenuReadme);
		layoutReadme->addWidget(enregistrer);

	readmeGenere->exec();
}

void CreerReadme::enregistrerReadme()
{
	QString fichier = QFileDialog::getSaveFileName(this, "Multiuso", Multiuso::lastPath() + "/README", "Fichier texte (*)");

	Multiuso::setLastPath(fichier);

	if (!fichier.isEmpty())
	{
		QFile readme(fichier);

		if (readme.open(QIODevice::WriteOnly | QIODevice::Text))
			readme.write(contenuReadme->toPlainText().toAscii());

		readme.close();
	}
}
