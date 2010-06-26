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

	actionZoomPlus = new QAction("Zoom avant", this);
		actionZoomPlus->setIcon(QIcon(":/icones/visionneur_images/actionZoomPlus.png"));
		actionZoomPlus->setShortcut(QKeySequence("Ctrl++"));
		connect(actionZoomPlus, SIGNAL(triggered()), this, SLOT(slotZoomPlus()));

	actionZoomNormal = new QAction("Taille normale", this);
		actionZoomNormal->setIcon(QIcon(":/icones/visionneur_images/actionZoomNormal.png"));
		actionZoomNormal->setShortcut(QKeySequence("Ctrl+0"));
		connect(actionZoomNormal, SIGNAL(triggered()), this, SLOT(slotZoomNormal()));
		
	actionZoomIdeal = new QAction("Taille idéale", this);
		actionZoomIdeal->setIcon(QIcon(":/icones/visionneur_images/actionZoomIdeal.png"));
		actionZoomIdeal->setShortcut(QKeySequence("F"));
		connect(actionZoomIdeal, SIGNAL(triggered()), this, SLOT(slotZoomIdeal()));

	actionZoomMoins = new QAction("Zoom arrière", this);
		actionZoomMoins->setIcon(QIcon(":/icones/visionneur_images/actionZoomMoins.png"));
		actionZoomMoins->setShortcut(QKeySequence("Ctrl+-"));
		connect(actionZoomMoins, SIGNAL(triggered()), this, SLOT(slotZoomMoins()));

	actionRotateLeft = new QAction("Faire pivoter à gauche", this);
		actionRotateLeft->setIcon(QIcon(":/icones/visionneur_images/actionRotateLeft.png"));
		connect(actionRotateLeft, SIGNAL(triggered()), this, SLOT(slotRotateLeft()));

	actionRotateRight = new QAction("Faire pivoter à droite", this);
		actionRotateRight->setIcon(QIcon(":/icones/visionneur_images/actionRotateRight.png"));
		connect(actionRotateRight, SIGNAL(triggered()), this, SLOT(slotRotateRight()));

	QToolBar *toolBar = addToolBar("Options");
		toolBar->addAction(actionFermer);
		toolBar->addAction(actionOuvrir);
		toolBar->addSeparator();
		toolBar->addAction(actionZoomPlus);
		toolBar->addAction(actionZoomNormal);
		toolBar->addAction(actionZoomIdeal);
		toolBar->addAction(actionZoomMoins);
		toolBar->addSeparator();
		toolBar->addAction(actionRotateLeft);
		toolBar->addAction(actionRotateRight);
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
		connect(onglets, SIGNAL(currentChanged(int)), this, SLOT(slotTabIndexChanged(int)));
			slotNouvelOnglet();

	setCentralWidget(onglets);

	filterWidget = new FilterWidget;
		connect(filterWidget, SIGNAL(newPictureAvailable(QPixmap)), this, SLOT(slotApplyEffects(QPixmap)));
		connect(filterWidget, SIGNAL(openFileRequested(QString)), this, SLOT(slotOuvrirFichier(QString)));
		slotTabIndexChanged(0);

	dockFilters = new QDockWidget("Filtres", this);
		dockFilters->setObjectName("Filtres");
		dockFilters->setWidget(filterWidget);
		addDockWidget(Qt::LeftDockWidgetArea, dockFilters);

	QSettings reglages(Multiuso::appDirPath() + "/reglages/visionneur_images.ini", QSettings::IniFormat);
		restoreState(reglages.value("etat_fenetre").toByteArray());
}

bool VisionneurImages::needNewTab()
{
	bool newTab = true;

	Picture *picture = onglets->widget(onglets->count() - 1)->findChild<Picture *>();

	if (picture == 0)
		return newTab;
	
	QString slashToAdd = "";

	if (Multiuso::currentOS() == "windows")
		slashToAdd = "/";

	if (picture->imgPath() == "file://" + slashToAdd + ":/images/fond_visionneur_images.png")
		newTab = false;
	
	if (picture->imgPath() == "file://" + slashToAdd + ":/images/fond_erreur_ouverture.png")
		newTab = false;

	return newTab;
}

void VisionneurImages::switchToLastIndex()
{
	onglets->setCurrentIndex(onglets->count() - 1);
}

Picture *VisionneurImages::currentLabel()
{
	return onglets->currentWidget()->findChild<Picture *>();
}

ScrollArea *VisionneurImages::currentScrollArea()
{
	return onglets->currentWidget()->findChild<ScrollArea *>();
}

void VisionneurImages::slotOuvrir()
{
	QString image = QFileDialog::getOpenFileName(this, "Multiuso", Multiuso::lastPath(),
			"Image (*.*)");
				
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
	QString slashToAdd = "";

	if (Multiuso::currentOS() == "windows")
		slashToAdd = "/";

	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_visionneur_images.png")
		return;
	
	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_erreur_ouverture.png")
		return;

	zoomer(1.2); // 120%
}

