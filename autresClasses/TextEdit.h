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

#ifndef HEADER_TEXTEDIT
#define HEADER_TEXTEDIT

#include "../CurrentIncludes.h"
#include <QDropEvent>

class TextEdit : public QTextEdit
{
	Q_OBJECT

	public:
		TextEdit();

	protected:
		void dropEvent(QDropEvent *event);

	public slots:
		bool sauvegardeOk();
		bool undoOk();
		bool redoOk();

		bool estGras();
		bool estItalique();
		bool estSouligne();
		bool estAligneGauche();
		bool estAligneCentre();
		bool estAligneDroite();
		bool estAligneJustifie();
		int tailleDuTexte();
		QFont policeDuTexte();

		QString fichierOuvert();
		bool estEnregistre();

	public slots:
		void setSauvegardeOk(bool ok);
		void setUndoOk(bool ok);
		void setRedoOk(bool ok);

		void setEstGras(bool ok);
		void setEstItalique(bool ok);
		void setEstSouligne(bool ok);
		void setEstAligneGauche(bool ok);
		void setEstAligneCentre(bool ok);
		void setEstAligneDroite(bool ok);
		void setEstAligneJustifie(bool ok);
		void setTailleDuTexte(int taille);
		void setPoliceDuTexte(QFont police);

		void setFichierOuvert(QString fichier);
		void setEstEnregistre(bool ok);

	signals:
		void ouvrirFichier(QString fichier);

	private:
		bool m_sauvegardeOk;
		bool m_undoOk;
		bool m_redoOk;

		bool m_estGras;
		bool m_estItalique;
		bool m_estSouligne;
		bool m_estAligneGauche;
		bool m_estAligneCentre;
		bool m_estAligneDroite;
		bool m_estAligneJustifie;
		int m_tailleDuTexte;
		QFont m_policeDuTexte;

		QString m_fichierOuvert;
		bool m_enregistre;
};

#endif
