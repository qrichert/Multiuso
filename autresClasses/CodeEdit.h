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

#ifndef HEADER_CODEEDIT
#define HEADER_CODEEDIT

#include "../CurrentIncludes.h"
#include <QResizeEvent>
#include <QPaintEvent>
#include <QDropEvent>
#include <QKeyEvent>
#include "classesHighlighters/HighlighterActionScript.h"
#include "classesHighlighters/HighlighterASM.h"
#include "classesHighlighters/HighlighterAutoIT.h"
#include "classesHighlighters/HighlighterBefunge.h"
#include "classesHighlighters/HighlighterBF.h"
#include "classesHighlighters/HighlighterC.h"
#include "classesHighlighters/HighlighterCaml.h"
#include "classesHighlighters/HighlighterCPP.h"
#include "classesHighlighters/HighlighterCSharp.h"
#include "classesHighlighters/HighlighterCSS.h"
#include "classesHighlighters/HighlighterD.h"
#include "classesHighlighters/HighlighterErlang.h"
#include "classesHighlighters/HighlighterFortran.h"
#include "classesHighlighters/HighlighterFSharp.h"
#include "classesHighlighters/HighlighterGLSL.h"
#include "classesHighlighters/HighlighterHaskell.h"
#include "classesHighlighters/HighlighterHTML.h"
#include "classesHighlighters/HighlighterINI.h"
#include "classesHighlighters/HighlighterIntercal.h"
#include "classesHighlighters/HighlighterIo.h"
#include "classesHighlighters/HighlighterJava.h"
#include "classesHighlighters/HighlighterJavaScript.h"
#include "classesHighlighters/HighlighterLexYacc.h"
#include "classesHighlighters/HighlighterLisp.h"
#include "classesHighlighters/HighlighterLua.h"
#include "classesHighlighters/HighlighterMiranda.h"
#include "classesHighlighters/HighlighterPascal.h"
#include "classesHighlighters/HighlighterPerl.h"
#include "classesHighlighters/HighlighterPHP.h"
#include "classesHighlighters/HighlighterPython.h"
#include "classesHighlighters/HighlighterRuby.h"
#include "classesHighlighters/HighlighterSQL.h"

class CodeEdit : public QPlainTextEdit
{
	Q_OBJECT

	public:
		CodeEdit();
		
		void initializeHighlighters();

		void setSavable(bool savable);
		bool isSavable();

		void setCurrentFileName(QString fileName);
		QString currentFileName();
		
		void setHighlighter(QString highlighter);
		QString getHighlighter();

	protected:
		void keyPressEvent(QKeyEvent *event);
		void dragEnterEvent(QDragEnterEvent *event);
		void dropEvent(QDropEvent *event);

	signals:
		void openFileRequested(QString file);

	private:
		bool m_isSavable;
		QString m_fileName;
		QString m_highlighter;

		HighlighterActionScript *highlighterActionScript;
		HighlighterASM *highlighterASM;
		HighlighterAutoIT *highlighterAutoIT;
		HighlighterBefunge *highlighterBefunge;
		HighlighterBF *highlighterBF;
		HighlighterC *highlighterC;
		HighlighterCaml *highlighterCaml;
		HighlighterCPP *highlighterCPP;
		HighlighterCSharp *highlighterCSharp;
		HighlighterCSS *highlighterCSS;
		HighlighterD *highlighterD;
		HighlighterErlang *highlighterErlang;
		HighlighterFortran *highlighterFortran;
		HighlighterFSharp *highlighterFSharp;
		HighlighterGLSL *highlighterGLSL;
		HighlighterHaskell *highlighterHaskell;
		HighlighterHTML *highlighterHTML;
		HighlighterINI *highlighterINI;
		HighlighterIntercal *highlighterIntercal;
		HighlighterIo *highlighterIo;
		HighlighterJava *highlighterJava;
		HighlighterJavaScript *highlighterJavaScript;
		HighlighterLexYacc *highlighterLexYacc;
		HighlighterLisp *highlighterLisp;
		HighlighterLua *highlighterLua;
		HighlighterMiranda *highlighterMiranda;
		HighlighterPascal *highlighterPascal;
		HighlighterPerl *highlighterPerl;
		HighlighterPHP *highlighterPHP;
		HighlighterPython *highlighterPython;
		HighlighterRuby *highlighterRuby;
		HighlighterSQL *highlighterSQL;
};

#endif
