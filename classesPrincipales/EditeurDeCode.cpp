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

	tabWidget->setTabText(tabWidget->currentIndex(), title);
	tabWidget->setTabIcon(tabWidget->currentIndex(), QIcon(":/icones/editeur_de_texte/non_enregistre.png"));

	currentCodeEdit()->document()->setModified(false);

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
}

void EditeurDeCode::highlighterFor(QString suffix)
{
	Q_UNUSED(suffix);
	/*QStringList highlightersC;
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

	if (highlightersC.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("C"));

	else if (highlightersCaml.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("Caml"));

	else if (highlightersCPP.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("C++"));

	else if (highlightersCSharp.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("C#"));

	else if (highlightersD.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("D"));

	else if (highlightersErlang.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("Erlang"));

	else if (highlightersFSharp.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("F#"));

	else if (highlightersHaskell.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("Haskell"));

	else if (highlightersHTML.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("HTML / (x)HTML"));

	else if (highlightersJava.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("Java"));

	else if (highlightersJavaScript.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("JavaScript"));

	else if (highlightersPerl.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("Perl"));

	else if (highlightersPHP.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("PHP"));

	else if (highlightersPython.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("Python"));

	else if (highlightersRuby.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("Ruby"));

	else if (highlightersSQL.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("SQL"));

	else
		colorisation->setCurrentIndex(colorisation->findText("Texte brut"));*/
}

void EditeurDeCode::setWebBrowser(QMainWindow *browser)
{
	p_webBrowser = browser;
}

