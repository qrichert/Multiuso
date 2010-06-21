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

#include "HighlighterCSharp.h"

HighlighterCSharp::HighlighterCSharp(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
	RegleDeSurlignage regle;

	formatMotCle.setForeground(Qt::darkBlue);
	formatMotCle.setFontWeight(QFont::Bold);

		QStringList modelesMotsCle;
			modelesMotsCle << "\\babstract\\b" << "\\bas\\b" << "\\bbase\\b" << "\\bbool\\b" << "\\bbreak\\b" << "\\bbyte\\b"
				<< "\\bcase\\b" << "\\bcatch\\b" << "\\bchar\\b" << "\\bchecked\\b" << "\\bclass\\b" << "\\bconst\\b"
				<< "\\bcontinue\\b" << "\\bdecimal\\b" << "\\bdefault\\b" << "\\bdelegate\\b" << "\\bdo\\b" << "\\bdouble\\b"
				<< "\\belse\\b" << "\\benum\\b" << "\\bexplicit\\b" << "\\bextern\\b" << "\\bfalse\\b" << "\\bfinally\\b"
				<< "\\bfixed\\b" << "\\bfloat\\b" << "\\bfor\\b" << "\\bforeach\\b" << "\\bif\\b" << "\\bimplicit\\b"
				<< "\\bis\\b" << "\\block\\b" << "\\blong\\b" << "\\bnamespace\\b" << "\\bevent\\b" << "\\bobject\\b"
				<< "\\boperator\\b" << "\\boverride\\b" << "\\bparams\\b" << "\\bprivate\\b" << "\\bgoto\\b" << "\\breadonly\\b"
				<< "\\bin\\b" << "\\bint\\b" << "\\binterface\\b" << "\\binternal\\b" << "\\bsizeof\\b" << "\\bstackalloc\\b"
				<< "\\bstatic\\b" << "\\bnew\\b" << "\\bnull\\b" << "\\bthis\\b" << "\\bout\\b" << "\\btypeof\\b"
				<< "\\bprotected\\b" << "\\bpublic\\b" << "\\bref\\b" << "\\breturn\\b" << "\\bsbyte\\b" << "\\bwhile\\b"
				<< "\\bstring\\b" << "\\bstruct\\b" << "\\bswitch\\b" << "\\bthrow\\b" << "\\btrue\\b" << "\\btry\\b"
				<< "\\buint\\b" << "\\bulong\\b" << "\\bunckecked\\b" << "\\bunsafe\\b" << "\\bushort\\b" << "\\busing\\b"
				<< "\\bvirtual\\b" << "\\bsealed\\b" << "\\bvolatile\\b" << "\\bshort\\b" << "\\bvoid\\b";

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

void HighlighterCSharp::highlightBlock(const QString &texte)
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
