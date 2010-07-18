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

// Filters

FilterWidget::FilterWidget()
{
	m_pixmap = *new QPixmap;
	m_isGif = false;

	QPushButton *buttonPhotoFilter = new QPushButton;
		buttonPhotoFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/photo_effect.png"));
		buttonPhotoFilter->setText("Effet photo");
	//	buttonPhotoFilter->setIconSize(QSize(32, 32));
		connect(buttonPhotoFilter, SIGNAL(clicked()), this, SLOT(slotPhotoFilter()));

	QPushButton *buttonColorFilter = new QPushButton;
		buttonColorFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/color_filter.png"));
		buttonColorFilter->setText("Filtre de couleur");
	//	buttonColorFilter->setIconSize(QSize(32, 32));
		connect(buttonColorFilter, SIGNAL(clicked()), this, SLOT(slotColorFilter()));

	QPushButton *buttonInvertColorsFilter = new QPushButton;
		buttonInvertColorsFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/inverser.png"));
		buttonInvertColorsFilter->setText("Inverser les couleurs");
	//	buttonInvertColorsFilter->setIconSize(QSize(32, 32));
		connect(buttonInvertColorsFilter, SIGNAL(clicked()), this, SLOT(slotInvertColorsFilter()));

	QPushButton *buttonMirrorHorizontalFilter = new QPushButton;
		buttonMirrorHorizontalFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/horizontal_mirror.png"));
		buttonMirrorHorizontalFilter->setText("Miroir horizontal");
	//	buttonMirrorHorizontalFilter->setIconSize(QSize(32, 32));
		connect(buttonMirrorHorizontalFilter, SIGNAL(clicked()), this, SLOT(slotMirrorHorizontalFilter()));

	QPushButton *buttonMirrorVerticalFilter = new QPushButton;
		buttonMirrorVerticalFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/vertical_mirror.png"));
		buttonMirrorVerticalFilter->setText("Miroir vertical");
	//	buttonMirrorVerticalFilter->setIconSize(QSize(32, 32));
		connect(buttonMirrorVerticalFilter, SIGNAL(clicked()), this, SLOT(slotMirrorVerticalFilter()));

	QPushButton *buttonReflectionFilter = new QPushButton;
		buttonReflectionFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/reflection.png"));
		buttonReflectionFilter->setText("Reflet");
	//	buttonReflectionFilter->setIconSize(QSize(32, 32));
		connect(buttonReflectionFilter, SIGNAL(clicked()), this, SLOT(slotReflectionFilter()));

	QPushButton *buttonBorderFilter = new QPushButton;
		buttonBorderFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/border.png"));
		buttonBorderFilter->setText("Bordure");
	//	buttonBorderFilter->setIconSize(QSize(32, 32));
		connect(buttonBorderFilter, SIGNAL(clicked()), this, SLOT(slotBorderFilter()));

	QPushButton *buttonRotationFilter = new QPushButton;
		buttonRotationFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/rotation.png"));
		buttonRotationFilter->setText("Rotation");
	//	buttonRotationFilter->setIconSize(QSize(32, 32));
		connect(buttonRotationFilter, SIGNAL(clicked()), this, SLOT(slotRotationFilter()));

	QPushButton *buttonResizeFilter = new QPushButton;
		buttonResizeFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/resize.png"));
		buttonResizeFilter->setText("Redimensionner");
	//	buttonResizeFilter->setIconSize(QSize(32, 32));
		connect(buttonResizeFilter, SIGNAL(clicked()), this, SLOT(slotResizeFilter()));

	QPushButton *buttonSave = new QPushButton("Enregistrer l'image");
		connect(buttonSave, SIGNAL(clicked()), this, SLOT(slotSave()));

	QPushButton *buttonSaveAs = new QPushButton("Enregistrer l'image sous...");
		connect(buttonSaveAs, SIGNAL(clicked()), this, SLOT(slotSaveAs()));

	QPushButton *buttonCancelChanges = new QPushButton("Annuler les modifications");
		connect(buttonCancelChanges, SIGNAL(clicked()), this, SIGNAL(cancelChanges()));

	QGridLayout *mainLayout = new QGridLayout(this);
		mainLayout->addWidget(buttonPhotoFilter, 0, 0, 1, 1);
		mainLayout->addWidget(buttonColorFilter, 1, 0, 1, 1);
		mainLayout->addWidget(buttonInvertColorsFilter, 2, 0, 1, 1);
		mainLayout->addWidget(buttonMirrorHorizontalFilter, 3, 0, 1, 1);
		mainLayout->addWidget(buttonMirrorVerticalFilter, 4, 0, 1, 1);
		mainLayout->addWidget(buttonReflectionFilter, 5, 0, 1, 1);
		mainLayout->addWidget(buttonBorderFilter, 6, 0, 1, 1);
		mainLayout->addWidget(buttonRotationFilter, 7, 0, 1, 1);
		mainLayout->addWidget(buttonResizeFilter, 8, 0, 1, 1);
		mainLayout->addWidget(new QLabel("<hr />"), 9, 0, 1, 1);
		mainLayout->addWidget(buttonSave, 10, 0, 1, 1);
		mainLayout->addWidget(buttonSaveAs, 11, 0, 1, 1);
		mainLayout->addWidget(new QLabel("<hr />"), 12, 0, 1, 1);
		mainLayout->addWidget(buttonCancelChanges, 13, 0, 1, 1);
		mainLayout->setAlignment(Qt::AlignTop);

	// Construction of the resize-picture dialog

	m_resizeDialog = new QDialog(this);
		m_resizeDialog->setWindowTitle("Multiuso");
		m_resizeDialog->setWindowIcon(QIcon(":/icones/visionneur_images/filtres/resize.png"));

	m_pictureX = new QSpinBox;
		m_pictureX->setSuffix("px");
		m_pictureX->setRange(1, 1000000);
		connect(m_pictureX, SIGNAL(editingFinished()), this, SLOT(resizePictureValueChangedX()));
		m_originalPictureX = 0;

	m_pictureY = new QSpinBox;
		m_pictureY->setSuffix("px");
		m_pictureY->setRange(1, 1000000);
		connect(m_pictureY, SIGNAL(editingFinished()), this, SLOT(resizePictureValueChangedY()));
		m_originalPictureY = 0;

	m_resizeProportional = new QPushButton(QIcon(":/icones/visionneur_images/filtres/resize_proportional.png"), "");
		m_resizeProportional->setCheckable(true);
		m_resizeProportional->setChecked(true);

	m_resizeReject = new QPushButton("Annuler");
		connect(m_resizeReject, SIGNAL(clicked()), m_resizeDialog, SLOT(reject()));

	m_resizeOk = new QPushButton("OK");
		connect(m_resizeOk, SIGNAL(clicked()), m_resizeDialog, SLOT(accept()));

	QGridLayout *resizeDialogLayout = new QGridLayout(m_resizeDialog);
		resizeDialogLayout->addWidget(new QLabel("Largeur : "), 0, 0, 1, 1);
		resizeDialogLayout->addWidget(m_pictureX, 0, 1, 1, 1);
		resizeDialogLayout->addWidget(new QLabel("Hauteur : "), 1, 0, 1, 1);
		resizeDialogLayout->addWidget(m_pictureY, 1, 1, 1, 1);
		resizeDialogLayout->addWidget(m_resizeProportional, 0, 2, 2, 1);
		resizeDialogLayout->addWidget(m_resizeReject, 2, 1, 1, 1);
		resizeDialogLayout->addWidget(m_resizeOk, 2, 2, 1, 1);
}

