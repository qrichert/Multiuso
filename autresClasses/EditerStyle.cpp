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

#include "EditerStyle.h"

EditerStyle::EditerStyle(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Éditer le style");
	setWindowIcon(QIcon(""));
	resize(Multiuso::screenWidth() / 2, Multiuso::screenHeight() / 2);

	style = new QTextEdit;
		style->setLineWrapMode(QTextEdit::NoWrap);

	enregistrer = new QPushButton("&Enregistrer");
		connect(enregistrer, SIGNAL(clicked()), this, SLOT(slotEnregistrer()));

	QVBoxLayout *layoutEditerStyle = new QVBoxLayout(this);
		layoutEditerStyle->addWidget(style);
		layoutEditerStyle->addWidget(enregistrer);
		layoutEditerStyle->setContentsMargins(0, 0, 0, 0);

	QFile fichier(Multiuso::appDirPath() + "/textes/style/style.mltsstyle");

	texteVerif = "";

	if (fichier.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		texteVerif = fichier.readAll();
		style->setPlainText(texteVerif);
	}

	else
	{
		QMessageBox::critical(this, "Multiuso", "Impossible d'ouvrir le fichier contenant le style !");
		this->accept();
	}

	fichier.close();
}

void EditerStyle::slotEnregistrer()
{
	QFile fichier(Multiuso::appDirPath() + "/textes/style/style.mltsstyle");

	if (fichier.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		texteVerif = style->toPlainText();
		fichier.write(texteVerif.toAscii());
	}

	else
	{
		QMessageBox::critical(this, "Multiuso", "Impossible d'enregistrer le style !");
		this->accept();
	}

	fichier.close();

	accept();
}

void EditerStyle::closeEvent(QCloseEvent *event)
{
	if (texteVerif != style->toPlainText())
	{
		int reponse = QMessageBox::question(this, "Multiuso", "Le style n'a pas été enregistré, toutes les modifications "
				"non enregistrées vont être perdues.<br />Continuer ?", QMessageBox::Yes | QMessageBox::No);

		if (reponse == QMessageBox::Yes)
			event->accept();

		else
			event->ignore();
	}

	else
	{
		event->accept();
	}
}
