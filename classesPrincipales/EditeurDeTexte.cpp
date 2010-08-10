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

#include "EditeurDeTexte.h"
#include "autresClasses/RechercherRemplacer.h"

EditeurDeTexte::EditeurDeTexte(QWidget *parent = 0) : QMainWindow(parent)
{
	createFirstToolBar();

		addToolBarBreak();

	createSecondToolBar();

	tabWidget = new QTabWidget;
		tabWidget->setMovable(true);
		tabWidget->setDocumentMode(true);
		connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeFile(int)));
		connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));

	setCentralWidget(tabWidget);

	newFile();
}

QToolBar *EditeurDeTexte::createFirstToolBar()
{
	QToolBar *toolBar = addToolBar("Actions");
		toolBar->setObjectName("Actions");

	a_new = new QAction("Nouveau", this);
		a_new->setIcon(QIcon(":/icones/editeur_de_texte/nouveau.png"));
		a_new->setShortcut(QKeySequence("Ctrl+N"));
		connect(a_new, SIGNAL(triggered()), this, SLOT(newFile()));
			toolBar->addAction(a_new);

	a_open = new QAction("Ouvrir", this);
		a_open->setIcon(QIcon(":/icones/editeur_de_texte/ouvrir.png"));
		a_open->setShortcut(QKeySequence("Ctrl+O"));
		connect(a_open, SIGNAL(triggered()), this, SLOT(openFile()));
			toolBar->addAction(a_open);
			// setMenu -> recentFiles

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
			toolBar->addAction(a_search);
			
	a_replace = new QAction("Remplacer", this);
		a_replace->setIcon(QIcon(":/icones/editeur_de_texte/rechercherRemplacer.png"));
		a_replace->setShortcut(QKeySequence("Ctrl+H"));
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
			
	a_printPDF = new QAction("Exporter en PDF", this);
		a_printPDF->setIcon(QIcon(":/icones/editeur_de_texte/imprimer_pdf.png"));
		connect(a_printPDF, SIGNAL(triggered()), this, SLOT(printPDF()));
			toolBar->addAction(a_printPDF);

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
	
	a_insertImage = new QAction("Insérer une image", this);
		a_insertImage->setIcon(QIcon(":/icones/editeur_de_texte/insererImage.png"));
		connect(a_insertImage, SIGNAL(triggered()), this, SLOT(insertImage()));
			toolBar->addAction(a_insertImage);

	a_repeatText = new QAction("Répéter du texte", this);
		a_repeatText->setIcon(QIcon(":/icones/editeur_de_texte/repeterTexte.png"));
			toolBar->addAction(a_repeatText);

	return toolBar;
}

