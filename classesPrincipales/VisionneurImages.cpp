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
	actionOuvrir = new QAction("Ouvrir", this);
		actionOuvrir->setIcon(QIcon(":/icones/visionneur_images/actionOuvrir"));
		actionOuvrir->setShortcut(QKeySequence("Ctrl+O"));
		connect(actionOuvrir, SIGNAL(triggered()), this, SLOT(slotOuvrir()));

	actionFermer = new QAction("Fermer", this);
		actionFermer->setIcon(QIcon(":/icones/visionneur_images/actionFermer"));
		actionFermer->setShortcut(QKeySequence("Ctrl+X"));
		connect(actionFermer, SIGNAL(triggered()), this, SLOT(slotFermer()));

	actionZoomPlus = new QAction("Zoom : +", this);
		actionZoomPlus->setIcon(QIcon(":/icones/visionneur_images/actionZoomPlus"));
		actionZoomPlus->setShortcut(QKeySequence("Ctrl++"));
		connect(actionZoomPlus, SIGNAL(triggered()), this, SLOT(slotZoomPlus()));

	actionZoomNormal = new QAction("Zoom : =", this);
		actionZoomNormal->setIcon(QIcon(":/icones/visionneur_images/actionZoomNormal"));
		actionZoomNormal->setShortcut(QKeySequence("Ctrl+0"));
		connect(actionZoomNormal, SIGNAL(triggered()), this, SLOT(slotZoomNormal()));

	actionZoomMoins = new QAction("Zoom : -", this);
		actionZoomMoins->setIcon(QIcon(":/icones/visionneur_images/actionZoomMoins"));
		actionZoomMoins->setShortcut(QKeySequence("Ctrl+-"));
		connect(actionZoomMoins, SIGNAL(triggered()), this, SLOT(slotZoomMoins()));

	actionDiaporama = new QAction("Diaporama", this);
		actionDiaporama->setIcon(QIcon(":/icones/visionneur_images/actionDiaporama"));
		connect(actionDiaporama, SIGNAL(triggered()), this, SLOT(slotDiaporama()));

	QToolBar *toolBar = addToolBar("Options");
		toolBar->addAction(actionOuvrir);
		toolBar->addAction(actionFermer);
		toolBar->addSeparator();
		toolBar->addAction(actionZoomPlus);
		toolBar->addAction(actionZoomNormal);
		toolBar->addAction(actionZoomMoins);
		toolBar->addSeparator();
		toolBar->addAction(actionDiaporama);
		toolBar->setObjectName("Options");
		toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	labelImage = new QLabel;
		labelImage->setBackgroundRole(QPalette::Base);
		labelImage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
		labelImage->setScaledContents(true);

	area = new QScrollArea;
		area->setBackgroundRole(QPalette::Base);
		area->setWidget(labelImage);
		area->setAlignment(Qt::AlignCenter);

	setCentralWidget(area);

	QSettings reglages(Multiuso::appDirPath() + "/reglages/visionneur_images.ini", QSettings::IniFormat);
		restoreState(reglages.value("etat_fenetre").toByteArray());

	slotOuvrirFichier(":/images/fond_visionneur_images");

	timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(changerDiapo()));
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
	slotOuvrirFichier(":/images/fond_visionneur_images");
}

void VisionneurImages::slotZoomPlus()
{
	zoomer(1.25);
}

void VisionneurImages::slotZoomNormal()
{
	labelImage->adjustSize();

	zoom = 1.0;
}

void VisionneurImages::slotZoomMoins()
{
	zoomer(0.8);
}

void VisionneurImages::slotDiaporama()
{
	if (actionDiaporama->text() == "Diaporama")
	{
		dossierDiaporama = QFileDialog::getExistingDirectory(this, "Multiuso", Multiuso::lastPath());
		
		Multiuso::setLastPath(dossierDiaporama);

		if (dossierDiaporama.isEmpty())
			return;

		QDir dir(dossierDiaporama);

		diapos = dir.entryList();
			diapos.removeOne(".");
			diapos.removeOne("..");

		diapoNumeroX = 0;

		int delai = QInputDialog::getInt(this, "Multiuso", "Délai entre les images (en secondes) :", 7, 1, 3600, 1);

		changerDiapo();

		timer->start(delai * 1000);

		actionDiaporama->setText("Arrêter le diaporama");
		actionDiaporama->setIcon(QIcon(":/icones/visionneur_images/actionDiaporamaStop"));
	}

	else
	{
		timer->stop();

		diapos.clear();

		actionDiaporama->setText("Diaporama");
		actionDiaporama->setIcon(QIcon(":/icones/visionneur_images/actionDiaporama"));

		slotOuvrirFichier(":/images/fond_visionneur_images");
	}
}

void VisionneurImages::slotOuvrirFichier(QString fichier)
{
	QImage image(fichier);

	if (image.isNull())
		return;

	labelImage->setPixmap(QPixmap::fromImage(image));
	labelImage->adjustSize();

	zoom = 1.0;
}

void VisionneurImages::zoomer(double facteurZoom)
{
	zoom *= facteurZoom;

	labelImage->resize(zoom * labelImage->pixmap()->size());

	ajusterScrollBar(area->horizontalScrollBar(), facteurZoom);
	ajusterScrollBar(area->verticalScrollBar(), facteurZoom);
}

void VisionneurImages::changerDiapo()
{
	QString imageAAfficher = dossierDiaporama + "/" + diapos.value(diapoNumeroX);

	slotOuvrirFichier(imageAAfficher);

	diapoNumeroX++;

	if (diapoNumeroX > diapos.size() - 1)
		diapoNumeroX = 0;
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
