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
		tabWidget->setTabsClosable(true);

	setCentralWidget(tabWidget);
}

QToolBar *EditeurDeTexte::createFirstToolBar()
{
	QToolBar *toolBar = addToolBar("Actions");
		toolBar->setObjectName("Actions");

	a_new = new QAction("Nouveau", this);
		a_new->setIcon(QIcon(":/icones/editeur_de_texte/nouveau.png"));
		a_new->setShortcut(QKeySequence("Ctrl+N"));
			toolBar->addAction(a_new);

	a_open = new QAction("Ouvrir", this);
		a_open->setIcon(QIcon(":/icones/editeur_de_texte/ouvrir.png"));
		a_open->setShortcut(QKeySequence("Ctrl+O"));
			toolBar->addAction(a_open);

	a_save = new QAction("Enregistrer", this);
		a_save->setIcon(QIcon(":/icones/editeur_de_texte/enregistrer.png"));
		a_save->setShortcut(QKeySequence("Ctrl+S"));
			toolBar->addAction(a_save);

	a_saveAs = new QAction("Enregistrer sous...", this);
		a_saveAs->setIcon(QIcon(":/icones/editeur_de_texte/enregistrerSous.png"));
		a_saveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));
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
			
	a_search = new QAction("Rechercher", this);
		a_search->setIcon(QIcon(":/icones/editeur_de_texte/rechercher.png"));
		a_search->setShortcut(QKeySequence("Ctrl+F"));
			toolBar->addAction(a_search);
			
	a_replace = new QAction("Remplacer", this);
		a_replace->setIcon(QIcon(":/icones/editeur_de_texte/rechercherRemplacer.png"));
		a_replace->setShortcut(QKeySequence("Ctrl+H"));
			toolBar->addAction(a_replace);

	a_printPreview = new QAction("Aperçu avant impression", this);
		a_printPreview->setIcon(QIcon(":/icones/editeur_de_texte/imprimer.png"));
			toolBar->addAction(a_printPreview);

	a_print = new QAction("Imprimer", this);
		a_print->setIcon(QIcon(":/icones/editeur_de_texte/imprimer.png"));
		a_print->setShortcut(QKeySequence("Ctrl+P"));
			toolBar->addAction(a_print);

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
		
	a_selectAll = new QAction("Tout sélectionner", this);
		a_selectAll->setIcon(QIcon(":/icones/editeur_de_texte/toutSelectionner.png"));
		a_selectAll->setShortcut(QKeySequence("Ctrl+A"));
			toolBar->addAction(a_selectAll);

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
			toolBar->addAction(a_bold);
			
	a_italic = new QAction("Italique", this);
		a_italic->setIcon(QIcon(":/icones/editeur_de_texte/italique.png"));
			toolBar->addAction(a_italic);
			
	a_underline = new QAction("Souligné", this);
		a_underline->setIcon(QIcon(":/icones/editeur_de_texte/souligne.png"));
			toolBar->addAction(a_underline);

	a_alignLeft = new QAction("Gauche", this);
		a_alignLeft->setIcon(QIcon(":/icones/editeur_de_texte/gauche.png"));
			toolBar->addAction(a_alignLeft);

	a_alignCenter = new QAction("Centré", this);
		a_alignCenter->setIcon(QIcon(":/icones/editeur_de_texte/centre.png"));
			toolBar->addAction(a_alignCenter);

	a_alignRight = new QAction("Droite", this);
		a_alignRight->setIcon(QIcon(":/icones/editeur_de_texte/droite.png"));
			toolBar->addAction(a_alignRight);

	a_alignJustify = new QAction("Justifié", this);
		a_alignJustify->setIcon(QIcon(":/icones/editeur_de_texte/justifie.png"));
			toolBar->addAction(a_alignJustify);

	a_selectColor = new QAction("Sélectionner une couleur", this);
		a_selectColor->setIcon(createColorIcon(Qt::black));
		connect(a_selectColor, SIGNAL(triggered()), this, SLOT(selectColor()));
			toolBar->addAction(a_selectColor);

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

void EditeurDeTexte::openFile(QString file)
{
	Q_UNUSED(file);
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

void EditeurDeTexte::selectColor()
{
	QColor old = Qt::black;

	QColor color = QColorDialog::getColor(old, this, "Sélectionner une couleur", QColorDialog::ShowAlphaChannel);

	if (!color.isValid())
		return;

	a_selectColor->setIcon(createColorIcon(color));
}
