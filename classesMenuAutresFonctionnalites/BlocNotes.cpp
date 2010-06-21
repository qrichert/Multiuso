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

#include "BlocNotes.h"

BlocNotes::BlocNotes(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Bloc-Notes");
	setWindowIcon(QIcon(":/icones/actions/actionBlocNotes.png"));

	resize(Multiuso::screenWidth() / 2, Multiuso::screenHeight() / 2);

	m_notes = new QPlainTextEdit;
		connect(m_notes, SIGNAL(textChanged()), this, SLOT(slotChangementDeTexte()));

	layoutPrincipal = new QVBoxLayout(this);
		layoutPrincipal->addWidget(m_notes);
		layoutPrincipal->setContentsMargins(0, 0, 0, 0);

	QFile fichierNotes(Multiuso::appDirPath() + "/textes/bloc_notes/notes.mltsnotes");

	if (fichierNotes.open(QIODevice::ReadOnly | QIODevice::Text))
		m_notes->insertPlainText(fichierNotes.readAll());

	else
		QMessageBox::warning(this, "Multiuso", "Impossible d'ouvrir le fichier contenant les notes.<br />Le texte que vous allez écrire risque de ne pas être enregistré !");

	fichierNotes.close();
}

void BlocNotes::slotChangementDeTexte()
{
	QFile notes(Multiuso::appDirPath() + "/textes/bloc_notes/notes.mltsnotes");

	if (notes.open(QIODevice::WriteOnly | QIODevice::Text))
		notes.write(m_notes->toPlainText().toAscii());

	else
		QMessageBox::warning(this, "Multiuso", "Impossible d'ouvrir le fichier contenant les notes.<br />Le dernier changement apporté au texte n'a pas été enregistré !");

	notes.close();
}
