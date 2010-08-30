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

#include "EditeurDeCode.h"
#include "classesPrincipales/NavigateurWeb.h"
#include "classesPrincipales/FenPrincipale.h"

EditeurDeCode::EditeurDeCode(QWidget *parent = 0) : QMainWindow(parent)
{
	createFirstToolBar();

	tabWidget = new QTabWidget;
		tabWidget->setMovable(true);
		tabWidget->setDocumentMode(true);
		tabWidget->setTabsClosable(true);
		connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeFile(int)));
		connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));

	setCentralWidget(tabWidget);

	newFile();

	pointeurSurParent = parent;
}

QToolBar *EditeurDeCode::createFirstToolBar()
{
	QToolBar *toolBar = addToolBar("Actions");
		toolBar->setObjectName("Actions");

	a_new = new QAction("Nouveau", this);
		a_new->setIcon(QIcon(":/icones/editeur_de_texte/nouveau.png"));
		a_new->setShortcut(QKeySequence("Ctrl+N"));
		connect(a_new, SIGNAL(triggered()), this, SLOT(newFile()));
			toolBar->addAction(a_new);

	QMenu *recentFilesMenu = new QMenu;
		RecentCodeFiles::setRecentFiles(recentFilesMenu, this);

	a_open = new QAction("Ouvrir", this);
		a_open->setIcon(QIcon(":/icones/editeur_de_texte/ouvrir.png"));
		a_open->setShortcut(QKeySequence("Ctrl+O"));
		a_open->setMenu(recentFilesMenu);
		connect(a_open, SIGNAL(triggered()), this, SLOT(openFile()));
			toolBar->addAction(a_open);

	a_save = new QAction("Enregistrer", this);
		a_save->setIcon(QIcon(":/icones/editeur_de_texte/enregistrer.png"));
		a_save->setShortcut(QKeySequence("Ctrl+S"));
		connect(a_save, SIGNAL(triggered()), this, SLOT(saveFile()));
			toolBar->addAction(a_save);

	a_saveAs = new QAction("Enregistrer sous...", this);
		a_saveAs->setIcon(QIcon(":/icones/editeur_de_texte/enregistrerSous.png"));
		a_saveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));
		connect(a_saveAs, SIGNAL(triggered()), this, SLOT(saveFileAs()));
			toolBar->addAction(a_saveAs);

	toolBar->addSeparator();

	a_undo = new QAction("Annuler", this);
		a_undo->setIcon(QIcon(":/icones/editeur_de_texte/annuler.png"));
		a_undo->setShortcut(QKeySequence("Ctrl+Z"));
		a_undo->setDisabled(true);
		connect(a_undo, SIGNAL(triggered()), this, SLOT(undo()));
			toolBar->addAction(a_undo);

	a_redo = new QAction("Rétablir", this);
		a_redo->setIcon(QIcon(":/icones/editeur_de_texte/retablir.png"));
		a_redo->setShortcut(QKeySequence("Ctrl+Y"));
		a_redo->setDisabled(true);
		connect(a_redo, SIGNAL(triggered()), this, SLOT(redo()));
			toolBar->addAction(a_redo);
			
	toolBar->addSeparator();
	
	a_search = new QAction("Rechercher", this);
		a_search->setIcon(QIcon(":/icones/editeur_de_texte/rechercher.png"));
		a_search->setShortcut(QKeySequence("Ctrl+F"));
		connect(a_search, SIGNAL(triggered()), this, SLOT(search()));
			toolBar->addAction(a_search);
			
	a_replace = new QAction("Remplacer", this);
		a_replace->setIcon(QIcon(":/icones/editeur_de_texte/rechercherRemplacer.png"));
		a_replace->setShortcut(QKeySequence("Ctrl+H"));
		connect(a_replace, SIGNAL(triggered()), this, SLOT(replace()));
			toolBar->addAction(a_replace);

	toolBar->addSeparator();
	
	a_printPreview = new QAction("Aperçu avant impression", this);
		a_printPreview->setIcon(QIcon(":/icones/editeur_de_texte/imprimer_apercu.png"));
		connect(a_printPreview, SIGNAL(triggered()), this, SLOT(printPreview()));
			toolBar->addAction(a_printPreview);

	a_print = new QAction("Imprimer", this);
		a_print->setIcon(QIcon(":/icones/editeur_de_texte/imprimer.png"));
		a_print->setShortcut(QKeySequence("Ctrl+P"));
		connect(a_print, SIGNAL(triggered()), this, SLOT(print()));
			toolBar->addAction(a_print);

	toolBar->addSeparator();
	
	a_copy = new QAction("Copier", this);
		a_copy->setIcon(QIcon(":/icones/editeur_de_texte/copier.png"));
		a_copy->setShortcut(QKeySequence("Ctrl+C"));
		connect(a_copy, SIGNAL(triggered()), this, SLOT(copy()));
			toolBar->addAction(a_copy);

	a_cut = new QAction("Couper", this);
		a_cut->setIcon(QIcon(":/icones/editeur_de_texte/couper.png"));
		a_cut->setShortcut(QKeySequence("Ctrl+X"));
		connect(a_cut, SIGNAL(triggered()), this, SLOT(cut()));
			toolBar->addAction(a_cut);

	a_paste = new QAction("Coller", this);
		a_paste->setIcon(QIcon(":/icones/editeur_de_texte/coller.png"));
		a_paste->setShortcut(QKeySequence("Ctrl+V"));
		connect(a_paste, SIGNAL(triggered()), this, SLOT(paste()));
			toolBar->addAction(a_paste);
			
	toolBar->addSeparator();
					
	a_selectAll = new QAction("Tout sélectionner", this);
		a_selectAll->setIcon(QIcon(":/icones/editeur_de_texte/toutSelectionner.png"));
		a_selectAll->setShortcut(QKeySequence("Ctrl+A"));
		connect(a_selectAll, SIGNAL(triggered()), this, SLOT(selectAll()));
			toolBar->addAction(a_selectAll);
		
	toolBar->addSeparator();

	a_repeatText = new QAction("Répéter du texte", this);
		a_repeatText->setIcon(QIcon(":/icones/editeur_de_texte/repeterTexte.png"));
		connect(a_repeatText, SIGNAL(triggered()), this, SLOT(repeatText()));
			toolBar->addAction(a_repeatText);

	toolBar->addSeparator();

	a_toUpper = new QAction("EN MAJUSCULE", this);
		a_toUpper->setIcon(QIcon(":/icones/editeur_de_texte/selectionMajuscule.png"));
		connect(a_toUpper, SIGNAL(triggered()), this, SLOT(toUpper()));
			toolBar->addAction(a_toUpper);

	a_toLower = new QAction("en miniscule", this);
		a_toLower->setIcon(QIcon(":/icones/editeur_de_texte/selectionMinuscule.png"));
		connect(a_toLower, SIGNAL(triggered()), this, SLOT(toLower()));
			toolBar->addAction(a_toLower);

	toolBar->addSeparator();
	
	QStringList s_highlighters;
		s_highlighters << "Texte brut" << "ActionScript" << "ASM" << "AutoIT" << "Befunge"
				<< "BrainFuck" << "C" << "Caml" << "C++" << "C#" << "CSS" << "D" << "Erlang"
				<< "Fortran" << "F#" << "GLSL" << "Haskell" << "HTML / (x)HTML / XML"
				<< "INI" << "Intercal" << "Io" << "Java" << "JavaScript" << "Lex et Yacc"
				<< "Lisp" << "Lua" << "Miranda" << "Pascal" << "Perl" << "PHP"
				<< "Python" << "Ruby" << "SQL";

	c_highlighters = new QComboBox;
		c_highlighters->addItems(s_highlighters);
		connect(c_highlighters, SIGNAL(currentIndexChanged(QString)), this, SLOT(highlighterChanged(QString)));
			toolBar->addWidget(c_highlighters);

	toolBar->addSeparator();

	a_openInWebBrowser = new QAction("Ouvrir dans le navigateur Web", this);
		a_openInWebBrowser->setIcon(QIcon(":/icones/editeur_de_code/openInWebBrowser.png"));
		connect(a_openInWebBrowser, SIGNAL(triggered()), this, SLOT(openInWebBrowser()));
			toolBar->addAction(a_openInWebBrowser);

	return toolBar;
}

