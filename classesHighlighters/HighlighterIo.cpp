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

#include "HighlighterIo.h"

HighlighterIo::HighlighterIo(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
        RegleDeSurlignage regle;

        formatMotCle.setForeground(Qt::darkBlue);
        formatMotCle.setFontWeight(QFont::Bold);

                QStringList modelesMotsCle;
                        modelesMotsCle << "and" << "break" << "else" << "elseif" << "exit" << "for" << "foreach" << "if" << "ifFalse" << "ifNil" <<
                                        "ifTrue" << "or" << "pass" << "raise" << "return" << "then" << "try" << "wait" << "while" << "yield" <<
                                        "activate" << "activeCoroCount" << "asString" << "block" << "catch" << "clone" << "collectGarbage" <<
                                        "compileString" << "continue" << "do" << "doFile" << "doMessage" << "doString" << "forward" <<
                                        "getSlot" << "getenv" << "hasSlot" << "isActive" << "isNil" << "isResumable" << "list" << "message" <<
                                        "method" << "parent" << "pause" << "perform" << "performWithArgList" << "print" << "proto" <<
                                        "raiseResumable" << "removeSlot" << "resend" << "resume" << "schedulerSleepSeconds" << "self" <<
                                        "sender" << "setSchedulerSleepSeconds" << "setSlot" << "shallowCopy" << "slotNames" << "super" <<
                                        "system" << "thisBlock" << "thisContext" << "thisMessage" << "type" << "uniqueId" << "updateSlot" <<
                                        "write" << "Array" << "AudioDevice" << "AudioMixer" << "Block" << "Box" << "Buffer" << "CFunction" << "CGI" <<
                                        "Color" << "Curses" << "DBM" << "DNSResolver" << "DOConnection" << "DOProxy" << "DOServer" <<
                                        "Date" << "Directory" << "Duration" << "DynLib" << "Error" << "Exception" << "FFT" << "File" <<
                                        "Fnmatch" << "Font" << "Future" << "GL" << "GLE" << "GLScissor" << "GLU" << "GLUCylinder" <<
                                        "GLUQuadric" << "GLUSphere" << "GLUT" << "Host" << "Image" << "Importer" << "LinkList" << "List" <<
                                        "Lobby" << "Locals" << "MD5" << "MP3Decoder" << "MP3Encoder" << "Map" << "Message" << "Movie" <<
                                        "NULL" << "Nil" << "Nop" << "Notifiction" << "Number" << "Object" << "OpenGL" << "Point" << "Protos" <<
                                        "Regex" << "SGMLTag" << "SQLite" << "Server" << "ShowMessage" << "SleepyCat" << "SleepyCatCursor" <<
                                        "Socket" << "SocketManager" << "Sound" << "Soup" << "Store" << "String" << "Tree" << "UDPSender" <<
                                        "UDPReceiver" << "URL" << "User" << "Warning" << "WeakLink";

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

        formatCommentaireUneLigne.setForeground(Qt::darkCyan);
            regle.modele = QRegExp("(//|\\#)[^\n]*");
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

void HighlighterIo::highlightBlock(const QString &texte)
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
