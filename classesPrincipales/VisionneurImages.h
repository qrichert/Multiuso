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

#ifndef HEADER_VISIONNEURIMAGES
#define HEADER_VISIONNEURIMAGES

#include "../CurrentIncludes.h"

class Picture : public QLabel
{
	Q_OBJECT

	public:
		Picture(QWidget * parent = 0) : QLabel(parent)
		{
			m_imgPath = "";
			m_zoom = 1.0;
			m_isGif = false;

			setAcceptDrops(true);
		}

		QString imgPath()
		{
			return m_imgPath;
		}

		double zoom()
		{
			return m_zoom;
		}

		void setZoom(double zoom)
		{
			m_zoom = zoom;
		}

		bool isGif()
		{
			return m_isGif;
		}

	protected:
		void mousePressEvent(QMouseEvent *event)
		{
			QString slashToAdd = "";

			if (Multiuso::currentOS() == "windows")
				slashToAdd = "/";

			if (m_imgPath == "file://" + slashToAdd + ":/images/fond_visionneur_images.png")
				return;
			
			if (m_imgPath == "file://" + slashToAdd + ":/images/fond_erreur_ouverture.png")
				return;

			if (event->button() == Qt::LeftButton)
			{
				QPixmap tempPixmap;
				QString saveTempPixmapPath;
			       
				if (movie() != NULL)
				{
					tempPixmap = movie()->currentPixmap();

					saveTempPixmapPath = m_imgPath;
				}
				
				else
				{
					tempPixmap = *pixmap();

					saveTempPixmapPath = Multiuso::tempPath() + "/" + QFileInfo(m_imgPath).fileName();
				
					tempPixmap.save(saveTempPixmapPath); // No "file://" for saving

					saveTempPixmapPath = "file://" + slashToAdd + saveTempPixmapPath; // Now we can add the "file://"
				}

				QList<QUrl> urls;
			       		urls << QUrl(saveTempPixmapPath);

				QMimeData *mimeData = new QMimeData;
					mimeData->setUrls(urls);
	
				if (tempPixmap.width() > tempPixmap.height() && tempPixmap.width() > 150)
					tempPixmap = tempPixmap.scaledToWidth(150);

				else if (tempPixmap.height() > tempPixmap.width() && tempPixmap.height() > 150)
					tempPixmap = tempPixmap.scaledToHeight(150);

				QPixmap pixmapToShow(tempPixmap.width() + 4, tempPixmap.height() + 4); // 2px of all sides.

				QPainter painter;
					painter.begin(&pixmapToShow);
						painter.setRenderHint(QPainter::Antialiasing, true);
						painter.setPen(Qt::transparent);
						painter.setBrush(Qt::white);
						painter.fillRect(0, 0, pixmapToShow.width(), pixmapToShow.height(), Qt::white);
						painter.drawPixmap(2, 2, tempPixmap);
					painter.end();
					painter.begin(&pixmapToShow);
						painter.setRenderHint(QPainter::Antialiasing, true);
						painter.setPen(Qt::NoPen);
						painter.setBrush(Qt::white);
						painter.setOpacity(0.4);
						painter.drawEllipse(-(pixmapToShow.width() - (pixmapToShow.width() / 6)),
								-(pixmapToShow.height() + (pixmapToShow.height() / 3)),
									pixmapToShow.width() * 2, pixmapToShow.height() * 2);
					painter.end();

				const QPixmap dragPixmap = pixmapToShow;
				
				QDrag *drag = new QDrag(this);
					drag->setMimeData(mimeData);
					drag->setPixmap(dragPixmap);
					drag->exec(Qt::CopyAction, Qt::CopyAction);
			}
		}

		void dragEnterEvent(QDragEnterEvent *event)
		{
			if (event->mimeData()->hasFormat("text/uri-list"))
				event->acceptProposedAction();
		}

		void dropEvent(QDropEvent *event)
		{
			const QMimeData *dropData = event->mimeData();

			if (dropData->hasUrls())
			{
				foreach (QUrl url, dropData->urls())
					emit openFileFromDrop(url);
			}
		}

	public slots:
		void setImage(QString imgPath)
		{
			if (m_isGif)
				setMovie(NULL);

			if (QFileInfo(imgPath).suffix().toLower() == "gif")
			{
				m_isGif = true;

				QMovie *movie = new QMovie(imgPath);
					setMovie(movie);
					movie->start();
			}

			else
			{
				m_isGif = false;
				setPixmap(QPixmap(imgPath));
			}

			QString slashToAdd = "";

			if (Multiuso::currentOS() == "windows")
				slashToAdd = "/";

			m_imgPath = "file://" + slashToAdd + imgPath;
		}