CodeEdit *EditeurDeCode::currentCodeEdit()
{
	return qobject_cast<CodeEdit *>(tabWidget->currentWidget());
}

CodeEdit *EditeurDeCode::codeEditAt(int index)
{
	return qobject_cast<CodeEdit *>(tabWidget->widget(index));
}

void EditeurDeCode::setPlainText(QString text)
{
	currentCodeEdit()->setPlainText(text);
}

void EditeurDeCode::openWebPage(QString content, QString title)
{
	if (currentCodeEdit()->currentFileName() != "NONE" || currentCodeEdit()->document()->isModified())
		newFile();

	setPlainText(content);
	
	currentCodeEdit()->setDocumentTitle(title);
	currentCodeEdit()->setCurrentFileName("NONE");
	currentCodeEdit()->document()->setModified(false);
	currentCodeEdit()->setSavable(true);

	tabWidget->setTabText(tabWidget->currentIndex(), title);
	tabWidget->setTabIcon(tabWidget->currentIndex(), QIcon(":/icones/editeur_de_texte/enregistre.png"));

	highlighterFor("html");
}

bool EditeurDeCode::isEverythingSaved()
{
	for (int i = 0; i < tabWidget->count(); i++)
	{
		if (codeEditAt(i)->document()->isModified())
			return false;
	}

	return true;
}

