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

#include "FeedTarsiers.h"

FeedTarsiers::FeedTarsiers(QWidget *parent) : QDialog(parent)
{
	setWindowTitle("FeedTarsiers");
	setWindowIcon(QIcon(":/icones/feed_tarsiers/tarsier_l_3.png"));
	setCursor(QCursor(QPixmap(":/icones/feed_tarsiers/cursor.png"), 21, 21));

	m_crickets = 5;
	m_score = 0;

	cricketsLabel = new CricketsLabel;
	scoreLabel = new ScoreLabel;

	sceneHome = new SceneHome;
		connect(sceneHome, SIGNAL(showInfos()), this, SLOT(showInfos()));

	sceneIntro = new SceneIntro;
		connect(sceneIntro, SIGNAL(goToHome()), this, SLOT(showHome()));

	scene = new FeedTarsiersScene;
		connect(scene, SIGNAL(cricketTaken()), this, SLOT(takeCricket()));
		connect(scene, SIGNAL(updateScore(int)), this, SLOT(updateScore(int)));
		connect(scene, SIGNAL(resetScores()), this, SLOT(resetScores()));

	buttonStartGame = new JungleButton("Démarrer");
		connect(buttonStartGame, SIGNAL(clicked()), this, SLOT(slotStartGame()));

	buttonStopGame = new JungleButton("Arrêter");
		buttonStopGame->setVisible(false);
		connect(buttonStopGame, SIGNAL(clicked()), this, SLOT(slotStopGame()));

	buttonRestartGame = new JungleButton("Redémarrer");
		buttonRestartGame->setDisabled(true);
		connect(buttonRestartGame, SIGNAL(clicked()), this, SLOT(slotRestartGame()));

	buttonPauseGame = new JungleButton("Pause");
		buttonPauseGame->setDisabled(true);
		connect(buttonPauseGame, SIGNAL(clicked()), this, SLOT(slotPauseGame()));

	buttonResumeGame = new JungleButton("Continuer");
		buttonResumeGame->setDisabled(true);
		connect(buttonResumeGame, SIGNAL(clicked()), this, SLOT(slotResumeGame()));

	view = new QGraphicsView;
		view->setScene(sceneHome);
		view->setFixedSize(402, 512);

	QGridLayout *mainLayout = new QGridLayout(this);
		mainLayout->addWidget(cricketsLabel, 0, 0, 2, 1);
		mainLayout->addWidget(buttonStartGame, 0, 1, 1, 1);
		mainLayout->addWidget(buttonStopGame, 0, 1, 1, 1);
		mainLayout->addWidget(buttonRestartGame, 0, 2, 1, 1);
		mainLayout->addWidget(scoreLabel, 0, 3, 2, 1);
		mainLayout->addWidget(buttonPauseGame, 1, 1, 1, 1);
		mainLayout->addWidget(buttonResumeGame, 1, 2, 1, 1);
		mainLayout->addWidget(view, 2, 0, 1, 4);
		mainLayout->setContentsMargins(0, 0, 0, 0);
}

void FeedTarsiers::slotStartGame()
{
	buttonStartGame->setVisible(false);
	buttonStopGame->setVisible(true);

	buttonRestartGame->setDisabled(false);

	buttonPauseGame->setDisabled(false);
	buttonResumeGame->setDisabled(true);

	showGame();
	scene->startGame();
}

void FeedTarsiers::slotStopGame()
{
	buttonStartGame->setVisible(true);
	buttonStopGame->setVisible(false);

	buttonRestartGame->setDisabled(true);

	buttonPauseGame->setDisabled(true);
	buttonResumeGame->setDisabled(true);

	showHome();
	scene->stopGame();
}

void FeedTarsiers::slotRestartGame()
{
	buttonPauseGame->setDisabled(false);
	buttonResumeGame->setDisabled(true);

	scene->restartGame();
}

void FeedTarsiers::slotPauseGame()
{
	buttonPauseGame->setDisabled(true);
	buttonResumeGame->setDisabled(false);

	scene->pauseGame();
}

void FeedTarsiers::slotResumeGame()
{
	buttonPauseGame->setDisabled(false);
	buttonResumeGame->setDisabled(true);

	scene->resumeGame();
}

