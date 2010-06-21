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

#include "HighlighterIntercal.h"

HighlighterIntercal::HighlighterIntercal(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
        RegleDeSurlignage regle;

        formatMotsCles.setForeground(Qt::darkBlue);
        formatMotsCles.setFontWeight(QFont::Bold);

        QStringList modelesMotsCle;
                modelesMotsCle << "DO" << "DOES" << "DONT" << "DON'T" << "NOT" << "PLEASE" <<
                                "PLEASENT" << "PLEASEN'T" << "MAYBE" << "STASH" << "RETRIEVE" <<
                                "NEXT" << "RESUME" << "FORGET" << "ABSTAIN" << "ABSTAINING" <<
                                "COME" << "FROM" << "CALCULATING" << "REINSTATE" << "IGNORE" <<
                                "REMEMBER" << "WRITE" << "IN" << "READ" << "OUT" << "GIVE" << "UP";

        foreach (const QString &modele, modelesMotsCle)
        {
            regle.modele = QRegExp(modele);
            regle.format = formatMotsCles;

            reglesDeSurlignage << regle;
        }

        formatSub.setForeground(QColor(151, 153, 234));
            regle.modele = QRegExp("SUB");
            regle.format = formatSub;
                reglesDeSurlignage << regle;

        formatAbstainFrom.setForeground(QColor(64, 112, 160));
            regle.modele = QRegExp("ABSTAIN\\s+FROM\\s+\\S+");
            regle.format = formatAbstainFrom;
                reglesDeSurlignage << regle;

        formatVariable.setForeground(QColor(187, 96, 213));
            regle.modele = QRegExp("[\\#\\.,;][0-9]+");
            regle.format = formatVariable;
                reglesDeSurlignage << regle;

        formatCommentairesUneLigne.setForeground(Qt::gray);
        formatCommentairesUneLigne.setFontItalic(true);
            regle.modele = QRegExp("(DO|PLEASE)\\s+NOTE[^\n]*");
            regle.format = formatCommentairesUneLigne;
                reglesDeSurlignage << regle;
}

void HighlighterIntercal::highlightBlock(const QString &texte)
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
