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

#ifndef HEADER_ORGANISATEUR
#define HEADER_ORGANISATEUR

#include "../CurrentIncludes.h"

class EditWidget : public QWidget
{
	Q_OBJECT

	public:
		EditWidget(QWidget *parent = 0) : QWidget(parent)
		{
			QPushButton *b_edit = new QPushButton;
				b_edit->setIcon(QIcon(":/icones/organisateur/modifier.png"));
				connect(b_edit, SIGNAL(clicked()), this, SIGNAL(editRequest()));	
				
				
			QPushButton *b_delete = new QPushButton;
				b_delete->setIcon(QIcon(":/icones/organisateur/supprimer.png"));
				connect(b_delete, SIGNAL(clicked()), this, SIGNAL(deleteRequest()));

			QHBoxLayout *layout = new QHBoxLayout(this);
				layout->addWidget(b_edit);
				layout->addWidget(b_delete);
		}

	signals:
		void editRequest();
		void deleteRequest();
};

class Organisateur : public QDialog
{
	Q_OBJECT

	public:
		Organisateur(QWidget *parent);

		void initializeTasks();

	public slots:
		void slotAddTask();
		void slotEditTask();
		void slotDeleteTask();

	private:
		QTableWidget *mainTable;

};

#endif
