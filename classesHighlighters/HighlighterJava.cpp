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

#include "HighlighterJava.h"

HighlighterJava::HighlighterJava(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
	RegleDeSurlignage regle;

	formatMotCle.setForeground(Qt::darkBlue);
	formatMotCle.setFontWeight(QFont::Bold);

		QStringList modelesMotsCle;
			modelesMotsCle << "\\babstract\\b" << "\\bboolean\\b" << "\\bbreak\\b" << "\\bbyte\\b" << "\\bcase\\b" << "\\bcatch\\b"
				<< "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b" << "\\bcontinue\\b" << "\\bdefault\\b" << "\\bdo\\b"
				<< "\\bdouble\\b" << "\\belse\\b" << "\\bextends\\b" << "\\bfinal\\b" << "\\bfinally\\b" << "\\bfloat\\b"
				<< "\\bfor\\b" << "\\bgoto\\b" << "\\bif\\b" << "\\bimplements\\b" << "\\bimport\\b" << "\\binstanceof\\b"
				<< "\\bint\\b" << "\\binterface\\b" << "\\blong\\b" << "\\bnative\\b" << "\\bnew\\b" << "\\bpackage\\b"
				<< "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b" << "\\breturn\\b" << "\\bshort\\b" << "\\bstatic\\b"
				<< "\\bstrictfp\\b" << "\\bsuper\\b" << "\\bswitch\\b" << "\\bsynchronized\\b" << "\\bthis\\b" << "\\bthrow\\b"
				<< "\\bthrows\\b" << "\\btransient\\b" << "\\btry\\b" << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwhile\\b";

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
		regle.modele = QRegExp("//[^\n]*");
		regle.format = formatCommentaireUneLigne;
			reglesDeSurlignage << regle;

	formatCommentaireMultiLigne.setForeground(Qt::darkCyan);

	formatDeQuotation.setFontWeight(QFont::Bold);
	formatDeQuotation.setForeground(QColor(157, 51, 148));
		regle.modele = QRegExp("\".*\"");
		regle.modele.setMinimal(true);
		regle.format = formatDeQuotation;
			reglesDeSurlignage << regle;

	formatFonctions.setFontItalic(true);
	formatFonctions.setForeground(Qt::black);
		regle.modele = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
		regle.format = formatFonctions;
			reglesDeSurlignage << regle;

	debutCommentaire = QRegExp("/\\*");
	finCommentaire = QRegExp("\\*/");
}

void HighlighterJava::highlightBlock(const QString &texte)
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