QToolBar *EditeurDeTexte::createSecondToolBar()
{
	QToolBar *toolBar = addToolBar("Options");
		toolBar->setObjectName("Options");
	
	a_bold = new QAction("Gras", this);
		a_bold->setIcon(QIcon(":/icones/editeur_de_texte/gras.png"));
		a_bold->setCheckable(true);
		connect(a_bold, SIGNAL(triggered()), this, SLOT(bold()));
			toolBar->addAction(a_bold);
			
	a_italic = new QAction("Italique", this);
		a_italic->setIcon(QIcon(":/icones/editeur_de_texte/italique.png"));
		a_italic->setCheckable(true);
		connect(a_italic, SIGNAL(triggered()), this, SLOT(italic()));
			toolBar->addAction(a_italic);
			
	a_underline = new QAction("Souligné", this);
		a_underline->setIcon(QIcon(":/icones/editeur_de_texte/souligne.png"));
		a_underline->setCheckable(true);
		connect(a_underline, SIGNAL(triggered()), this, SLOT(underline()));
			toolBar->addAction(a_underline);
	
	toolBar->addSeparator();

	QActionGroup *groupAlignment = new QActionGroup(this);
		connect(groupAlignment, SIGNAL(triggered(QAction *)), this, SLOT(alignment(QAction *)));

	a_alignLeft = groupAlignment->addAction("Gauche");
		a_alignLeft->setIcon(QIcon(":/icones/editeur_de_texte/gauche.png"));
		a_alignLeft->setCheckable(true);
			toolBar->addAction(a_alignLeft);

	a_alignCenter = groupAlignment->addAction("Centré");
		a_alignCenter->setIcon(QIcon(":/icones/editeur_de_texte/centre.png"));
		a_alignCenter->setCheckable(true);
			toolBar->addAction(a_alignCenter);

	a_alignRight = groupAlignment->addAction("Droite");
		a_alignRight->setIcon(QIcon(":/icones/editeur_de_texte/droite.png"));
		a_alignRight->setCheckable(true);
			toolBar->addAction(a_alignRight);

	a_alignJustify = groupAlignment->addAction("Justifié");
		a_alignJustify->setIcon(QIcon(":/icones/editeur_de_texte/justifie.png"));
		a_alignJustify->setCheckable(true);
			toolBar->addAction(a_alignJustify);
	
	toolBar->addSeparator();

	a_fontSize = new QComboBox;
		
		foreach (int size, QFontDatabase().standardSizes())
			a_fontSize->addItem(QString::number(size));

		connect(a_fontSize, SIGNAL(activated(QString)), this, SLOT(fontSize(QString)));
		a_fontSize->setCurrentIndex(a_fontSize->findText(QString::number(QApplication::font().pointSize())));
			toolBar->addWidget(a_fontSize);

		toolBar->addWidget(new QLabel(" "));

	a_font = new QFontComboBox;
		connect(a_font, SIGNAL(activated(QString)), this, SLOT(font(QString)));
			toolBar->addWidget(a_font);

	a_selectColor = new QAction("Couleur de texte", this);
		a_selectColor->setIcon(createColorIcon(Qt::black));
		connect(a_selectColor, SIGNAL(triggered()), this, SLOT(selectColor()));
			toolBar->addAction(a_selectColor);
			
	a_selectBackgroundColor = new QAction("Couleur de fond", this);
		a_selectBackgroundColor->setIcon(createBackgroundColorIcon(Qt::white));
		connect(a_selectBackgroundColor, SIGNAL(triggered()), this, SLOT(selectBackgroundColor()));
			toolBar->addAction(a_selectBackgroundColor);

	toolBar->addSeparator();

	a_toUpper = new QAction("EN MAJUSCULE", this);
		a_toUpper->setIcon(QIcon(":/icones/editeur_de_texte/selectionMajuscule.png"));
		connect(a_toUpper, SIGNAL(triggered()), this, SLOT(toUpper()));
			toolBar->addAction(a_toUpper);

	a_toLower = new QAction("en miniscule", this);
		a_toLower->setIcon(QIcon(":/icones/editeur_de_texte/selectionMinuscule.png"));
		connect(a_toLower, SIGNAL(triggered()), this, SLOT(toLower()));
			toolBar->addAction(a_toLower);

	return toolBar;
}

QIcon EditeurDeTexte::createColorIcon(QColor color)
{
	QPixmap icon(32, 32);
		icon.fill(Qt::transparent);

	QPixmap ellipse(32, 32);
		ellipse.fill(Qt::transparent);

		QPainter painter;
			painter.begin(&icon);
				painter.setRenderHint(QPainter::Antialiasing, true);
				painter.setPen(Qt::NoPen);
				painter.setBrush(color);
				painter.drawRoundedRect(0, 0, 32, 32, (qreal) 5, (qreal) 5);
			painter.end();
			painter.begin(&ellipse);
				painter.setRenderHint(QPainter::Antialiasing, true);
				painter.setPen(Qt::NoPen);
				painter.setBrush(Qt::white);
				painter.setOpacity(0.3);
				painter.drawEllipse(-(32 - (32 / 6)),
							-(32 + (32 / 3)),
								32 * 2, 32 * 2);
			painter.end();
			painter.begin(&icon);
				painter.setRenderHint(QPainter::Antialiasing, true);
				painter.setPen(Qt::NoPen);
				painter.setBrush(ellipse);
				painter.drawRoundedRect(0, 0, 32, 32, (qreal) 5, (qreal) 5);
			painter.end();

	return QIcon(icon);
}