void FilterWidget::setPixmap(QPixmap pixmap)
{
	if (m_pixmap.paintingActive())
		return;

	if (!pixmap.isNull())
		m_pixmap = pixmap;

	else
		m_pixmap = *new QPixmap;
}

void FilterWidget::setIsGif(bool isGif)
{
	m_isGif = isGif;
}

void FilterWidget::slotPhotoFilter()
{
	if (m_pixmap.isNull() || m_isGif)
		return;

	bool ok;

	int radius = QInputDialog::getInt(this, "Multiuso", "Taille du rayon (en px) :<br />"
			"<em>Pour les coins arrondis (0 pour aucun arrondissement)</em>",
				0, 0, 250, 1, &ok);

	if (!ok)
		return;

	int borderWidth = 0;

	if (m_pixmap.width() > m_pixmap.height())
		borderWidth = m_pixmap.height() / 20;

	else
		borderWidth = m_pixmap.width() / 20;

	QPixmap newPixmap(m_pixmap.width() + (borderWidth * 2), m_pixmap.height() + (borderWidth * 2));
		newPixmap.fill(Qt::transparent);

	QPainter painter;
		painter.begin(&newPixmap); // First we add a white border to the "main" pixmap
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.setPen(Qt::white);
			painter.setBrush(Qt::white);
			painter.drawRoundedRect(0, 0, newPixmap.width(), newPixmap.height(), radius, radius);
			painter.drawPixmap(borderWidth, borderWidth, m_pixmap);
		painter.end();

	QPixmap reflectionPixmap(newPixmap.width() + (borderWidth * 1.5), newPixmap.height() + (borderWidth * 1.5)); // "+ (borderWidth) * 2" to have a greater ellipse
		reflectionPixmap.fill(Qt::transparent);

		painter.begin(&reflectionPixmap); // Then we draw an ellipse
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.setPen(Qt::NoPen);
			painter.setBrush(Qt::white);
			painter.setOpacity(0.3);
			painter.drawEllipse(-(reflectionPixmap.width() - (reflectionPixmap.width() / 6)),
				-(reflectionPixmap.height() + (reflectionPixmap.height() / 3)),
					reflectionPixmap.width() * 2, reflectionPixmap.height() * 2);
		painter.end();

		painter.begin(&newPixmap); // Finally we aply the ellipse of the "main" pixmap with rounded corners
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.setPen(Qt::NoPen);
			painter.setBrush(reflectionPixmap);
			painter.drawRoundedRect(0, 0, newPixmap.width(), newPixmap.height(), radius, radius);
		painter.end();

	m_pixmap = QPixmap(newPixmap);

	emit newPictureAvailable(newPixmap);
}

