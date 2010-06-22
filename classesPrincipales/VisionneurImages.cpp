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

#include "VisionneurImages.h"

VisionneurImages::VisionneurImages(QWidget *parent = 0) : QMainWindow(parent)
{
	actionAddTab = new QAction("Ajouter un onglet", this);
		actionAddTab->setIcon(QIcon(":/icones/visionneur_images/actionAddTab.png"));
		actionAddTab->setShortcut(QKeySequence("Ctrl+T"));
		connect(actionAddTab, SIGNAL(triggered()), this, SLOT(slotNouvelOnglet()));

	actionRemoveTab = new QAction("Supprimer un onglet", this);
		actionRemoveTab->setIcon(QIcon(":/icones/visionneur_images/actionRemoveTab.png"));
		actionRemoveTab->setShortcut(QKeySequence("Ctrl+W"));
		connect(actionRemoveTab, SIGNAL(triggered()), this, SLOT(slotRemoveTab()));

	actionFermer = new QAction("Fermer", this);
		actionFermer->setIcon(QIcon(":/icones/visionneur_images/actionFermer.png"));
		actionFermer->setShortcut(QKeySequence("Ctrl+X"));
		connect(actionFermer, SIGNAL(triggered()), this, SLOT(slotFermer()));

	actionOuvrir = new QAction("Ouvrir", this);
		actionOuvrir->setIcon(QIcon(":/icones/visionneur_images/actionOuvrir.png"));
		actionOuvrir->setShortcut(QKeySequence("Ctrl+O"));
		connect(actionOuvrir, SIGNAL(triggered()), this, SLOT(slotOuvrir()));

	actionZoomPlus = new QAction("Zoom : +", this);
		actionZoomPlus->setIcon(QIcon(":/icones/visionneur_images/actionZoomPlus.png"));
		actionZoomPlus->setShortcut(QKeySequence("Ctrl++"));
		connect(actionZoomPlus, SIGNAL(triggered()), this, SLOT(slotZoomPlus()));

	actionZoomNormal = new QAction("Zoom : =", this);
		actionZoomNormal->setIcon(QIcon(":/icones/visionneur_images/actionZoomNormal.png"));
		actionZoomNormal->setShortcut(QKeySequence("Ctrl+0"));
		connect(actionZoomNormal, SIGNAL(triggered()), this, SLOT(slotZoomNormal()));

	actionZoomMoins = new QAction("Zoom : -", this);
		actionZoomMoins->setIcon(QIcon(":/icones/visionneur_images/actionZoomMoins.png"));
		actionZoomMoins->setShortcut(QKeySequence("Ctrl+-"));
		connect(actionZoomMoins, SIGNAL(triggered()), this, SLOT(slotZoomMoins()));
	
	QToolBar *toolBar = addToolBar("Options");
		toolBar->addAction(actionFermer);
		toolBar->addAction(actionOuvrir);
		toolBar->addSeparator();
		toolBar->addAction(actionZoomPlus);
		toolBar->addAction(actionZoomNormal);
		toolBar->addAction(actionZoomMoins);
		toolBar->setObjectName("Options");
		toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		
	QToolButton *buttonAddTab = new QToolButton;
		buttonAddTab->setDefaultAction(actionAddTab);
		buttonAddTab->setAutoRaise(true);
		
	QToolButton *buttonRemoveTab = new QToolButton;
		buttonRemoveTab->setDefaultAction(actionRemoveTab);
		buttonRemoveTab->setAutoRaise(true);

	onglets = new QTabWidget;
		onglets->setMovable(true);
		onglets->setDocumentMode(true);
		onglets->setCornerWidget(buttonAddTab, Qt::BottomLeftCorner);
		onglets->setCornerWidget(buttonRemoveTab, Qt::BottomRightCorner);
		connect(onglets, SIGNAL(tabCloseRequested(int)), this, SLOT(slotRemoveTab(int)));
			slotNouvelOnglet();

	setCentralWidget(onglets);

	QSettings reglages(Multiuso::appDirPath() + "/reglages/visionneur_images.ini", QSettings::IniFormat);
		restoreState(reglages.value("etat_fenetre").toByteArray());
}

bool VisionneurImages::needNewTab()
{
	bool newTab = true;

	if (onglets->tabText(onglets->count() - 1) == "(aucune image)")
		newTab = false;

	return newTab;
}

void VisionneurImages::switchToLastIndex()
{
	onglets->setCurrentIndex(onglets->count() - 1);
}

QLabel *VisionneurImages::currentLabel()
{
	return onglets->currentWidget()->findChild<QLabel *>();
}

QScrollArea *VisionneurImages::currentScrollArea()
{
	return onglets->currentWidget()->findChild<QScrollArea *>();
}

