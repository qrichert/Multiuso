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

#include "HighlighterASM.h"

HighlighterASM::HighlighterASM(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
        RegleDeSurlignage regle;

        formatMotsCles.setForeground(Qt::darkBlue);
        formatMotsCles.setFontWeight(QFont::Bold);

        QStringList modelesMotsCle;
                modelesMotsCle << "aaa" << "aad" << "aam" << "aas" << "adc" << "add" << "and" << "call" << "cbw" << "clc" << "cld" << "cli" << "cmc" << "cmp" <<
                                "cmps" << "cmpsb" << "cmpsw" << "cwd" << "daa" << "das" << "dec" << "div" << "esc" << "hlt" << "idiv" << "imul" << "in" << "inc" <<
                                "int" << "into" << "iret" << "ja" << "jae" << "jb" << "jbe" << "jc" << "jcxz" << "je" << "jg" << "jge" << "jl" << "jle" << "jmp" <<
                                "jna" << "jnae" << "jnb" << "jnbe" << "jnc" << "jne" << "jng" << "jnge" << "jnl" << "jnle" << "jno" << "jnp" << "jns" << "jnz" <<
                                "jo" << "jp" << "jpe" << "jpo" << "js" << "jz" << "lahf" << "lds" << "lea" << "les" << "lods" << "lodsb" << "lodsw" << "loop" <<
                                "loope" << "loopew" << "loopne" << "loopnew" << "loopnz" << "loopnzw" << "loopw" << "loopz" << "loopzw" << "mov" <<
                                "movs" << "movsb" << "movsw" << "mul" << "neg" << "nop" << "not" << "or" << "out" << "pop" << "popf" << "push" << "pushf" <<
                                "rcl" << "rcr" << "ret" << "retf" << "retn" << "rol" << "ror" << "sahf" << "sal" << "sar" << "sbb" << "scas" << "scasb" << "scasw" <<
                                "shl" << "shr" << "stc" << "std" << "sti" << "stos" << "stosb" << "stosw" << "sub" << "test" << "wait" << "xchg" << "xlat" <<
                                "xlatb" << "xor" << "bound" << "enter" << "ins" << "insb" << "insw" << "leave" << "outs" << "outsb" << "outsw" << "popa" << "pusha" << "pushw" <<
                                "arpl" << "lar" << "lsl" << "sgdt" << "sidt" << "sldt" << "smsw" << "str" << "verr" << "verw" << "clts" << "lgdt" << "lidt" << "lldt" << "lmsw" << "ltr" <<
                                "bsf" << "bsr" << "bt" << "btc" << "btr" << "bts" << "cdq" << "cmpsd" << "cwde" << "insd" << "iretd" << "iretdf" << "iretf" <<
                                "jecxz" << "lfs" << "lgs" << "lodsd" << "loopd" << "looped" << "loopned" << "loopnzd" << "loopzd" << "lss" << "movsd" <<
                                "movsx" << "movzx" << "outsd" << "popad" << "popfd" << "pushad" << "pushd" << "pushfd" << "scasd" << "seta" << "setae" <<
                                "setb" << "setbe" << "setc" << "sete" << "setg" << "setge" << "setl" << "setle" << "setna" << "setnae" << "setnb" << "setnbe" <<
                                "setnc" << "setne" << "setng" << "setnge" << "setnl" << "setnle" << "setno" << "setnp" << "setns" << "setnz" << "seto" << "setp" <<
                                "setpe" << "setpo" << "sets" << "setz" << "shld" << "shrd" << "stosd" << "bswap" << "cmpxchg" << "invd" << "invlpg" << "wbinvd" << "xadd" << "lock" <<
                                "rep" << "repe" << "repne" << "repnz" << "repz" << "f2xm1" << "fabs" << "fadd" << "faddp" << "fbld" << "fbstp" << "fchs" << "fclex" << "fcom" <<
                                "fcomp" << "fcompp" << "fdecstp" << "fdisi" << "fdiv" << "fdivp" << "fdivr" << "fdivrp" << "feni" << "ffree" << "fiadd" << "ficom" << "ficomp" << "fidiv" <<
                                "fidivr" << "fild" << "fimul" << "fincstp" << "finit" << "fist" << "fistp" << "fisub" << "fisubr" << "fld" << "fld1" <<
                                "fldcw" << "fldenv" << "fldenvw" << "fldl2e" << "fldl2t" << "fldlg2" << "fldln2" << "fldpi" << "fldz" << "fmul" <<
                                "fmulp" << "fnclex" << "fndisi" << "fneni" << "fninit" << "fnop" << "fnsave" << "fnsavew" << "fnstcw" << "fnstenv" <<
                                "fnstenvw" << "fnstsw" << "fpatan" << "fprem" << "fptan" << "frndint" << "frstor" << "frstorw" << "fsave" <<
                                "fsavew" << "fscale" << "fsqrt" << "fst" << "fstcw" << "fstenv" << "fstenvw" << "fstp" << "fstsw" << "fsub" << "fsubp" <<
                                "fsubr" << "fsubrp" << "ftst" << "fwait" << "fxam" << "fxch" << "fxtract" << "fyl2x" << "fyl2xp1" <<
                                "fsetpm" << "fcos" << "fldenvd" << "fnsaved" << "fnstenvd" << "fprem1" << "frstord" << "fsaved" << "fsin" << "fsincos" <<
                                "fstenvd" << "fucom" << "fucomp" << "fucompp" << "ah" << "al" << "ax" << "bh" << "bl" << "bp" << "bx" << "ch" << "cl" << "cr0" <<
                                "cr2" << "cr3" << "cs" << "cx" << "dh" << "di" << "dl" << "dr0" << "dr1" << "dr2" << "dr3" << "dr6" << "dr7" << "ds" <<
                                "dx" << "eax" << "ebp" << "ebx" << "ecx" << "edi" << "edx" << "es" << "esi" << "esp" << "fs" << "gs" << "si" << "sp" << "ss" <<
                                "st" << "tr3" << "tr4" << "tr5" << "tr6" << "tr7" << "186" << "286" << "286c" << "286p" << "287" << "386" << "386c" << "386p" << "387" << "486" << "486p" <<
                                "8086" << "8087" << "alpha" << "break" << "code" << "const" << "continue" << "cref" << "data" << "data" <<
                                "dosseg" << "else" << "elseif" << "endif" << "endw" << "err" << "err1" << "err2" << "errb" <<
                                "errdef" << "errdif" << "errdifi" << "erre" << "erridn" << "erridni" << "errnb" << "errndef" <<
                                "errnz" << "exit" << "fardata" << "if" << "lall" << "lfcond" << "list" << "listall" <<
                                "listif" << "listmacro" << "listmacroall" << " model" << "no87" << "nocref" << "nolist" <<
                                "nolistif" << "nolistmacro" << "radix" << "repeat" << "sall" << "seq" << "sfcond" << "stack" <<
                                "startup" << "tfcond" << "type" << "until" << "untilcxz" << "while" << "xall" << "xcref" <<
                                "xlist" << "alias" << "align" << "assume" << "catstr" << "comm" << "comment" << "db" << "dd" << "df" << "dq" <<
                                "dt" << "dup" << "dw" << "echo" << "elseif1" << "elseif2" << "elseifb" << "elseifdef" << "elseifdif" <<
                                "elseifdifi" << "elseife" << "elseifidn" << "elseifidni" << "elseifnb" << "elseifndef" << "end" <<
                                "endm" << "endp" << "ends" << "eq" << " equ" << "even" << "exitm" << "extern" << "externdef" << "extrn" << "for" <<
                                "forc" << "ge" << "goto" << "group" << "high" << "highword" << "if1" << "if2" << "ifb" << "ifdef" << "ifdif" <<
                                "ifdifi" << "ife" << " ifidn" << "ifidni" << "ifnb" << "ifndef" << "include" << "includelib" << "instr" << "invoke" <<
                                "irp" << "irpc" << "label" << "le" << "length" << "lengthof" << "local" << "low" << "lowword" << "lroffset" <<
                                "macro" << "mask" << "mod" << "msfloat" << "name" << "ne" << "offset" << "opattr" << "option" << "org" << "out" <<
                                "page" << "popcontext" << "private" << "proc" << "proto" << "ptr" << "public" << "purge" << "pushcontext" << "record" <<
                                "rept" << "seg" << "segment" << "short" << "size" << "sizeof" << "sizestr" << "struc" << "struct" <<
                                "substr" << "subtitle" << "subttl" << "textequ" << "this" << "title" << "typedef" << "union" << "width" <<
                                "real4" << " real8" << "real10" << "sbyte" << "sdword" << "sign" << "stdcall" << "sword" << "syscall" << "tbyte" <<
                                "vararg" << "word" << "zero" << "flat" << "near32" << "far32" <<
                                "abs" << "all" << "assumes" << "at" << "casemap" << "common" << "compact" <<
                                "cpu" << "dotname" << "emulator" << "epilogue" << "error" << "export" << "expr16" << "expr32" << "farstack" <<
                                "forceframe" << "huge" << "language" << "large" << "listing" << "ljmp" << "loadds" << "m510" << "medium" << "memory" <<
                                "nearstack" << "nodotname" << "noemulator" << "nokeyword" << "noljmp" << "nom510" << "none" << "nonunique" <<
                                "nooldmacros" << "nooldstructs" << "noreadonly" << "noscoped" << "nosignextend" << "nothing" <<
                                "notpublic" << "oldmacros" << "oldstructs" << "os_dos" << "para" << "prologue" <<
                                "readonly" << "req" << "scoped" << "setif2" << "smallstack" << "tiny" << "use16" << "use32" << "uses";

        foreach (const QString &modele, modelesMotsCle)
        {
            regle.modele = QRegExp(modele);
            regle.format = formatMotsCles;

            reglesDeSurlignage << regle;
        }

        formatMotsClesRegExp1.setForeground(Qt::darkBlue);
        formatMotsClesRegExp1.setFontWeight(QFont::Bold);
            regle.modele = QRegExp("\\.\\w+");
            regle.format = formatMotsClesRegExp1;
                reglesDeSurlignage << regle;

        formatMotsClesRegExp2.setForeground(Qt::darkBlue);
        formatMotsClesRegExp2.setFontWeight(QFont::Bold);
            regle.modele = QRegExp("%\\w+");
            regle.format = formatMotsClesRegExp2;
                reglesDeSurlignage << regle;

        formatNombres.setForeground(QColor(64, 112, 160));
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
            regle.modele = QRegExp(";[^\n]*");
            regle.format = formatCommentairesUneLigne;
                reglesDeSurlignage << regle;
}

void HighlighterASM::highlightBlock(const QString &texte)
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
