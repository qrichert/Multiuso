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

#include "Tarsier.h"

Tarsier::Tarsier()
{
	m_direction = "l";

	if (Multiuso::randomNumber(0, 1))
		m_direction = "r";

	m_state = 0;

	m_isPaused = false;

	m_tarsierAnimation = NULL;
}

QString Tarsier::direction()
{
	return m_direction;
}

int Tarsier::state()
{
	return m_state;
}

void Tarsier::climb()
{
	setPixmap(QPixmap(":/icones/feed_tarsiers/tarsier_" + m_direction + "_1.png"));

	m_state = 1;
}

void Tarsier::walk()
{
	setPixmap(QPixmap(":/icones/feed_tarsiers/tarsier_" + m_direction + "_2.png"));

	m_state = 2;
}

void Tarsier::takeCricket()
{
	setPixmap(QPixmap(":/icones/feed_tarsiers/tarsier_" + m_direction + "_3.png"));

	m_state = 3;
}

void Tarsier::jump()
{
	setPixmap(QPixmap(":/icones/feed_tarsiers/tarsier_" + m_direction + "_4.png"));

	m_state = 4;
}
