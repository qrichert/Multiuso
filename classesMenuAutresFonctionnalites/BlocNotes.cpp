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

	m_noteTitle = new QLineEdit; // Bouton ajouter à côté

	m_tabWidget = new QTabWidget;
		m_tabWidget->setDocumentMode(true);
		m_tabWidget->setTabsClosable(true);
		m_tabWidget->setMovable(true);

	m_tabWidgetLayout = new QVBoxLayout;
		m_tabWidgetLayout->addWidget(m_tabWidget);

	m_container = new QWidget;

	QVBoxLayout *containerLayout = new QVBoxLayout;
		containerLayout->addWidget(m_container);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
		mainLayout->addWidget(m_noteTitle);
		mainLayout->addLayout(containerLayout);
		mainLayout->addWidget(Multiuso::closeButton(this));

	loadNotes();
}

void BlocNotes::loadNotes()
{
	QSettings settings(Multiuso::appDirPath() + "/reglages/bloc_notes.ini", QSettings::IniFormat);

	foreach (QString page, settings.childGroups())
	{
		addTab();

		m_tabWidget->widget(m_tabWidget->count() - 1)->findChild<QPlainTextEdit *>()->setPlainText(settings.value(page + "/content").toString());
	
		connect(m_tabWidget->widget(m_tabWidget->count() - 1)->findChild<QPlainTextEdit *>(), SIGNAL(textChanged()), this, SLOT(saveText()));
	}

	if (m_tabWidget->count() == 0)
		addTab();

	updateView();
}

void BlocNotes::updateView()
{
	if (m_tabWidget->count() == 1)
		m_container->setLayout(m_tabWidget->currentWidget()->layout());

	else
		m_container->setLayout(m_tabWidgetLayout);
}

QPlainTextEdit *BlocNotes::currentTextEdit()
{
	return m_tabWidget->currentWidget()->findChild<QPlainTextEdit *>();
}

void BlocNotes::addTab()
{
	QPlainTextEdit *textEdit = new QPlainTextEdit;
	
	QVBoxLayout *textEditLayout = new QVBoxLayout;
		textEditLayout->addWidget(textEdit);

	QWidget *container = new QWidget;
		container->setLayout(textEditLayout);

	m_tabWidget->addTab(container, "Nouvelle note");
}

void BlocNotes::saveText()
{
	QPlainTextEdit *textEdit = qobject_cast<QPlainTextEdit *>(sender());

	if (textEdit == 0)
		return;

	QSettings settings(Multiuso::appDirPath() + "/reglages/bloc_notes.ini", QSettings::IniFormat);
		settings.setValue(QString::number(m_tabWidget->currentIndex()) + "/content", textEdit->toPlainText());
}
