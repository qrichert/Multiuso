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

#include "TextEdit.h"

TextEdit::TextEdit()
{
	m_sauvegardeOk = false;
	m_undoOk = false;
	m_redoOk = false;

	m_estGras = false;
	m_estItalique = false;
	m_estSouligne = false;
	m_estAligneGauche = true;
	m_estAligneCentre = false;
	m_estAligneDroite = false;
	m_estAligneJustifie = false;
	m_tailleDuTexte = 11;

	if (Multiuso::currentOS() == "windows")
		m_policeDuTexte = QFont("Comics Sans MS");
	else
		m_policeDuTexte = QFont("DejaVu Sans");

	m_fichierOuvert = "";

	m_enregistre = true;

	setTabStopWidth(40);
}

void TextEdit::dropEvent(QDropEvent *event)
{
	const QMimeData *data = event->mimeData();

	if (data->hasUrls())
	{
		QList<QUrl> urls = data->urls();

		for (int i = 0; i < urls.size(); i ++)
		{
			QString url = urls.at(i).path();

			if (Multiuso::currentOS() == "windows")
				url = url.right(url.length() - 1);

			emit ouvrirFichier(url);
		}
	}

	event->acceptProposedAction();
}


bool TextEdit::sauvegardeOk()
{
	return m_sauvegardeOk;
}

bool TextEdit::undoOk()
{
	return m_undoOk;
}

bool TextEdit::redoOk()
{
	return m_redoOk;
}

bool TextEdit::estGras()
{
	return m_estGras;
}

bool TextEdit::estItalique()
{
	return m_estItalique;
}

bool TextEdit::estSouligne()
{
	return m_estSouligne;
}

bool TextEdit::estAligneGauche()
{
	return m_estAligneGauche;
}

bool TextEdit::estAligneCentre()
{
	return m_estAligneCentre;
}

bool TextEdit::estAligneDroite()
{
	return m_estAligneDroite;
}

bool TextEdit::estAligneJustifie()
{
	return m_estAligneJustifie;
}

int TextEdit::tailleDuTexte()
{
	return m_tailleDuTexte;
}

QFont TextEdit::policeDuTexte()
{
	return m_policeDuTexte;
}

QString TextEdit::fichierOuvert()
{
	return m_fichierOuvert;
}

bool TextEdit::estEnregistre()
{
	return m_enregistre;
}

void TextEdit::setSauvegardeOk(bool ok)
{
	m_sauvegardeOk = ok;
}

void TextEdit::setUndoOk(bool ok)
{
	m_undoOk = ok;
}

void TextEdit::setRedoOk(bool ok)
{
	m_redoOk = ok;
}


void TextEdit::setEstGras(bool ok)
{
	m_estGras = ok;
}

void TextEdit::setEstItalique(bool ok)
{
	m_estItalique = ok;
}

void TextEdit::setEstSouligne(bool ok)
{
	m_estSouligne = ok;
}

void TextEdit::setEstAligneGauche(bool ok)
{
	m_estAligneGauche = ok;
}

void TextEdit::setEstAligneCentre(bool ok)
{
	m_estAligneCentre = ok;
}

void TextEdit::setEstAligneDroite(bool ok)
{
	m_estAligneDroite = ok;
}

void TextEdit::setEstAligneJustifie(bool ok)
{
	m_estAligneJustifie = ok;
}

void TextEdit::setTailleDuTexte(int taille)
{
	m_tailleDuTexte = taille;
}

void TextEdit::setPoliceDuTexte(QFont police)
{
	m_policeDuTexte = police;
}

void TextEdit::setFichierOuvert(QString fichier)
{
	m_fichierOuvert = fichier;
}

void TextEdit::setEstEnregistre(bool ok)
{
	m_enregistre = ok;
}

