/*

Copyright © 2010 Shydow (www.siteduzero.com)

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

#include "HighlighterHaskell.h"

HighlighterHaskell::HighlighterHaskell(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
	RegleDeSurlignage regle;

	formatMotCle.setForeground(Qt::darkBlue);
	formatMotCle.setFontWeight(QFont::Bold);

		QStringList modelesMotsCle;
			modelesMotsCle << "\\bas\\b" << "\\bcase\\b" << "\\bof\\b" << "\\bclass\\b" << "\\bdata\\b" << "\\bdefault\\b"
				<< "\\bderiving\\b" << "\\bdo\\b" << "\\bforall\\b" << "\\bforeign\\b" << "\\bhiding\\b" << "\\bif\\b"
				<< "\\bthen\\b" << "\\belse\\b" << "\\bimport\\b" << "\\binfix\\b" << "\\binfixl\\b" << "\\binfixr\\b"
				<< "\\binstance\\b" << "\\blet\\b" << "\\bin\\b" << "\\bmdo\\b" << "\\bmodule\\b" << "\\bnewtype\\b"
				<< "\\bqualified\\b" << "\\btype\\b" << "\\bwhere\\b";

			foreach (const QString &modele, modelesMotsCle)
			{
				regle.modele = QRegExp(modele);
				regle.format = formatMotCle;

				reglesDeSurlignage << regle;
			}

	formatChiffres.setForeground(Qt::magenta);
		regle.modele = QRegExp("[0-9]");
		regle.format = formatChiffres;
			reglesDeSurlignage << regle;

	formatCommentaireUneLigne.setForeground(Qt::darkCyan);
		regle.modele = QRegExp("\\-\\-[^\n]*");
		regle.format = formatCommentaireUneLigne;
			reglesDeSurlignage << regle;

	formatDeQuotation.setFontWeight(QFont::Bold);
	formatDeQuotation.setForeground(QColor(157, 51, 148));
		regle.modele = QRegExp("\".*\"");
		regle.modele.setMinimal(true);
		regle.format = formatDeQuotation;
			reglesDeSurlignage << regle;

	formatParentheses.setFontWeight(QFont::Bold);
	formatParentheses.setForeground(Qt::green);
		regle.modele = QRegExp("[\\(\\)]");
		regle.format = formatParentheses;
			reglesDeSurlignage << regle;
}

void HighlighterHaskell::highlightBlock(const QString &texte)
{
	foreach (const RegleDeSurlignage &regle, reglesDeSurlignage)
	{
		QRegExp expression(regle.modele);

		int index = expression.indexIn(texte);

		while (index >= 0)
		{
			int longueur = expression.matchedLength();
			setFormat(index, longueur, regle.format);
			index = expression.indexIn(texte, index + longueur);
		}
	}

	setCurrentBlockState(0);
}
