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
#include "autresClasses/AireAffichageLignes.h"
#include "classesPrincipales/EditeurDeCode.h"

CodeEdit::CodeEdit(QObject *parent = 0)
{
	pointeurSurParent = parent;

	aireLignes = new AireAffichageLignes(this);

	m_sauvegardeOk = false;
	m_undoOk = false;
	m_redoOk = false;

	m_fichierOuvert = "";

	m_enregistre = true;

	m_colorisation = 0;

	setAcceptDrops(true);
	setLineWrapMode(QPlainTextEdit::NoWrap);
	setTabStopWidth(40);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(MajLargeurAireLignes(int)));
	connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(MajAireLignes(QRect, int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(surlignerLigneCourante()));

	MajLargeurAireLignes(0);

	connect(this, SIGNAL(textChanged()), this, SLOT(codeChanged()));

	colorisationSyntaxiqueAutoIT = new HighlighterAutoIT(new QTextDocument);
	colorisationSyntaxiqueBF = new HighlighterBF(new QTextDocument);
	colorisationSyntaxiqueC = new HighlighterC(new QTextDocument);
	colorisationSyntaxiqueCaml = new HighlighterCaml(new QTextDocument);
	colorisationSyntaxiqueCPP = new HighlighterCPP(new QTextDocument);
	colorisationSyntaxiqueCSharp = new HighlighterCSharp(new QTextDocument);
	colorisationSyntaxiqueCSS = new HighlighterCSS(new QTextDocument);
	colorisationSyntaxiqueD = new HighlighterD(new QTextDocument);
	colorisationSyntaxiqueErlang = new HighlighterErlang(new QTextDocument);
	colorisationSyntaxiqueFSharp = new HighlighterFSharp(new QTextDocument);
	colorisationSyntaxiqueGLSL = new HighlighterGLSL(new QTextDocument);
	colorisationSyntaxiqueHaskell = new HighlighterHaskell(new QTextDocument);
	colorisationSyntaxiqueHTML = new HighlighterHTML(new QTextDocument);
	colorisationSyntaxiqueINI = new HighlighterINI(new QTextDocument);
	colorisationSyntaxiqueIntercal = new HighlighterIntercal(new QTextDocument);
	colorisationSyntaxiqueJava = new HighlighterJava(new QTextDocument);
	colorisationSyntaxiqueLexYacc = new HighlighterLexYacc(new QTextDocument);
	colorisationSyntaxiqueLisp = new HighlighterLisp(new QTextDocument);
	colorisationSyntaxiquePerl = new HighlighterPerl(new QTextDocument);
	colorisationSyntaxiquePHP = new HighlighterPHP(new QTextDocument);
	colorisationSyntaxiquePython = new HighlighterPython(new QTextDocument);
	colorisationSyntaxiqueRuby = new HighlighterRuby(new QTextDocument);
	colorisationSyntaxiqueSQL = new HighlighterSQL(new QTextDocument);
	colorisationSyntaxiqueActionScript = new HighlighterActionScript(new QTextDocument);
	colorisationSyntaxiqueASM = new HighlighterASM(new QTextDocument);
	colorisationSyntaxiqueBefunge = new HighlighterBefunge(new QTextDocument);
	colorisationSyntaxiqueFortran = new HighlighterFortran(new QTextDocument);
	colorisationSyntaxiqueIo = new HighlighterIO(new QTextDocument);
	colorisationSyntaxiqueJavaScript = new HighlighterJavaScript(new QTextDocument);
	colorisationSyntaxiqueLua = new HighlighterLua(new QTextDocument);
	colorisationSyntaxiqueMiranda = new HighlighterMiranda(new QTextDocument);
	colorisationSyntaxiquePascal = new HighlighterPascal(new QTextDocument);
}