void FilterWidget::slotColorFilter()
{
	if (m_pixmap.isNull() || m_isGif)
		return;

	QColor color = QColorDialog::getColor(Qt::blue, this, "Multiuso");

	if (!color.isValid())
		return;

	bool ok;

	double opacity = QInputDialog::getDouble(this, "Multiuso",
		"Opacité du filtre :<br /><em>(0 : transparent, 100 : opaque)</em>", 40.0, 0.0, 100.0, 1, &ok);

	if (!ok || opacity == 0)
		return;

	int radius = QInputDialog::getInt(this, "Multiuso", "Taille du rayon (en px) :<br />"
		"<em>Pour les coins arrondis (0 pour aucun arrondissement)</em>",
			0, 0, 250, 1, &ok);

	if (!ok)
		return;

	opacity /= 100;

	QPainter painter;
		painter.begin(&m_pixmap);
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.setPen(Qt::NoPen);
			painter.setBrush(color);
			painter.setOpacity(opacity);
			painter.drawRoundedRect(0, 0, m_pixmap.width(), m_pixmap.height(), radius, radius);
		painter.end();

	emit newPictureAvailable(m_pixmap);
}

void FilterWidget::slotInvertColorsFilter()
{
	if (m_pixmap.isNull() || m_isGif)
		return;

	QImage image(m_pixmap.toImage());
		image.invertPixels();

	m_pixmap = QPixmap::fromImage(image);

	emit newPictureAvailable(m_pixmap);
}

