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

#ifndef HEADER_TELECHARGERFICHIER
#define HEADER_TELECHARGERFICHIER

#include "../CurrentIncludes.h"
#include <QUrlInfo>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class TelechargerFichier : public QDialog
{
	Q_OBJECT

	public:
		TelechargerFichier(QWidget *parent);

		void closeEvent(QCloseEvent *event);

	public slots:
		void ajouterTelechargement();
		void nouveauTelechargement(const QString lien);
		void telechargementContinue(qint64 recu, qint64 total);
		void annulerTelechargement();
		void finTelechargement();

	signals:
		void DesactiverIcone();
		void MajEtat();

	private:
		QLineEdit *lineLien;
		QTableWidget *listeTelechargements;
		QList<QProgressBar *> progressionsTelechargements;
		QStringList nomsFichiers;

	private:
		QList<QNetworkReply *> reponses;
};

#endif
