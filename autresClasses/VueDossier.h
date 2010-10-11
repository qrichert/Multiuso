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

#ifndef HEADER_VUEDOSSIER
#define HEADER_VUEDOSSIER

#include "../CurrentIncludes.h"

enum CopyCutAction
{
	COPY, CUT, NONE
};

class CopyCutObject
{
	public:
		CopyCutObject()
		{
			m_currentAction = NONE;
			m_fileLink = "";
			m_fileType = "";
		}

		void setCurrentAction(CopyCutAction currentAction)
		{
			m_currentAction = currentAction;
		}

		CopyCutAction currentAction()
		{
			return m_currentAction;
		}

		void setFileLink(QString fileLink)
		{
			m_fileLink = fileLink;
		}

		QString fileLink()
		{
			return m_fileLink;
		}
		
		void setFileType(QString fileType)
		{
			m_fileType = fileType;
		}

		QString fileType()
		{
			return m_fileType;
		}

	private:
		CopyCutAction m_currentAction;
		QString m_fileLink;
		QString m_fileType;
};

class ListWidgetItem : public QListWidgetItem, public QWidget
{
	public:
		ListWidgetItem(QListWidget *parent = 0, int type = Type) : QListWidgetItem(parent, type) { initialize(); }
		ListWidgetItem(const QString &text, QListWidget *parent = 0, int type = Type) : QListWidgetItem(text, parent, type) { initialize(); }
		ListWidgetItem(const QIcon &icon, const QString &text, QListWidget *parent = 0, int type = Type) : QListWidgetItem(icon, text, parent, type) { initialize(); }
		ListWidgetItem(const QListWidgetItem &other) : QListWidgetItem(other) { initialize(); }

		void initialize()
		{
		}

		void setName(QString name)
		{
			m_name = name;
		}

		QString name()
		{
			return m_name;
		}

		void setSize(QString size)
		{
			m_size = size;
		}

		QString size()
		{
			return m_size;
		}

		void setType(QString type)
		{
			m_type = type;
		}

		QString type()
		{
			return m_type;
		}

		void setPath(QString path)
		{
			m_path = path;
		}

		QString path()
		{
			return m_path;
		}

		void setLastModified(QString lastModified)
		{
			m_lastModified = lastModified;
		}

		QString lastModified()
		{
			return m_lastModified;
		}

		void setToolTip(const QString &toolTip)
		{
			QListWidgetItem::setToolTip(toolTip);
		}
	
	private:
		QString m_name;
		QString m_size;
		QString m_type;
		QString m_path;
		QString m_lastModified;
};

class ListWidget : public QListWidget
{
	Q_OBJECT

	public:
		ListWidget(QWidget *parent = 0) : QListWidget(parent)
		{
			setContextMenuPolicy(Qt::CustomContextMenu);
			setViewMode(QListView::IconMode);
			setIconSize(QSize(50, 50));
			setResizeMode(QListView::Adjust);
			setMovement(QListView::Snap);
			setGridSize(QSize(135, 100));

			setSelectionMode(QAbstractItemView::SingleSelection);
			setDragEnabled(true);
			viewport()->setAcceptDrops(true);
			setDropIndicatorShown(true);
		}
	
	protected:
		void mousePressEvent(QMouseEvent *event)
		{
			if (event->button() == Qt::LeftButton)
				dragStartPos = event->pos();
			
			QListWidget::mousePressEvent(event);
		}

		void mouseMoveEvent(QMouseEvent *event)
		{
			if (!(event->buttons() & Qt::LeftButton) || !(itemAt(dragStartPos)))
			{
				QListWidget::mouseMoveEvent(event);

				return;
			}

			if ((event->pos() - dragStartPos).manhattanLength() < QApplication::startDragDistance())
				return;

			QDrag *drag = new QDrag(this);

				ListWidgetItem *item = static_cast<ListWidgetItem *>(currentItem());

				if (item == 0)
					return;

				QIcon icon = Multiuso::iconForFile(Multiuso::addSlash(item->path()) +  item->name(), item->type());

				drag->setPixmap(icon.pixmap(icon.actualSize(QSize(42, 42))));

				QString slashToAdd = "";

				if (Multiuso::currentOS() == "windows")
					slashToAdd = "/";
				
				QList<QUrl> allUrls;
			       		allUrls << QUrl("file://" + slashToAdd + Multiuso::addSlash(item->path()) + item->name());

				QMimeData *data = new QMimeData;
					data->setUrls(allUrls);

				drag->setMimeData(data);

			drag->exec(Qt::MoveAction, Qt::MoveAction);
		}

		void dragEnterEvent(QDragEnterEvent *event)
		{
			event->acceptProposedAction();
		}

		void dropEvent(QDropEvent *event)
		{
			const QMimeData *data = event->mimeData();
			
			Qt::DropAction action = event->dropAction();
		
			if (!data->hasUrls() || action != Qt::MoveAction)
				return;

			foreach (QUrl url, data->urls())
			{
				QCoreApplication::processEvents();

				QString s_url = url.path();

				if (Multiuso::currentOS() == "windows")
					s_url = s_url.right(s_url.length() - 1);
	
				emit moveRequested(s_url, event->pos());
			}
		}

	signals:
		void moveRequested(QString file, QPoint pos);

	private:
		QPoint dragStartPos;
};

class VueDossier : public QWidget
{
	Q_OBJECT

	public:
		VueDossier();

		void setFenPrincipale(QWidget *FP);

		void lister();
		int folderSize(QDir dir);
		QString precedent();
		QString suivant();
		void setModifierPosition(bool choix);
		void setAfficherDossiersCaches(bool choix);
		bool isAfficherDossiersCaches();
		QString chemin();

	public slots:
		void ouvrir(QListWidgetItem *item);
		void ouvrirMenu(QPoint);
		void menuSupprimer();
		void menuRenommer();
		void menuCouper();
		void menuCopier();
		void menuColler();
		void menuCreerDossier();
		void menuCreerFichier();
		void menuProperties();

		void setChemin(QString chemin);

		void moveFile(QString file, QPoint pos);

	signals:
		void debutChargement();
		void finChargement();
		void demandeUpdate();
		void openFileRequested(QString file);
	
	private:
		QString m_chemin;
		QStringList m_historique;
		int position;
		bool modifierPosition;
		bool afficherDossiersCaches;

		CopyCutObject *copyCutObject;

		ListWidget *m_vue;
};

#endif