void FilterWidget::slotMirrorHorizontalFilter()
{
	if (m_pixmap.isNull() || m_isGif)
		return;

	QImage image(m_pixmap.toImage());
		image = image.mirrored(true, false);

	m_pixmap = QPixmap::fromImage(image);

	emit newPictureAvailable(m_pixmap);
}

void FilterWidget::slotMirrorVerticalFilter()
{
	if (m_pixmap.isNull() || m_isGif)
		return;

	QImage image(m_pixmap.toImage());
		image = image.mirrored(false, true);

	m_pixmap = QPixmap::fromImage(image);

	emit newPictureAvailable(m_pixmap);
}

void FilterWidget::slotReflectionFilter()
{
	if (m_pixmap.isNull() || m_isGif)
		return;

	QColor color = QColorDialog::getColor(Qt::white, this, "Choisissez la couleur de fond");

	if (!color.isValid())
		return;

	bool ok;

	double percentOfPicture = QInputDialog::getDouble(this, "Multiuso",
		"Le reflet fait X\% de la hauteur de l'image originale :", 50.0, 0.0, 100.0, 1, &ok);

	if (!ok || percentOfPicture == 0)
		return;

	percentOfPicture /= 100;

	QImage image(m_pixmap.toImage());
		image = image.mirrored(false, true);

	QPixmap reflection = QPixmap::fromImage(image);
		reflection = reflection.copy(0, 0, reflection.width(), reflection.height() * percentOfPicture);

	QPixmap newPixmap(m_pixmap.width(), m_pixmap.height() + reflection.height());
		newPixmap.fill(Qt::transparent);

	QLinearGradient gradient(0, -(reflection.height() / 2), 0, reflection.height());
		gradient.setColorAt(0, Qt::transparent);
		gradient.setColorAt(1, color);

	QBrush gradientBrush(gradient);

	QPainter painter;
		painter.begin(&reflection);
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.fillRect(0, 0, reflection.width(), reflection.height(), gradientBrush);
		painter.end();
		painter.begin(&newPixmap);
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.drawPixmap(0, 0, m_pixmap);
			painter.drawPixmap(0, m_pixmap.height(), reflection);
		painter.end();

	m_pixmap = QPixmap(newPixmap);

	emit newPictureAvailable(m_pixmap);
}

void FilterWidget::slotBorderFilter()
{
	if (m_pixmap.isNull() || m_isGif)
		return;

	QColor color = QColorDialog::getColor(Qt::white, this, "Choisissez la couleur de la bordure", QColorDialog::ShowAlphaChannel);

	if (!color.isValid())
		return;

	bool ok;

	int borderWidth = QInputDialog::getInt(this, "Multiuso", "Épaisseur de la bordure (en px) :", 1, 1, 2147483647, 1, &ok);

	if (!ok)
		return;

	int radius = QInputDialog::getInt(this, "Multiuso", "Taille du rayon (en px) :<br />"
			"<em>Pour les coins arrondis (0 pour aucun arrondissement)</em>",
				0, 0, 250, 1, &ok);

	if (!ok)
		return;

	QPixmap newPixmap(m_pixmap.width() + (borderWidth * 2), m_pixmap.height() + (borderWidth * 2));
		newPixmap.fill(Qt::transparent);

	QPainter painter;
		painter.begin(&newPixmap);
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.setPen(Qt::NoPen);
			painter.setBrush(color);
			painter.drawRoundedRect(0, 0, newPixmap.width(), newPixmap.height(), radius, radius);
			painter.drawPixmap(borderWidth, borderWidth, m_pixmap);
		painter.end();

	m_pixmap = QPixmap(newPixmap);

	emit newPictureAvailable(m_pixmap);
}

