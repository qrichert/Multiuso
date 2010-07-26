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

#include "Messagerie.h"

Messagerie::Messagerie(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Messagerie");
	setWindowIcon(QIcon(":/icones/actions/actionMessagerie.png"));

	connectionWidget = new ConnectionWidget;

	mainWidget = new QStackedWidget;
		mainWidget->addWidget(connectionWidget);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
		mainLayout->addWidget(mainWidget);
}
