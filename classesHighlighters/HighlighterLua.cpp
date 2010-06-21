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

#include "HighlighterLua.h"

HighlighterLua::HighlighterLua(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
        RegleDeSurlignage regle;

        formatMotsCles.setForeground(Qt::darkBlue);
        formatMotsCles.setFontWeight(QFont::Bold);

        QStringList modelesMotsCle;
                modelesMotsCle << "and" << "break" << "do" << "else" << "elseif" << "end" <<
                                "false" << "for" << "function" << "if" <<
                                "in" << "local" << "nil" << "not" << "or" << "repeat" << "return" <<
                                "then" << "true" << "until" << "while" <<
                                "_VERSION" << "assert" << "collectgarbage" << "dofile" << "error" <<
                                "gcinfo" << "loadfile" << "loadstring" <<
                                "print" << "tonumber" << "tostring" << "type" << "unpack" <<
                                "_ALERT" << "_ERRORMESSAGE" << "_INPUT" << "_PROMPT" << "_OUTPUT" <<
                                "_STDERR" << "_STDIN" << "_STDOUT" << "call" << "dostring" <<
                                "foreach" << "foreachi" << "getn" << "globals" << "newtype" <<
                                "rawget" << "rawset" << "require" << "sort" << "tinsert" << "tremove" <<
                                "abs" << "acos" << "asin" << "atan" << "atan2" << "ceil" << "cos" << "deg" << "exp" <<
                                "floor" << "format" << "frexp" << "gsub" << "ldexp" << "log" << "log10" << "max" <<
                                "min" << "mod" << "rad" << "random" << "randomseed" <<
                                "sin" << "sqrt" << "strbyte" << "strchar" << "strfind" << "strlen" << "strlower" <<
                                "strrep" << "strsub" << "strupper" << "tan" <<
                                "openfile" << "closefile" << "readfrom" << "writeto" << "appendto" <<
                                "remove" << "rename" << "flush" << "seek" << "tmpfile" << "tmpname" << "read" << "write" <<
                                "clock" << "date" << "difftime" << "execute" << "exit" << "getenv" << "setlocale" << "time" <<
                                "_G" << "getfenv" << "getmetatable" << "ipairs" << "loadlib" << "next" << "pairs" << "pcall" <<
                                "rawegal" << "setfenv" << "setmetatable" << "xpcall" <<
                                "string" << "table" << "math" << "coroutine" << "io" << "os" << "debug";

        foreach (const QString &modele, modelesMotsCle)
        {
            regle.modele = QRegExp(modele);
            regle.format = formatMotsCles;

            reglesDeSurlignage << regle;
        }

        formatMotsClesRegExp.setForeground(Qt::darkBlue);
        formatMotsClesRegExp.setFontWeight(QFont::Bold);
            regle.modele = QRegExp("(table|string|math|io|os|coroutine)\\.\\w+");
            regle.format = formatMotsClesRegExp;
                reglesDeSurlignage << regle;

        formatCommentairesMultiLigne.setForeground(Qt::darkGray);
        formatCommentairesMultiLigne.setFontItalic(true);

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
                regle.modele = QRegExp("\\-\\-[^\n]*");
                regle.format = formatCommentairesUneLigne;
                        reglesDeSurlignage << regle;

        debutCommentaire = QRegExp("\\W\\-\\-\\[\\[");
        finCommentaire = QRegExp("\\]\\]");
}

void HighlighterLua::highlightBlock(const QString &texte)
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
