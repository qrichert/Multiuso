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

#include "FeedTarsiersScene.h"

FeedTarsiersScene::FeedTarsiersScene()
{
	setSceneRect(0, 0, 400, 510);
	setBackgroundBrush(QPixmap(":/icones/feed_tarsiers/background.png"));

	timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(addTarsier()));

	m_isPaused = false;
}

void FeedTarsiersScene::deleteAllTarsiers()
{
	foreach (Tarsier *tarsier, m_tarsiers)
	{
		tarsier->deleteLater();
		tarsier = NULL;
	}

	m_tarsiers.clear();
}

void FeedTarsiersScene::startGame()
{
	m_climbDuration = 5000;
	m_walkDuration = 1000;
	m_takeCricketDuration = 800;
	m_timerDelay = 1500; // One every two seconds

	timer->start(m_timerDelay);
}

void FeedTarsiersScene::stopGame()
{
	timer->stop(); // First we stop the creation of tarsiers
	deleteAllTarsiers(); // Whereupon we delete all the tarsiers :'(

	m_climbDuration = 5000;
	m_walkDuration = 1000;
	m_takeCricketDuration = 800;
	m_timerDelay = 1500;

	emit resetScores();
}

void FeedTarsiersScene::restartGame()
{
	stopGame();
	startGame();
}

void FeedTarsiersScene::pauseGame()
{
	timer->stop();

	foreach (Tarsier *tarsier, m_tarsiers)
	{
		tarsier->tarsierAnimation()->pause();
		tarsier->setPaused(true);
	}

	m_isPaused = true;
}

void FeedTarsiersScene::resumeGame()
{
	foreach (Tarsier *tarsier, m_tarsiers)
	{
		tarsier->tarsierAnimation()->resume();
		tarsier->setPaused(false);
	}

	timer->start(m_timerDelay);

	m_isPaused = false;
}

void FeedTarsiersScene::addTarsier()
{
	Tarsier *tarsier = new Tarsier;
		connect(tarsier, SIGNAL(moveToCageRequest()), this, SLOT(moveTarsierToCage()));
		connect(tarsier, SIGNAL(takeCricketRequest()), this, SLOT(tarsierTakeCricket()));
		connect(tarsier, SIGNAL(jumpRequest()), this, SLOT(tarsierJump()));
		connect(tarsier, SIGNAL(fed()), this, SLOT(tarsierFed()));
		connect(tarsier, SIGNAL(deleteRequest()), this, SLOT(tarsierDelete()));

	m_tarsiers << tarsier;

	addItem(tarsier);

	QPropertyAnimation *anim = new QPropertyAnimation(tarsier, "pos");
		anim->setDuration(m_climbDuration);

		if (tarsier->direction() == "l")
		{
			anim->setKeyValueAt(0, QPoint(110, 485));
			anim->setKeyValueAt(0.1, QPoint(120, 455));
			anim->setKeyValueAt(0.4, QPoint(135, 315));
			anim->setKeyValueAt(1, QPoint(110, 35));
		}

		else // direction == r
		{
			anim->setKeyValueAt(0, QPoint(250, 485));
			anim->setKeyValueAt(0.2, QPoint(215, 370));
			anim->setKeyValueAt(0.8, QPoint(220, 190));
			anim->setKeyValueAt(1, QPoint(225, 30));
		}

	connect(anim, SIGNAL(finished()), tarsier, SLOT(emitMoveToCageRequest()));
	tarsier->setTarsierAnimation(anim);

	tarsier->climb();
	anim->start();
}

void FeedTarsiersScene::moveTarsierToCage()
{
	Tarsier *tarsier = qobject_cast<Tarsier *>(sender());

	if (tarsier == 0)
		return;

	QPropertyAnimation *anim = new QPropertyAnimation(tarsier, "pos");
		anim->setDuration(m_walkDuration);

		if (tarsier->direction() == "l")
		{
			anim->setKeyValueAt(0, QPoint(115, 40));
			anim->setKeyValueAt(1, QPoint(150, 30));
		}

		else // direction == r
		{
			anim->setKeyValueAt(0, QPoint(220, 20));
			anim->setKeyValueAt(1, QPoint(185, 25));
		}

	connect(anim, SIGNAL(finished()), tarsier, SLOT(emitTakeCricketRequest()));
	tarsier->setTarsierAnimation(anim);

	tarsier->walk();
	anim->start();
}

