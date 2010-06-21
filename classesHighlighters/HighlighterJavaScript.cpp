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

#include "HighlighterJavaScript.h"

HighlighterJavaScript::HighlighterJavaScript(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
        RegleDeSurlignage regle;

        formatMotsCles.setForeground(Qt::darkBlue);
        formatMotsCles.setFontWeight(QFont::Bold);

        QStringList modelesMotsCle;
                modelesMotsCle << "as" << "break" << "case" << "catch" << "continue" << "decodeURI" << "delete" << "do" <<
                                "else" << "encodeURI" << "eval" << "finally" << "for" << "if" << "in" << "is" << "item" <<
                                "instanceof" << "return" << "switch" << "this" << "throw" << "try" << "typeof" << "void" <<
                                "while" << "write" << "with" << "class" << "const" << "default" << "debugger" << "export" <<
                                "extends" << "false" << "function" << "import" << "namespace" << "new" << "null" <<
                                "package" << "private" << "protected" << "public" << "super" << "true" << "use" << "var" <<
                                "alert" << "back" << "blur" << "close" << "confirm" << "focus" << "forward" << "home" <<
                                "name" << "navigate" << "onblur" << "onerror" << "onfocus" << "onload" << "onmove" <<
                                "onresize" << "onunload" << "open" << "print" << "prompt" << "scroll" << "status" <<
                                "stop";

        foreach (const QString &modele, modelesMotsCle)
        {
            regle.modele = QRegExp(modele);
            regle.format = formatMotsCles;

            reglesDeSurlignage << regle;
        }

        formatCommentairesMultiLigne.setForeground(Qt::darkCyan);

        formatNombres.setForeground(Qt::darkMagenta);
                regle.modele = QRegExp("[0-9]+");
                regle.format = formatNombres;
                        reglesDeSurlignage << regle;

        formatDeQuotationSimple.setForeground(Qt::darkYellow);
            regle.modele = QRegExp("'.*'");
            regle.format = formatDeQuotationSimple;
                reglesDeSurlignage << regle;

        formatDeQuotationDouble.setForeground(Qt::darkYellow);
            regle.modele = QRegExp("\".*\"");
            regle.format = formatDeQuotationDouble;
                reglesDeSurlignage << regle;

        formatCommentairesUneLigne.setForeground(Qt::gray);
        formatCommentairesUneLigne.setFontItalic(true);
                regle.modele = QRegExp("//[^\n]*");
                regle.format = formatCommentairesUneLigne;
                        reglesDeSurlignage << regle;

        debutCommentaire = QRegExp("/\\*");
        finCommentaire = QRegExp("\\*/");
}

void HighlighterJavaScript::highlightBlock(const QString &texte)
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

                setFormat(indexDebut, longueurCommentaire, formatCommentairesMultiLigne);
                indexDebut = debutCommentaire.indexIn(texte, indexDebut + longueurCommentaire);
        }
}
