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

#include "HighlighterBF.h"

HighlighterBF::HighlighterBF(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
	RegleDeSurlignage regle;
	
	formatCommentaires.setForeground(Qt::gray);
	formatCommentaires.setFontItalic(true);
		regle.modele = QRegExp(".");
		regle.format = formatCommentaires;
			reglesDeSurlignage << regle;

	formatPlusMoinsSuivantPrecedent.setForeground(Qt::darkGreen);
		regle.modele = QRegExp("[\\+\\-<>]");
		regle.format = formatPlusMoinsSuivantPrecedent;
			reglesDeSurlignage << regle;
	
	formatBoucles.setForeground(Qt::darkBlue);
	formatBoucles.setFontWeight(QFont::Bold);
		regle.modele = QRegExp("[\\[\\]]");
		regle.format = formatBoucles;
			reglesDeSurlignage << regle;
			
	formatPointVirgule.setForeground(Qt::darkRed);
	formatPointVirgule.setFontWeight(QFont::Bold);
		regle.modele = QRegExp("[\\.,]"); // Faut-il echapper , ?
		regle.format = formatPointVirgule;
			reglesDeSurlignage << regle;
}

void HighlighterBF::highlightBlock(const QString &texte)
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
