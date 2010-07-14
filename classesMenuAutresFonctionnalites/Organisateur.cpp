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

	m_sortBy = new QComboBox;
		m_sortBy->addItems(QStringList() << "Tout" << "Priorité très élevée" << "Priorité élevée"
				<< "Priorité normale" << "Priorité faible" << "Priorité très faible");
		connect(m_sortBy, SIGNAL(currentIndexChanged(QString)), this, SLOT(initializeTasks(QString)));

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
		mainLayout->addWidget(new QLabel("Double-cliquez sur une tâche pour l'afficher"), 0, 0, 1, 2);
		mainLayout->addWidget(m_sortBy, 0, 2, 1, 1);
		mainLayout->addWidget(addTask, 0, 3, 1, 1);
		mainLayout->addWidget(mainTable, 1, 0, 1, 4);

	m_sortBy->setCurrentIndex(m_sortBy->findText("Tout"));
	initializeTasks();
}

void Organisateur::addTasksToTable(QList<QStringList> tasks)
{
	foreach (QStringList line, tasks)
	{
		QString m_title = line.value(0);

			if (m_title.size() > 15)
				m_title = m_title.left(12) + "...";

		QString m_content = line.value(1);
			
			if (m_content.size() > 40)
				m_content = m_content.left(37) + "...";

		QString m_priority = line.value(2);
		
		int newRowCount = mainTable->rowCount() + 1;
	
		QTableWidgetItem *itemNumber = new QTableWidgetItem(QString::number(newRowCount));
			itemNumber->setFlags(itemNumber->flags() & ~Qt::ItemIsEditable);

		EditWidget *editTaskWidget = new EditWidget;
			connect(editTaskWidget, SIGNAL(editRequest()), this, SLOT(slotEditTask()));
			connect(editTaskWidget, SIGNAL(deleteRequest()), this, SLOT(slotDeleteTask()));
			
		QTableWidgetItem *itemTitle = new QTableWidgetItem(m_title);
			itemTitle->setFlags(itemTitle->flags() & ~Qt::ItemIsEditable);

		QTableWidgetItem *itemContent = new QTableWidgetItem(m_content);
			itemContent->setFlags(itemContent->flags() & ~Qt::ItemIsEditable);
		
		QTableWidgetItem *itemPriority = new QTableWidgetItem(m_priority);
			itemPriority->setFlags(itemPriority->flags() & ~Qt::ItemIsEditable);

		mainTable->setRowCount(newRowCount);
			mainTable->setItem(newRowCount - 1, 0, itemNumber);
			mainTable->setCellWidget(newRowCount - 1, 1, editTaskWidget);
			mainTable->setItem(newRowCount - 1, 2, itemTitle);
			mainTable->setItem(newRowCount - 1, 3, itemContent);
			mainTable->setItem(newRowCount - 1, 4, itemPriority);
	}
}

void Organisateur::initializeTasks(QString sortBy)
{
	priorityVeryHigh.clear();
	priorityHigh.clear();
	priorityNormal.clear();
	priorityLow.clear();
	priorityVeryLow.clear();

	QStringList headerLabels;
		headerLabels << "#" << "-" << "Titre" << "Contenu" << "Priorité";

	mainTable->clear();
	mainTable->setRowCount(0);
	mainTable->setHorizontalHeaderLabels(headerLabels);

	QSettings settings(Multiuso::appDirPath() + "/reglages/task_manager.ini", QSettings::IniFormat);
	
	foreach (QString group, settings.childGroups())
	{
		QStringList task = settings.value(group + "/content").value<QStringList>();

		if (task.value(2) == "Très élevée")
			priorityVeryHigh << task;

		else if (task.value(2) == "Élevée")
			priorityHigh << task;

		else if (task.value(2) == "Normale")
			priorityNormal << task;

		else if (task.value(2) == "Faible")
			priorityLow << task;

		else if (task.value(2) == "Très faible")
			priorityVeryLow << task;
	}

	if (sortBy == "Tout" || sortBy == "")
	{
		addTasksToTable(priorityVeryHigh);
		addTasksToTable(priorityHigh);
		addTasksToTable(priorityNormal);
		addTasksToTable(priorityLow);
		addTasksToTable(priorityVeryLow);
	}
	
	else if (sortBy == "Priorité très élevée")
	{
		addTasksToTable(priorityVeryHigh);
	}

	else if (sortBy == "Priorité élevée")
	{
		addTasksToTable(priorityHigh);
	}

	else if (sortBy == "Priorité normale")
	{
		addTasksToTable(priorityNormal);
	}

	else if (sortBy == "Priorité faible")
	{
		addTasksToTable(priorityLow);
	}

	else if (sortBy == "Priorité très faible")
	{
		addTasksToTable(priorityVeryLow);
	}

	mainTable->resizeColumnsToContents();
	mainTable->horizontalHeader()->setStretchLastSection(true);
}

void Organisateur::slotAddTask()
{
	QDialog *dialog = new QDialog(this);
		dialog->setWindowTitle("Ajouter une tâche");
		dialog->setWindowIcon(QIcon(":/icones/actions/actionOrganisteur.png"));

			QLineEdit *taskTitle = new QLineEdit;

			QTextEdit *taskContent = new QTextEdit;

			QComboBox *taskPriority = new QComboBox;
				taskPriority->addItems(QStringList() << "Très élevée" << "Élevée" << "Normale"
						<< "Faible" << "Très faible");
				taskPriority->setCurrentIndex(taskPriority->findText("Normale"));

			QPushButton *acceptDialog = new QPushButton("OK");
				connect(acceptDialog, SIGNAL(clicked()), dialog, SLOT(accept()));
				
			QPushButton *rejectDialog = new QPushButton("Annuler");
				connect(rejectDialog, SIGNAL(clicked()), dialog, SLOT(reject()));

			QFormLayout *dialogContent = new QFormLayout;
				dialogContent->addRow("Titre : ", taskTitle);
				dialogContent->addRow("Tâche : ", taskContent);
				dialogContent->addRow("Priorité : ", taskPriority);

			QHBoxLayout *buttonsLayout = new QHBoxLayout;
				buttonsLayout->addWidget(acceptDialog);
				buttonsLayout->addWidget(rejectDialog);
				buttonsLayout->setAlignment(Qt::AlignRight);

			QVBoxLayout *dialogMainLayout = new QVBoxLayout(dialog);
				dialogMainLayout->addLayout(dialogContent);
				dialogMainLayout->addLayout(buttonsLayout);

	if (!dialog->exec() == QDialog::Accepted)
		return;

	QString m_taskTitle = taskTitle->text();
	QString m_taskContent = taskContent->toPlainText();
	QString m_taskPriority = taskPriority->currentText();

	// Saving infos
		QSettings settings(Multiuso::appDirPath() + "/reglages/task_manager.ini", QSettings::IniFormat);
			
			int nextID = settings.value("nextID").toInt() + 1;

			settings.setValue("nextID", nextID);
			settings.setValue(QString::number(nextID) + "/content", QStringList() << m_taskTitle
					<< m_taskContent << m_taskPriority);

	initializeTasks(m_sortBy->currentText());
}

void Organisateur::slotEditTask()
{
}

void Organisateur::slotDeleteTask()
{
}

