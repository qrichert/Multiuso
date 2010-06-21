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

#ifndef HEADER_BASEDOCKWIDGET
#define HEADER_BASEDOCKWIDGET

#include <QtPlugin>

class BaseDockWidget : public QObject
{
	public:
		virtual QString nom() = 0; // windowTitle()
		virtual QString icone() = 0; // windowIcon()
		virtual int aireDockWidget() = 0; // Emplacement du QDockWidget
		
		/*
		 * 1 = Haut
		 * 2 = Bas
		 * 3 = Gauche
		 * 4 = Droite
		*/
		
		virtual QWidget *contenuDockWidget() = 0; // Retourne le widget contenu dans le QDockWidget

	protected:
		QString m_nom; // setWindowTitle()
		QString m_icone; // setWindowIcon()
		int m_aireDockWidget;
};

Q_DECLARE_INTERFACE(BaseDockWidget, "Multiuso.BaseDockWidget")

#endif