void FilterWidget::slotRotationFilter()
{
	if (m_pixmap.isNull() || m_isGif)
		return;

	QDialog *dialog = new QDialog(this);
		dialog->setWindowTitle("Multiuso");

		// Construction of the dialog
			QLabel *label1 = new QLabel("Effectuer une rotation de ");

			QDoubleSpinBox *degrees = new QDoubleSpinBox;
				degrees->setRange(-360, 360);
				degrees->setValue(45);
				degrees->setSuffix("°");

			QLabel *label2 = new QLabel(" autour de l'axe ");

			QComboBox *axis = new QComboBox;
				axis->addItems(QStringList() << "X" << "Y" << "Z");
				axis->setCurrentIndex(axis->findText("Z"));

				QHBoxLayout *layout1 = new QHBoxLayout;
					layout1->addWidget(label1);
					layout1->addWidget(degrees);
					layout1->addWidget(label2);
					layout1->addWidget(axis);

			QPushButton *reject = new QPushButton("Annuler");
				connect(reject, SIGNAL(clicked()), dialog, SLOT(reject()));

			QPushButton *accept = new QPushButton("OK");
				connect(accept, SIGNAL(clicked()), dialog, SLOT(accept()));

				QHBoxLayout *layout2 = new QHBoxLayout;
					layout2->addWidget(reject);
					layout2->addWidget(accept);
					layout2->setAlignment(Qt::AlignRight);

			QVBoxLayout *mainLayout = new QVBoxLayout;
				mainLayout->addLayout(layout1);
				mainLayout->addLayout(layout2);

	dialog->setLayout(mainLayout);

	if (dialog->exec() != QDialog::Accepted)
	{
		dialog->deleteLater();

		return;
	}

	// Degrees

	qreal rotationDegrees = (qreal) degrees->value();

	// Axis

	Qt::Axis rotationAxis;

	if (axis->currentText() == "X")
		rotationAxis = Qt::XAxis;

	else if (axis->currentText() == "Y")
		rotationAxis = Qt::YAxis;

	else // == Z
		rotationAxis = Qt::ZAxis;

	// Construction of the matrix

	QTransform transform;
		transform.rotate(rotationDegrees, rotationAxis);

	// Application of the matrix

	QImage image = m_pixmap.toImage();
		image = image.transformed(transform);

	m_pixmap = QPixmap::fromImage(image);

	emit newPictureAvailable(m_pixmap);
}

