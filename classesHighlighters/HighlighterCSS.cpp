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

#include "HighlighterCSS.h"

HighlighterCSS::HighlighterCSS(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
        RegleDeSurlignage regle;

        formatMotsCles.setForeground(Qt::darkBlue);
        formatMotsCles.setFontWeight(QFont::Bold);

        QStringList modelesMotsCle;
                modelesMotsCle << "aqua" << "azimuth" << "background-attachment" << "background-color" <<
                                "background-image" << "background-position" << "background-repeat" <<
                                "background" << "black" << "blue" << "border-bottom-color" <<
                                "border-bottom-style" << "border-bottom-width" << "border-left-color" <<
                                "border-left-style" << "border-left-width" << "border-right" <<
                                "border-right-color" << "border-right-style" << "border-right-width" <<
                                "border-top-color" << "border-top-style" <<
                                "border-top-width" << "border-bottom" << "border-collapse" <<
                                "border-left" << "border-width" << "border-color" << "border-spacing" <<
                                "border-style" << "border-top" << "border" << "caption-side" << "clear" <<
                                "clip" << "color" << "content" << "counter-increment" << "counter-reset" <<
                                "cue-after" << "cue-before" << "cue" << "cursor" << "direction" << "display" <<
                                "elevation" << "empty-cells" << "float" << "font-family" << "font-size" <<
                                "font-size-adjust" << "font-stretch" << "font-style" << "font-variant" <<
                                "font-weight" << "font" << "line-height" << "letter-spacing" <<
                                "list-style" << "list-style-image" << "list-style-position" <<
                                "list-style-type" << "margin-bottom" << "margin-left" << "margin-right" <<
                                "margin-top" << "margin" << "marker-offset" << "marks" << "max-height" <<
                                "max-width" << "min-height" << "min-width" << "orphans" << "outline" <<
                                "outline-color" << "outline-style" << "outline-width" << "overflow" <<
                                "padding-bottom" << "padding-left" << "padding-right" << "padding-top" <<
                                "padding" << "page" << "page-break-after" << "page-break-before" <<
                                "page-break-inside" << "pause-after" << "pause-before" << "pause" <<
                                "pitch" << "pitch-range" << "play-during" << "position" << "quotes" <<
                                "richness" << "right" << "size" << "speak-header" << "speak-numeral" <<
                                "speak-punctuation" << "speak" << "speech-rate" << "stress" <<
                                "table-layout" << "text-align" << "text-decoration" << "text-indent" <<
                                "text-shadow" << "text-transform" << "top" << "unicode-bidi" <<
                                "vertical-align" << "visibility" << "voice-family" << "volume" <<
                                "white-space" << "widows" << "width" << "word-spacing" << "z-index" <<
                                "bottom" << "left" << "height" <<
                                "above" << "absolute" << "always" << "armenian" << "aural" << "auto" <<
                                "avoid" << "baseline" << "behind" << "below" << "bidi-override" << "blink" <<
                                "block" << "bold" << "bolder" << "both" << "capitalize" << "center-left" <<
                                "center-right" << "center" << "circle" << "cjk-ideographic" <<
                                "close-quote" << "collapse" << "condensed" << "continuous" << "crop" <<
                                "crosshair" << "cross" << "cursive" << "dashed" << "decimal-leading-zero" <<
                                "decimal" << "default" << "digits" << "disc" << "dotted" << "double" <<
                                "e-resize" << "embed" << "extra-condensed" << "extra-expanded" <<
                                "expanded" << "fantasy" << "far-left" << "far-right" << "faster" << "fast" <<
                                "fixed" << "fuchsia" << "georgian" << "gray" << "green" << "groove" <<
                                "hebrew" << "help" << "hidden" << "hide" << "higher" << "high" <<
                                "hiragana-iroha" << "hiragana" << "icon" << "inherit" << "inline-table" <<
                                "inline" << "inset" << "inside" << "invert" << "italic" << "justify" <<
                                "katakana-iroha" << "katakana" << "landscape" << "larger" << "large" <<
                                "left-side" << "leftwards" << "level" << "lighter" << "lime" <<
                                "line-through" << "list-item" << "loud" << "lower-alpha" << "lower-greek" <<
                                "lower-roman" << "lowercase" << "ltr" << "lower" << "low" << "maroon" <<
                                "medium" << "message-box" << "middle" << "mix" << "monospace" << "n-resize" <<
                                "narrower" << "navy" << "ne-resize" << "no-close-quote" <<
                                "no-open-quote" << "no-repeat" << "none" << "normal" << "nowrap" <<
                                "nw-resize" << "oblique" << "olive" << "once" << "open-quote" << "outset" <<
                                "outside" << "overline" << "pointer" << "portrait" << "purple" << "px" <<
                                "red" << "relative" << "repeat-x" << "repeat-y" << "repeat" << "rgb" <<
                                "ridge" << "right-side" << "rightwards" << "s-resize" << "sans-serif" <<
                                "scroll" << "se-resize" << "semi-condensed" << "semi-expanded" <<
                                "separate" << "serif" << "show" << "silent" << "silver" << "slow" << "slower" <<
                                "small-caps" << "small-caption" << "smaller" << "soft" << "solid" <<
                                "spell-out" << "square" << "static" << "status-bar" << "super" <<
                                "sw-resize" << "table-caption" << "table-cell" << "table-column" <<
                                "table-column-group" << "table-footer-group" << "table-header-group" <<
                                "table-row" << "table-row-group" << "teal" << "text" << "text-bottom" <<
                                "text-top" << "thick" << "thin" << "transparent" << "ultra-condensed" <<
                                "ultra-expanded" << "underline" << "upper-alpha" << "upper-latin" <<
                                "upper-roman" << "uppercase" << "url" << "visible" << "w-resize" << "wait" <<
                                "white" << "wider" << "x-fast" << "x-high" << "x-large" << "x-loud" <<
                                "x-low" << "x-small" << "x-soft" << "xx-large" << "xx-small" << "yellow" <<
                                "yes";

        foreach (const QString &modele, modelesMotsCle)
        {
            regle.modele = QRegExp(modele);
            regle.format = formatMotsCles;

            reglesDeSurlignage << regle;
        }

        formatCommentairesMultiLigne.setForeground(Qt::darkCyan);

        formatNombres.setForeground(Qt::darkGreen);
                regle.modele = QRegExp("[0-9]+");
                regle.format = formatNombres;
                        reglesDeSurlignage << regle;

        formatPropriete.setForeground(Qt::darkBlue);
        formatPropriete.setFontWeight(QFont::Bold);
                regle.modele = QRegExp("[\\.\\#][a-zA-Z]+");
                regle.format = formatPropriete;
                        reglesDeSurlignage << regle;

        formatCouleurHex.setForeground(Qt::red);
        formatCouleurHex.setFontWeight(QFont::Bold);
                regle.modele = QRegExp("\\#[0-9]+");
                regle.format = formatCouleurHex;
                        reglesDeSurlignage << regle;

        formatCommentairesUneLigne.setForeground(Qt::gray);
        formatCommentairesUneLigne.setFontItalic(true);
                regle.modele = QRegExp("//[^\n]*");
                regle.format = formatCommentairesUneLigne;
                        reglesDeSurlignage << regle;

        debutCommentaire = QRegExp("/\\*");
        finCommentaire = QRegExp("\\*/");
}

void HighlighterCSS::highlightBlock(const QString &texte)
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