void CodeEdit::dessinageAireLignes(QPaintEvent *event)
{
	QPainter painter(aireLignes);
		painter.fillRect(event->rect(), QColor(Qt::lightGray).lighter(120));

	QTextBlock block = firstVisibleBlock();

	int numeroBlock = block.blockNumber();
	int haut = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bas = haut + (int) blockBoundingRect(block).height();

	while (block.isValid() && haut <= event->rect().bottom())
	{
		if (block.isVisible() && bas >= event->rect().top())
		{
			QString numero = QString::number(numeroBlock + 1);

			painter.setPen(Qt::black);
			painter.drawText(-1, haut, aireLignes->width() -1, fontMetrics().height(), Qt::AlignRight, numero);

			block = block.next();
			haut = bas;
			bas = haut + (int) blockBoundingRect(block).height();

			numeroBlock++;
		}
	}
}

int CodeEdit::largeurAireLignes()
{
	int chiffres = 1;
	int max = qMax(1, blockCount());

	while (max >= 10)
	{
		max /= 10;
		chiffres++;
	}

	int espace = 7 + fontMetrics().width(QLatin1Char('9')) * chiffres;

	return espace;
}

void CodeEdit::MajLargeurAireLignes(int)
{
	setViewportMargins(largeurAireLignes(), 0, 0, 0);
}

void CodeEdit::MajAireLignes(QRect rect, int dy)
{
	if (dy)
		aireLignes->scroll(0, dy);

	if (rect.contains(viewport()->rect()))
		MajLargeurAireLignes(0);
}

void CodeEdit::surlignerLigneCourante()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly())
	{
		QTextEdit::ExtraSelection selection;

		QColor couleurLigne = QColor(Qt::lightGray).lighter(120);

		selection.format.setBackground(couleurLigne);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}

void CodeEdit::resizeEvent(QResizeEvent *event)
{
	QPlainTextEdit::resizeEvent(event);

	QRect cr = contentsRect();

	aireLignes->setGeometry(QRect(cr.left(), cr.top(), largeurAireLignes(), cr.height()));
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
			QString url = urls.at(i).path();

			if (Multiuso::currentOS() == "windows")
				url = url.right(url.length() - 1);

			emit ouvrirFichier(url);
		}
	}
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

void CodeEdit::codeChanged()
{
	emit newCode(toPlainText());
}