QIcon EditeurDeTexte::createBackgroundColorIcon(QColor color)
{
	QPixmap icon(32, 32);
		icon.fill(Qt::transparent);

	QPixmap ellipse(32, 32);
		ellipse.fill(Qt::transparent);

		QPainter painter;
			painter.begin(&icon);
				painter.setRenderHint(QPainter::Antialiasing, true);
				painter.setPen(Qt::NoPen);
				painter.setBrush(color);
				painter.drawRoundedRect(0, 0, 32, 32, (qreal) 5, (qreal) 5);
			painter.end();
			painter.begin(&ellipse);
				painter.setRenderHint(QPainter::Antialiasing, true);
				painter.setPen(Qt::NoPen);
				painter.setBrush(Qt::white);
				painter.setOpacity(0.3);
				painter.drawEllipse(-(32 - (32 / 6)),
							-(32 + (32 / 3)),
								32 * 2, 32 * 2);
			painter.end();
			painter.begin(&icon);
				painter.setRenderHint(QPainter::Antialiasing, true);
				painter.setPen(Qt::black);
				
					if (color == Qt::black)
						painter.setPen(Qt::white);
			
				painter.drawText(0, 0, 32, 32, Qt::AlignCenter, "A");
				painter.setPen(Qt::NoPen);
				painter.setBrush(ellipse);
				painter.drawRoundedRect(0, 0, 32, 32, (qreal) 5, (qreal) 5);
			painter.end();

	return QIcon(icon);
}

TextEdit *EditeurDeTexte::currentTextEdit()
{
	return qobject_cast<TextEdit *>(tabWidget->currentWidget());
}

TextEdit *EditeurDeTexte::textEditAt(int index)
{
	return qobject_cast<TextEdit *>(tabWidget->widget(index));
}

void EditeurDeTexte::mergeTextCharFormat(QTextCharFormat format)
{
	QTextCursor cursor = currentTextEdit()->textCursor();

	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);

	cursor.mergeCharFormat(format);
	currentTextEdit()->mergeCurrentCharFormat(format);
}

bool EditeurDeTexte::isEverythingSaved()
{
	return true;
}

void EditeurDeTexte::sauvegarderEtat()
{
	QSettings enregistrer(Multiuso::appDirPath() + "/ini/editeur_de_texte.ini", QSettings::IniFormat);
		enregistrer.setValue("etat_fenetre", saveState());
}

void EditeurDeTexte::closeFile(int index)
{
	if (textEditAt(index)->document()->isModified())
	{
		int answer = QMessageBox::warning(this, "Multiuso", "Tous les changements non sauvegardés apportés à "
				"« " + currentTextEdit()->documentTitle() + " » seront définitivement perdus.<br />"
				"Voulez-vous continuer ?", QMessageBox::Yes | QMessageBox::No);

		if (answer == QMessageBox::No)
			return;
	}

	textEditAt(index)->deleteLater();
	tabWidget->removeTab(index);

	if (tabWidget->count() <= 1)
		tabWidget->setTabsClosable(false);

	else
		tabWidget->setTabsClosable(true);
}

void EditeurDeTexte::newFile()
{
	TextEdit *textEdit = new TextEdit;
		textEdit->setDocumentTitle("Nouveau document");
		textEdit->setCurrentFileName("NONE");
		textEdit->document()->setModified(false);
		textEdit->setSavable(true);
		connect(textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)), this, SLOT(currentCharFormatChanged(QTextCharFormat)));
		connect(textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
		connect(textEdit, SIGNAL(openFileRequested(QString)), this, SLOT(openFile(QString)));
		connect(textEdit, SIGNAL(undoAvailable(bool)), a_undo, SLOT(setEnabled(bool)));
		connect(textEdit, SIGNAL(redoAvailable(bool)), a_redo, SLOT(setEnabled(bool)));
		connect(textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));

	tabWidget->addTab(textEdit, QIcon(":/icones/editeur_de_texte/enregistre.png"), "Nouveau document");
	tabWidget->setTabsClosable(true);
	
	if (tabWidget->count() <= 1)
		tabWidget->setTabsClosable(false);

	else
		tabWidget->setTabsClosable(true);
}

void EditeurDeTexte::openFile()
{
	QString file = QFileDialog::getOpenFileName(this, "Multiuso", Multiuso::lastPath(), "Fichier texte (*)");

	if (file.isEmpty())
		return;

	Multiuso::setLastPath(file);
	openFile(file);
}

