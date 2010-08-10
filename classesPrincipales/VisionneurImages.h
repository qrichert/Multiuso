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
		FilterWidget();

		void setPixmap(QPixmap pixmap);
		void setIsGif(bool isGif);

	public slots:
		void slotPhotoFilter();
		void slotColorFilter();
		void slotInvertColorsFilter();
		void slotMirrorHorizontalFilter();
		void slotMirrorVerticalFilter();
		void slotReflectionFilter();
		void slotBorderFilter();
		void slotRotationFilter();
		void slotResizeFilter();
		void resizePictureValueChangedX();
		void resizePictureValueChangedY();
		void slotSave();
		void slotSaveAs();

	signals:
		void newPictureAvailable(QPixmap pixmap);
		void savePictureRequested(QPixmap pixmap);
		void openFileRequested(QString file);
		void cancelChanges();

	private:
		QPixmap m_pixmap;
		bool m_isGif;

		QDialog *m_resizeDialog;
		QSpinBox *m_pictureX;
		int m_originalPictureX;
		QSpinBox *m_pictureY;
		int m_originalPictureY;
		QPushButton *m_resizeProportional;
		QPushButton *m_resizeReject;
		QPushButton *m_resizeOk;
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
		void slotPrint(QPrinter *printer = 0);

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
		QAction *actionPrint;
};

#endif