void EditeurDeCode::sauvegarderEtat()
{
	QSettings enregistrer(Multiuso::appDirPath() + "/ini/editeur_de_texte.ini", QSettings::IniFormat);
		enregistrer.setValue("etat_fenetre", saveState());
}

void EditeurDeCode::closeFile(int index)
{
	if (codeEditAt(index)->document()->isModified())
	{
		int answer = QMessageBox::warning(this, "Multiuso", "Tous les changements non sauvegardés apportés à "
				"« " + currentCodeEdit()->documentTitle() + " » seront définitivement perdus.<br />"
				"Voulez-vous continuer ?", QMessageBox::Yes | QMessageBox::No);

		if (answer == QMessageBox::No)
			return;
	}

	
	if (tabWidget->count() - 1 == 0)
		newFile();
	
	codeEditAt(index)->deleteLater();
	tabWidget->removeTab(index);

}

void EditeurDeCode::newFile()
{
	CodeEdit *codeEdit = new CodeEdit;
		codeEdit->setDocumentTitle("Nouveau document");
		codeEdit->setCurrentFileName("NONE");
		codeEdit->document()->setModified(false);
		codeEdit->setSavable(true);
		connect(codeEdit, SIGNAL(openFileRequested(QString)), this, SLOT(openFile(QString)));
		connect(codeEdit, SIGNAL(undoAvailable(bool)), a_undo, SLOT(setEnabled(bool)));
		connect(codeEdit, SIGNAL(redoAvailable(bool)), a_redo, SLOT(setEnabled(bool)));
		connect(codeEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));

	tabWidget->addTab(codeEdit, QIcon(":/icones/editeur_de_texte/enregistre.png"), "Nouveau document");
	tabWidget->setCurrentIndex(tabWidget->indexOf(codeEdit));
	
	c_highlighters->setCurrentIndex(c_highlighters->findText("Texte brut"));
}

void EditeurDeCode::openFileFromAction()
{
	QAction *action = qobject_cast<QAction *>(sender());

	if (action == 0)
		return;

	openFile(action->toolTip());
}

void EditeurDeCode::openFile()
{
	QString file = QFileDialog::getOpenFileName(this, "Multiuso", Multiuso::lastPath(), "Fichier texte (*)");

	if (file.isEmpty())
		return;

	Multiuso::setLastPath(file);
	openFile(file);
}