void EditeurDeTexte::openFile(QString file)
{
	QFile openFile(file);

	if (!openFile.exists())
	{
		QMessageBox::critical(this, "Multiuso", "Le fichier n'existe pas !");

		return;
	}

	if (!openFile.open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(this, "Multiuso", "Impossible de lire le fichier !");

		return;
	}

	if (currentTextEdit()->document()->isModified())
	{
		int answer = QMessageBox::warning(this, "Multiuso", "Tous les changements non sauvegardés apportés à<br />"
				"« " + currentTextEdit()->documentTitle() + " » seront définitivement perdus.<br />"
				"Voulez-vous continuer ?", QMessageBox::Yes | QMessageBox::No);

		if (answer == QMessageBox::No)
			return;
	}
	
	setCursor(Qt::WaitCursor);

	currentTextEdit()->setSavable(false);

		QByteArray ba = openFile.readAll();
			openFile.close();

		QTextCodec *codec = Qt::codecForHtml(ba);
		QString str = codec->toUnicode(ba);

		if (Qt::mightBeRichText(str))
		{
			currentTextEdit()->setHtml(str);
		}

		else
		{
			str = QString::fromLocal8Bit(ba);
			currentTextEdit()->setPlainText(str);
		}

	currentTextEdit()->setDocumentTitle(QFileInfo(file).fileName());
	currentTextEdit()->setCurrentFileName(QFileInfo(file).absoluteFilePath());
	currentTextEdit()->document()->setModified(false);
	currentTextEdit()->setSavable(true);

	tabWidget->setTabText(tabWidget->indexOf(currentTextEdit()), QFileInfo(file).fileName());
	tabWidget->setTabIcon(tabWidget->indexOf(currentTextEdit()), QIcon(":/icones/editeur_de_texte/enregistre.png"));

	setCursor(Qt::ArrowCursor);
}

bool EditeurDeTexte::saveFile()
{
	if (!currentTextEdit()->isSavable())
		return false;

	if (currentTextEdit()->currentFileName() == "NONE")
		return saveFileAs();

	else
		return saveFile(currentTextEdit()->currentFileName());
}

bool EditeurDeTexte::saveFile(QString file)
{
	if (!currentTextEdit()->isSavable())
		return false;

	setCursor(Qt::WaitCursor);
	
	QTextDocumentWriter writer(file);

		if (!writer.write(currentTextEdit()->document()))
		{
			setCursor(Qt::ArrowCursor);
			
			QMessageBox::critical(this, "Multiuso", "Impossible d'écrire dans le fichier :<br /><em>"
					+ QFileInfo(file).fileName() + "</em>");
		
			return false;
		}

	currentTextEdit()->setDocumentTitle(QFileInfo(file).fileName());
	currentTextEdit()->setCurrentFileName(QFileInfo(file).absoluteFilePath());
	currentTextEdit()->document()->setModified(false);

	tabWidget->setTabText(tabWidget->indexOf(currentTextEdit()), QFileInfo(file).fileName());
	tabWidget->setTabIcon(tabWidget->indexOf(currentTextEdit()), QIcon(":/icones/editeur_de_texte/enregistre.png"));

	setCursor(Qt::ArrowCursor);

	return true;
}

bool EditeurDeTexte::saveFileAs()
{
	if (!currentTextEdit()->isSavable())
		return false;

	QString fileName = currentTextEdit()->documentTitle();

	QString file = QFileDialog::getSaveFileName(this, "Multiuso", Multiuso::lastPath() + "/" + fileName,
			"Fichier HTML (*.html *.htm);;Texte ODF (*.odt);;Tous les fichiers (*)");

	if (file.isEmpty())
		return false;

	if (!(file.endsWith(".html", Qt::CaseInsensitive)
			|| file.endsWith(".htm", Qt::CaseInsensitive)
			|| file.endsWith(".odt", Qt::CaseInsensitive)))
				file += ".html";

	Multiuso::setLastPath(file);
	return saveFile(file);
}

void EditeurDeTexte::undo()
{
	currentTextEdit()->undo();
}

void EditeurDeTexte::redo()
{
	currentTextEdit()->redo();
}

/*void EditeurDeTexte::search()
{
}*/

/*void EditeurDeTexte::replace()
{
}*/

void EditeurDeTexte::printPreview()
{
	QPrinter printer(QPrinter::HighResolution);

	QPrintPreviewDialog preview(&printer, this);
		connect(&preview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(printPreview(QPrinter *)));
			preview.exec();
}

