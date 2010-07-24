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

#ifndef HEADER_AGENDA
#define HEADER_AGENDA

#include "../CurrentIncludes.h"

class Agenda : public QDialog
{
	Q_OBJECT

	public:
		Agenda(QWidget *parent = 0, QDate date = QDate());

	protected:
		void accept()
		{
			if (champDeSaisie->toPlainText().isEmpty())
				QDialog::reject();

			else
				QDialog::accept();
		}

		void reject()
		{
			accept();
		}

	public slots:
		void slotChangementDeTexte();

	private:
		QTextEdit *champDeSaisie;
		QString groupName;
};

#endif