void EditeurDeCode::openFile(QString file)
{
	QFile openFile(file);

	if (!openFile.exists())
	{
		QMessageBox::critical(this, "Multiuso", "Le fichier n'existe pas !");

		return;
	}

	if (!openFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this, "Multiuso", "Impossible de lire le fichier !");

		return;
	}

	if (currentCodeEdit()->document()->isModified())
	{
		int answer = QMessageBox::warning(this, "Multiuso", "Tous les changements non sauvegardés apportés à<br />"
				"« " + currentCodeEdit()->documentTitle() + " » seront définitivement perdus.<br />"
				"Voulez-vous continuer ?", QMessageBox::Yes | QMessageBox::No);

		if (answer == QMessageBox::No)
			return;
	}
	
	setCursor(Qt::WaitCursor);

	currentCodeEdit()->setSavable(false);

		QTextStream in(&openFile);
			in.setCodec("UTF-8");

		QString str;

			while (!in.atEnd())
				str += in.readLine() + "\n";
	
		openFile.close();

		currentCodeEdit()->setPlainText(str);

	currentCodeEdit()->setDocumentTitle(QFileInfo(file).fileName());
	currentCodeEdit()->setCurrentFileName(QFileInfo(file).absoluteFilePath());
	currentCodeEdit()->document()->setModified(false);
	currentCodeEdit()->setSavable(true);

	tabWidget->setTabText(tabWidget->indexOf(currentCodeEdit()), QFileInfo(file).fileName());
	tabWidget->setTabIcon(tabWidget->indexOf(currentCodeEdit()), QIcon(":/icones/editeur_de_texte/enregistre.png"));

	RecentCodeFiles::addFile(file);
	RecentCodeFiles::setRecentFiles(a_open->menu(), this);
	
	highlighterFor(QFileInfo(file).suffix());

	setCursor(Qt::ArrowCursor);
}

bool EditeurDeCode::saveFile()
{
	if (!currentCodeEdit()->isSavable())
		return false;

	if (currentCodeEdit()->currentFileName() == "NONE")
		return saveFileAs();

	else
		return saveFile(currentCodeEdit()->currentFileName());
}

bool EditeurDeCode::saveFile(QString file)
{
	if (!currentCodeEdit()->isSavable())
		return false;

	setCursor(Qt::WaitCursor);
	
	QFile w_file(file);

		if (!w_file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			setCursor(Qt::ArrowCursor);
			
			QMessageBox::critical(this, "Multiuso", "Impossible d'écrire dans le fichier :<br /><em>"
					+ QFileInfo(file).fileName() + "</em>");
		
			return false;
		}
		
		w_file.write(currentCodeEdit()->toPlainText().toAscii());
		w_file.close();

	currentCodeEdit()->setDocumentTitle(QFileInfo(file).fileName());
	currentCodeEdit()->setCurrentFileName(QFileInfo(file).absoluteFilePath());
	currentCodeEdit()->document()->setModified(false);

	tabWidget->setTabText(tabWidget->indexOf(currentCodeEdit()), QFileInfo(file).fileName());
	tabWidget->setTabIcon(tabWidget->indexOf(currentCodeEdit()), QIcon(":/icones/editeur_de_texte/enregistre.png"));

	setCursor(Qt::ArrowCursor);

	return true;
}

bool EditeurDeCode::saveFileAs()
{
	if (!currentCodeEdit()->isSavable())
		return false;

	QString fileName = currentCodeEdit()->documentTitle();

	QString file = QFileDialog::getSaveFileName(this, "Multiuso", Multiuso::lastPath() + "/" + fileName, "Tous les fichiers (*)");

	if (file.isEmpty())
		return false;

	Multiuso::setLastPath(file);
	
	RecentCodeFiles::addFile(file);
	RecentCodeFiles::setRecentFiles(a_open->menu(), this);

	return saveFile(file);
}

void EditeurDeCode::undo()
{
	currentCodeEdit()->undo();
}

void EditeurDeCode::redo()
{
	currentCodeEdit()->redo();
}

