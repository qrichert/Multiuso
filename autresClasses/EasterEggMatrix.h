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

#ifndef HEADER_EASTEREGGMATRIX
#define HEADER_EASTEREGGMATRIX

#include "../CurrentIncludes.h"

class EasterEggMatrix : public QObject
{
	Q_OBJECT

	public:
		EasterEggMatrix(QObject *parent) : QObject(parent)
		{
			m_userName = QDir::home().dirName();
				m_userName = Multiuso::firstLetterToUpper(m_userName);
		}
		
		void start()
		{
			#ifdef Q_WS_X11
				std::cout << "\033[32;32m\n";
			#endif

			#ifdef Q_WS_MAC
				std::cout << "\033[32;32m\n";
			#endif

			#ifdef Q_WS_WIN
				if (system("color 02")) {}
			#endif

			clear();
		}
		
		void end()
		{
			#ifdef Q_WS_X11
				std::cout <<  "\033[0m\n";
			#endif

			#ifdef Q_WS_MAC
				std::cout <<  "\033[0m\n";
			#endif

			#ifdef Q_WS_WIN
				if (system("color 08")) {}
			#endif

			clear();
		}
		
		void clear()
		{
			#ifdef Q_WS_X11
				if (system("clear")) {}
			#endif

			#ifdef Q_WS_MAC
				if (system("clear")) {}
			#endif

			#ifdef Q_WS_WIN
				if (system("cls")) {}
			#endif
		}

	public slots:
		void showText()
		{
			static int step = 0;
				step++;

			clear();

			if (step == 1)
			{
				start();

				std::cout << "Wake up, " + m_userName.toStdString() + "...\n";
				
				QTimer::singleShot(15000, this, SLOT(showText()));
			}
			
			else if (step == 2)
			{
				std::cout << "The Matrix has you...\n";
				
				QTimer::singleShot(7000, this, SLOT(showText()));
			}
			
			else if (step == 3)
			{
				std::cout << "Follow the white rabbit.\n";

				QTimer::singleShot(8000, this, SLOT(showText()));
			}
			
			else if (step == 4)
			{
				std::cout << "Knock, knock, " + m_userName.toStdString() + ".\n";

				QTimer::singleShot(10000, this, SLOT(showText()));
			}
			
			else if (step == 5)
			{
				end();
			}
		}

	private:
		QString m_userName;
};

#endif