void VisionneurImages::slotOuvrir()
{
	QString image = QFileDialog::getOpenFileName(this, "Multiuso", Multiuso::lastPath(),
			"Image (*.png *.jpg *.jpeg *.bmp *.gif *.pbm *.pgm *.ppm *.xbm *.xpm *.svg"
				"*.PNG *.JPG *.JPEG *.BMP *.GIF *.PBM *.PGM *.PPM *.XBM *.XPM *.SVG");
				
	Multiuso::setLastPath(image);

	if (!image.isNull())
		slotOuvrirFichier(image);
}

void VisionneurImages::slotFermer()
{
	slotOuvrirFichier(":/images/fond_visionneur_images.png");
}

void VisionneurImages::slotZoomPlus()
{
	zoomer(1.25);
}

void VisionneurImages::slotZoomNormal()
{
	currentLabel()->adjustSize();

	zoom = 1.0;
}

void VisionneurImages::slotZoomMoins()
{
	zoomer(0.8);
}

void VisionneurImages::slotOuvrirFichier(QString fichier)
{
	QImage image(fichier);

	if (image.isNull())
		return;

	currentLabel()->setPixmap(QPixmap::fromImage(image));
	currentLabel()->adjustSize();

	QString name = QFileInfo(fichier).fileName();

	if (fichier == ":/images/fond_visionneur_images.png")
		name = "(aucune image)";

	if (name.length() > 20)
		name = name.left(17) + "...";

	onglets->setTabText(onglets->currentIndex(), name);
	onglets->setTabIcon(onglets->currentIndex(), QIcon(fichier));

	zoom = 1.0;
}

void VisionneurImages::slotOpenFileFromDrop(QUrl url)
{
	QString urlPath = url.path();
	
	if (Multiuso::currentOS() == "windows")
		urlPath = urlPath.right(urlPath.length() - 1);

	if (QFileInfo(urlPath).exists())
	{
		if (needNewTab())
			slotNouvelOnglet();

		else
			switchToLastIndex();

		slotOuvrirFichier(urlPath);
	}
}

void VisionneurImages::zoomer(double facteurZoom)
{
	zoom *= facteurZoom;

	currentLabel()->resize(zoom * currentLabel()->pixmap()->size());

	ajusterScrollBar(currentScrollArea()->horizontalScrollBar(), facteurZoom);
	ajusterScrollBar(currentScrollArea()->verticalScrollBar(), facteurZoom);
}

void VisionneurImages::ajusterScrollBar(QScrollBar *scrollBar, double facteurZoom)
{
	scrollBar->setValue(int(facteurZoom * scrollBar->value() + ((facteurZoom - 1) * scrollBar->pageStep() / 2)));
}

void VisionneurImages::sauvegarderEtat()
{
	QSettings reglages(Multiuso::appDirPath() + "/reglages/visionneur_images.ini", QSettings::IniFormat);
		reglages.setValue("etat_fenetre", saveState());
}

QWidget *VisionneurImages::newTab()
{
	Picture *labelImage = new Picture;
		labelImage->setBackgroundRole(QPalette::Base);
		labelImage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
		labelImage->setScaledContents(true);
		connect(labelImage, SIGNAL(openFileFromDrop(QUrl)), this, SLOT(slotOpenFileFromDrop(QUrl)));

	ScrollArea *area = new ScrollArea;
		area->setBackgroundRole(QPalette::Base);
		area->setWidget(labelImage);
		area->setAlignment(Qt::AlignCenter);
		connect(area, SIGNAL(openFileFromDrop(QUrl)), this, SLOT(slotOpenFileFromDrop(QUrl)));

	QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(area);
		layout->setContentsMargins(0, 0, 0, 0);

	QWidget *widget = new QWidget;
		widget->setLayout(layout);

	return widget;
}

void VisionneurImages::slotNouvelOnglet()
{
	onglets->addTab(newTab(), "");
	onglets->setCurrentIndex(onglets->count() - 1);

	slotOuvrirFichier(":/images/fond_visionneur_images.png");

	if (onglets->count() > 1)
	{
		onglets->setTabsClosable(true);
		actionRemoveTab->setEnabled(true);
	}

	else
	{
		onglets->setTabsClosable(false);
		actionRemoveTab->setEnabled(false);
	}
}

void VisionneurImages::slotRemoveTab()
{
	slotRemoveTab(onglets->currentIndex());
}

void VisionneurImages::slotRemoveTab(int index)
{
	onglets->widget(index)->deleteLater();
	onglets->removeTab(index);
	
	if (onglets->count() > 1)
	{
		onglets->setTabsClosable(true);
		actionRemoveTab->setEnabled(true);
	}

	else
	{
		onglets->setTabsClosable(false);
		actionRemoveTab->setEnabled(false);
	}
}
