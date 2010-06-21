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

#include "HighlighterMiranda.h"

HighlighterMiranda::HighlighterMiranda(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
        RegleDeSurlignage regle;

        formatMotsCles.setForeground(Qt::darkBlue);
        formatMotsCles.setFontWeight(QFont::Bold);

        QStringList modelesMotsCle;
                modelesMotsCle << "goto" << "break" << "return" << "continue" << "asm" <<
                                "if" << "otherwise" << "num" << "char" << "bool" << "abstype" <<
                                "div" << "where" << "mod" << "readvals" << "show" << "type" <<
                                "with" << "sys_message" << "False" << "True" << "Appendfile" <<
                                "Closefile" << "Exit" << "Stderr" << "Stdout" << "System" <<
                                "Tofile" << "abs" << "and" << "arctan" << "cjustify" << "code" <<
                                "concat" << "const" << "converse" << "cos" << "decode" <<
                                "digit" << "drop" << "dropwhile" << "entier" << "error" <<
                                "exp" << "filemode" << "filter" << "foldl" << "foldl1" << "foldr" <<
                                "foldr1" << "force" << "fst" << "getenv" << "hd" << "hugenum" <<
                                "id" << "index" << "init" << "integer" << "iterate" << "last" <<
                                "lay" << "layn" << "letter" << "limit" << "lines" << "ljustify" <<
                                "log" << "log10" << "map" << "map2" << "max" << "max2" <<
                                "member" << "merge" << "min" << "min2" << "mkset" <<
                                "neg" << "numval" << "or" << "pi" << "postfix" <<
                                "product" << "read" << "rep" << "repeat" << "reverse" <<
                                "rjustify" << "scan" << "seq" << "showfloat" <<
                                "shownum" << "showscaled" << "sin" << "snd" << "sort" <<
                                "spaces" << "sqrt" << "subtract" << "sum" << "system" <<
                                "take" << "takewhile" << "tinynum" << "tl" << "transpose" <<
                                "undef" << "until" << "zip2" << "zip3" << "zip4" << "zip5" <<
                                "zip6" << "zip";

        foreach (const QString &modele, modelesMotsCle)
        {
            regle.modele = QRegExp(modele);
            regle.format = formatMotsCles;

            reglesDeSurlignage << regle;
        }

        formatNombres.setForeground(Qt::magenta);
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
                regle.modele = QRegExp("\\|\\|[^\n]*");
                regle.format = formatCommentairesUneLigne;
                        reglesDeSurlignage << regle;

        debutCommentaire = QRegExp("/\\*");
        finCommentaire = QRegExp("\\*/");
}

void HighlighterMiranda::highlightBlock(const QString &texte)
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
