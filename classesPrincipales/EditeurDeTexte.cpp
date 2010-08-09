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
			toolBar->addAction(a_undo);

	a_redo = new QAction("Rétablir", this);
		a_redo->setIcon(QIcon(":/icones/editeur_de_texte/retablir.png"));
		a_redo->setShortcut(QKeySequence("Ctrl+Y"));
			toolBar->addAction(a_redo);

	toolBar->addSeparator();
		
	a_delete = new QAction("Supprimer", this);
		a_delete->setIcon(QIcon(":/icones/editeur_de_texte/supprimer.png"));
		a_delete->setShortcut(QKeySequence("Suppr"));
			toolBar->addAction(a_delete);
			
	a_selectAll = new QAction("Tout sélectionner", this);
		a_selectAll->setIcon(QIcon(":/icones/editeur_de_texte/toutSelectionner.png"));
		a_selectAll->setShortcut(QKeySequence("Ctrl+A"));
			toolBar->addAction(a_selectAll);
			
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
			toolBar->addAction(a_printPreview);

	a_print = new QAction("Imprimer", this);
		a_print->setIcon(QIcon(":/icones/editeur_de_texte/imprimer.png"));
		a_print->setShortcut(QKeySequence("Ctrl+P"));
			toolBar->addAction(a_print);

	toolBar->addSeparator();
	
	a_copy = new QAction("Copier", this);
		a_copy->setIcon(QIcon(":/icones/editeur_de_texte/copier.png"));
		a_copy->setShortcut(QKeySequence("Ctrl+C"));
			toolBar->addAction(a_copy);

	a_cut = new QAction("Couper", this);
		a_cut->setIcon(QIcon(":/icones/editeur_de_texte/couper.png"));
		a_cut->setShortcut(QKeySequence("Ctrl+X"));
			toolBar->addAction(a_cut);

	a_paste = new QAction("Coller", this);
		a_paste->setIcon(QIcon(":/icones/editeur_de_texte/coller.png"));
		a_paste->setShortcut(QKeySequence("Ctrl+V"));
			toolBar->addAction(a_paste);
		
	toolBar->addSeparator();
	
	a_insertImage = new QAction("Insérer une image", this);
		a_insertImage->setIcon(QIcon(":/icones/editeur_de_texte/insererImage.png"));
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
			toolBar->addAction(a_bold);
			
	a_italic = new QAction("Italique", this);
		a_italic->setIcon(QIcon(":/icones/editeur_de_texte/italique.png"));
		a_italic->setCheckable(true);
			toolBar->addAction(a_italic);
			
	a_underline = new QAction("Souligné", this);
		a_underline->setIcon(QIcon(":/icones/editeur_de_texte/souligne.png"));
		a_underline->setCheckable(true);
			toolBar->addAction(a_underline);
	
	toolBar->addSeparator();

	a_alignLeft = new QAction("Gauche", this);
		a_alignLeft->setIcon(QIcon(":/icones/editeur_de_texte/gauche.png"));
		a_alignLeft->setCheckable(true);
			toolBar->addAction(a_alignLeft);

	a_alignCenter = new QAction("Centré", this);
		a_alignCenter->setIcon(QIcon(":/icones/editeur_de_texte/centre.png"));
		a_alignCenter->setCheckable(true);
			toolBar->addAction(a_alignCenter);

	a_alignRight = new QAction("Droite", this);
		a_alignRight->setIcon(QIcon(":/icones/editeur_de_texte/droite.png"));
		a_alignRight->setCheckable(true);
			toolBar->addAction(a_alignRight);

	a_alignJustify = new QAction("Justifié", this);
		a_alignJustify->setIcon(QIcon(":/icones/editeur_de_texte/justifie.png"));
		a_alignJustify->setCheckable(true);
			toolBar->addAction(a_alignJustify);
	
	toolBar->addSeparator();

	a_fontSize = new QComboBox;
		toolBar->addWidget(a_fontSize);

	a_font = new QFontComboBox;
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
			toolBar->addAction(a_toUpper);

	a_toLower = new QAction("en miniscule", this);
		a_toLower->setIcon(QIcon(":/icones/editeur_de_texte/selectionMinuscule.png"));
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
				painter.setRenderHint(QPainter::Antialiasing);
				painter.setPen(Qt::NoPen);
				painter.setBrush(color);
				painter.drawRoundedRect(0, 0, 32, 32, (qreal) 5, (qreal) 5);
			painter.end();
			painter.begin(&ellipse);
				painter.setRenderHint(QPainter::Antialiasing);
				painter.setPen(Qt::NoPen);
				painter.setBrush(Qt::white);
				painter.setOpacity(0.3);
				painter.drawEllipse(-(32 - (32 / 6)),
							-(32 + (32 / 3)),
								32 * 2, 32 * 2);
			painter.end();
			painter.begin(&icon);
				painter.setRenderHint(QPainter::Antialiasing);
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
				painter.setRenderHint(QPainter::Antialiasing);
				painter.setPen(Qt::NoPen);
				painter.setBrush(color);
				painter.drawRoundedRect(0, 0, 32, 32, (qreal) 5, (qreal) 5);
			painter.end();
			painter.begin(&ellipse);
				painter.setRenderHint(QPainter::Antialiasing);
				painter.setPen(Qt::NoPen);
				painter.setBrush(Qt::white);
				painter.setOpacity(0.3);
				painter.drawEllipse(-(32 - (32 / 6)),
							-(32 + (32 / 3)),
								32 * 2, 32 * 2);
			painter.end();
			painter.begin(&icon);
				painter.setRenderHint(QPainter::Antialiasing);
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
		int answer = QMessageBox::warning(this, "Multiuso", "Tous les changements non sauvegardés apportés à "
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

void EditeurDeTexte::saveFile()
{
	if (!currentTextEdit()->isSavable())
		return;

	if (currentTextEdit()->currentFileName() == "NONE")
		saveFileAs();

	else
		saveFile(currentTextEdit()->currentFileName());
}

void EditeurDeTexte::saveFile(QString file)
{
	if (!currentTextEdit()->isSavable())
		return;

	setCursor(Qt::WaitCursor);
	
	QTextDocumentWriter writer(file);

		if (!writer.write(currentTextEdit()->document()))
		{
			QMessageBox::critical(this, "Multiuso", "Impossible d'écrire dans le fichier :<br /><em>"
					+ QFileInfo(file).fileName() + "</em>");

			setCursor(Qt::ArrowCursor);
		
			return;
		}

	currentTextEdit()->setDocumentTitle(QFileInfo(file).fileName());
	currentTextEdit()->setCurrentFileName(QFileInfo(file).absoluteFilePath());
	currentTextEdit()->document()->setModified(false);

	tabWidget->setTabText(tabWidget->indexOf(currentTextEdit()), QFileInfo(file).fileName());
	tabWidget->setTabIcon(tabWidget->indexOf(currentTextEdit()), QIcon(":/icones/editeur_de_texte/enregistre.png"));

	setCursor(Qt::ArrowCursor);
}

void EditeurDeTexte::saveFileAs()
{
	if (!currentTextEdit()->isSavable())
		return;

	QString fileName = currentTextEdit()->documentTitle();

	QString file = QFileDialog::getSaveFileName(this, "Multiuso", Multiuso::lastPath() + "/" + fileName,
			"Fichier HTML (*.html *.htm);;Texte ODF (*.odt);;Tous les fichiers (*)");

	if (file.isEmpty())
		return;

	if (!(file.endsWith(".html", Qt::CaseInsensitive)
			|| file.endsWith(".htm", Qt::CaseInsensitive)
			|| file.endsWith(".odt", Qt::CaseInsensitive)))
				file += ".html";

	Multiuso::setLastPath(file);
	saveFile(file);
}

void EditeurDeTexte::selectColor()
{
	QColor old = currentTextEdit()->currentCharFormat().foreground().color();
	QColor color = QColorDialog::getColor(old, this, "Sélectionnez une couleur", QColorDialog::ShowAlphaChannel);

	if (!color.isValid())
		return;

	a_selectColor->setIcon(createColorIcon(color));
}

void EditeurDeTexte::selectBackgroundColor()
{
	QColor old = currentTextEdit()->currentCharFormat().background().color();
	QColor color = QColorDialog::getColor(old, this, "Sélectionnez une couleur", QColorDialog::ShowAlphaChannel);

	if (!color.isValid())
		return;

	a_selectBackgroundColor->setIcon(createBackgroundColorIcon(color));
}

void EditeurDeTexte::currentCharFormatChanged(QTextCharFormat format)
{
	fontChanged(format.font());
	colorChanged(format.foreground().color());
	backgroundColorChanged(format.background().color());
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