void VisionneurImages::slotZoomNormal()
{
	QString slashToAdd = "";

	if (Multiuso::currentOS() == "windows")
		slashToAdd = "/";

	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_visionneur_images.png")
		return;
	
	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_erreur_ouverture.png")
		return;

	currentLabel()->adjustSize();
}

void VisionneurImages::slotZoomIdeal()
{
	QString slashToAdd = "";

	if (Multiuso::currentOS() == "windows")
		slashToAdd = "/";

	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_visionneur_images.png")
		return;
	
	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_erreur_ouverture.png")
		return;

	slotZoomNormal();

	if (currentLabel()->width() < currentScrollArea()->width()
		&& currentLabel()->width() < currentScrollArea()->width())
	{
		currentLabel()->setZoom(1.0);
		
		return;
	}

	QSize size = currentLabel()->size();
		size.scale(currentScrollArea()->size(), Qt::KeepAspectRatio);
		size *= 0.95; // 95%

	currentLabel()->resize(size);

	currentLabel()->setZoom(1.0);
}

void VisionneurImages::slotZoomMoins()
{
	QString slashToAdd = "";

	if (Multiuso::currentOS() == "windows")
		slashToAdd = "/";

	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_visionneur_images.png")
		return;
	
	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_erreur_ouverture.png")
		return;

	zoomer(0.8); // 80%
}

void VisionneurImages::slotRotateLeft()
{
	QString slashToAdd = "";

	if (Multiuso::currentOS() == "windows")
		slashToAdd = "/";

	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_visionneur_images.png")
		return;
	
	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_erreur_ouverture.png")
		return;

	QTransform transform;
		transform.rotate(-90);

	QPixmap newPixmap = *currentLabel()->pixmap();
		newPixmap = newPixmap.transformed(transform);

	currentLabel()->setPixmap(newPixmap);

	slotZoomIdeal();
}

void VisionneurImages::slotRotateRight()
{
	QString slashToAdd = "";

	if (Multiuso::currentOS() == "windows")
		slashToAdd = "/";

	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_visionneur_images.png")
		return;
	
	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_erreur_ouverture.png")
		return;

	QTransform transform;
		transform.rotate(90);

	QPixmap newPixmap = *currentLabel()->pixmap();
		newPixmap = newPixmap.transformed(transform);

	currentLabel()->setPixmap(newPixmap);
	
	slotZoomIdeal();
}

void VisionneurImages::slotOuvrirFichier(QString fichier)
{
	if (currentLabel()->imgPath() == fichier) // If the open-requested file is the current shown file
		return;
	
	QStringList suffixes;
		suffixes << "png" << "jpg" << "jpeg" << "bmp" << "gif"<< "pbm"
			<< "pgm" << "ppm" << "xbm" << "xpm" << "svg";

	if (!suffixes.contains(QFileInfo(fichier).suffix().toLower())) // If the file to show isn't a valid picture...
		fichier = ":/images/fond_erreur_ouverture.png"; // ...we show an error

	QIcon image(fichier); // Only used for the tab icon.

	if (image.isNull())
		return;

	currentLabel()->setImage(fichier);
	currentLabel()->adjustSize();

	slotZoomIdeal();
	
	QString name = QFileInfo(fichier).fileName();

	if (fichier == ":/images/fond_visionneur_images.png")
		name = "(aucune image)";

	else if (fichier == ":/images/fond_erreur_ouverture.png")
		name = "(erreur)";

	if (name.length() > 20)
		name = name.left(17) + "...";

	onglets->setTabText(onglets->currentIndex(), name);
	onglets->setTabIcon(onglets->currentIndex(), image);
	slotTabIndexChanged(0);
}

void VisionneurImages::slotOpenFileFromDrop(QUrl url)
{
	if (currentLabel()->imgPath() == url.toString())
		return;

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

void VisionneurImages::slotApplyEffects(QPixmap pixmap)
{
	currentLabel()->setPixmap(pixmap);
	slotZoomIdeal();
}

void VisionneurImages::zoomer(double facteurZoom)
{
	currentLabel()->setZoom(currentLabel()->zoom() * facteurZoom);

	currentLabel()->resize(currentLabel()->zoom() * currentLabel()->pixmap()->size());

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
		labelImage->setBackgroundRole(QPalette::Midlight);
		labelImage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
		labelImage->setScaledContents(true);
		connect(labelImage, SIGNAL(openFileFromDrop(QUrl)), this, SLOT(slotOpenFileFromDrop(QUrl)));

	ScrollArea *area = new ScrollArea;
		area->setBackgroundRole(QPalette::Midlight);
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

void VisionneurImages::slotTabIndexChanged(int)
{
	QString slashToAdd = "";

	if (Multiuso::currentOS() == "windows")
		slashToAdd = "/";

	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_visionneur_images.png")
	{
		if (filterWidget)
			filterWidget->setPixmap(0);

		return;
	}
	
	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_erreur_ouverture.png")
	{
		if (filterWidget)
			filterWidget->setPixmap(0);

		return;
	}

	if (filterWidget && !QPixmap(currentLabel()->imgPath().remove("file://" + slashToAdd)).isNull())
		filterWidget->setPixmap(QPixmap(currentLabel()->imgPath().remove("file://" + slashToAdd)));
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