/*EditeurDeCode::EditeurDeCode(QWidget *parent) : QMainWindow(parent)
{
	toolBarOptions = addToolBar("Options du code");
		toolBarOptions->setObjectName("Options du code");

	creerOptionsTexte();

	QToolButton *buttonNewTab = new QToolButton;
		buttonNewTab->setDefaultAction(nouveau);
		buttonNewTab->setAutoRaise(true);

	onglets = new QTabWidget;
		onglets->setMovable(true);
		onglets->setTabsClosable(true);
		onglets->setDocumentMode(true);
		onglets->setCornerWidget(buttonNewTab, Qt::TopRightCorner);
		connect(onglets, SIGNAL(tabCloseRequested(int)), this, SLOT(slotFermerOnglet(int)));
		connect(onglets, SIGNAL(currentChanged(int)), this, SLOT(slotChangementOnglet(int)));
		slotNouveau();

	pointeurSurParent = parent;
	webBrowserPointer = NULL;

	setCentralWidget(onglets);

	QSettings restaurer(Multiuso::appDirPath() + "/ini/editeur_de_code.ini", QSettings::IniFormat);
		restoreState(restaurer.value("etat_fenetre").toByteArray());
}

void EditeurDeCode::creerOptionsTexte()
{
	nouveau = new QAction("Nouveau (Ctrl + N)", this);
		nouveau->setIcon(QIcon(":/icones/editeur_de_code/nouveau.png"));
		nouveau->setShortcut(QKeySequence("Ctrl+N"));
		connect(nouveau, SIGNAL(triggered()), this, SLOT(slotNouveau()));
		toolBarOptions->addAction(nouveau);

	enregistrer = new QAction("Enregistrer (Ctrl + s)", this);
		enregistrer->setIcon(QIcon(":/icones/editeur_de_code/enregistrer.png"));
		enregistrer->setShortcut(QKeySequence("Ctrl+S"));
		enregistrer->setDisabled(true);
		connect(enregistrer, SIGNAL(triggered()), this, SLOT(slotEnregistrer()));
		toolBarOptions->addAction(enregistrer);

	enregistrerSous = new QAction("Enregistrer sous... (Ctrl + Shift + S)", this);
		enregistrerSous->setIcon(QIcon(":/icones/editeur_de_code/enregistrerSous.png"));
		enregistrerSous->setShortcut(QKeySequence("Ctrl+Shift+S"));
		connect(enregistrerSous, SIGNAL(triggered()), this, SLOT(slotEnregistrerSous()));
		toolBarOptions->addAction(enregistrerSous);

	ouvrir = new QAction("Ouvrir... (Ctrl + 0)", this);
		ouvrir->setIcon(QIcon(":/icones/editeur_de_code/ouvrir.png"));
		ouvrir->setShortcut(QKeySequence("Ctrl+O"));
		connect(ouvrir, SIGNAL(triggered()), this, SLOT(slotOuvrir()));
		toolBarOptions->addAction(ouvrir);


	toolBarOptions->addSeparator();


	annuler = new QAction("Annuler (Ctrl + Z)", this);
		annuler->setIcon(QIcon(":/icones/editeur_de_code/annuler.png"));
		annuler->setShortcut(QKeySequence("Ctrl+Z"));
		annuler->setDisabled(true);
		connect(annuler, SIGNAL(triggered()), this, SLOT(slotAnnuler()));
		toolBarOptions->addAction(annuler);

	retablir = new QAction("Rétablir (Ctrl + Y)", this);
		retablir->setIcon(QIcon(":/icones/editeur_de_code/retablir.png"));
		retablir->setShortcut(QKeySequence("Ctrl+Y"));
		retablir->setDisabled(true);
		connect(retablir, SIGNAL(triggered()), this, SLOT(slotRetablir()));
		toolBarOptions->addAction(retablir);


	toolBarOptions->addSeparator();


	supprimerSelection = new QAction("Supprimer (Suppr)", this);
		supprimerSelection->setIcon(QIcon(":/icones/editeur_de_code/supprimer.png"));
		supprimerSelection->setShortcut(QKeySequence("Suppr"));
		connect(supprimerSelection, SIGNAL(triggered()), this, SLOT(slotSupprimerSelection()));
		toolBarOptions->addAction(supprimerSelection);


	toolBarOptions->addSeparator();


	rechercher = new QAction("Rechercher... (Ctrl + F)", this);
		rechercher->setIcon(QIcon(":/icones/editeur_de_code/rechercher.png"));
		rechercher->setShortcut(QKeySequence("Ctrl+F"));
		connect(rechercher, SIGNAL(triggered()), this, SLOT(slotRechercher()));
		toolBarOptions->addAction(rechercher);

	rechercherRemplacer = new QAction("Rechercher/Remplacer... (Ctrl + H)", this);
		rechercherRemplacer->setIcon(QIcon(":/icones/editeur_de_code/rechercherRemplacer.png"));
		rechercherRemplacer->setShortcut(QKeySequence("Ctrl+H"));
		connect(rechercherRemplacer, SIGNAL(triggered()), this, SLOT(slotRechercherRemplacer()));
		toolBarOptions->addAction(rechercherRemplacer);


	toolBarOptions->addSeparator();


	imprimer = new QAction("Imprimer (Ctrl + P)", this);
		imprimer->setIcon(QIcon(":/icones/editeur_de_code/imprimer.png"));
		imprimer->setShortcut(QKeySequence("Ctrl+P"));
		connect(imprimer, SIGNAL(triggered()), this, SLOT(slotImprimer()));
		toolBarOptions->addAction(imprimer);


	toolBarOptions->addSeparator();


	copier = new QAction("Copier (Ctrl + C)", this);
		copier->setIcon(QIcon(":/icones/editeur_de_code/copier.png"));
		copier->setShortcut(QKeySequence("Ctrl+C"));
		connect(copier, SIGNAL(triggered()), this, SLOT(slotCopier()));
		toolBarOptions->addAction(copier);

	couper = new QAction("Couper (Ctrl + X)", this);
		couper->setIcon(QIcon(":/icones/editeur_de_code/couper.png"));
		couper->setShortcut(QKeySequence("Ctrl+X"));
		connect(couper, SIGNAL(triggered()), this, SLOT(slotCouper()));
		toolBarOptions->addAction(couper);

	coller = new QAction("Coller (Ctrl + V)", this);
		coller->setIcon(QIcon(":/icones/editeur_de_code/coller.png"));
		coller->setShortcut(QKeySequence("Ctrl+V"));
		connect(coller, SIGNAL(triggered()), this, SLOT(slotColler()));
		toolBarOptions->addAction(coller);

	toutSelectionner = new QAction("Tout sélectionner (Ctrl + A)", this);
		toutSelectionner->setIcon(QIcon(":/icones/editeur_de_code/toutSelectionner.png"));
		toutSelectionner->setShortcut(QKeySequence("Ctrl+A"));
		connect(toutSelectionner, SIGNAL(triggered()), this, SLOT(slotToutSelectionner()));
		toolBarOptions->addAction(toutSelectionner);

	repeterTexte = new QAction("Répéter du texte", this);
		repeterTexte->setIcon(QIcon(":/icones/editeur_de_code/repeterTexte.png"));
		connect(repeterTexte, SIGNAL(triggered()), this, SLOT(slotRepeterTexte()));
		toolBarOptions->addAction(repeterTexte);


	toolBarOptions->addSeparator();


	selectionMajuscule = new QAction("Sélection en majuscule", this);
		selectionMajuscule->setIcon(QIcon(":/icones/editeur_de_code/selectionMajuscule.png"));
		connect(selectionMajuscule, SIGNAL(triggered()), this, SLOT(slotSelectionMajuscule()));
		toolBarOptions->addAction(selectionMajuscule);

	selectionMinuscule = new QAction("Sélection en minuscule", this);
		selectionMinuscule->setIcon(QIcon(":/icones/editeur_de_code/selectionMinuscule.png"));
		connect(selectionMinuscule, SIGNAL(triggered()), this, SLOT(slotSelectionMinuscule()));
		toolBarOptions->addAction(selectionMinuscule);


	toolBarOptions->addSeparator();


	colorisation = new QComboBox;
		colorisation->addItem("Texte brut");
		colorisation->addItem("ActionScript");
		colorisation->addItem("ASM");
		colorisation->addItem("AutoIT");
		colorisation->addItem("Befunge");
		colorisation->addItem("Brainfuck");
		colorisation->addItem("C");
		colorisation->addItem("Caml");
		colorisation->addItem("C++");
		colorisation->addItem("C#");
		colorisation->addItem("CSS");
		colorisation->addItem("D");
		colorisation->addItem("Erlang");
		colorisation->addItem("F#");
		colorisation->addItem("Fortran");
		colorisation->addItem("GLSL");
		colorisation->addItem("Haskell");
		colorisation->addItem("HTML / (x)HTML / XML");
		colorisation->addItem("INI");
		colorisation->addItem("Intercal");
		colorisation->addItem("Io");
		colorisation->addItem("Java");
		colorisation->addItem("JavaScript");
		colorisation->addItem("Lex et Yacc");
		colorisation->addItem("Lisp");
		colorisation->addItem("Lua");
		colorisation->addItem("Miranda");
		colorisation->addItem("Pascal");
		colorisation->addItem("Perl");
		colorisation->addItem("PHP");
		colorisation->addItem("Python");
		colorisation->addItem("Ruby");
		colorisation->addItem("SQL");
		colorisation->setCurrentIndex(0);
		toolBarOptions->addWidget(colorisation);
		connect(colorisation, SIGNAL(currentIndexChanged(int)), this, SLOT(changementHighlighter(int)));


	toolBarOptions->addSeparator();


	openInWebBrowser = new QAction("Ouvrir dans le navigateur Web", this);
		openInWebBrowser->setIcon(QIcon(":/icones/editeur_de_code/openInWebBrowser.png"));
		connect(openInWebBrowser, SIGNAL(triggered()), this, SLOT(slotOpenInWebBrowser()));
		toolBarOptions->addAction(openInWebBrowser);
}

void EditeurDeCode::slotOpenInWebBrowser()
{
	if (webBrowserPointer == NULL)
		return;

	NavigateurWeb *browser = qobject_cast<NavigateurWeb *>(webBrowserPointer);

	if (browser == 0)
		return;

	browser->openCodeEditDocument(onglets->tabText(onglets->currentIndex()), pageActuelle()->toPlainText());

	FenPrincipale *fen = qobject_cast<FenPrincipale *>(pointeurSurParent);

	if (!Multiuso::openTabsList(fen->tabWidget()).contains("Navigateur Web"))
		fen->homeTab()->openTab("Navigateur Web");

	fen->setTabIndex(Multiuso::tabIndexOf("Navigateur Web", fen->tabWidget()));
}

void EditeurDeCode::slotChangementTexte()
{
	QSettings reglages(Multiuso::appDirPath() + "/ini/editeur_de_code.ini", QSettings::IniFormat);

	if (reglages.value("enregistrement/enregistrement_automatique").toBool())
		slotEnregistrer();

	onglets->setTabIcon(onglets->currentIndex(), QIcon(":/icones/editeur_de_code/non_enregistre.png"));
	pageActuelle()->setEstEnregistre(false);
	pageActuelle()->setSauvegardeOk(true);

	enregistrer->setEnabled(true);
}

void EditeurDeCode::slotFermerOnglet(int onglet)
{
	onglets->setCurrentIndex(onglet);

	int reponse = QMessageBox::Yes;

	if (!pageActuelle()->estEnregistre())
		reponse = QMessageBox::question(this, "Multiuso", "Toutes les modifications non enregistrées seront perdues<br />"
				"Voulez-vous continuer ?", QMessageBox::Yes | QMessageBox::No);

	if (reponse == QMessageBox::Yes)
	{
		if (onglets->count() - 1 == 0)
			slotNouveau();

		onglets->removeTab(onglet);
	}
}

int EditeurDeCode::highlighterIndex(QString highlighterName)
{
	return colorisation->findText(highlighterName);
}

void EditeurDeCode::openContent(QString content, QString title)
{
	if (onglets->count() == 1 && onglets->tabText(onglets->currentIndex()) == "Nouveau document" && pageActuelle()->estEnregistre())
	{
	}

	else
	{
		slotNouveau();
	}

	onglets->setCurrentIndex(onglets->count());

	pageActuelle()->setPlainText(content);

	onglets->setTabText(onglets->currentIndex(), title);
	onglets->setTabIcon(onglets->currentIndex(), QIcon(":/icones/editeur_de_code/non_enregistre.png"));

	pageActuelle()->setEstEnregistre(false);
	pageActuelle()->setSauvegardeOk(true);

	enregistrer->setDisabled(false);

	highlighterFor("html");
}

void EditeurDeCode::changementHighlighter(int highlighter)
{
	pageActuelle()->changeHighlighter(highlighter);
	pageActuelle()->setColorisation(highlighter);
}

void EditeurDeCode::sauvegarderEtat()
{
	QSettings enregistrer(Multiuso::appDirPath() + "/ini/editeur_de_code.ini", QSettings::IniFormat);
		enregistrer.setValue("etat_fenetre", saveState());
}

bool EditeurDeCode::isEverythingSaved()
{
	for (int i = 0; i < onglets->count(); i++)
	{
		onglets->setCurrentIndex(i);

		CodeEdit *codeEdit = onglets->currentWidget()->findChild<CodeEdit *>();

		if (!codeEdit->estEnregistre())
			return false;
	}

	return true;
}

void EditeurDeCode::slotChangementOnglet(int onglet)
{
	onglet = 0;

	slotRemettreValeurs();
}

QWidget *EditeurDeCode::nouvelOnglet()
{
	CodeEdit *champDeSaisie = new CodeEdit(this);
		connect(champDeSaisie, SIGNAL(textChanged()), this, SLOT(slotChangementTexte()));
		connect(champDeSaisie, SIGNAL(redoAvailable(bool)), this, SLOT(slotAMRedo(bool)));
		connect(champDeSaisie, SIGNAL(undoAvailable(bool)), this, SLOT(slotAMUndo(bool)));
		connect(champDeSaisie, SIGNAL(ouvrirFichier(QString)), this, SLOT(slotOuvrirFichier(QString)));

	QVBoxLayout *layoutChampDeSaisie = new QVBoxLayout;
		layoutChampDeSaisie->addWidget(champDeSaisie);
		layoutChampDeSaisie->setContentsMargins(0, 0, 0, 0);

	QWidget *widgetConteneur = new QWidget;
		widgetConteneur->setLayout(layoutChampDeSaisie);

	return widgetConteneur;
}

CodeEdit *EditeurDeCode::pageActuelle()
{
	return onglets->currentWidget()->findChild<CodeEdit *>();
}

QString EditeurDeCode::titreTabCourrant()
{
	return onglets->tabText(onglets->currentIndex());
}

void EditeurDeCode::slotAMRedo(bool available)
{
	retablir->setEnabled(available);
	pageActuelle()->setRedoOk(available);
}

void EditeurDeCode::slotAMUndo(bool available)
{
	annuler->setEnabled(available);
	pageActuelle()->setUndoOk(available);
}

void EditeurDeCode::slotRemettreValeurs()
{
	annuler->setEnabled(pageActuelle()->undoOk());
	retablir->setEnabled(pageActuelle()->redoOk());

	colorisation->setCurrentIndex(pageActuelle()->colorisationCourante());
}


void EditeurDeCode::slotSelectionMajuscule()
{
	QString texte = pageActuelle()->textCursor().selectedText();
		texte = texte.toUpper();

	pageActuelle()->textCursor().removeSelectedText();
	pageActuelle()->textCursor().insertText(texte);
}

void EditeurDeCode::slotSelectionMinuscule()
{
	QString texte = pageActuelle()->textCursor().selectedText();
		texte = texte.toLower();

	pageActuelle()->textCursor().removeSelectedText();
	pageActuelle()->textCursor().insertText(texte);
}

void EditeurDeCode::slotNouveau()
{
	onglets->addTab(nouvelOnglet(), QIcon(":/icones/editeur_de_code/enregistre.png"), "Nouveau document");
	onglets->setCurrentIndex(onglets->count() - 1);
}

void EditeurDeCode::setTextActuel(QString texte)
{
	pageActuelle()->setPlainText(texte);
}

void EditeurDeCode::slotEnregistrer()
{
	QFile fichier(pageActuelle()->fichierOuvert());

	if (fichier.exists() && fichier.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		fichier.write(pageActuelle()->toPlainText().toAscii());

		pageActuelle()->setEstEnregistre(true);
		pageActuelle()->setSauvegardeOk(false);

		enregistrer->setDisabled(true);

		onglets->setTabIcon(onglets->currentIndex(), QIcon(":/icones/editeur_de_code/enregistre.png"));
	}

	else
	{
		slotEnregistrerSous();
	}

	fichier.close();
}

void EditeurDeCode::slotEnregistrerSous()
{
	QString cheminDuFichier = QFileDialog::getSaveFileName(this, "Multiuso", Multiuso::lastPath() + "/Nouveau document.mltstxt", "Texte (*)");

	Multiuso::setLastPath(cheminDuFichier);

	QFile fichier(cheminDuFichier);

	if (fichier.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QFileInfo infosFichier(fichier);

		fichier.write(pageActuelle()->toPlainText().toAscii());

		QString titre = infosFichier.fileName();

		onglets->setTabText(onglets->currentIndex(), titre);
		onglets->setTabIcon(onglets->currentIndex(), QIcon(":/icones/editeur_de_code/enregistre.png"));

		pageActuelle()->setEstEnregistre(true);
		pageActuelle()->setSauvegardeOk(false);
		pageActuelle()->setFichierOuvert(cheminDuFichier);

		enregistrer->setDisabled(true);
	}

	fichier.close();
}

void EditeurDeCode::slotOuvrir()
{
	QString cheminFichier = QFileDialog::getOpenFileName(this, "Multiuso", Multiuso::lastPath(), "Texte (*)");

	Multiuso::setLastPath(cheminFichier);

	if (!cheminFichier.isEmpty())
		slotOuvrirFichier(cheminFichier);
}

void EditeurDeCode::openFile(QString file)
{
	QFile fichier(cheminFichier);

	if (fichier.exists() && fichier.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		if (onglets->count() == 1 && onglets->tabText(onglets->currentIndex()) == "Nouveau document" && pageActuelle()->estEnregistre())
		{
		}

		else
		{
			slotNouveau();
		}

		onglets->setCurrentIndex(onglets->count());

		QFileInfo infosFichier(fichier);

		pageActuelle()->setPlainText(fichier.readAll());

		onglets->setTabText(onglets->currentIndex(), infosFichier.fileName());
		onglets->setTabIcon(onglets->currentIndex(), QIcon(":/icones/editeur_de_code/enregistre.png"));

		pageActuelle()->setEstEnregistre(true);
		pageActuelle()->setSauvegardeOk(false);
		pageActuelle()->setFichierOuvert(cheminFichier);

		enregistrer->setDisabled(true);
	}

	fichier.close();

	highlighterFor(QFileInfo(fichier).suffix());
}*/
