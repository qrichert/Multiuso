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

#include "Organisateur.h"

Organisateur::Organisateur(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Organisteur");
	setWindowIcon(QIcon(":/icones/actions/actionOrganisteur.png"));

	resize(Multiuso::screenWidth() / 2, Multiuso::screenHeight() / 2);

	QPushButton *addTask = new QPushButton("Ajouter une tâche");
		connect(addTask, SIGNAL(clicked()), this, SLOT(slotAddTask()));

	QStringList headerLabels;
		headerLabels << "#" << "-" << "Titre" << "Contenu" << "Priorité";

	mainTable = new QTableWidget(0, headerLabels.size());
		mainTable->setShowGrid(false);
		mainTable->setHorizontalHeaderLabels(headerLabels);
		mainTable->verticalHeader()->hide();
		mainTable->setSelectionBehavior(QAbstractItemView::SelectRows);
		mainTable->setSelectionMode(QAbstractItemView::SingleSelection);
		mainTable->resizeColumnsToContents();
		mainTable->horizontalHeader()->setStretchLastSection(true);

	QGridLayout *mainLayout = new QGridLayout(this);
		mainLayout->addWidget(new QLabel("Double-cliquez sur une tâche pour l'afficher"), 0, 0, 1, 3);
		mainLayout->addWidget(addTask, 0, 3, 1, 1);
		mainLayout->addWidget(mainTable, 1, 0, 1, 4);
}

void Organisateur::slotAddTask()
{
	int newRowCount = mainTable->rowCount() + 1;

	QTableWidgetItem *itemNumber = new QTableWidgetItem(QString::number(newRowCount) + "iho");
		itemNumber->setFlags(itemNumber->flags() & ~Qt::ItemIsEditable);

	mainTable->setRowCount(newRowCount);
		mainTable->setItem(newRowCount, 0, itemNumber);

	mainTable->resizeColumnsToContents();
	mainTable->horizontalHeader()->setStretchLastSection(true);
}
