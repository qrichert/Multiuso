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

#ifndef HEADER_FEEDTARSIERSSCENE
#define HEADER_FEEDTARSIERSSCENE

#include "../CurrentIncludes.h"
#include "Tarsier.h"

class FeedTarsiersScene : public QGraphicsScene
{
	Q_OBJECT

	public:
		FeedTarsiersScene();

		void deleteAllTarsiers();

		void startGame();
		void stopGame();
		void restartGame();
		void pauseGame();
		void resumeGame();

	public slots:
		void addTarsier();
		void moveTarsierToCage();
		void tarsierTakeCricket();
		void tarsierJump();
		void tarsierFed();
		void tarsierDelete();
		void fruitDelete();

	signals:
		void cricketTaken();
		void updateScore(int plus);
		void resetScores();

	private:
		QList<Tarsier *> m_tarsiers;

		int m_climbDuration;
		int m_walkDuration;
		int m_takeCricketDuration;
		int m_timerDelay;

		QTimer *timer;

		bool m_isPaused;
};

#endif
