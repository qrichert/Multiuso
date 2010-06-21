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

#include "EasterEggCafe.h"

EasterEggCafe::EasterEggCafe(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Mmmmmmmhhhhhh café !");
	setWindowIcon(QIcon(":/easter_eggs/icone_easter_egg_cafe.png"));
	setFixedSize(640, 485);

	QMdiArea *area1 = new QMdiArea;
		area1->setBackground(QPixmap(":/easter_eggs/easter_egg_cafe_1.png"));
		
	QMdiArea *area2 = new QMdiArea;
		area2->setBackground(QPixmap(":/easter_eggs/easter_egg_cafe_2.png"));

	stackedWidget = new QStackedWidget;
		stackedWidget->addWidget(area1);
		stackedWidget->addWidget(area2);
		stackedWidget->setCurrentIndex(0);

	QPushButton *switchButton = new QPushButton("", stackedWidget);
		switchButton->setFlat(true);
		switchButton->setUpdatesEnabled(false);
		switchButton->setFixedSize(640, 485);
		connect(switchButton, SIGNAL(clicked()), this, SLOT(switchState()));
		
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
		mainLayout->addWidget(stackedWidget);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		
	step = 0;
}

void EasterEggCafe::switchState()
{
	step ++;

	if (step == 1)
		stackedWidget->setCurrentIndex(step);
}
