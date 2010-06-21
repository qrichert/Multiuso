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

#include "HighlighterLisp.h"

HighlighterLisp::HighlighterLisp(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
        RegleDeSurlignage regle;

        formatMotsCles.setForeground(Qt::darkBlue);
        formatMotsCles.setFontWeight(QFont::Bold);

        QStringList modelesMotsCle;
                modelesMotsCle << "not" << "defun" << "princ" << "when" << "eval" <<
                                "apply" << "funcall" << "quote" << "identity" << "function" <<
                                "complement" << "backquote" << "lambda" << "set" << "setq" <<
                                "setf" << "defmacro" << "gensym" << "make" << "symbol" <<
                                "intern" << "name" << "value" << "plist" << "get" <<
                                "getf" << "putprop" << "remprop" << "hash" << "array" <<
                                "aref" << "car" << "cdr" << "caar" << "cadr" << "cdar" <<
                                "cddr" << "caaar" << "caadr" << "cadar" << "caddr" << "cdaar" <<
                                "cdadr" << "cddar" << "cdddr" << "caaaar" << "caaadr" <<
                                "caadar" << "caaddr" << "cadaar" << "cadadr" << "caddar" <<
                                "cadddr" << "cdaaar" << "cdaadr" << "cdadar" << "cdaddr" <<
                                "cddaar" << "cddadr" << "cdddar" << "cddddr" << "cons" <<
                                "list" << "append" << "reverse" << "last" << "nth" <<
                                "nthcdr" << "member" << "assoc" << "subst" << "sublis" <<
                                "nsubst" << "nsublis" << "remove" << "length" <<
                                "mapc" << "mapcar" << "mapl" << "maplist" << "mapcan" <<
                                "mapcon" << "rplaca" << "rplacd" << "nconc" << "delete" <<
                                "atom" << "symbolp" << "numberp" << "boundp" << "null" <<
                                "listp" << "consp" << "minusp" << "zerop" << "plusp" <<
                                "evenp" << "oddp" << "eq" << "eql" << "equal" << "cond" <<
                                "case" << "and" << "or" << "let" << "l" << "if" << "prog" <<
                                "prog1" << "prog2" << "progn" << "go" << "return" <<
                                "do" << "dolist" << "dotimes" << "catch" << "throw" <<
                                "error" << "cerror" << "break" << "continue" << "errset" <<
                                "baktrace" << "evalhook" << "truncate" << "float" <<
                                "rem" << "min" << "max" << "abs" << "sin" << "cos" << "tan" <<
                                "expt" << "exp" << "sqrt" << "random" << "logand" << "logior" <<
                                "logxor" << "lognot" << "bignums" << "logeqv" <<
                                "lognand" << "lognor" << "logorc2" << "logtest" << "logbitp" <<
                                "logcount" << "integer" << "nil" << "parse-integer";

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

        formatParentheses.setForeground(Qt::darkGreen);
        formatParentheses.setFontWeight(QFont::Bold);
            regle.modele = QRegExp("[\\(\\)]");
            regle.format = formatParentheses;
                reglesDeSurlignage << regle;

        formatDeQuotation.setForeground(Qt::darkYellow);
            regle.modele = QRegExp("\".*\"");
            regle.modele.setMinimal(true);
            regle.format = formatDeQuotation;
                reglesDeSurlignage << regle;

        formatCommentairesUneLigne.setForeground(Qt::gray);
        formatCommentairesUneLigne.setFontItalic(true);
            regle.modele = QRegExp(";[^\n]*");
            regle.format = formatCommentairesUneLigne;
                reglesDeSurlignage << regle;
}

void HighlighterLisp::highlightBlock(const QString &texte)
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
