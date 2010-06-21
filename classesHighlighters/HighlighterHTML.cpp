/*

Copyright © 2009-2010 Quentin RICHERT
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

#include "HighlighterHTML.h"

HighlighterHTML::HighlighterHTML(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
	RegleDeSurlignage regle;

	formatValeursHTML.setFontItalic(true);
	formatValeursHTML.setForeground(QColor(255, 138, 0));
		regle.modele = QRegExp("\\&[^\n]*;");
		regle.modele.setMinimal(true);
		regle.format = formatValeursHTML;
			reglesDeSurlignage << regle;

	formatCommentaireMultiLigne.setForeground(Qt::darkCyan);

	formatBalises.setFontItalic(true);
	formatBalises.setForeground(Qt::blue);
		regle.modele = QRegExp("<[^\n]*>");
		regle.modele.setMinimal(true);
		regle.format = formatBalises;
			reglesDeSurlignage << regle;

	formatAttributs.setForeground(Qt::red);
		regle.modele = QRegExp("[a-zA-Z0-9]{1,}=\".*\"");
		regle.modele.setMinimal(true);
		regle.format = formatAttributs;
			reglesDeSurlignage << regle;

	formatDeQuotation.setForeground(QColor(157, 51, 148));
		regle.modele = QRegExp("\".*\"");
		regle.modele.setMinimal(true);
		regle.format = formatDeQuotation;
			reglesDeSurlignage << regle;

	formatDoctype.setForeground(Qt::darkGreen);
		regle.modele = QRegExp("<!DOCTYPE [^\n]*>");
		regle.modele.setMinimal(true);
		regle.format = formatDoctype;
			reglesDeSurlignage << regle;
			
	// <Shydow (www.siteduzero.com)>
		formatCData.setForeground(QColor(255, 127, 80));
			regle.modele = QRegExp("<\\!\\[CDATA\\[.+\\]\\]>");
			regle.format = formatCData;
			reglesDeSurlignage << regle;
	// </Shydow (www.siteduzero.com)>

	debutCommentaire = QRegExp("<!--");
	finCommentaire = QRegExp("-->");
}

void HighlighterHTML::highlightBlock(const QString &texte)
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

	int indexDebut = 0;

	if (previousBlockState() != 1)
	{
		indexDebut = debutCommentaire.indexIn(texte);
	}

	while (indexDebut >= 0)
	{
		int indexFin = finCommentaire.indexIn(texte, indexDebut);
		int longueurCommentaire;

		if (indexFin == -1)
		{
			setCurrentBlockState(1);
			longueurCommentaire = texte.length() - indexDebut;
		}

		else
		{
			longueurCommentaire = indexFin - indexDebut + finCommentaire.matchedLength();
		}

		setFormat(indexDebut, longueurCommentaire, formatCommentaireMultiLigne);
		indexDebut = debutCommentaire.indexIn(texte, indexDebut + longueurCommentaire);
	}
}
