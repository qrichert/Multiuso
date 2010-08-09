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

#include "TextEdit.h"

TextEdit::TextEdit()
{	
	setAcceptDrops(true);
	
	setTabStopWidth(40);

	m_isSavable = false;
}

void TextEdit::setSavable(bool savable)
{
	m_isSavable = savable;
}

bool TextEdit::isSavable()
{
	return m_isSavable;
}

void TextEdit::setCurrentFileName(QString fileName)
{
	m_fileName = fileName;
}

QString TextEdit::currentFileName()
{
	return m_fileName;
}

void TextEdit::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void TextEdit::dropEvent(QDropEvent *event)
{
	const QMimeData *data = event->mimeData();

	if (data->hasUrls())
	{
		QList<QUrl> urls = data->urls();

		for (int i = 0; i < urls.size(); i ++)
		{
			QCoreApplication::processEvents();

			QString url = urls.at(i).path();

			if (Multiuso::currentOS() == "windows")
				url = url.right(url.length() - 1);

			emit openFileRequested(url);
		}
	}
}
