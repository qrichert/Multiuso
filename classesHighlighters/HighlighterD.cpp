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

#include "HighlighterD.h"

HighlighterD::HighlighterD(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
	RegleDeSurlignage regle;

	formatMotCle.setForeground(Qt::darkBlue);
	formatMotCle.setFontWeight(QFont::Bold);

		QStringList modelesMotsCle;
			modelesMotsCle << "\\babstract\\b" << "\\balias\\b" << "\\balign\\b" << "\\basm\\b" << "\\bassert\\b" << "\\bauto\\b"
				<< "\\bbody\\b" << "\\bbool\\b" << "\\bbreak\\b" << "\\bbyte\\b" << "\\bcase\\b" << "\\bcatch\\b"
				<< "\\bcast\\b" << "\\bcdouble\\b" << "\\bcent\\b" << "\\bcfloat\\b" << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
				<< "\\bcontinue\\b" << "\\bcreal\\b" << "\\bdchar\\b" << "\\bdebug\\b" << "\\bdefault\\b" << "\\bdelegate\\b" << "\\bdelete\\b"
				<< "\\bdeprecated\\b" << "\\bdo\\b" << "\\bdouble\\b" << "\\belse\\b" << "\\benum\\b" << "\\bexport\\b"
				<< "\\bextern\\b" << "\\bfalse\\b" << "\\bfinal\\b" << "\\bfinally\\b" << "\\bfloat\\b" << "\\bfor\\b"
				<< "\\bforeach\\b" << "\\bforeach_reverse\\b" << "\\bfunction\\b" << "\\bgoto\\b" << "\\bidouble\\b" << "\\bif\\b"
				<< "\\bifloat\\b" << "\\bimmutable\\b" << "\\bimport\\b" << "\\bin\\b" << "\\binout\\b" << "\\bint\\b"
				<< "\\binterface\\b" << "\\binvariant\\b" << "\\bireal\\b" << "\\bis\\b" << "\\blazy\\b" << "\\blong\\b"
				<< "\\bmacro\\b" << "\\bmixin\\b" << "\\bmodule\\b" << "\\bnew\\b" << "\\bnothrow\\b" << "\\bnull\\b"
				<< "\\bout\\b" << "\\boverride\\b" << "\\bpackage\\b" << "\\bpragma\\b" << "\\bprivate\\b" << "\\bprotected\\b"
				<< "\\bpublic\\b" << "\\bpure\\b" << "\\breal\\b" << "\\bref\\b" << "\\breturn\\b" << "\\bscope\\b"
				<< "\\bshared\\b" << "\\bshort\\b" << "\\bstatic\\b" << "\\bstruct\\b" << "\\bsuper\\b" << "\\bswitch\\b"
				<< "\\bsynchronized\\b" << "\\btemplate\\b" << "\\bthis\\b" << "\\bthrow\\b" << "\\btrue\\b" << "\\btry\\b"
				<< "\\btypeid\\b" << "\\btypedef\\b" << "\\btypeof\\b" << "\\bubyte\\b" << "\\bucent\\b" << "\\buint\\b"
				<< "\\bulong\\b" << "\\bunion\\b" << "\\bunittest\\b" << "\\bushort\\b" << "\\bversion\\b" << "\\bvoid\\b"
				<< "\\bvolatile\\b" << "\\bwchar\\b" << "\\bwhile\\b" << "\\bwith\\b";

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

	formatDeQuotationWysiwygOuHex.setFontWeight(QFont::Bold);
	formatDeQuotationWysiwygOuHex.setForeground(QColor(157, 51, 148));
		regle.modele = QRegExp("[rq]\".*\"");
		regle.modele.setMinimal(true);
		regle.format = formatDeQuotationWysiwygOuHex;
			reglesDeSurlignage << regle;


	formatFonctions.setFontItalic(true);
	formatFonctions.setForeground(Qt::black);
		regle.modele = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
		regle.format = formatFonctions;
			reglesDeSurlignage << regle;

	debutCommentaire = QRegExp("(/\\*|/\\+)");
	finCommentaire = QRegExp("(\\*/|\\+/)");
}

void HighlighterD::highlightBlock(const QString &texte)
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

