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

#include "HighlighterGLSL.h"

HighlighterGLSL::HighlighterGLSL(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
        RegleDeSurlignage regle;

        formatMotCle.setForeground(Qt::darkBlue);
        formatMotCle.setFontWeight(QFont::Bold);

                QStringList modelesMotsCle;
                        modelesMotsCle << "if" << "else" << "for" << "while" << "do" << "break" << "continue" << "asm" <<
                                            "switch" << "case" << "default" << "return" << "discard" <<
                                            "namespace" << "using" << "sizeof" << "cast" <<
                                            "const" << "uniform" << "attribute" << "centroid" << "varying" << "invariant" <<
                                            "in" << "out" << "inout" << "input" << "output" << "typedef" << "volatile" <<
                                            "public" << "static" << "extern" << "external" << "packed" <<
                                            "inline" << "noinline" << "noperspective" << "flat" <<
                                            "void" << "bool" << "int" << "long" << "short" << "float" << "half" << "fixed" <<
                                            "unsigned" << "lowp" << "mediump" << "highp" << "precision" <<
                                            "vec2" << "vec3" << "vec4" << "bvec2" << "bvec3" << "bvec4" <<
                                            "dvec2" << "dvec3" << "dvec4" << "fvec2" << "fvec3" << "fvec4" <<
                                            "hvec2" << "hvec3" << "hvec4" << "ivec2" << "ivec3" << "ivec4" <<
                                            "mat2" << "mat3" << "mat4" << "mat2x2" << "mat3x2" << "mat4x2" <<
                                            "mat2x3" << "mat3x3" << "mat4x3" << "mat2x4" << "mat3x4" << "mat4x4" <<
                                            "sampler1D" << "sampler2D" << "sampler3D" << "samplerCube" <<
                                            "sampler1DShadow" << "sampler2DShadow" <<
                                            "struct" << "class" << "union" << "enum" << "interface" << "template" <<
                                            "this" << "false" << "true" << "radians" << "degrees" << "sin" << "cos" << "tan" << "asin" << "acos" << "atan" <<
                                            "pow" << "exp2" << "log2" << "sqrt" << "inversesqrt" << "abs" << "sign" << "ceil" <<
                                            "floor" << "fract" << "mod" << "min" << "max" << "clamp" << "mix" << "step" <<
                                            "smoothstep" << "length" << "distance" << "dot" << "cross" << "normalize" <<
                                            "ftransform" << "faceforward" << "reflect" << "matrixCompMult" << "equal" <<
                                            "lessThan" << "lessThanEqual" << "greaterThan" << "greaterThanEqual" <<
                                            "notEqual" << "any" << "all" << "not" << "texture1D" << "texture1DProj" <<
                                            "texture1DLod" << "texture1DProjLod" << "texture2D" << "texture2DProj" <<
                                            "texture2DLod" << "texture2DProjLod" << "texture3D" << "texture3DProj" <<
                                            "texture3DLod" << "texture3DProjLod" << "textureCube" << "textureCubeLod" <<
                                            "shadow1D" << "shadow1DProj" << "shadow1DLod" << "shadow1DProjLod" <<
                                            "shadow2D" << "shadow2DProj" << "shadow2DLod" << "shadow2DProjLod" <<
                                            "noise1" << "noise2" << "noise3" << "noise4";

                        foreach (const QString &modele, modelesMotsCle)
                        {
                                regle.modele = QRegExp(modele);
                                regle.format = formatMotCle;

                                reglesDeSurlignage << regle;
                        }

        formatChiffres.setForeground(Qt::magenta);
            regle.modele = QRegExp("[0-9]");
            regle.format = formatChiffres;
                reglesDeSurlignage << regle;

        formatMotCleGL.setForeground(Qt::darkBlue);
        formatMotCleGL.setFontWeight(QFont::Bold);
            regle.modele = QRegExp("gl_\\w+");
            regle.format = formatMotCleGL;
                reglesDeSurlignage << regle;

        formatPreprocesseur.setFontWeight(QFont::Bold);
        formatPreprocesseur.setForeground(Qt::darkGreen);
            regle.modele = QRegExp("#[^\n]*");
            regle.format = formatPreprocesseur;
                reglesDeSurlignage << regle;

        formatCommentaireUneLigne.setForeground(Qt::darkCyan);
            regle.modele = QRegExp("//[^\n]*");
            regle.format = formatCommentaireUneLigne;
                reglesDeSurlignage << regle;

        formatCommentaireMultiLigne.setForeground(Qt::darkCyan);

        formatDeQuotation.setFontWeight(QFont::Bold);
        formatDeQuotation.setForeground(Qt::darkYellow);
            regle.modele = QRegExp("\".*\"");
            regle.modele.setMinimal(true);
            regle.format = formatDeQuotation;
                reglesDeSurlignage << regle;

        debutCommentaire = QRegExp("/\\*");
        finCommentaire = QRegExp("\\*/");
}

void HighlighterGLSL::highlightBlock(const QString &texte)
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

                setFormat(indexDebut, longueurCommentaire, formatCommentaireMultiLigne);
                indexDebut = debutCommentaire.indexIn(texte, indexDebut + longueurCommentaire);
        }
}