void EditeurDeCode::search()
{
	QDialog *searchDialog = new QDialog(this);
		searchDialog->setWindowTitle("Rechercher");

	QLineEdit *textToFind = new QLineEdit;
		textToFind->setText(currentCodeEdit()->textCursor().selectedText());

	QCheckBox *findBackward = new QCheckBox;
	QCheckBox *findCaseSensitively = new QCheckBox;
	QCheckBox *findWholeWords = new QCheckBox;

	QFormLayout *contentLayout = new QFormLayout;
		contentLayout->addRow("Rechercher :", textToFind);
		contentLayout->addRow("Rechercher en arrière :", findBackward);
		contentLayout->addRow("Recherche sensible à la casse :", findCaseSensitively);
		contentLayout->addRow("Rechercher uniqument les mots entiers :", findWholeWords);

	QVBoxLayout *searchDialogLayout = new QVBoxLayout(searchDialog);
		searchDialogLayout->addLayout(contentLayout);
		searchDialogLayout->addLayout(Multiuso::dialogButtons(searchDialog, "Annuler", "Rechercher"));

	if (searchDialog->exec() == QDialog::Accepted)
	{
		QFlags<QTextDocument::FindFlag> findFlags;

			bool f_backward = findBackward->isChecked();
			bool f_case = findCaseSensitively->isChecked();
			bool f_whole = findWholeWords->isChecked();

			if (f_backward)
				findFlags = QFlags<QTextDocument::FindFlag>(QTextDocument::FindBackward);

			else if (f_backward && f_case)
				findFlags = QFlags<QTextDocument::FindFlag>(QTextDocument::FindBackward
										| QTextDocument::FindCaseSensitively);

			else if (f_backward && f_whole)
				findFlags = QFlags<QTextDocument::FindFlag>(QTextDocument::FindBackward
										| QTextDocument::FindWholeWords);

			else if (f_case)
				findFlags = QFlags<QTextDocument::FindFlag>(QTextDocument::FindCaseSensitively);

			else if (f_case && f_whole)
				findFlags = QFlags<QTextDocument::FindFlag>(QTextDocument::FindCaseSensitively
										| QTextDocument::FindWholeWords);

			else if (f_whole)
				findFlags = QFlags<QTextDocument::FindFlag>(QTextDocument::FindWholeWords);

			else if (f_backward && f_case && f_whole)
				findFlags = QFlags<QTextDocument::FindFlag>(QTextDocument::FindBackward
										| QTextDocument::FindCaseSensitively
										| QTextDocument::FindWholeWords);

		search(textToFind->text(), findFlags);
	}

	searchDialog->deleteLater();
}

void EditeurDeCode::search(QString word, QFlags<QTextDocument::FindFlag> findFlags)
{
	currentCodeEdit()->textCursor().movePosition(QTextCursor::Start);

	int answer;

	do
	{
		if (!currentCodeEdit()->find(word, findFlags))
		{
			QMessageBox::critical(this, "Multiuso", "« " + word + " » n'a pas été trouvé !");

			return;
		}

		answer = QMessageBox::question(this, "Multiuso", "Rechercher le mot suivant ?", QMessageBox::Yes | QMessageBox::No);
	}
	while (answer == QMessageBox::Yes);
}

void EditeurDeCode::replace()
{
	QDialog *searchDialog = new QDialog(this);
		searchDialog->setWindowTitle("Rechercher");

	QLineEdit *textToFind = new QLineEdit;
		textToFind->setText(currentCodeEdit()->textCursor().selectedText());

	QLineEdit *textToReplace = new QLineEdit;
	QCheckBox *findBackward = new QCheckBox;
	QCheckBox *findCaseSensitively = new QCheckBox;
	QCheckBox *findWholeWords = new QCheckBox;

	QFormLayout *contentLayout = new QFormLayout;
		contentLayout->addRow("Rechercher :", textToFind);
		contentLayout->addRow("Remplacer par :", textToReplace);
		contentLayout->addRow("Rechercher en arrière :", findBackward);
		contentLayout->addRow("Recherche sensible à la casse :", findCaseSensitively);
		contentLayout->addRow("Rechercher uniqument les mots entiers :", findWholeWords);

	QVBoxLayout *searchDialogLayout = new QVBoxLayout(searchDialog);
		searchDialogLayout->addLayout(contentLayout);
		searchDialogLayout->addLayout(Multiuso::dialogButtons(searchDialog, "Annuler", "Rechercher"));

	if (searchDialog->exec() == QDialog::Accepted)
	{
		QFlags<QTextDocument::FindFlag> findFlags;

			bool f_backward = findBackward->isChecked();
			bool f_case = findCaseSensitively->isChecked();
			bool f_whole = findWholeWords->isChecked();

			if (f_backward)
				findFlags = QFlags<QTextDocument::FindFlag>(QTextDocument::FindBackward);

			else if (f_backward && f_case)
				findFlags = QFlags<QTextDocument::FindFlag>(QTextDocument::FindBackward
										| QTextDocument::FindCaseSensitively);

			else if (f_backward && f_whole)
				findFlags = QFlags<QTextDocument::FindFlag>(QTextDocument::FindBackward
										| QTextDocument::FindWholeWords);

			else if (f_case)
				findFlags = QFlags<QTextDocument::FindFlag>(QTextDocument::FindCaseSensitively);

			else if (f_case && f_whole)
				findFlags = QFlags<QTextDocument::FindFlag>(QTextDocument::FindCaseSensitively
										| QTextDocument::FindWholeWords);

			else if (f_whole)
				findFlags = QFlags<QTextDocument::FindFlag>(QTextDocument::FindWholeWords);

			else if (f_backward && f_case && f_whole)
				findFlags = QFlags<QTextDocument::FindFlag>(QTextDocument::FindBackward
										| QTextDocument::FindCaseSensitively
										| QTextDocument::FindWholeWords);

		replace(textToFind->text(), textToReplace->text(), findFlags);
	}

	searchDialog->deleteLater();
}

