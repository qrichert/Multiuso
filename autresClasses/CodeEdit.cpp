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
	
	setWordWrapMode(QTextOption::NoWrap);

	initializeHighlighters();

	lineNumberArea = new LineNumberArea(this);
		connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
		connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
		connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
	
	updateLineNumberAreaWidth(0);
	
	highlightCurrentLine();
}

void CodeEdit::initializeHighlighters()
{
	highlighterActionScript = new HighlighterActionScript(NULL);
	highlighterASM = new HighlighterASM(NULL);
	highlighterAutoIT = new HighlighterAutoIT(NULL);
	highlighterBefunge = new HighlighterBefunge(NULL);
	highlighterBF = new HighlighterBF(NULL);
	highlighterC = new HighlighterC(NULL);
	highlighterCaml = new HighlighterCaml(NULL);
	highlighterCPP = new HighlighterCPP(NULL);
	highlighterCSharp = new HighlighterCSharp(NULL);
	highlighterCSS = new HighlighterCSS(NULL);
	highlighterD = new HighlighterD(NULL);
	highlighterErlang = new HighlighterErlang(NULL);
	highlighterFortran = new HighlighterFortran(NULL);
	highlighterFSharp = new HighlighterFSharp(NULL);
	highlighterGLSL = new HighlighterGLSL(NULL);
	highlighterHaskell = new HighlighterHaskell(NULL);
	highlighterHTML = new HighlighterHTML(NULL);
	highlighterINI = new HighlighterINI(NULL);
	highlighterIntercal = new HighlighterIntercal(NULL);
	highlighterIo = new HighlighterIo(NULL);
	highlighterJava = new HighlighterJava(NULL);
	highlighterJavaScript = new HighlighterJavaScript(NULL);
	highlighterLexYacc = new HighlighterLexYacc(NULL);
	highlighterLisp = new HighlighterLisp(NULL);
	highlighterLua = new HighlighterLua(NULL);
	highlighterMiranda = new HighlighterMiranda(NULL);
	highlighterPascal = new HighlighterPascal(NULL);
	highlighterPerl = new HighlighterPerl(NULL);
	highlighterPHP = new HighlighterPHP(NULL);
	highlighterPython = new HighlighterPython(NULL);
	highlighterRuby = new HighlighterRuby(NULL);
	highlighterSQL = new HighlighterSQL(NULL);
}

