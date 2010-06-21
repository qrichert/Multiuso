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

#include "HighlighterLexYacc.h"

HighlighterLexYacc::HighlighterLexYacc(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
	RegleDeSurlignage regle;

	formatMotCle.setForeground(Qt::darkBlue);
	formatMotCle.setFontWeight(QFont::Bold);
		regle.modele = QRegExp("%.+\\S");
		regle.modele.setMinimal(true);
		regle.format = formatMotCle;
			reglesDeSurlignage << regle;
	
	formatChiffres.setForeground(Qt::magenta);
		regle.modele = QRegExp("[0-9]");
		regle.format = formatChiffres;
			reglesDeSurlignage << regle;
			
	formatSeparation.setForeground(Qt::darkMagenta);
	formatSeparation.setFontWeight(QFont::Bold);
		regle.modele = QRegExp("%%");
		regle.format = formatSeparation;
			reglesDeSurlignage << regle;
	
	formatCode.setForeground(Qt::darkMagenta);
		regle.modele = QRegExp("%\\{[.\\s]*\\}%");
		regle.modele.setMinimal(true);
		regle.format = formatCode;
			reglesDeSurlignage << regle;
	
	formatYY.setForeground(Qt::darkGreen);
		regle.modele = QRegExp("yy\\S+");
		regle.modele.setMinimal(true);
		regle.format = formatYY;
			reglesDeSurlignage << regle;
}

void HighlighterLexYacc::highlightBlock(const QString &texte)
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