void EditeurDeTexte::printPreview(QPrinter *printer)
{
	currentTextEdit()->print(printer);
}

void EditeurDeTexte::print()
{
	QPrinter printer(QPrinter::HighResolution);

	QPrintDialog *printDialog = new QPrintDialog(&printer, this);
		printDialog->setWindowTitle("Imprimer « " + currentTextEdit()->documentTitle() + " »");

		if (currentTextEdit()->textCursor().hasSelection())
			printDialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);

	if (printDialog->exec() == QDialog::Accepted)
		currentTextEdit()->print(&printer);

	printDialog->deleteLater();
}

void EditeurDeTexte::printPDF()
{
	QString file = QFileDialog::getSaveFileName(this, "Multiuso", Multiuso::lastPath() +
			QFileInfo(currentTextEdit()->documentTitle()).baseName() + "/", "*.pdf");

	if (file.isEmpty())
		return;

	Multiuso::setLastPath(file);

	if (!file.endsWith(".pdf", Qt::CaseInsensitive))
		file += ".pdf";

	QPrinter printer(QPrinter::HighResolution);
		printer.setOutputFormat(QPrinter::PdfFormat);
		printer.setOutputFileName(file);

	currentTextEdit()->print(&printer);
}

void EditeurDeTexte::copy()
{
	currentTextEdit()->copy();
}

void EditeurDeTexte::cut()
{
	currentTextEdit()->cut();
}

void EditeurDeTexte::paste()
{
	currentTextEdit()->paste();
}

void EditeurDeTexte::selectAll()
{
	currentTextEdit()->selectAll();
}

void EditeurDeTexte::insertImage()
{
	QString image = QFileDialog::getOpenFileName(this, "Multiuso", Multiuso::lastPath(),
			"Image (*.*)");

	if (image.isNull())
		return;

	Multiuso::setLastPath(image);
	
	QStringList suffixes;
		suffixes << "png" << "jpg" << "jpeg" << "bmp" << "gif" << "pbm"
			<< "pgm" << "ppm" << "xbm" << "xpm" << "svg";
	
	QPixmap pixmap(image);

	if (!suffixes.contains(QFileInfo(image).suffix().toLower())
			|| pixmap.isNull())
	{
		QMessageBox::critical(this, "Multiuso", "Image invalide !");

		return;
	}

	if (currentTextEdit()->currentFileName() == "NONE" && !currentTextEdit()->documentTitle().endsWith(".odt"))
	{
		int answer = QMessageBox::question(this, "Multiuso", "Ce document n'a pas encore été enregistré "
			"et de ce fait, Multiuso ne peut connaître le répertoire dans lequel il doit copier l'image.<br />"
			"Voulez-vous enregistrer le fichier ?", QMessageBox::Yes | QMessageBox::No);

		if (answer == QMessageBox::No)
			return;

		if (!saveFileAs())
			return;
	}

	QString path;

	if (!currentTextEdit()->documentTitle().endsWith(".odt"))
	{
       		path = QFileInfo(currentTextEdit()->currentFileName()).path();
		path = Multiuso::addSlash(path) + QFileInfo(currentTextEdit()->currentFileName()).baseName();
		path = Multiuso::addSlash(path);
	}

	else // If the file is an ODF file, we can read it anymore, so we don't need to save the file.
	{
		path = Multiuso::addSlash(Multiuso::tempPath());
	}

	QDir().mkpath(path);

	QString fileName = "{" + QString::number(Multiuso::randomNumber(1000, 4999)) + "}-"
				"{" + QString::number(Multiuso::randomNumber(5000, 9999)) + "}.png";

	pixmap.save(path + fileName);

	QTextImageFormat textImage;
		textImage.setName(path + fileName);

	currentTextEdit()->textCursor().insertImage(textImage);
}

/*void EditeurDeTexte::repeatText()
{
}*/

void EditeurDeTexte::bold()
{
	QTextCharFormat format;
		format.setFontWeight(a_bold->isChecked() ? QFont::Bold : QFont::Normal);

	mergeTextCharFormat(format);
}

void EditeurDeTexte::italic()
{
	QTextCharFormat format;
		format.setFontItalic(a_italic->isChecked());

	mergeTextCharFormat(format);
}

