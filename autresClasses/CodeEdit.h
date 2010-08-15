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
#include "classesHighlighters/HighlighterAutoIT.h"
#include "classesHighlighters/HighlighterBF.h"
#include "classesHighlighters/HighlighterC.h"
#include "classesHighlighters/HighlighterCaml.h"
#include "classesHighlighters/HighlighterCPP.h"
#include "classesHighlighters/HighlighterCSharp.h"
#include "classesHighlighters/HighlighterCSS.h"
#include "classesHighlighters/HighlighterD.h"
#include "classesHighlighters/HighlighterErlang.h"
#include "classesHighlighters/HighlighterFSharp.h"
#include "classesHighlighters/HighlighterGLSL.h"
#include "classesHighlighters/HighlighterHaskell.h"
#include "classesHighlighters/HighlighterHTML.h"
#include "classesHighlighters/HighlighterINI.h"
#include "classesHighlighters/HighlighterIntercal.h"
#include "classesHighlighters/HighlighterJava.h"
#include "classesHighlighters/HighlighterLexYacc.h"
#include "classesHighlighters/HighlighterLisp.h"
#include "classesHighlighters/HighlighterPerl.h"
#include "classesHighlighters/HighlighterPHP.h"
#include "classesHighlighters/HighlighterPython.h"
#include "classesHighlighters/HighlighterRuby.h"
#include "classesHighlighters/HighlighterSQL.h"
#include "classesHighlighters/HighlighterActionScript.h"
#include "classesHighlighters/HighlighterASM.h"
#include "classesHighlighters/HighlighterBefunge.h"
#include "classesHighlighters/HighlighterFortran.h"
#include "classesHighlighters/HighlighterIo.h"
#include "classesHighlighters/HighlighterJavaScript.h"
#include "classesHighlighters/HighlighterLua.h"
#include "classesHighlighters/HighlighterMiranda.h"
#include "classesHighlighters/HighlighterPascal.h"

class CodeEdit : public QPlainTextEdit
{
	Q_OBJECT

	public:
		CodeEdit(QObject *parent);
		void dessinageAireLignes(QPaintEvent *event);
		int largeurAireLignes();

		void changeHighlighter(int highlighter);

		void indent();

	protected:
		void resizeEvent(QResizeEvent *event);
		void dragEnterEvent(QDragEnterEvent *event);
		void dropEvent(QDropEvent *event);
		void keyPressEvent(QKeyEvent *event);

	public:
		bool sauvegardeOk();
		bool undoOk();
		bool redoOk();

		QString fichierOuvert();
		bool estEnregistre();

		int colorisationCourante();

	public:
		void setSauvegardeOk(bool ok);
		void setUndoOk(bool ok);
		void setRedoOk(bool ok);

		void setFichierOuvert(QString fichier);
		void setEstEnregistre(bool ok);

		void setColorisation(int item);

	public slots:
		void MajLargeurAireLignes(int);
		void MajAireLignes(QRect rect, int dy);
		void surlignerLigneCourante();
		
	public slots:
		void codeChanged();

	signals:
		void ouvrirFichier(QString fichier);
		void newCode(QString newCode);

	private:
		bool m_sauvegardeOk;
		bool m_undoOk;
		bool m_redoOk;

		QString m_fichierOuvert;
		bool m_enregistre;

		int m_colorisation;

	private:
		QObject *pointeurSurParent;

		QWidget *aireLignes;

	private:
		HighlighterAutoIT *colorisationSyntaxiqueAutoIT;
		HighlighterBF *colorisationSyntaxiqueBF;
		HighlighterC *colorisationSyntaxiqueC;
		HighlighterCaml *colorisationSyntaxiqueCaml;
		HighlighterCPP *colorisationSyntaxiqueCPP;
		HighlighterCSharp *colorisationSyntaxiqueCSharp;
		HighlighterCSS *colorisationSyntaxiqueCSS;
		HighlighterD *colorisationSyntaxiqueD;
		HighlighterErlang *colorisationSyntaxiqueErlang;
		HighlighterFSharp *colorisationSyntaxiqueFSharp;
		HighlighterGLSL *colorisationSyntaxiqueGLSL;
		HighlighterHaskell *colorisationSyntaxiqueHaskell;
		HighlighterHTML *colorisationSyntaxiqueHTML;
		HighlighterINI *colorisationSyntaxiqueINI;
		HighlighterIntercal *colorisationSyntaxiqueIntercal;
		HighlighterJava *colorisationSyntaxiqueJava;
		HighlighterLexYacc *colorisationSyntaxiqueLexYacc;
		HighlighterLisp *colorisationSyntaxiqueLisp;
		HighlighterPerl *colorisationSyntaxiquePerl;
		HighlighterPHP *colorisationSyntaxiquePHP;
		HighlighterPython *colorisationSyntaxiquePython;
		HighlighterRuby *colorisationSyntaxiqueRuby;
		HighlighterSQL *colorisationSyntaxiqueSQL;
		HighlighterActionScript *colorisationSyntaxiqueActionScript;
		HighlighterASM *colorisationSyntaxiqueASM;
		HighlighterBefunge *colorisationSyntaxiqueBefunge;
		HighlighterFortran *colorisationSyntaxiqueFortran;
		HighlighterIO *colorisationSyntaxiqueIo;
		HighlighterJavaScript *colorisationSyntaxiqueJavaScript;
		HighlighterLua *colorisationSyntaxiqueLua;
		HighlighterMiranda *colorisationSyntaxiqueMiranda;
		HighlighterPascal *colorisationSyntaxiquePascal;
};

#endif
