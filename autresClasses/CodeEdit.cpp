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
	
	initializeHighlighters();
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
	
	highlighterActionScript->setParent(NULL);
	highlighterASM->setParent(NULL);
	highlighterAutoIT->setParent(NULL);
	highlighterBefunge->setParent(NULL);
	highlighterBF->setParent(NULL);
	highlighterC->setParent(NULL);
	highlighterCaml->setParent(NULL);
	highlighterCPP->setParent(NULL);
	highlighterCSharp->setParent(NULL);
	highlighterCSS->setParent(NULL);
	highlighterD->setParent(NULL);
	highlighterErlang->setParent(NULL);
	highlighterFortran->setParent(NULL);
	highlighterFSharp->setParent(NULL);
	highlighterGLSL->setParent(NULL);
	highlighterHaskell->setParent(NULL);
	highlighterHTML->setParent(NULL);
	highlighterINI->setParent(NULL);
	highlighterIntercal->setParent(NULL);
	highlighterIo->setParent(NULL);
	highlighterJava->setParent(NULL);
	highlighterJavaScript->setParent(NULL);
	highlighterLexYacc->setParent(NULL);
	highlighterLisp->setParent(NULL);
	highlighterLua->setParent(NULL);
	highlighterMiranda->setParent(NULL);
	highlighterPascal->setParent(NULL);
	highlighterPerl->setParent(NULL);
	highlighterPHP->setParent(NULL);
	highlighterPython->setParent(NULL);
	highlighterRuby->setParent(NULL);
	highlighterSQL->setParent(NULL);
	
	if (highlighter == "ActionScript")
		highlighterActionScript->setParent(document());

	else if (highlighter == "ASM")
		highlighterASM->setParent(document());

	else if (highlighter == "AutoIT")
		highlighterAutoIT->setParent(document());

	else if (highlighter == "Befunge")
		highlighterBefunge->setParent(document());

	else if (highlighter == "BrainFuck")
		highlighterBF->setParent(document());

	else if (highlighter == "C")
		highlighterC->setParent(document());

	else if (highlighter == "Caml")
		highlighterCaml->setParent(document());

	else if (highlighter == "C++")
		highlighterCPP->setParent(document());

	else if (highlighter == "C#")
		highlighterCSharp->setParent(document());

	else if (highlighter == "CSS")
		highlighterCSS->setParent(document());

	else if (highlighter == "D")
		highlighterD->setParent(document());

	else if (highlighter == "Erlang")
		highlighterErlang->setParent(document());

	else if (highlighter == "Fortran")
		highlighterFortran->setParent(document());

	else if (highlighter == "F#")
		highlighterFSharp->setParent(document());

	else if (highlighter == "GLSL")
		highlighterGLSL->setParent(document());

	else if (highlighter == "Haskell")
		highlighterHaskell->setParent(document());

	else if (highlighter == "HTML / (x)HTML / XML")
		highlighterHTML->setParent(document());

	else if (highlighter == "INI")
		highlighterINI->setParent(document());

	else if (highlighter == "Intercal")
		highlighterIntercal->setParent(document());

	else if (highlighter == "Io")
		highlighterIo->setParent(document());

	else if (highlighter == "Java")
		highlighterJava->setParent(document());

	else if (highlighter == "JavaScript")
		highlighterJavaScript->setParent(document());

	else if (highlighter == "Lex et Yacc")
		highlighterLexYacc->setParent(document());

	else if (highlighter == "Lisp")
		highlighterLisp->setParent(document());

	else if (highlighter == "Lua")
		highlighterLua->setParent(document());

	else if (highlighter == "Miranda")
		highlighterMiranda->setParent(document());

	else if (highlighter == "Pascal")
		highlighterPascal->setParent(document());

	else if (highlighter == "Perl")
		highlighterPerl->setParent(document());

	else if (highlighter == "PHP")
		highlighterPHP->setParent(document());

	else if (highlighter == "Python")
		highlighterPython->setParent(document());

	else if (highlighter == "Ruby")
		highlighterRuby->setParent(document());

	else if (highlighter == "SQL")
		highlighterSQL->setParent(document());
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
