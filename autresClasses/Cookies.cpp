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

#include "Cookies.h"

QList<QNetworkCookie> Cookies::cookiesForUrl (const QUrl &url) const
{
	return QNetworkCookieJar::cookiesForUrl(url);
}

bool Cookies::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url)
{
	return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
}

Cookies::Cookies(QObject *parent) : QNetworkCookieJar(parent)
{
	QFile cookieFile(Multiuso::appDirPath() + "/navigateurWeb/autre/cookies.mltscookie");

	if (cookieFile.exists() && cookieFile.open(QIODevice::ReadOnly))
	{
		QList<QNetworkCookie> list;

		QByteArray line;

		while (!(line = cookieFile.readLine()).isNull())
			list.append(QNetworkCookie::parseCookies(line));

		setAllCookies(list);
	}
}

Cookies::~Cookies()
{
	QFile cookieFile(Multiuso::appDirPath() + "/navigateurWeb/autre/cookies.mltscookie");

	if (cookieFile.open(QIODevice::WriteOnly))
	{
		QTextStream out(&cookieFile);

		foreach(const QNetworkCookie &cookie, allCookies())
		{
			if (!cookie.isSessionCookie())
				out << cookie.toRawForm() << endl;
		}
	}
}
