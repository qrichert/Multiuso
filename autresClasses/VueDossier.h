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
		void menuCreerDossier();
		void menuCreerFichier();
		void menuProperties();

		void setChemin(QString chemin);

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

		QListWidget *m_vue;
};

#endif
