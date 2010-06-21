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

#ifndef HEADER_LINEEDIT
#define HEADER_LINEEDIT

#include "../CurrentIncludes.h"

class LineEdit : public QLineEdit
{
	Q_OBJECT

	public:
		LineEdit(QWidget *parent = 0) : QLineEdit(parent)
		{
			if (parent != 0)
				setParent(parent);

			//createContents();
		}

		LineEdit(const QString &contents, QWidget *parent = 0) : QLineEdit(parent)
		{
			setText(contents);
			
			if (parent != 0)
				setParent(parent);

			//createContents();
		}

	public:
		void createContents()
		{
			QPushButton *clearLine = new QPushButton(this);
				clearLine->setIcon(QIcon(":/ftp/supprimer.png"));
				connect(clearLine, SIGNAL(clicked()), this, SLOT(clear()));
		}
};

#endif