	signals:
		void openFileFromDrop(QUrl url);

	private:
		QString m_imgPath;
		double m_zoom;
		bool m_isGif;
};

class ScrollArea : public QScrollArea
{
	Q_OBJECT

	public:
		ScrollArea(QWidget * parent = 0) : QScrollArea(parent)
		{
			setAcceptDrops(true);
		}

	protected:
		void dragEnterEvent(QDragEnterEvent *event)
		{
			if (event->mimeData()->hasFormat("text/uri-list"))
				event->acceptProposedAction();
		}

		void dropEvent(QDropEvent *event)
		{
			const QMimeData *dropData = event->mimeData();
				
			if (dropData->hasUrls())
			{
				foreach (QUrl url, dropData->urls())
					emit openFileFromDrop(url);
			}
		}

	signals:
		void openFileFromDrop(QUrl url);
};

class FilterWidget : public QWidget
{
	Q_OBJECT

	public:
		FilterWidget()
		{
			m_pixmap = *new QPixmap;
			m_isGif = false;

			QPushButton *buttonPhotoFilter = new QPushButton;
				buttonPhotoFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/photo_effect.png"));
				buttonPhotoFilter->setToolTip("Effet photo");
				buttonPhotoFilter->setIconSize(QSize(32, 32));
				connect(buttonPhotoFilter, SIGNAL(clicked()), this, SLOT(slotPhotoFilter()));

			QPushButton *buttonColorFilter = new QPushButton;
				buttonColorFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/color_filter.png"));
				buttonColorFilter->setToolTip("Filtre de couleur");
				buttonColorFilter->setIconSize(QSize(32, 32));
				connect(buttonColorFilter, SIGNAL(clicked()), this, SLOT(slotColorFilter()));

			QPushButton *buttonInvertColorsFilter = new QPushButton;
				buttonInvertColorsFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/inverser.png"));
				buttonInvertColorsFilter->setToolTip("Inverser les couleurs");
				buttonInvertColorsFilter->setIconSize(QSize(32, 32));
				connect(buttonInvertColorsFilter, SIGNAL(clicked()), this, SLOT(slotInvertColorsFilter()));

			QPushButton *buttonMirrorHorizontalFilter = new QPushButton;
				buttonMirrorHorizontalFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/horizontal_mirror.png"));
				buttonMirrorHorizontalFilter->setToolTip("Miroir horizontal");
				buttonMirrorHorizontalFilter->setIconSize(QSize(32, 32));
				connect(buttonMirrorHorizontalFilter, SIGNAL(clicked()), this, SLOT(slotMirrorHorizontalFilter()));
				
			QPushButton *buttonMirrorVerticalFilter = new QPushButton;
				buttonMirrorVerticalFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/vertical_mirror.png"));
				buttonMirrorVerticalFilter->setToolTip("Miroir vertical");
				buttonMirrorVerticalFilter->setIconSize(QSize(32, 32));
				connect(buttonMirrorVerticalFilter, SIGNAL(clicked()), this, SLOT(slotMirrorVerticalFilter()));

			QPushButton *buttonReflectionFilter = new QPushButton;
				buttonReflectionFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/reflection.png"));
				buttonReflectionFilter->setToolTip("Reflet");
				buttonReflectionFilter->setIconSize(QSize(32, 32));
				connect(buttonReflectionFilter, SIGNAL(clicked()), this, SLOT(slotReflectionFilter()));
				
			QPushButton *buttonBorderFilter = new QPushButton;
				buttonBorderFilter->setIcon(QIcon(":/icones/visionneur_images/filtres/border.png"));
				buttonBorderFilter->setToolTip("Bordure");
				buttonBorderFilter->setIconSize(QSize(32, 32));
				connect(buttonBorderFilter, SIGNAL(clicked()), this, SLOT(slotBorderFilter()));

			QPushButton *buttonSave = new QPushButton("Enregistrer l'image");
				connect(buttonSave, SIGNAL(clicked()), this, SLOT(slotSave()));

			QPushButton *buttonSaveAs = new QPushButton("Enregistrer l'image sous...");
				connect(buttonSaveAs, SIGNAL(clicked()), this, SLOT(slotSaveAs()));

			QPushButton *buttonCancelChanges = new QPushButton("Annuler les modifications");
				connect(buttonCancelChanges, SIGNAL(clicked()), this, SIGNAL(cancelChanges()));

			QGridLayout *mainLayout = new QGridLayout(this);
				mainLayout->addWidget(buttonPhotoFilter, 0, 0, 1, 1);
				mainLayout->addWidget(buttonColorFilter, 0, 1, 1, 1);
				mainLayout->addWidget(buttonInvertColorsFilter, 0, 2, 1, 1);
				mainLayout->addWidget(buttonMirrorHorizontalFilter, 1, 0, 1, 1);
				mainLayout->addWidget(buttonMirrorVerticalFilter, 1, 1, 1, 1);
				mainLayout->addWidget(buttonReflectionFilter, 1, 2, 1, 1);
				mainLayout->addWidget(buttonBorderFilter, 2, 0, 1, 1);
				mainLayout->addWidget(new QLabel("<hr />"), 3, 0, 1, 3);
				mainLayout->addWidget(buttonSave, 4, 0, 1, 3);
				mainLayout->addWidget(buttonSaveAs, 5, 0, 1, 3);
				mainLayout->addWidget(new QLabel("<hr />"), 6, 0, 1, 3);
				mainLayout->addWidget(buttonCancelChanges, 7, 0, 1, 3);
				mainLayout->setAlignment(Qt::AlignTop);
		}