void EditeurDeCode::replace(QString word, QString word2, QFlags<QTextDocument::FindFlag> findFlags)
{
	currentCodeEdit()->textCursor().movePosition(QTextCursor::Start);

	int answer;

	do
	{
		if (!currentCodeEdit()->find(word, findFlags))
		{
			QMessageBox::critical(this, "Multiuso", "« " + word + " » n'a pas été trouvé !");

			return;
		}

		answer = QMessageBox::question(this, "Multiuso", "Remplacer cette occurence ?",
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::YesToAll);

		if (answer == QMessageBox::Yes)
		{
			currentCodeEdit()->textCursor().insertText(word2);
		}

		else if (answer == QMessageBox::YesToAll)
		{
			currentCodeEdit()->textCursor().insertText(word2); // To replace current selection.

			while (currentCodeEdit()->find(word, findFlags))
				currentCodeEdit()->textCursor().insertText(word2);

			return;
		}
	}
	while (answer != QMessageBox::Cancel);
}

void EditeurDeCode::printPreview()
{
	QPrinter printer(QPrinter::HighResolution);

	QPrintPreviewDialog preview(&printer, this);
		connect(&preview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(printPreview(QPrinter *)));
			preview.exec();
}

void EditeurDeCode::printPreview(QPrinter *printer)
{
	currentCodeEdit()->print(printer);
}

void EditeurDeCode::print()
{
	QPrinter printer(QPrinter::HighResolution);

	QPrintDialog *printDialog = new QPrintDialog(&printer, this);
		printDialog->setWindowTitle("Imprimer « " + currentCodeEdit()->documentTitle() + " »");

		if (currentCodeEdit()->textCursor().hasSelection())
			printDialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);

	if (printDialog->exec() == QDialog::Accepted)
		currentCodeEdit()->print(&printer);

	printDialog->deleteLater();
}

void EditeurDeCode::copy()
{
	currentCodeEdit()->copy();
}

void EditeurDeCode::cut()
{
	currentCodeEdit()->cut();
}

void EditeurDeCode::paste()
{
	currentCodeEdit()->paste();
}

void EditeurDeCode::selectAll()
{
	currentCodeEdit()->selectAll();
}

void EditeurDeCode::repeatText()
{
	CodeEdit *te = currentCodeEdit();
	QTextCursor cursor = te->textCursor();

	if (!cursor.hasSelection())
	{
		QMessageBox::information(this, "Multiuso", "Sélectionnez le texte à répéter !");

		return;
	}

	QString text = cursor.selectedText();

	bool ok;

	int howManyTimes = QInputDialog::getInt(this, "Multiuso", "Combien de fois voulez-vous "
			"répéter ce texte ?<br /><em>(texte d'origine inclus)</em>", 36,
			1, 2147483647, 1, &ok);

	if (!ok || howManyTimes == 1)
		return;

	for (int i = 0; i < howManyTimes; i++)
	{
		QCoreApplication::processEvents();
	
		te->textCursor().insertText(text);
	}
}

void EditeurDeCode::toUpper()
{
	QTextCursor cursor = currentCodeEdit()->textCursor();

	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);
	
	QString text = cursor.selectedText().toUpper();
	cursor.insertText(text);
}

