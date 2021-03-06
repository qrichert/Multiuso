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

class RecentTextFiles : public QObject
{
	Q_OBJECT

	public:
		static void addFile(QString file)
		{
			QSettings settings(Multiuso::appDirPath() + "/ini/editeur_de_texte.ini", QSettings::IniFormat);

			QStringList allFiles = settings.value("recent_files").value<QStringList>();

			if (allFiles.contains(file))
				allFiles.removeOne(file);

			if (allFiles.count() == 5)
				allFiles.removeLast();

			allFiles.prepend(file);

			settings.setValue("recent_files", allFiles);
		}

		static void setRecentFiles(QMenu *menu, QWidget *parent)
		{
			menu->clear();

			QSettings settings(Multiuso::appDirPath() + "/ini/editeur_de_texte.ini", QSettings::IniFormat);

			QStringList allFiles = settings.value("recent_files").value<QStringList>();

			for (int i = 0; i < allFiles.count(); i++)
			{
				QString file = allFiles.value(i);

				QAction *action = new QAction("&" + QString::number(i + 1) + " " + QFileInfo(file).fileName(), parent);
					action->setToolTip(file);
					connect(action, SIGNAL(triggered()), parent, SLOT(openFileFromAction()));

				menu->addAction(action);
			}
		}
};

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

		void mergeTextCharFormat(QTextCharFormat format);

		bool isEverythingSaved();
		void sauvegarderEtat();

	public slots:
		void closeFile(int index);
		void newFile();
		void openFileFromAction();
		void openFile();
		void openFile(QString file);
		bool saveFile();
		bool saveFile(QString file);
		bool saveFileAs();
		void undo();
		void redo();
		void search();
		void search(QString word, QFlags<QTextDocument::FindFlag> findFlags);
		void replace();
		void replace(QString word, QString word2, QFlags<QTextDocument::FindFlag> findFlags);
		void printPreview();
		void printPreview(QPrinter *printer);
		void print();
		void printPDF();
		void copy();
		void cut();
		void paste();
		void selectAll();
		void insertImage();
		void insertTable();
		void repeatText();

		void bold();
		void italic();
		void underline();
		void alignment(QAction *action);
		void fontSize(QString size);
		void font(QString font);
		void selectColor();
		void selectBackgroundColor();
		void toUpper();
		void toLower();

		void textChanged();

		void currentCharFormatChanged(QTextCharFormat format);
		void cursorPositionChanged();

		void fontChanged(QFont font);
		void colorChanged(QColor color);
		void backgroundColorChanged(QColor color);
		void alignmentChanged(Qt::Alignment alignment);

		void currentChanged(int index);

	private:
		QTabWidget *tabWidget;

		QAction *a_new;
		QAction *a_open;
		QAction *a_save;
		QAction *a_saveAs;
		QAction *a_undo;
		QAction *a_redo;
		QAction *a_search;
		QAction *a_replace;
		QAction *a_printPreview;
		QAction *a_print;
		QAction *a_printPDF;
		QAction *a_copy;
		QAction *a_cut;
		QAction *a_paste;
		QAction *a_selectAll;
		QAction *a_insertImage;
		QAction *a_insertTable;
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