void CodeEdit::changeHighlighter(int highlighter)
{
	EditeurDeCode *codeEditor = qobject_cast<EditeurDeCode *>(pointeurSurParent);

	colorisationSyntaxiqueAutoIT->setDocument(new QTextDocument);
	colorisationSyntaxiqueBF->setDocument(new QTextDocument);
	colorisationSyntaxiqueC->setDocument(new QTextDocument);
	colorisationSyntaxiqueCaml->setDocument(new QTextDocument);
	colorisationSyntaxiqueCPP->setDocument(new QTextDocument);
	colorisationSyntaxiqueCSharp->setDocument(new QTextDocument);
	colorisationSyntaxiqueCSS->setDocument(new QTextDocument);
	colorisationSyntaxiqueD->setDocument(new QTextDocument);
	colorisationSyntaxiqueErlang->setDocument(new QTextDocument);
	colorisationSyntaxiqueFSharp->setDocument(new QTextDocument);
	colorisationSyntaxiqueGLSL->setDocument(new QTextDocument);
	colorisationSyntaxiqueHaskell->setDocument(new QTextDocument);
	colorisationSyntaxiqueHTML->setDocument(new QTextDocument);
	colorisationSyntaxiqueINI->setDocument(new QTextDocument);
	colorisationSyntaxiqueIntercal->setDocument(new QTextDocument);
	colorisationSyntaxiqueJava->setDocument(new QTextDocument);
	colorisationSyntaxiqueLexYacc->setDocument(new QTextDocument);
	colorisationSyntaxiqueLisp->setDocument(new QTextDocument);
	colorisationSyntaxiquePerl->setDocument(new QTextDocument);
	colorisationSyntaxiquePHP->setDocument(new QTextDocument);
	colorisationSyntaxiquePython->setDocument(new QTextDocument);
	colorisationSyntaxiqueRuby->setDocument(new QTextDocument);
	colorisationSyntaxiqueSQL->setDocument(new QTextDocument);
	colorisationSyntaxiqueActionScript->setDocument(new QTextDocument);
	colorisationSyntaxiqueASM->setDocument(new QTextDocument);
	colorisationSyntaxiqueBefunge->setDocument(new QTextDocument);
	colorisationSyntaxiqueFortran->setDocument(new QTextDocument);
	colorisationSyntaxiqueIo->setDocument(new QTextDocument);
	colorisationSyntaxiqueJavaScript->setDocument(new QTextDocument);
	colorisationSyntaxiqueLua->setDocument(new QTextDocument);
	colorisationSyntaxiqueMiranda->setDocument(new QTextDocument);
	colorisationSyntaxiquePascal->setDocument(new QTextDocument);

	if (highlighter == codeEditor->highlighterIndex("AutoIT"))
		colorisationSyntaxiqueAutoIT->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Brainfuck"))
		colorisationSyntaxiqueBF->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("C"))
		colorisationSyntaxiqueC->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Caml"))
		colorisationSyntaxiqueCaml->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("C++"))
		colorisationSyntaxiqueCPP->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("C#"))
		colorisationSyntaxiqueCSharp->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("CSS"))
		colorisationSyntaxiqueCSS->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("D"))
		colorisationSyntaxiqueD->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Erlang"))
		colorisationSyntaxiqueErlang->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("F#"))
		colorisationSyntaxiqueFSharp->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("GLSL"))
		colorisationSyntaxiqueGLSL->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Haskell"))
		colorisationSyntaxiqueHaskell->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("HTML / (x)HTML / XML"))
		colorisationSyntaxiqueHTML->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("INI"))
		colorisationSyntaxiqueINI->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Intercal"))
		colorisationSyntaxiqueIntercal->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Java"))
		colorisationSyntaxiqueJava->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Lex et Yacc"))
		colorisationSyntaxiqueLexYacc->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Lisp"))
		colorisationSyntaxiqueLisp->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Perl"))
		colorisationSyntaxiquePerl->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("PHP"))
		colorisationSyntaxiquePHP->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Python"))
		colorisationSyntaxiquePython->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Ruby"))
		colorisationSyntaxiqueRuby->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("SQL"))
		colorisationSyntaxiqueSQL->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("ActionScript"))
		colorisationSyntaxiqueActionScript->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("ASM"))
		colorisationSyntaxiqueASM->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Befunge"))
		colorisationSyntaxiqueBefunge->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Fortran"))
		colorisationSyntaxiqueFortran->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Io"))
		colorisationSyntaxiqueIo->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("JavaScript"))
		colorisationSyntaxiqueJavaScript->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Lua"))
		colorisationSyntaxiqueLua->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Miranda"))
		colorisationSyntaxiqueMiranda->setDocument(document());

	else if (highlighter == codeEditor->highlighterIndex("Pascal"))
		colorisationSyntaxiquePascal->setDocument(document());
}

bool CodeEdit::sauvegardeOk()
{
	return m_sauvegardeOk;
}

bool CodeEdit::undoOk()
{
	return m_undoOk;
}

bool CodeEdit::redoOk()
{
	return m_redoOk;
}

QString CodeEdit::fichierOuvert()
{
	return m_fichierOuvert;
}

bool CodeEdit::estEnregistre()
{
	return m_enregistre;
}

int CodeEdit::colorisationCourante()
{
	return m_colorisation;
}

void CodeEdit::setSauvegardeOk(bool ok)
{
	m_sauvegardeOk = ok;
}

void CodeEdit::setUndoOk(bool ok)
{
	m_undoOk = ok;
}

void CodeEdit::setRedoOk(bool ok)
{
	m_redoOk = ok;
}


void CodeEdit::setFichierOuvert(QString fichier)
{
	m_fichierOuvert = fichier;
}

void CodeEdit::setEstEnregistre(bool ok)
{
	m_enregistre = ok;
}

void CodeEdit::setColorisation(int item)
{
	m_colorisation = item;
}

