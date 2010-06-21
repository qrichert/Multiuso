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

#ifndef HEADER_BASEACTION
#define HEADER_BASEACTION

#include <QtPlugin>

class BaseAction : public QObject
{
	Q_OBJECT
	
	public:
		virtual QString nom() = 0; // text()
		virtual QString icone() = 0; // icon()
		virtual QString aide() = 0; // toolTip()
		virtual QString informations() = 0; // statusTip()
		virtual double menuParent() = 0; // Menu auquel appartient l'action

		/*
		 * "Multiuso" → 1
		 * "Édition" → 2
		 * "Affichage" → 3
		 * "Autres fonctionnalités" → 4
		 * 	"Programmation" → 4.1
		 *	"Mini-Jeux" → 4.2
		 * "Aide" → 5
		 */

	public slots:
		virtual void ouvrir() = 0; // Connecté au signal triggered() de l'action

	protected:
		QString m_nom; // setText()
		QString m_icone; // setIcon()
		QString m_aide; // setToolTip()
		QString m_informations; // setStatusTip()
		double m_menuParent; // Changer le menu auquel appartient l'action
};

Q_DECLARE_INTERFACE(BaseAction, "Multiuso.BaseAction")

#endif
