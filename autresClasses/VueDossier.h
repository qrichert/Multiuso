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

class VueDossier : public QWidget
{
	Q_OBJECT

	public:
		VueDossier();

		void setFenPrincipale(QWidget *FP);

		void lister();
		int folderSize(QDir dir);
		void vider();
		void ajouterListe(QList<QStandardItem *> items);
		QString precedent();
		QString suivant();
		void setModifierPosition(bool choix);
		void setAfficherDossiersCaches(bool choix);
		bool isAfficherDossiersCaches();

	public slots:
		void ouvrir(QModelIndex);
		void ouvrirMenu(QPoint);
		void menuSupprimer();
		void menuRenommer();
		void menuCreerDossier();
		void menuCreerFichier();

	public slots:
		void setChemin(QString chemin);

	public slots:
		QString chemin();

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

	private:
		QStandardItemModel *m_modele;
		QTableView *m_vue;
};

#endif
