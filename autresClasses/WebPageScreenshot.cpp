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

#include "WebPageScreenshot.h"

WebPageScreenshot::WebPageScreenshot()
{
	page = new QWebPage;

	connect(page, SIGNAL(loadFinished(bool)), this, SLOT(slotFinished(bool)));
}

void WebPageScreenshot::slotFinished(bool finished)
{
	if (!finished)
		emit screenshotNotTaken();

	else
		takeScreenshotOf(page);
}

void WebPageScreenshot::takeScreenshotOf(QString url)
{
	page->mainFrame()->load(QUrl(url));	
}

void WebPageScreenshot::takeScreenshotOf(QWebView *webView)
{
	takeScreenshotOf(webView->page());
}

void WebPageScreenshot::takeScreenshotOf(QWebPage *webPage)
{
	QSize pageSize = webPage->viewportSize();

	webPage->setViewportSize(webPage->mainFrame()->contentsSize()); // On redimensionne la taille de la page à celle du contenu pour éviter les barres de défilement.
	
	QPixmap pixmap(webPage->viewportSize()); // On crée une image de la même taille que la page.
	
	QPainter painter;
		painter.begin(&pixmap); // On "démarre" le painter en lui donnant comme surface de dessin la nouvelle image.
			webPage->mainFrame()->render(&painter); // On dit au painter de dessiner le cadre principal de la page.
		painter.end(); // On "arrête" le painter.

	webPage->setViewportSize(pageSize); // To keep the same size as before.
		
	emit screenshotTaken(webPage->mainFrame()->title(), webPage->mainFrame()->url().toString(), pixmap);
}
