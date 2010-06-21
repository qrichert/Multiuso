/*

Copyright © 2009-2010 Quentin RICHERT

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

#ifndef HEADER_WEBPAGESCREENSHOT
#define HEADER_WEBPAGESCREENSHOT

#include "../CurrentIncludes.h"
#include <QtWebKit>

class WebPageScreenshot : public QObject
{
	Q_OBJECT

	public:
		WebPageScreenshot();
		
		void takeScreenshotOf(QString url);
		void takeScreenshotOf(QWebView *webView);
		void takeScreenshotOf(QWebPage *webPage);
		
	public slots:
		void slotFinished(bool finished);
		
	signals:
		void screenshotTaken(QString title, QString url, QPixmap pixmap);
		void screenshotNotTaken();

	private:
		QWebPage *page;
};

#endif
