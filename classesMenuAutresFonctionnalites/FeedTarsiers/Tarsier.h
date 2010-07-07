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

#ifndef HEADER_TARSIER
#define HEADER_TARSIER

#include "../CurrentIncludes.h"

class Fruit : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
	Q_PROPERTY(qreal scale READ scale WRITE setScale)

	public:
		Fruit()
		{
			setPixmap(QPixmap(":/icones/feed_tarsiers/fruit_" + QString::number(Multiuso::randomNumber(1, 10)) + ".png"));

			anim = new QPropertyAnimation(this, "scale");
				anim->setDuration(500);
				anim->setKeyValueAt(0, 1);
				anim->setKeyValueAt(1, 0.5);
		}

		void startAnim()
		{
			anim->start();
		}

	public slots:
		void emitDeleteRequest()
		{
			emit deleteRequest();
		}

	signals:
		void deleteRequest();

	private:
		QPropertyAnimation *anim;

};

class Tarsier : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)

	public:
		Tarsier();

		QString direction();
		int state();

		void climb();
		void walk();
		void takeCricket();
		void jump();

		void setPaused(bool paused)
		{
			m_isPaused = paused;
		}

		void setTarsierAnimation(QPropertyAnimation *animation)
		{
			m_tarsierAnimation = animation;
		}

		QPropertyAnimation *tarsierAnimation()
		{
			return m_tarsierAnimation;
		}

	private:
		void mousePressEvent(QGraphicsSceneMouseEvent *event)
		{
			if (event->button() == Qt::LeftButton && !m_isPaused)
				emit fed();
		}

	public slots:
		void emitMoveToCageRequest()
		{
			emit moveToCageRequest();
		}

		void emitTakeCricketRequest()
		{
			emit takeCricketRequest();
		}

		void emitJumpRequest()
		{
			emit jumpRequest();
		}

		void emitDeleteRequest()
		{
			emit deleteRequest();
		}

	signals:
		void moveToCageRequest();
		void takeCricketRequest();
		void jumpRequest();
		void fed();
		void deleteRequest();

	private:
		QString m_direction;
		int m_state;
		bool m_isPaused;

		QPropertyAnimation *m_tarsierAnimation;
};

#endif

