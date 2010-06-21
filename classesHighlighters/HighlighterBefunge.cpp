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

#include "HighlighterBefunge.h"

HighlighterBefunge::HighlighterBefunge(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
        RegleDeSurlignage regle;

        formatDirections.setForeground(QColor(187, 96, 213));
                regle.modele = QRegExp("[\\^v<>\\|_]");
                regle.format = formatDirections;
                        reglesDeSurlignage << regle;

        formatNombres.setForeground(Qt::darkYellow);
                regle.modele = QRegExp("[0-9]+");
                regle.format = formatNombres;
                        reglesDeSurlignage << regle;

        formatOperateurs.setForeground(QColor(64, 160, 112));
                regle.modele = QRegExp("[\\+\\-\\*/%\\!\\?:\\\\\\$]");
                regle.format = formatOperateurs;
                        reglesDeSurlignage << regle;

        formatAutresInstructions.setForeground(QColor(41, 105, 73));
                regle.modele = QRegExp("[\\.,\\#pg&~@]");
                regle.format = formatAutresInstructions;
                        reglesDeSurlignage << regle;

        formatDeQuotation.setForeground(QColor(64, 112, 160));
                regle.modele = QRegExp("\".*\"");
                regle.format = formatDeQuotation;
                        reglesDeSurlignage << regle;
}

void HighlighterBefunge::highlightBlock(const QString &texte)
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