		void setPixmap(QPixmap pixmap)
		{
			if (m_pixmap.paintingActive())
				return;

			if (!pixmap.isNull())
				m_pixmap = pixmap;

			else
				m_pixmap = *new QPixmap;
		}

		void setIsGif(bool isGif)
		{
			m_isGif = isGif;
		}

	public slots:
		void slotPhotoFilter()
		{
			if (m_pixmap.isNull() || m_isGif)
				return;

			int borderWidth = 0;
			
			if (m_pixmap.width() > m_pixmap.height())
				borderWidth = m_pixmap.height() / 20;
			
			else
				borderWidth = m_pixmap.width() / 20;

			QPixmap newPixmap(m_pixmap.width() + (borderWidth * 2), m_pixmap.height() + (borderWidth * 2));

			QPainter painter;
				painter.begin(&newPixmap);
					painter.setRenderHint(QPainter::Antialiasing, true);
					painter.setPen(Qt::white);
					painter.setBrush(Qt::white);
					painter.fillRect(0, 0, newPixmap.width(), newPixmap.height(), Qt::white);
					painter.drawPixmap(borderWidth, borderWidth, m_pixmap);
				painter.end();
				painter.begin(&newPixmap);
					painter.setRenderHint(QPainter::Antialiasing, true);
					painter.setPen(Qt::NoPen);
					painter.setBrush(Qt::white);
					painter.setOpacity(0.4);
					painter.drawEllipse(-(newPixmap.width() - (newPixmap.width() / 6)),
								-(newPixmap.height() + (newPixmap.height() / 3)),
									newPixmap.width() * 2, newPixmap.height() * 2);
				painter.end();

			m_pixmap = QPixmap(newPixmap);

			emit newPictureAvailable(newPixmap);
		}

		void slotColorFilter()
		{
			if (m_pixmap.isNull() || m_isGif)
				return;

			QColor color = QColorDialog::getColor(Qt::blue, NULL, "Multiuso");

			if (!color.isValid())
				return;

			bool ok;

			double opacity = QInputDialog::getDouble(new QWidget, "Multiuso",
				"Opacité du filtre :<br /><em>(0 : transparent, 100 : opaque)</em>", 40.0, 0.0, 100.0, 2, &ok);

			if (!ok || opacity == 0)
				return;

			opacity /= 100;

			QPainter painter;
				painter.begin(&m_pixmap);
				painter.setOpacity(opacity);
				painter.fillRect(0, 0, m_pixmap.width(), m_pixmap.height(), color);
				painter.end();
			
			emit newPictureAvailable(m_pixmap);
		}
	
		void slotInvertColorsFilter()
		{
			if (m_pixmap.isNull() || m_isGif)
				return;

			QImage image(m_pixmap.toImage());
				image.invertPixels();

			m_pixmap = QPixmap::fromImage(image);
			
			emit newPictureAvailable(m_pixmap);
		}
		
		void slotMirrorHorizontalFilter()
		{
			if (m_pixmap.isNull() || m_isGif)
				return;

			QImage image(m_pixmap.toImage());
				image = image.mirrored(true, false);

			m_pixmap = QPixmap::fromImage(image);
			
			emit newPictureAvailable(m_pixmap);
		}

