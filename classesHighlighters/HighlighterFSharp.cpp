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

#include "HighlighterFSharp.h"

HighlighterFSharp::HighlighterFSharp(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
	RegleDeSurlignage regle;

	formatMotCle.setForeground(Qt::darkBlue);
	formatMotCle.setFontWeight(QFont::Bold);

		QStringList modelesMotsCle;
			modelesMotsCle << "\\babstract\\b" << "\\band\\b" << "\\bas\\b" << "\\bassert\\b" << "\\basr\\b" << "\\bbegin\\b"
				<< "\\bclass\\b" << "\\bdefault\\b" << "\\bdelegate\\b" << "\\bdo\\b" << "\\bdone\\b" << "\\bdowncast\\b"
				<< "\\bdownto\\b" << "\\belse\\b" << "\\bend\\b" << "\\bexception\\b" << "\\bfalse\\b" << "\\bfinally\\b"
				<< "\\bfor\\b" << "\\bfun\\b" << "\\bfunction\\b" << "\\bif\\b" << "\\bin\\b" << "\\binherit\\b"
				<< "\\binline\\b" << "\\binterface\\b" << "\\bland\\b" << "\\blor\\b" << "\\blsl\\b" << "\\blsr\\b"
				<< "\\blxor\\b" << "\\bmatch\\b" << "\\bmember\\b" << "\\bmod\\b" << "\\bmodule\\b" << "\\bmutable\\b"
				<< "\\bnamespace\\b" << "\\bnew\\b" << "\\bnull\\b" << "\\bof\\b" << "\\bopen\\b" << "\\bor\\b"
				<< "\\boverride\\b" << "\\brec\\b" << "\\bsig\\b" << "\\bstatic\\b" << "\\bstruct\\b" << "\\bthen\\b"
				<< "\\bto\\b" << "\\btrue\\b" << "\\btry\\b" << "\\btype\\b" << "\\bval\\b" << "\\bwhen\\b"
				<< "\\bupcast\\b" << "\\bwhile\\b" << "\\bwith\\b";

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

	formatCommentaireMultiLigne.setForeground(Qt::darkCyan);

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

	/* nomdevariable.quelquechose */
	formatPointFonction.setForeground(Qt::blue);
		regle.modele = QRegExp(".+\\.");
		regle.format = formatPointFonction;
			reglesDeSurlignage << regle;

	debutCommentaire = QRegExp("\\(\\*");
	finCommentaire = QRegExp("\\*\\)");
}

void HighlighterFSharp::highlightBlock(const QString &texte)
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
