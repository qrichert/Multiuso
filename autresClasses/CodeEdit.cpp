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

#include "CodeEdit.h"
#include "classesPrincipales/EditeurDeCode.h"

CodeEdit::CodeEdit()
{	
	setAcceptDrops(true);
	
	setTabStopWidth(40);

	m_isSavable = false;
}

void CodeEdit::keyPressEvent(QKeyEvent *event)
{
	QPlainTextEdit::keyPressEvent(event);

	if (!QSettings(Multiuso::appDirPath() + "/ini/editeur_de_code.ini", QSettings::IniFormat).value("saisie/indentation_automatique").toBool())
		return;

	switch (event->key())
	{
		case Qt::Key_Return:
		case Qt::Key_Enter:
			indent();
		break;
	}
}

void CodeEdit::indent()
{
	QString text = toPlainText().left(textCursor().position());
		text = text.replace(QRegExp("(.+)\n"), "\\1");
		text = text.replace(QRegExp("(.+)\n([^\n]*)"), "\\2");

	while (text.startsWith("\t"))
	{
		text = text.right(text.length() - 1);
		textCursor().insertText("\t");
	}
}

void CodeEdit::setSavable(bool savable)
{
	m_isSavable = savable;
}

bool CodeEdit::isSavable()
{
	return m_isSavable;
}

void CodeEdit::setCurrentFileName(QString fileName)
{
	m_fileName = fileName;
}

QString CodeEdit::currentFileName()
{
	return m_fileName;
}

void CodeEdit::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void CodeEdit::dropEvent(QDropEvent *event)
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