		void slotMirrorVerticalFilter()
		{
			if (m_pixmap.isNull() || m_isGif)
				return;

			QImage image(m_pixmap.toImage());
				image = image.mirrored(false, true);

			m_pixmap = QPixmap::fromImage(image);
			
			emit newPictureAvailable(m_pixmap);
		}

		void slotReflectionFilter()
		{
			if (m_pixmap.isNull() || m_isGif)
				return;
			
			QColor color = QColorDialog::getColor(Qt::white, NULL, "Choisissez la couleur de fond");

			if (!color.isValid())
				return;

			QImage image(m_pixmap.toImage());
				image = image.mirrored(false, true);

			QPixmap reflection = QPixmap::fromImage(image);
				reflection = reflection.copy(0, 0, reflection.width(), reflection.height() / 2);

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

		void slotBorderFilter()
		{
			if (m_pixmap.isNull() || m_isGif)
				return;
			
			QColor color = QColorDialog::getColor(Qt::white, NULL, "Choisissez la couleur de la bordure", QColorDialog::ShowAlphaChannel);

			if (!color.isValid())
				return;

			bool ok;

			int borderWidth = QInputDialog::getInt(NULL, "Multiuso", "Épaisseur de la bordure (en px) :", 1, 1, 2147483647, 1, &ok);

			if (!ok)
				return;

			QPixmap newPixmap(m_pixmap.width() + (borderWidth * 2), m_pixmap.height() + (borderWidth * 2));
				newPixmap.fill(color);

			QPainter painter;
				painter.begin(&newPixmap);
					painter.drawPixmap(borderWidth, borderWidth, m_pixmap);
				painter.end();
			
			m_pixmap = QPixmap(newPixmap);

			emit newPictureAvailable(m_pixmap);
		}

		void slotSave()
		{
			if (m_pixmap.isNull() || m_isGif)
				return;

			emit savePictureRequested(m_pixmap);
		}

		void slotSaveAs()
		{
			if (m_pixmap.isNull() || m_isGif)
				return;

			QString file = QFileDialog::getSaveFileName(this, "Multiuso",
					Multiuso::lastPath() + "/Nouvelle image.png", "Image (*.*)");

			Multiuso::setLastPath(file);

			if (!file.isEmpty() && m_pixmap.save(file))
				emit openFileRequested(file);
		}

	signals:
		void newPictureAvailable(QPixmap pixmap);
		void savePictureRequested(QPixmap pixmap);
		void openFileRequested(QString file);
		void cancelChanges();

	private:
		QPixmap m_pixmap;
		bool m_isGif;
};

class VisionneurImages : public QMainWindow
{
	Q_OBJECT

	public:
		VisionneurImages(QWidget *parent);
		
		bool needNewTab();
		void switchToLastIndex();

		Picture *currentLabel();
		ScrollArea *currentScrollArea();

	protected:
		void wheelEvent(QWheelEvent *event)
		{
			if (event->delta() > 0)
				slotZoomPlus();

			else if (event->delta() < 0)
				slotZoomMoins();
		}
	
	public slots:
		void slotOuvrir();
		void slotFermer();
		void slotZoomPlus();
		void slotZoomNormal();
		void slotZoomIdeal();
		void slotZoomMoins();
		void slotRotateLeft();
		void slotRotateRight();

		void slotOuvrirFichier(QString fichier);
		void slotOpenFileFromDrop(QUrl url);
		void slotApplyEffects(QPixmap pixmap);
		void slotSavePicture(QPixmap pixmap);
		void slotCancelChanges();
		void zoomer(double facteurZoom);
		void ajusterScrollBar(QScrollBar *scrollBar, double facteurZoom);
		void sauvegarderEtat();

		QWidget *newTab();
		void slotNouvelOnglet();
		void slotTabIndexChanged(int);
		void slotRemoveTab();
		void slotRemoveTab(int index);

	private:
		QTabWidget *onglets;
		FilterWidget *filterWidget;
		QDockWidget *dockFilters;

		QAction *actionAddTab;
		QAction *actionRemoveTab;

		QAction *actionOuvrir;
		QAction *actionFermer;
		QAction *actionZoomPlus;
		QAction *actionZoomNormal;
		QAction *actionZoomIdeal;
		QAction *actionZoomMoins;
		QAction *actionRotateLeft;
		QAction *actionRotateRight;
};

#endif
