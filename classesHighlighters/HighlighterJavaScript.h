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

#ifndef HEADER_HIGHLIGHTERJAVASCRIPT
#define HEADER_HIGHLIGHTERJAVASCRIPT

#include "../CurrentIncludes.h"
#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

class HighlighterJavaScript : public QSyntaxHighlighter
{
        Q_OBJECT

        public:
                HighlighterJavaScript(QTextDocument *parent);

        protected:
                void highlightBlock(const QString &texte);

        private:
                struct RegleDeSurlignage
                {
                        QRegExp modele;
                        QTextCharFormat format;
                };

                QVector<RegleDeSurlignage> reglesDeSurlignage;

                QRegExp debutCommentaire;
                QRegExp finCommentaire;

                QTextCharFormat formatMotsCles;
                QTextCharFormat formatNombres;
                QTextCharFormat formatDeQuotationSimple;
                QTextCharFormat formatDeQuotationDouble;
                QTextCharFormat formatCommentairesUneLigne;
                QTextCharFormat formatCommentairesMultiLigne;
};

#endif
