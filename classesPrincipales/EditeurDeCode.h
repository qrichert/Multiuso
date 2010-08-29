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

#ifndef HEADER_EDITEUR_DE_CODE
#define HEADER_EDITEUR_DE_CODE

#include "../CurrentIncludes.h"
#include "autresClasses/CodeEdit.h"

class RecentCodeFiles : public QObject
{
	Q_OBJECT

	public:
		static void addFile(QString file)
		{
			QSettings settings(Multiuso::appDirPath() + "/ini/editeur_de_code.ini", QSettings::IniFormat);

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

			QSettings settings(Multiuso::appDirPath() + "/ini/editeur_de_code.ini", QSettings::IniFormat);

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

class EditeurDeCode : public QMainWindow
{
	Q_OBJECT

	public:
		EditeurDeCode(QWidget *parent);

		QToolBar *createFirstToolBar();
		
		CodeEdit *currentCodeEdit();
		CodeEdit *codeEditAt(int index);

		void setPlainText(QString text);
		void openWebPage(QString content, QString title);

		bool isEverythingSaved();
		void sauvegarderEtat();
		
		void highlighterFor(QString suffix);
		void setWebBrowser(QMainWindow *browser);

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
		void copy();
		void cut();
		void paste();
		void selectAll();
		void repeatText();

		void toUpper();
		void toLower();

		void textChanged();

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
		QAction *a_copy;
		QAction *a_cut;
		QAction *a_paste;
		QAction *a_selectAll;
		QAction *a_repeatText;

		QAction *a_toUpper;
		QAction *a_toLower;

		QMainWindow *p_webBrowser;
};

#endif

/*class EditeurDeCode : public QMainWindow
{
	Q_OBJECT

	public:
		EditeurDeCode(QWidget *parent = 0);

		void highlighterFor(QString extension);
		int highlighterIndex(QString highlighterName);
		void openContent(QString content, QString title);
		void setWebBrowser(QMainWindow *browser);

		void sauvegarderEtat();
		bool isEverythingSaved();

		void newFile();
		void setTextActuel(QString texte);
		void slotOuvrirFichier(QString cheminFichier);

		void slotAnnuler();
		void slotRetablir();

		void slotOpenInWebBrowser();

	private:
		QLineEdit *l_rechercher;
		QLineEdit *l_remplacer;

	private:
		QTextEdit *texteARepeter;
		QLineEdit *nombreDeFois;

	private:
		QWidget *pointeurSurParent;
		QMainWindow *webBrowserPointer;
};

#endif*/
