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

#include "HighlighterCaml.h"

HighlighterCaml::HighlighterCaml(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
	RegleDeSurlignage regle;

	formatMotCle.setForeground(Qt::darkBlue);
	formatMotCle.setFontWeight(QFont::Bold);

		QStringList modelesMotsCle;
			modelesMotsCle << "\\band\\b" << "\\bas\\b" << "\\basr\\b" << "\\bbegin\\b" << "\\bclass\\b" << "\\bclosed\\b"
				<< "\\bconstraint\\b" << "\\bdo\\b" << "\\bdone\\b" << "\\bdownto\\b" << "\\belse\\b" << "\\bend\\b"
				<< "\\bexception\\b" << "\\bexternal\\b" << "\\bfailwith\\b" << "\\bfalse\\b" << "\\bflush\\b" << "\\bfor\\b" << "\\bfun\\b"
				<< "\\bfunction\\b" << "\\bfunctor\\b" << "\\bif\\b" << "\\bin\\b" << "\\binclude\\b" << "\\binherit\\b" << "\\bincr\\b"
				<< "\\bland\\b" << "\\blet\\b" << "\\bload\\b" << "\\blos\\b" << "\\blsl\\b" << "\\blsr\\b"
				<< "\\blxor\\b" << "\\bmatch\\b" << "\\bmethod\\b" << "\\bmod\\b" << "\\bmodule\\b" << "\\bmutable\\b"
				<< "\\bnew\\b" << "\\bnot\\b" << "\\bof\\b" << "\\bopen\\b" << "\\boption\\b" << "\\bor\\b"
				<< "\\bparser\\b" << "\\bprivate\\b" << "\\bref\\b" << "\\brec\\b" << "\\braise\\b" << "\\bregexp\\b"
				<< "\\bsig\\b" << "\\bstruct\\b" << "\\bstdout\\b" << "\\bstdin\\b" << "\\bstderr\\b" << "\\bthen\\b"
				<< "\\bto\\b" << "\\btrue\\b" << "\\btry\\b" << "\\btype\\b" << "\\bval\\b" << "\\bvirtual\\b"
				<< "\\bwhen\\b" << "\\bwhile\\b" << "\\bwith\\b";

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

	formatDeQuotation.setForeground(Qt::red);
		regle.modele = QRegExp("\".*\"");
		regle.modele.setMinimal(true);
		regle.format = formatDeQuotation;
			reglesDeSurlignage << regle;

	formatListe.setForeground(Qt::darkGreen);
		regle.modele = QRegExp("\\[.*\\]");
		regle.modele.setMinimal(true);
		regle.format = formatListe;
			reglesDeSurlignage << regle;

	formatVector.setForeground(Qt::darkBlue);
		regle.modele = QRegExp("\\[\\|.*\\|\\]");
		regle.modele.setMinimal(true);
		regle.format = formatVector;
			reglesDeSurlignage << regle;

	debutCommentaire = QRegExp("\\(\\*");
	finCommentaire = QRegExp("\\*\\)");
}

void HighlighterCaml::highlightBlock(const QString &texte)
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
