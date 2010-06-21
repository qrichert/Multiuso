/*

Copyright © 2009-2010 Quentin RICHERT
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

#ifndef HEADER_HIGHLIGHTERHTML
#define HEADER_HIGHLIGHTERHTML

#include "../CurrentIncludes.h"
#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

class HighlighterHTML : public QSyntaxHighlighter
{
	Q_OBJECT

	public:
		HighlighterHTML(QTextDocument *parent);

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

		QTextCharFormat formatValeursHTML;
		QTextCharFormat formatDoctype;
		QTextCharFormat formatCommentaireMultiLigne;
		QTextCharFormat formatDeQuotation;
		QTextCharFormat formatBalises;
		QTextCharFormat formatAttributs;
		// <Shydow (www.siteduzero.com)>
			QTextCharFormat formatCData;
		// </Shydow (www.siteduzero.com)>
};

#endif
