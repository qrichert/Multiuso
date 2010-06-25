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
				QList<QUrl> urls;
			       		urls << QUrl(m_imgPath);

				QMimeData *mimeData = new QMimeData;
					mimeData->setUrls(urls);

				QPixmap tempPixmap = *pixmap();

				if (tempPixmap.width() > tempPixmap.height() && tempPixmap.width() > 150)
					tempPixmap = tempPixmap.scaledToWidth(150);

				else if (tempPixmap.height() > tempPixmap.width() && tempPixmap.height() > 150)
					tempPixmap = tempPixmap.scaledToHeight(150);

				QPixmap pixmapToShow(tempPixmap.width() + 4, tempPixmap.height() + 4); // 2px of all sides.

				QPainter painter;
					painter.begin(&pixmapToShow);
						painter.setPen(Qt::white);
						painter.setBrush(Qt::white);
						painter.fillRect(0, 0, pixmapToShow.width(), pixmapToShow.height(), Qt::white);
						painter.drawPixmap(2, 2, tempPixmap);
					painter.end();
					painter.begin(&pixmapToShow);
						painter.setPen(Qt::white);
						painter.setBrush(Qt::white);
						painter.setOpacity(0.4);
						painter.drawEllipse(-(pixmapToShow.width()), -(pixmapToShow.height() + (pixmapToShow.height() / 3)),
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
			setPixmap(QPixmap(imgPath));

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
			m_pixmap = 0;

			QPushButton *buttonPhotoFilter = new QPushButton("Effet photo");
				connect(buttonPhotoFilter, SIGNAL(clicked()), this, SLOT(slotPhotoFilter()));

			QPushButton *buttonSaveAs = new QPushButton("Enregistrer l'image sous...");
				connect(buttonSaveAs, SIGNAL(clicked()), this, SLOT(slotSaveAs()));

			QVBoxLayout *mainLayout = new QVBoxLayout(this);
				mainLayout->addWidget(buttonPhotoFilter);
				mainLayout->addWidget(new QLabel("<hr />"));
				mainLayout->addWidget(buttonSaveAs);
				mainLayout->setAlignment(Qt::AlignTop);
		}

		void setPixmap(QPixmap pixmap)
		{
			if (m_pixmap.paintingActive())
				return;

			if (!pixmap.isNull())
				m_pixmap = pixmap;

			else
				m_pixmap = 0;
		}

	public slots:
		void slotPhotoFilter()
		{
			if (m_pixmap.isNull())
				return;

			int borderWidth = 0;
			
			if (m_pixmap.width() > m_pixmap.height())
				borderWidth = m_pixmap.height() / 20;
			
			else
				borderWidth = m_pixmap.width() / 20;

			QPixmap newPixmap(m_pixmap.width() + (borderWidth * 2), m_pixmap.height() + (borderWidth * 2));

			QPainter painter;
				painter.begin(&newPixmap);
					painter.setPen(Qt::white);
					painter.setBrush(Qt::white);
					painter.fillRect(0, 0, newPixmap.width(), newPixmap.height(), Qt::white);
					painter.drawPixmap(borderWidth, borderWidth, m_pixmap);
				painter.end();
				painter.begin(&newPixmap);
					painter.setPen(Qt::white);
					painter.setBrush(Qt::white);
					painter.setOpacity(0.4);
					painter.drawEllipse(-(newPixmap.width()), -(newPixmap.height() + (newPixmap.height() / 3)),
							newPixmap.width() * 2, newPixmap.height() * 2);
				painter.end();

			m_pixmap = newPixmap;

			emit newPictureAvailable(newPixmap);
		}

		void slotSaveAs()
		{
			if (m_pixmap.isNull())
				return;

			QString file = QFileDialog::getSaveFileName(this, "Multiuso",
					Multiuso::lastPath() + "/Nouvelle image.png", "Image (*.*)");

			Multiuso::setLastPath(file);

			if (!file.isEmpty() && m_pixmap.save(file))
				emit openFileRequested(file);
		}

	signals:
		void newPictureAvailable(QPixmap pixmap);
		void openFileRequested(QString file);

	private:
		QPixmap m_pixmap;
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

		void slotOuvrirFichier(QString fichier);
		void slotOpenFileFromDrop(QUrl url);
		void slotApplyEffects(QPixmap pixmap);
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
};

#endif
