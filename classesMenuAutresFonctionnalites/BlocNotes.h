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

#ifndef HEADER_BLOC_NOTES
#define HEADER_BLOC_NOTES

#include "../CurrentIncludes.h"

class BlocNotes : public QDialog
{
	Q_OBJECT

	public:
		BlocNotes(QWidget *parent);

		void loadNotes();
		void updateView();
		QPlainTextEdit *currentTextEdit();

	public slots:
		void addTab();
		void saveText();

	private:
		QLineEdit *m_noteTitle;
		QTabWidget *m_tabWidget;
		QVBoxLayout *m_tabWidgetLayout;
		QWidget *m_container;

};

#endif