void CodeEdit::keyPressEvent(QKeyEvent *event)
{
	QPlainTextEdit::keyPressEvent(event);

	QSettings settings(Multiuso::appDirPath() + "/ini/editeur_de_code.ini", QSettings::IniFormat);

	if (!settings.value("saisie/indentation_automatique").toBool())
		return;

	switch (event->key())
	{
		case Qt::Key_Return:
		case Qt::Key_Enter:

			QString text = toPlainText().left(textCursor().position());
				text = text.replace(QRegExp("^(.+)\n$"), "\\1");
				text = text.replace(QRegExp("^(.+)\n([^\n]*)$"), "\\2");

			while (text.startsWith("\t"))
			{
				text = text.right(text.length() - 1);
				textCursor().insertText("\t");
			}

		break;
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

void CodeEdit::setHighlighter(QString highlighter)
{
	m_highlighter = highlighter;
	
	highlighterActionScript->setDocument(NULL);
	highlighterASM->setDocument(NULL);
	highlighterAutoIT->setDocument(NULL);
	highlighterBefunge->setDocument(NULL);
	highlighterBF->setDocument(NULL);
	highlighterC->setDocument(NULL);
	highlighterCaml->setDocument(NULL);
	highlighterCPP->setDocument(NULL);
	highlighterCSharp->setDocument(NULL);
	highlighterCSS->setDocument(NULL);
	highlighterD->setDocument(NULL);
	highlighterErlang->setDocument(NULL);
	highlighterFortran->setDocument(NULL);
	highlighterFSharp->setDocument(NULL);
	highlighterGLSL->setDocument(NULL);
	highlighterHaskell->setDocument(NULL);
	highlighterHTML->setDocument(NULL);
	highlighterINI->setDocument(NULL);
	highlighterIntercal->setDocument(NULL);
	highlighterIo->setDocument(NULL);
	highlighterJava->setDocument(NULL);
	highlighterJavaScript->setDocument(NULL);
	highlighterLexYacc->setDocument(NULL);
	highlighterLisp->setDocument(NULL);
	highlighterLua->setDocument(NULL);
	highlighterMiranda->setDocument(NULL);
	highlighterPascal->setDocument(NULL);
	highlighterPerl->setDocument(NULL);
	highlighterPHP->setDocument(NULL);
	highlighterPython->setDocument(NULL);
	highlighterRuby->setDocument(NULL);
	highlighterSQL->setDocument(NULL);
	
	if (highlighter == "ActionScript")
		highlighterActionScript->setDocument(document());

	else if (highlighter == "ASM")
		highlighterASM->setDocument(document());

	else if (highlighter == "AutoIT")
		highlighterAutoIT->setDocument(document());

	else if (highlighter == "Befunge")
		highlighterBefunge->setDocument(document());

	else if (highlighter == "BrainFuck")
		highlighterBF->setDocument(document());

	else if (highlighter == "C")
		highlighterC->setDocument(document());

	else if (highlighter == "Caml")
		highlighterCaml->setDocument(document());

	else if (highlighter == "C++")
		highlighterCPP->setDocument(document());

	else if (highlighter == "C#")
		highlighterCSharp->setDocument(document());

	else if (highlighter == "CSS")
		highlighterCSS->setDocument(document());

	else if (highlighter == "D")
		highlighterD->setDocument(document());

	else if (highlighter == "Erlang")
		highlighterErlang->setDocument(document());

	else if (highlighter == "Fortran")
		highlighterFortran->setDocument(document());

	else if (highlighter == "F#")
		highlighterFSharp->setDocument(document());

	else if (highlighter == "GLSL")
		highlighterGLSL->setDocument(document());

	else if (highlighter == "Haskell")
		highlighterHaskell->setDocument(document());

	else if (highlighter == "HTML / (x)HTML / XML")
		highlighterHTML->setDocument(document());

	else if (highlighter == "INI")
		highlighterINI->setDocument(document());

	else if (highlighter == "Intercal")
		highlighterIntercal->setDocument(document());

	else if (highlighter == "Io")
		highlighterIo->setDocument(document());

	else if (highlighter == "Java")
		highlighterJava->setDocument(document());

	else if (highlighter == "JavaScript")
		highlighterJavaScript->setDocument(document());

	else if (highlighter == "Lex et Yacc")
		highlighterLexYacc->setDocument(document());

	else if (highlighter == "Lisp")
		highlighterLisp->setDocument(document());

	else if (highlighter == "Lua")
		highlighterLua->setDocument(document());

	else if (highlighter == "Miranda")
		highlighterMiranda->setDocument(document());

	else if (highlighter == "Pascal")
		highlighterPascal->setDocument(document());

	else if (highlighter == "Perl")
		highlighterPerl->setDocument(document());

	else if (highlighter == "PHP")
		highlighterPHP->setDocument(document());

	else if (highlighter == "Python")
		highlighterPython->setDocument(document());

	else if (highlighter == "Ruby")
		highlighterRuby->setDocument(document());

	else if (highlighter == "SQL")
		highlighterSQL->setDocument(document());
}

QString CodeEdit::getHighlighter()
{
	return m_highlighter;
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

void CodeEdit::resizeEvent(QResizeEvent *event)
{
	QPlainTextEdit::resizeEvent(event);

	QRect cr = contentsRect();
	lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
	QPainter painter(lineNumberArea);
		painter.fillRect(event->rect(), QColor(Qt::lightGray).lighter(120));

	QTextBlock block = firstVisibleBlock();

	int blockNumber = block.blockNumber();

	int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int) blockBoundingRect(block).height();

	while (block.isValid() && top <= event->rect().bottom())
	{
		if (block.isVisible() && bottom >= event->rect().top())
		{
			QString number = QString::number(blockNumber + 1);

			painter.setPen(Qt::black);
			painter.drawText(-1, top, lineNumberArea->width() - 1, fontMetrics().height(),
					Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + (int) blockBoundingRect(block).height();
		blockNumber++;
	}
}

int CodeEdit::lineNumberAreaWidth()
{
	int digits = 1;

	int max = qMax(blockCount(), 1);

	while (max >= 10)
	{
		max /= 10;
		digits++;
	}

	return 7 + fontMetrics().width(QLatin1Char('9')) * digits;
}

void CodeEdit::updateLineNumberAreaWidth(int)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEdit::highlightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly())
	{
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(Qt::lightGray).lighter(120);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();

		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}

void CodeEdit::updateLineNumberArea(QRect rect, int dy)
{
	if (dy)
		lineNumberArea->scroll(0, dy);

	else
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

	if (rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}
