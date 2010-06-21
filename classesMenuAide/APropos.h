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

#ifndef HEADER_APROPOS
#define HEADER_APROPOS

#include "../CurrentIncludes.h"

class StandardItem : public QStandardItem
{
	public:
		StandardItem(const QString &text) : QStandardItem(text)
		{
			setEditable(false);
		}
		
		StandardItem(const QIcon &icon, const QString &text) : QStandardItem(icon, text)
		{
			setEditable(false);
		}
		
		StandardItem(int rows, int columns = 1) : QStandardItem(rows, columns)
		{
			setEditable(false);
		}
};

class APropos : public QDialog
{
	Q_OBJECT

	public:
		APropos(QWidget *parent);

		void populateModel(QStandardItemModel *model);

	public slots:
		void slotSwitchView();
		void slotLicence();

	// <Easter Egg>

	public slots:
		void slotEasterEgg();

	// </Easter Egg>

	private:
		QStandardItemModel *contributorsModel;
		QTreeView *contributorsContent;

		QStackedWidget *stackedContenu;
		QStackedWidget *stackedBoutons;
};

#endif