void EditeurDeTexte::underline()
{
	QTextCharFormat format;
		format.setFontUnderline(a_underline->isChecked());

	mergeTextCharFormat(format);
}

void EditeurDeTexte::alignment(QAction *action)
{
	if (action == a_alignLeft)
		currentTextEdit()->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);

	else if (action == a_alignCenter)
		currentTextEdit()->setAlignment(Qt::AlignHCenter);

	else if (action == a_alignRight)
		currentTextEdit()->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);

	else if (action == a_alignJustify)
		currentTextEdit()->setAlignment(Qt::AlignJustify);
}

void EditeurDeTexte::fontSize(QString size)
{
	qreal fontPointSize = size.toFloat();

	if (fontPointSize > 0)
	{
		QTextCharFormat format;
			format.setFontPointSize(fontPointSize);
	
		mergeTextCharFormat(format);
	}
}

void EditeurDeTexte::font(QString font)
{
	QTextCharFormat format;
		format.setFontFamily(font);

	mergeTextCharFormat(format);
}

void EditeurDeTexte::selectColor()
{
	QColor old = currentTextEdit()->currentCharFormat().foreground().color();
	QColor color = QColorDialog::getColor(old, this, "Sélectionnez une couleur");

	if (!color.isValid())
		return;
	
	QTextCharFormat format;
		format.setForeground(color);

	mergeTextCharFormat(format);

	a_selectColor->setIcon(createColorIcon(color));
}

void EditeurDeTexte::selectBackgroundColor()
{
	QColor old = currentTextEdit()->currentCharFormat().background().color();
	QColor color = QColorDialog::getColor(old, this, "Sélectionnez une couleur");

	if (!color.isValid())
		return;

	QTextCharFormat format;
		format.setBackground(color);
	
	mergeTextCharFormat(format);

	a_selectBackgroundColor->setIcon(createBackgroundColorIcon(color));
}

void EditeurDeTexte::toUpper()
{
	QTextCursor cursor = currentTextEdit()->textCursor();

	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);
	
	QString text = cursor.selectedText().toUpper();
	cursor.insertText(text);
}

void EditeurDeTexte::toLower()
{
	QTextCursor cursor = currentTextEdit()->textCursor();

	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);

	QString text = cursor.selectedText().toLower();
	cursor.insertText(text);
}

void EditeurDeTexte::textChanged()
{
	QSettings settings(Multiuso::appDirPath() + "/ini/editeur_de_texte.ini", QSettings::IniFormat);

	if (settings.value("enregistrement/enregistrement_automatique").toBool())
	{
		saveFile();

		return;
	}

	tabWidget->setTabIcon(tabWidget->indexOf(currentTextEdit()), QIcon(":/icones/editeur_de_texte/non_enregistre.png"));
}

void EditeurDeTexte::currentCharFormatChanged(QTextCharFormat format)
{
	fontChanged(format.font());
	colorChanged(format.foreground().color());
	backgroundColorChanged(format.background().color().isValid() ? format.background().color() : Qt::white);
}

void EditeurDeTexte::cursorPositionChanged()
{
	alignmentChanged(currentTextEdit()->alignment());
}

void EditeurDeTexte::fontChanged(QFont font)
{
	a_fontSize->setCurrentIndex(a_fontSize->findText(QString::number(font.pointSize())));
	a_font->setCurrentIndex(a_font->findText(QFontInfo(font).family()));
	a_bold->setChecked(font.bold());
	a_italic->setChecked(font.italic());
	a_underline->setChecked(font.underline());
}

void EditeurDeTexte::colorChanged(QColor color)
{
	a_selectColor->setIcon(createColorIcon(color));
}

void EditeurDeTexte::backgroundColorChanged(QColor color)
{
	a_selectBackgroundColor->setIcon(createBackgroundColorIcon(color));
}

void EditeurDeTexte::alignmentChanged(Qt::Alignment alignment)
{
	a_alignLeft->setChecked(alignment & Qt::AlignLeft);
	a_alignCenter->setChecked(alignment & Qt::AlignHCenter);
	a_alignRight->setChecked(alignment & Qt::AlignRight);
	a_alignJustify->setChecked(alignment & Qt::AlignJustify);
}

void EditeurDeTexte::currentChanged(int)
{
	currentCharFormatChanged(currentTextEdit()->currentCharFormat());
	cursorPositionChanged();
}
