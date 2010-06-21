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

#include "HighlighterAutoIT.h"

HighlighterAutoIT::HighlighterAutoIT(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
        RegleDeSurlignage regle;

        formatMotsCles.setForeground(Qt::darkBlue);
        formatMotsCles.setFontWeight(QFont::Bold);

        QStringList modelesMotsCle;
                modelesMotsCle << "And" << "ByRef" << "Case" << "Const" << "ContinueCase" << "ContinueLoop" <<
                                "Default" << "Dim" << "Do" << "Else" << "ElseIf" << "EndFunc" << "EndIf" << "EndSelect" <<
                                "EndSwitch" << "EndWith" << "Enum" << "Exit" << "ExitLoop" << "False" << "For" << "Func" <<
                                "Global" << "If" << "In" << "Local" << "Next" << "Not" << "Or" << "ReDim" << "Return" <<
                                "Select" << "Step" << "Switch" << "Then" << "To" << "True" << "Until" << "WEnd" << "While" <<
                                "With";

        foreach (const QString &modele, modelesMotsCle)
        {
            regle.modele = QRegExp(modele);
            regle.format = formatMotsCles;

            reglesDeSurlignage << regle;
        }

        formatMotsClesRegExp.setForeground(Qt::darkBlue);
        formatMotsClesRegExp.setFontWeight(QFont::Bold);
            regle.modele = QRegExp("(Array|Auto|Choose|Clip|Color|Date|Day|Debug|Event|File|GDI|GUI|IE|Mem|Mouse|Named" \
                                   "|Math|Net|Now|Path|Screen|Security|Sound|SQLite|String|Temp|Time|vi|Win|Word|Dll|Console" \
                                   "|Control|Dir|Drive|Obj|Tray|TCP)\\w+");
            regle.format = formatMotsClesRegExp;
                reglesDeSurlignage << regle;

        formatMotsClesRegExp2.setForeground(Qt::darkBlue);
        formatMotsClesRegExp2.setFontWeight(QFont::Bold);
            regle.modele = QRegExp("@\\w+");
            regle.format = formatMotsClesRegExp2;
                reglesDeSurlignage << regle;

        formatNombres.setForeground(Qt::darkGreen);
            regle.modele = QRegExp("[0-9]+");
            regle.format = formatNombres;
                reglesDeSurlignage << regle;

        formatVariables.setForeground(Qt::red);
            regle.modele = QRegExp("\\$\\w+");
            regle.format = formatVariables;
                reglesDeSurlignage << regle;

        formatCommentairesUneLigne.setForeground(Qt::gray);
        formatCommentairesUneLigne.setFontItalic(true);
            regle.modele = QRegExp(";[^\n]*");
            regle.format = formatCommentairesUneLigne;
                reglesDeSurlignage << regle;
}

void HighlighterAutoIT::highlightBlock(const QString &texte)
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