void FeedTarsiersScene::tarsierTakeCricket()
{
	Tarsier *tarsier = qobject_cast<Tarsier *>(sender());

	if (tarsier == 0)
		return;

	QPropertyAnimation *anim = new QPropertyAnimation(tarsier, "pos");
		anim->setDuration(m_takeCricketDuration);

		if (tarsier->direction() == "l")
		{
			anim->setKeyValueAt(0, QPoint(150, 10));
			anim->setKeyValueAt(0.2, QPoint(148, 10));
			anim->setKeyValueAt(0.4, QPoint(150, 10));
			anim->setKeyValueAt(0.6, QPoint(148, 10));
			anim->setKeyValueAt(1, QPoint(150, 10));
		}

		else // direction == r
		{
			anim->setKeyValueAt(0, QPoint(190, 15));
			anim->setKeyValueAt(0.2, QPoint(192, 15));
			anim->setKeyValueAt(0.4, QPoint(190, 15));
			anim->setKeyValueAt(0.6, QPoint(192, 15));
			anim->setKeyValueAt(1, QPoint(190, 15));
		}

	connect(anim, SIGNAL(finished()), tarsier, SLOT(emitJumpRequest()));
	tarsier->setTarsierAnimation(anim);

	tarsier->takeCricket();
	anim->start();
}

void FeedTarsiersScene::tarsierJump()
{
	Tarsier *tarsier = qobject_cast<Tarsier *>(sender());

	if (tarsier == 0)
		return;

	emit cricketTaken();

	QPropertyAnimation *anim = new QPropertyAnimation(tarsier, "pos");
		anim->setDuration(1000);

		if (tarsier->direction() == "l")
		{
			anim->setKeyValueAt(0, QPoint(150, tarsier->pos().y()));
			anim->setKeyValueAt(1, QPoint(-100, tarsier->pos().y() - 25)); // - 25 to fall
		}

		else // direction == r
		{
			anim->setKeyValueAt(0, QPoint(185, tarsier->pos().y()));
			anim->setKeyValueAt(1, QPoint(500, tarsier->pos().y() - 25));
		}

	connect(anim, SIGNAL(finished()),  tarsier, SLOT(emitDeleteRequest()));
	tarsier->setTarsierAnimation(anim);

	tarsier->jump();
	anim->start();
}

void FeedTarsiersScene::tarsierFed()
{
	Tarsier *tarsier = qobject_cast<Tarsier *>(sender());

	if (tarsier == 0)
		return;

	tarsier->tarsierAnimation()->deleteLater();

	emit updateScore(1);

	QPropertyAnimation *anim = new QPropertyAnimation(tarsier, "pos");
		anim->setDuration(1000);

		if (tarsier->direction() == "l")
		{
			anim->setKeyValueAt(0, QPoint(150, tarsier->pos().y()));
			anim->setKeyValueAt(1, QPoint(-100, tarsier->pos().y() - 25)); // - 25 to fall
		}

		else // direction == r
		{
			anim->setKeyValueAt(0, QPoint(185, tarsier->pos().y()));
			anim->setKeyValueAt(1, QPoint(500, tarsier->pos().y() - 25));
		}

	connect(anim, SIGNAL(finished()),  tarsier, SLOT(emitDeleteRequest()));
	tarsier->setTarsierAnimation(anim);

	// The thrown fruit

	Fruit *fruit = new Fruit;
		connect(fruit, SIGNAL(deleteRequest()), this, SLOT(fruitDelete()));

	QPropertyAnimation *animFruit = new QPropertyAnimation(fruit, "pos");
		animFruit->setDuration(500);
		animFruit->setKeyValueAt(0, QPoint(180, 550));

		if (tarsier->direction() == "l")
			animFruit->setKeyValueAt(1, QPoint(45, tarsier->pos().y()));

		else
			animFruit->setKeyValueAt(1, QPoint(350, tarsier->pos().y()));

	addItem(fruit);

	// Now we can anim all these items

	tarsier->jump();
	anim->start();

	animFruit->start();
	fruit->startAnim();

	QTimer::singleShot(500, fruit, SLOT(emitDeleteRequest()));
}

void FeedTarsiersScene::tarsierDelete()
{
	Tarsier *tarsier = qobject_cast<Tarsier *>(sender());

	if (tarsier == 0)
		return;

	m_tarsiers.removeOne(tarsier);

	tarsier->deleteLater();
	tarsier = NULL;

	addTarsier();
}

void FeedTarsiersScene::fruitDelete()
{
	Fruit *fruit = qobject_cast<Fruit *>(sender());

	if (fruit == 0)
		return;

	fruit->deleteLater();
	fruit = 0;
}
