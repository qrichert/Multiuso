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

#ifndef HEADER_EDITEUR_DE_TEXTE
#define HEADER_EDITEUR_DE_TEXTE

#include "../CurrentIncludes.h"
#include "autresClasses/TextEdit.h"

class EditeurDeTexte : public QMainWindow
{
	Q_OBJECT

	public:
		EditeurDeTexte(QWidget *parent);

		QToolBar *createFirstToolBar();
		QToolBar *createSecondToolBar();
		
		QIcon createColorIcon(QColor color);
		QIcon createBackgroundColorIcon(QColor color);

		TextEdit *currentTextEdit();
		TextEdit *textEditAt(int index);

		bool isEverythingSaved();
		void sauvegarderEtat();

	public slots:
		void closeFile(int index);
		void newFile();
		void openFile();
		void openFile(QString file);
		void saveFile();
		void saveFile(QString file);
		void saveFileAs();
		void selectColor();
		void selectBackgroundColor();

		void currentCharFormatChanged(QTextCharFormat format);
		void cursorPositionChanged();

		void fontChanged(QFont font);
		void colorChanged(QColor color);
		void backgroundColorChanged(QColor color);
		void alignmentChanged(Qt::Alignment alignment);

	private:
		QTabWidget *tabWidget;

		QAction *a_new;
		QAction *a_open;
		QAction *a_save;
		QAction *a_saveAs;
		QAction *a_undo;
		QAction *a_redo;
		QAction *a_delete;
		QAction *a_search;
		QAction *a_replace;
		QAction *a_printPreview;
		QAction *a_print;
		QAction *a_copy;
		QAction *a_cut;
		QAction *a_paste;
		QAction *a_selectAll;
		QAction *a_insertImage;
		QAction *a_repeatText;

		QAction *a_bold;
		QAction *a_italic;
		QAction *a_underline;
		QAction *a_alignLeft;
		QAction *a_alignCenter;
		QAction *a_alignRight;
		QAction *a_alignJustify;
		QComboBox *a_fontSize;
		QFontComboBox *a_font;
		QAction *a_selectColor;
		QAction *a_selectBackgroundColor;
		QAction *a_toUpper;
		QAction *a_toLower;
};

#endif