void FilterWidget::slotResizeFilter()
{
	if (m_pixmap.isNull() || m_isGif)
		return;

	m_originalPictureX = m_pixmap.width();
		m_pictureX->setValue(m_originalPictureX);

	m_originalPictureY = m_pixmap.height();
		m_pictureY->setValue(m_originalPictureY);

	if (m_resizeDialog->exec() != QDialog::Accepted)
		return;

	m_pixmap = m_pixmap.scaled(m_pictureX->value(), m_pictureY->value(),
			Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	emit newPictureAvailable(m_pixmap);
}

void FilterWidget::resizePictureValueChangedX()
{
	if (!m_resizeProportional->isChecked())
		return;

	qreal factor = (qreal) m_pictureX->value() / m_originalPictureX;

	m_pictureY->setValue(m_originalPictureY * factor);
}

void FilterWidget::resizePictureValueChangedY()
{
	if (!m_resizeProportional->isChecked())
		return;

	qreal factor = (qreal) m_pictureY->value() / m_originalPictureY;

	m_pictureX->setValue(m_originalPictureX * factor);
}


void FilterWidget::slotSave()
{
	if (m_pixmap.isNull() || m_isGif)
		return;

	emit savePictureRequested(m_pixmap);
}

void FilterWidget::slotSaveAs()
{
	if (m_pixmap.isNull() || m_isGif)
		return;

	QString file = QFileDialog::getSaveFileName(this, "Multiuso",
	Multiuso::lastPath() + "/Nouvelle image.png", "Image (*.*)");

	Multiuso::setLastPath(file);

	if (!file.isEmpty() && m_pixmap.save(file))
		emit openFileRequested(file);
}

// Picture Viewer

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

	filterWidget = new FilterWidget;
		connect(filterWidget, SIGNAL(newPictureAvailable(QPixmap)), this, SLOT(slotApplyEffects(QPixmap)));
		connect(filterWidget, SIGNAL(savePictureRequested(QPixmap)), this, SLOT(slotSavePicture(QPixmap)));
		connect(filterWidget, SIGNAL(openFileRequested(QString)), this, SLOT(slotOuvrirFichier(QString)));
		connect(filterWidget, SIGNAL(cancelChanges()), this, SLOT(slotCancelChanges()));

	dockFilters = new QDockWidget("Filtres", this);
		dockFilters->setObjectName("Filtres");
		dockFilters->setWidget(filterWidget);
		addDockWidget(Qt::LeftDockWidgetArea, dockFilters);

	onglets = new QTabWidget;
		onglets->setMovable(true);
		onglets->setDocumentMode(true);
		onglets->setCornerWidget(buttonAddTab, Qt::BottomLeftCorner);
		onglets->setCornerWidget(buttonRemoveTab, Qt::BottomRightCorner);
		connect(onglets, SIGNAL(tabCloseRequested(int)), this, SLOT(slotRemoveTab(int)));
		connect(onglets, SIGNAL(currentChanged(int)), this, SLOT(slotTabIndexChanged(int)));
			slotNouvelOnglet();

	setCentralWidget(onglets);

	QSettings reglages(Multiuso::appDirPath() + "/ini/visionneur_images.ini", QSettings::IniFormat);
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
	if (currentLabel()->isGif())
		return;

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
	if (currentLabel()->isGif())
		return;

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
	if (currentLabel()->isGif())
		return;

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
	if (currentLabel()->isGif())
		return;

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
	if (currentLabel()->isGif())
		return;

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
	if (currentLabel()->isGif())
		return;

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

	QString slashToAdd = "";

	if (Multiuso::currentOS() == "windows")
		slashToAdd = "/";

	if (url.toString().startsWith("file://" + slashToAdd + Multiuso::tempPath())) // If the picture is in the temporary directory
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
	onglets->setTabIcon(onglets->currentIndex(), pixmap);

	slotZoomIdeal();
}

void VisionneurImages::slotSavePicture(QPixmap pixmap)
{
	QString slashToAdd = "";

	if (Multiuso::currentOS() == "windows")
		slashToAdd = "/";

	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_visionneur_images.png")
		return;

	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_erreur_ouverture.png")
		return;

	pixmap.save(currentLabel()->imgPath().remove("file://" + slashToAdd));
}

void VisionneurImages::slotCancelChanges()
{
	QString slashToAdd = "";

	if (Multiuso::currentOS() == "windows")
		slashToAdd = "/";

	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_visionneur_images.png")
		return;

	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_erreur_ouverture.png")
		return;

	slotOuvrirFichier(currentLabel()->imgPath().remove("file://" + slashToAdd));
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
	QSettings reglages(Multiuso::appDirPath() + "/ini/visionneur_images.ini", QSettings::IniFormat);
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
	if (currentLabel()->imgPath().isEmpty())
		return;

	QString slashToAdd = "";

	if (Multiuso::currentOS() == "windows")
		slashToAdd = "/";

	if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_visionneur_images.png")
	{
		if (filterWidget)
		{
			filterWidget->setPixmap(*new QPixmap);
			filterWidget->setIsGif(false);
		}
	}

	else if (currentLabel()->imgPath() == "file://" + slashToAdd + ":/images/fond_erreur_ouverture.png")
	{
		if (filterWidget)
		{
			filterWidget->setPixmap(*new QPixmap);
			filterWidget->setIsGif(false);
		}
	}

	else if (filterWidget && !QPixmap(currentLabel()->imgPath().remove("file://" + slashToAdd)).isNull())
	{
		filterWidget->setPixmap(QPixmap(currentLabel()->imgPath().remove("file://" + slashToAdd)));
		filterWidget->setIsGif(QFileInfo(currentLabel()->imgPath()).suffix().toLower() == "gif");
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
