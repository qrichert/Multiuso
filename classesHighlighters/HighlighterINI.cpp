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

#include "HighlighterINI.h"

HighlighterINI::HighlighterINI(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
        RegleDeSurlignage regle;

        formatChiffres.setForeground(Qt::magenta);
            regle.modele = QRegExp("[0-9]");
            regle.format = formatChiffres;
                reglesDeSurlignage << regle;

        formatSection.setForeground(Qt::darkBlue);
        formatSection.setFontWeight(QFont::Bold);
            regle.modele = QRegExp("\\[.+\\]");
            regle.modele.setMinimal(true);
            regle.format = formatSection;
                reglesDeSurlignage << regle;

        formatItem.setForeground(Qt::red);
            regle.modele = QRegExp(".+=");
            regle.format = formatItem;
                reglesDeSurlignage << regle;

        formatCommentaireUneLigne.setForeground(Qt::darkGray);
        formatCommentaireUneLigne.setFontItalic(true);
            regle.modele = QRegExp(";[^\n]*");
            regle.format = formatCommentaireUneLigne;
                reglesDeSurlignage << regle;

        formatDeQuotation.setFontWeight(QFont::Bold);
        formatDeQuotation.setForeground(Qt::darkYellow);
            regle.modele = QRegExp("\".*\"");
            regle.modele.setMinimal(true);
            regle.format = formatDeQuotation;
                reglesDeSurlignage << regle;
}

void HighlighterINI::highlightBlock(const QString &texte)
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
