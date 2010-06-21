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

#include "HighlighterPHP.h"

HighlighterPHP::HighlighterPHP(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
        RegleDeSurlignage regle;

        formatMotsCles.setForeground(Qt::darkBlue);
        formatMotsCles.setFontWeight(QFont::Bold);

        QStringList modelesMotsCle;
                modelesMotsCle << "as" << "break" << "case" << "continue" << "default" << "do" << "else" << "elseif" <<
                                "endfor" << "endforeach" << "endif" << "endswitch" << "endwhile" << "for" <<
                                "foreach" << "if" << "include" << "include_once" << "require" << "require_once" <<
                                "return" << "switch" << "throw" << "while" << "class" << "const" << "declare" <<
                                "extends" << "function" << "global" << "interface" << "namespace" << "new" <<
                                "private" << "protected" << "public" << "self" << "use" << "var";

        foreach (const QString &modele, modelesMotsCle)
        {
            regle.modele = QRegExp(modele);
            regle.format = formatMotsCles;

            reglesDeSurlignage << regle;
        }

        formatMotsClesRegExp.setForeground(Qt::darkBlue);
        formatMotsClesRegExp.setFontWeight(QFont::Bold);
            regle.modele = QRegExp("(aggregate|apache|array|base|bcompiler|bind|bitset|call|classkit" \
                                   "|com|connection|convert|cpdf|ctype|crack|curl|date|db|debug|domxml" \
                                   "|event|fbird|fdf|file|filter|ftp|gmp|gz|hash|http|i18|ibase|iconv" \
                                   "|image|imap|ini|ldap|libxml|mb|mcrypt|msql|mssql|mysql|ntuser|ob" \
                                   "|odbc|openssl|output|parsekit|pdf|pg|php|pop3|posix|preg|radius|reg" \
                                   "|runkit|session|smtp|snmp|socket|spl|sqlit|ssh|stats|stream|substr" \
                                   "|svn|thread|tidy|timezone|variant|win|xml|xpath|yp|zend|zip|zlib)" \
                                   "\\w+");
            regle.format = formatMotsClesRegExp;
                reglesDeSurlignage << regle;

        formatCommentairesMultiLigne.setForeground(Qt::darkGray);
        formatCommentairesMultiLigne.setFontItalic(true);

        formatNombres.setForeground(Qt::magenta);
                regle.modele = QRegExp("[0-9]+");
                regle.format = formatNombres;
                        reglesDeSurlignage << regle;

        formatVariable.setForeground(Qt::darkMagenta);
                regle.modele = QRegExp("\\$[A-Za-z]+");
                regle.format = formatVariable;
                        reglesDeSurlignage << regle;

        formatDeQuotationSimple.setForeground(Qt::darkYellow);
            regle.modele = QRegExp("'.*'");
            regle.format = formatDeQuotationSimple;
                reglesDeSurlignage << regle;

        formatDebut.setForeground(Qt::red);
            regle.modele = QRegExp("<\\?php");
            regle.format = formatDebut;
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

void HighlighterPHP::highlightBlock(const QString &texte)
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