void EditeurDeCode::toLower()
{
	QTextCursor cursor = currentCodeEdit()->textCursor();

	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);

	QString text = cursor.selectedText().toLower();
	cursor.insertText(text);
}

void EditeurDeCode::highlighterChanged(QString highlighter)
{
	currentCodeEdit()->setHighlighter(highlighter);
}

void EditeurDeCode::openInWebBrowser()
{
	if (p_webBrowser == NULL)
		return;

	NavigateurWeb *browser = qobject_cast<NavigateurWeb *>(p_webBrowser);

	if (browser == 0)
		return;

	browser->openCodeEditDocument(currentCodeEdit()->documentTitle(), currentCodeEdit()->toPlainText());

	FenPrincipale *fen = qobject_cast<FenPrincipale *>(pointeurSurParent);

	if (!Multiuso::openTabsList(fen->tabWidget()).contains("Navigateur Web"))
		fen->homeTab()->openTab("Navigateur Web");

	fen->setTabIndex(Multiuso::tabIndexOf("Navigateur Web", fen->tabWidget()));
}

void EditeurDeCode::textChanged()
{
	QSettings settings(Multiuso::appDirPath() + "/ini/editeur_de_code.ini", QSettings::IniFormat);

	if (settings.value("enregistrement/enregistrement_automatique").toBool())
	{
		saveFile();

		return;
	}

	if (currentCodeEdit()->document()->isModified())
		tabWidget->setTabIcon(tabWidget->indexOf(currentCodeEdit()), QIcon(":/icones/editeur_de_texte/non_enregistre.png"));
}

void EditeurDeCode::currentChanged(int)
{
	c_highlighters->setCurrentIndex(c_highlighters->findText(currentCodeEdit()->getHighlighter()));
}

void EditeurDeCode::highlighterFor(QString suffix)
{
	QStringList highlightersC;
		highlightersC << "c" << "h";

	QStringList highlightersCaml;
		highlightersCaml << "ml" << "mli";

	QStringList highlightersCPP;
		highlightersCPP << "cpp" << "c++" << "cc" << "cxx" << "hpp" << "h++";

	QStringList highlightersCSharp;
		highlightersCSharp << "cs";

	QStringList highlightersD;
		highlightersD << "d" << "di";

	QStringList highlightersErlang;
		highlightersErlang << "erl";

	QStringList highlightersFSharp;
		highlightersFSharp << "fs";

	QStringList highlightersHaskell;
		highlightersHaskell << "hs";

	QStringList highlightersHTML;
		highlightersHTML << "htm" << "html" << "cfm" << "chm" << "xml";

	QStringList highlightersJava;
		highlightersJava << "java" << "jar" << "class";

	QStringList highlightersJavaScript;
		highlightersJavaScript << "js";

	QStringList highlightersPerl;
		highlightersPerl << "p" << "pl";

	QStringList highlightersPHP;
		highlightersPHP << "php";

	QStringList highlightersPython;
		highlightersPython << "py" << "pyw";

	QStringList highlightersRuby;
		highlightersRuby << "rb";

	QStringList highlightersSQL;
		highlightersSQL << "sql" << "rsd" << "trn" << "fdb" << "ldf" << "mdf" << "ndf";

	if (highlightersC.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("C"));

	else if (highlightersCaml.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("Caml"));

	else if (highlightersCPP.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("C++"));

	else if (highlightersCSharp.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("C#"));

	else if (highlightersD.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("D"));

	else if (highlightersErlang.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("Erlang"));

	else if (highlightersFSharp.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("F#"));

	else if (highlightersHaskell.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("Haskell"));

	else if (highlightersHTML.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("HTML / (x)HTML / XML"));

	else if (highlightersJava.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("Java"));

	else if (highlightersJavaScript.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("JavaScript"));

	else if (highlightersPerl.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("Perl"));

	else if (highlightersPHP.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("PHP"));

	else if (highlightersPython.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("Python"));

	else if (highlightersRuby.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("Ruby"));

	else if (highlightersSQL.contains(suffix))
		c_highlighters->setCurrentIndex(c_highlighters->findText("SQL"));

	else
		c_highlighters->setCurrentIndex(c_highlighters->findText("Texte brut"));
}

void EditeurDeCode::setWebBrowser(QMainWindow *browser)
{
	p_webBrowser = browser;
}
