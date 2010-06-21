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

#ifndef HEADER_BASEONGLET
#define HEADER_BASEONGLET

#include <QtPlugin>

class BaseOnglet : public QObject
{
	public:
		virtual QString nom() = 0; // setTabText()
		virtual QString icone() = 0; // setTabIcon()

		virtual QWidget *contenuTab() = 0; // Retourne le widget contenu dans l'onglet du QTabWidget

	protected:
		QString m_nom; // setText()
		QString m_icone; // setIcon()
};

Q_DECLARE_INTERFACE(BaseOnglet, "Multiuso.BaseOnglet")

#endif
