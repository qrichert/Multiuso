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

#include "HighlighterPython.h"

HighlighterPython::HighlighterPython(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
	RegleDeSurlignage regle;

	formatMotCle.setForeground(Qt::darkBlue);
	formatMotCle.setFontWeight(QFont::Bold);

		QStringList modelesMotsCle;
			modelesMotsCle << "\\band\\b" << "\\bassert\\b" << "\\bbreak\\b" << "\\bclass\\b" << "\\bcontinue\\b" << "\\bdef\\b"
				       << "\\bdel\\b" << "\\belif\\b" << "\\belse\\b" << "\\bexcept\\b" << "\\bexec\\b" << "\\bfinally\\b"
				       << "\\bfor\\b" << "\\bfrom\\b" << "\\bglobal\\b" << "\\bif\\b" << "\\bimport\\b" << "\\bin\\b"
				       << "\\bis\\b" << "\\blambda\\b" << "\\bnot\\b" << "\\bor\\b" << "\\bpass\\b" << "\\bprint\\b"
				       << "\\braise\\b" << "\\breturn\\b" << "\\btry\\b" << "\\bwhile\\b" << "\\byield\\b";

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
		regle.modele = QRegExp("#[^\n]*");
		regle.format = formatCommentaireUneLigne;
			reglesDeSurlignage << regle;

	formatCommentaireMultiLigne.setForeground(Qt::darkCyan);

	formatDeQuotation.setFontWeight(QFont::Bold);
	formatDeQuotation.setForeground(QColor(157, 51, 148));
		regle.modele = QRegExp("['\"].*['\"]"); /* Faut-il echapper le caractere ' ? */
		regle.modele.setMinimal(true);
		regle.format = formatDeQuotation;
			reglesDeSurlignage << regle;

	debutCommentaire = QRegExp("\\-\\-\\-");
	finCommentaire = QRegExp("\\-\\-\\-");
}

void HighlighterPython::highlightBlock(const QString &texte)
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

