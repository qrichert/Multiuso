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

#include "HighlighterFortran.h"

HighlighterFortran::HighlighterFortran(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
        RegleDeSurlignage regle;

        formatMotsCles.setForeground(Qt::darkBlue);
        formatMotsCles.setFontWeight(QFont::Bold);

        QStringList modelesMotsCle;
                modelesMotsCle << "allocate" << "block" << "call" << "case" << "contains" << "continue" << "cycle" << "deallocate" <<
                                "default" << "do" << "else" << "elseif" << "elsewhere" << "end" << "enddo" << "endif" << "endwhere" <<
                                "entry" << "exit" << "function" << "go" << "goto" << "if" << "interface" << "module" << "nullify" << "only" <<
                                "operator" << "procedure" << "program" << "recursive" << "return" << "select" << "stop" <<
                                "subroutine" << "then" << "to" << "where" << "while" <<
                                "access" << "action" << "advance" << "blank" << "blocksize" << "carriagecontrol" <<
                                "delim" << "direct" << "eor" << "err" << "exist" << "file" << "flen" << "fmt" << "form" << "formatted" <<
                                "iostat" << "name" << "named" << "nextrec" << "nml" << "number" << "opened" << "pad" << "position" <<
                                "readwrite" << "recl" << "sequential" << "status" << "unformatted" << "unit" <<
                                "allocatable" << "character" << "common" << "complex" << "data" << "dimension" << "double" <<
                                "equivalence" << "external" << "implicit" << "in" << "inout" << "integer" << "intent" << "intrinsic" <<
                                "kind" << "logical" << "namelist" << "none" << "optional" << "out" << "parameter" << "pointer" <<
                                "private" << "public" << "real" << "result" << "save" << "sequence" << "target" << "type" << "use" <<
                                "abs" << "achar" << "acos" << "adjustl" << "adjustr" << "aimag" << "aint" << "all" << "allocated" <<
                                "anint" << "any" << "asin" << "atan" << "atan2" << "bit_size" << "break" << "btest" << "carg" <<
                                "ceiling" << "char" << "cmplx" << "conjg" << "cos" << "cosh" << "cpu_time" << "count" << "cshift" <<
                                "date_and_time" << "dble" << "digits" << "dim" << "dot_product" << "dprod dvchk" <<
                                "eoshift" << "epsilon" << "error" << "exp" << "exponent" << "floor" << "flush" << "fraction" <<
                                "getcl" << "huge" << "iachar" << "iand" << "ibclr" << "ibits" << "ibset" << "ichar" << "ieor" << "index" <<
                                "int" << "intrup" << "invalop" << "ior" << "iostat_msg" << "ishft" << "ishftc" << "lbound" <<
                                "len" << "len_trim" << "lge" << "lgt" << "lle" << "llt" << "log" << "log10" << "matmul" << "max" << "maxexponent" <<
                                "maxloc" << "maxval" << "merge" << "min" << "minexponent" << "minloc" << "minval" << "mod" << "modulo" <<
                                "mvbits" << "nbreak" << "ndperr" << "ndpexc" << "nearest" << "nint" << "not" << "offset" << "ovefl" <<
                                "pack" << "precfill" << "precision" << "present" << "product" << "prompt" << "radix" <<
                                "random_number" << "random_seed" << "range" << "repeat" << "reshape" << "rrspacing" <<
                                "scale" << "scan" << "segment" << "selected_int_kind" << "selected_real_kind" <<
                                "set_exponent" << "shape" << "sign" << "sin" << "sinh" << "size" << "spacing" << "spread" << "sqrt" <<
                                "sum system" << "system_clock" << "tan" << "tanh" << "timer" << "tiny" << "transfer" << "transpose" <<
                                "trim" << "ubound" << "undfl" << "unpack" << "val" << "verify";

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
                regle.modele = QRegExp("\\![^\n]*");
                regle.format = formatCommentairesUneLigne;
                        reglesDeSurlignage << regle;
}

void HighlighterFortran::highlightBlock(const QString &texte)
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
