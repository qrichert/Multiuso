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

#include "BlocNotes.h"

BlocNotes::BlocNotes(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Bloc-Notes");
	setWindowIcon(QIcon(":/icones/actions/actionBlocNotes.png"));

	resize(Multiuso::screenWidth() / 2, Multiuso::screenHeight() / 2);

	QAction *actionAddTab = new QAction(this);
		actionAddTab->setIcon(QIcon(":/icones/bloc_notes/actionAddTab.png"));
		connect(actionAddTab, SIGNAL(triggered()), this, SLOT(addTab()));

	QToolButton *buttonAddTab = new QToolButton;
		buttonAddTab->setDefaultAction(actionAddTab);
		buttonAddTab->setAutoRaise(true);

	m_tabWidget = new QTabWidget;
		m_tabWidget->setDocumentMode(true);
		//m_tabWidget->setTabPosition(QTabWidget::East);
		m_tabWidget->setCornerWidget(buttonAddTab, Qt::BottomLeftCorner);
		connect(m_tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(removeTab(int)));
		connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

	m_containerLayout = new QVBoxLayout;
		m_containerLayout->addWidget(m_tabWidget);
		m_containerLayout->setContentsMargins(0, 0, 0, 0);

	m_container = new QWidget;
		m_container->setLayout(m_containerLayout);

	QVBoxLayout *containerLayout = new QVBoxLayout;
		containerLayout->addWidget(m_container);
		containerLayout->setContentsMargins(0, 0, 0, 0);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
		mainLayout->addLayout(containerLayout);
		mainLayout->addWidget(Multiuso::closeButton(this));
		mainLayout->setContentsMargins(4, 4, 4, 4);

	loadNotes();
}

void BlocNotes::loadNotes()
{
	QSettings settings(Multiuso::appDirPath() + "/reglages/bloc_notes.ini", QSettings::IniFormat);
		
	int last_index = settings.value("last_index").toInt();

	foreach (QString page, settings.childGroups())
	{
		addTab(false);

		currentTextEdit(m_tabWidget->count() - 1)->setPlainText(
			settings.value(page + "/content").toString());
	
		connect(currentTextEdit(m_tabWidget->count() - 1),
			SIGNAL(textChanged()), this, SLOT(saveText()));
	}

	if (m_tabWidget->count() == 0)
		addTab();

	updateView();

	m_tabWidget->setCurrentIndex(last_index);
}

void BlocNotes::updateView()
{
	if (m_tabWidget->count() == 1)
		m_tabWidget->setTabsClosable(false);

	else
		m_tabWidget->setTabsClosable(true);

	for (int i = 0; i < m_tabWidget->count(); i++)
	{
		QString text = currentTextEdit(i)->toPlainText();

		if (text.isEmpty())
			text = "(vide)";

		else	
			text = text.left(text.indexOf("\n"));

		if (text.size() > 17)
			text = text.left(14) + "...";

		m_tabWidget->setTabText(i, text);
	}
}

QPlainTextEdit *BlocNotes::currentTextEdit(int index)
{
	if (index == -1)
		return m_tabWidget->currentWidget()->findChild<QPlainTextEdit *>();
	
	else	
		return m_tabWidget->widget(index)->findChild<QPlainTextEdit *>();
}

void BlocNotes::addTab(bool connect)
{
	QPlainTextEdit *textEdit = new QPlainTextEdit;

	if (connect)
		QObject::connect(textEdit, SIGNAL(textChanged()), this, SLOT(saveText()));

	
	QVBoxLayout *textEditLayout = new QVBoxLayout;
		textEditLayout->addWidget(textEdit);
		textEditLayout->setContentsMargins(0, 0, 0, 0);

	QWidget *container = new QWidget;
		container->setLayout(textEditLayout);

	m_tabWidget->addTab(container, "(vide)");

	updateView();

	m_tabWidget->setCurrentIndex(m_tabWidget->count() - 1);
}

void BlocNotes::removeTab(int index)
{
	m_tabWidget->widget(index)->deleteLater();
	m_tabWidget->removeTab(index);

	QFile::remove(Multiuso::appDirPath() + "/reglages/bloc_notes.ini");

	QSettings settings(Multiuso::appDirPath() + "/reglages/bloc_notes.ini", QSettings::IniFormat);

	for (int i = 0; i < m_tabWidget->count(); i++)
		settings.setValue(QString::number(i) + "/content", currentTextEdit(i)->toPlainText());

	updateView();
	
	tabChanged(m_tabWidget->currentIndex());
}

void BlocNotes::tabChanged(int tab)
{
	QSettings settings(Multiuso::appDirPath() + "/reglages/bloc_notes.ini", QSettings::IniFormat);
		settings.setValue("last_index", tab);
}

void BlocNotes::saveText()
{
	QPlainTextEdit *textEdit = qobject_cast<QPlainTextEdit *>(sender());

	if (textEdit == 0)
		return;

	QSettings settings(Multiuso::appDirPath() + "/reglages/bloc_notes.ini", QSettings::IniFormat);
		settings.setValue(QString::number(m_tabWidget->currentIndex()) + "/content", textEdit->toPlainText());


	updateView();
}
