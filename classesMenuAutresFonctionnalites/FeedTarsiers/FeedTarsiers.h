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

#ifndef HEADER_FEEDTARSIERS
#define HEADER_FEEDTARSIERS

#include "../CurrentIncludes.h"
#include "FeedTarsiersScene.h"

#define LABEL_HEIGHT 65

class CricketsLabel : public QWidget
{
	Q_OBJECT

	public:
		CricketsLabel()
		{
			setFixedHeight(LABEL_HEIGHT);

			m_crickets = 5;

			labelNumber = new QLabel;
				updateLabel();

			labelCricket = new QLabel;
				labelCricket->setPixmap(QPixmap(":/icones/feed_tarsiers/cricket.png"));

			QHBoxLayout *mainLayout = new QHBoxLayout(this);
				mainLayout->addWidget(labelNumber);
				mainLayout->addWidget(labelCricket);
				mainLayout->setAlignment(Qt::AlignLeft);
		}

		void updateLabel()
		{
			QString color = "black";

			if (m_crickets <= 3)
				color = "orange";

			if (m_crickets <= 1)
				color = "red";

			labelNumber->setText("<span style='font-size:45px;color:" + color + ";'>" +
					QString::number(m_crickets) + "</span>");
		}

		void setCricketsCount(int count)
		{
			m_crickets = count;

			updateLabel();
		}

	private:
		int m_crickets;
		QLabel *labelNumber;
		QLabel *labelCricket;
};

class ScoreLabel : public QWidget
{
	Q_OBJECT

	public:
		ScoreLabel()
		{
			setFixedHeight(LABEL_HEIGHT);

			m_score = 0;

			labelScore = new QLabel("<center>"
							"<span style='font-size:18px;'>Score :</span>"
						"</center>");

			labelNumber = new QLabel("<center>"
							"<span style='font-size:30px;'>" + QString::number(m_score) + "</span>"
						"</center>");

			QVBoxLayout *mainLayout = new QVBoxLayout(this);
				mainLayout->addWidget(labelScore);
				mainLayout->addWidget(labelNumber);
				mainLayout->setContentsMargins(0, 0, 0, 0);
		}

		void updateLabel()
		{
			labelNumber->setText("<center>"
							"<span style='font-size:30px;'>" + QString::number(m_score) + "</span>"
						"</center>");
		}

		void setScore(int score)
		{
			m_score = score;

			updateLabel();
		}

		int score()
		{
			return m_score;
		}

	private:
		int m_score;
		QLabel *labelScore;
		QLabel *labelNumber;
};

class JungleButton : public QPushButton
{
	Q_OBJECT

	public:
		JungleButton(QString text) : QPushButton(text)
		{
			m_text = text;

			setStyleSheet("border:2px solid lightgreen;"
					"background-color:lightgreen;"
					"color:green;"
					"font-size:15px;");
		}

	private:
		void enterEvent(QEvent *)
		{
			setStyleSheet("border:2px solid green;"
					"background-color:lightgreen;"
					"color:green;"
					"font-size:15px;");
		}

		void leaveEvent(QEvent *)
		{
			setStyleSheet("border:2px solid lightgreen;"
					"background-color:lightgreen;"
					"color:green;"
					"font-size:15px;");
		}

		void mousePressEvent(QMouseEvent *)
		{
			setStyleSheet("border:2px solid red;"
					"background-color:lightgreen;"
					"color:green;"
					"font-size:15px;");
		}

		void mouseReleaseEvent(QMouseEvent *)
		{
			setStyleSheet("border:2px solid green;"
					"background-color:lightgreen;"
					"color:green;"
					"font-size:15px;");

			emit clicked();
		}

	private:
		QString m_text;
};

class ScoreDialog : public QDialog
{
	Q_OBJECT

	public:
		ScoreDialog(QWidget *parent, int score) : QDialog(parent)
		{
			setWindowTitle("Scores");
			setWindowIcon(QIcon(":/icones/feed_tarsiers/tarsier_l_3.png"));

			m_score = score;

			QDir().mkpath(Multiuso::appDirPath() + "/textes/feed_tarsiers/");

			QFile file(Multiuso::appDirPath() + "/textes/feed_tarsiers/scores.mltsscores");

				if (file.open(QIODevice::Append | QIODevice::Text))
					file.write(QString("<strong>" + QString::number(m_score) + "</strong> pts<br />").toAscii());

				file.close();

			QLabel *label = new QLabel(
				"<center>"
					"<span style='font-size:20px;'>Votre score est de :</span><br />"
					"<span style='font-size:50px;'>" + QString::number(m_score) + "</span>"
				"</center>");

			JungleButton *buttonShowScores = new JungleButton("Voir les scores");
				connect(buttonShowScores, SIGNAL(clicked()), this, SLOT(showScores()));

			JungleButton *buttonOk = new JungleButton("Ok");
				connect(buttonOk, SIGNAL(clicked()), this, SLOT(accept()));

			QGridLayout *layout = new QGridLayout(this);
				layout->addWidget(label, 0, 0, 1, 3);
				layout->addWidget(new QLabel("<hr />"), 1, 0, 1, 3);
				layout->addWidget(buttonShowScores, 2, 1, 1, 1);
				layout->addWidget(buttonOk, 2, 2, 1, 1);
		}

	public slots:
		void showScores()
		{
			QString scores = "<span style='color:red;'>Impossible d'ouvrir le fichier contenant les scores !</span>";

			QFile file(Multiuso::appDirPath() + "/textes/feed_tarsiers/scores.mltsscores");

				if (file.open(QIODevice::ReadOnly | QIODevice::Text))
					scores = file.readAll();

				file.close();

			QDialog newDialog(this);
				newDialog.setWindowTitle("Scores");
				newDialog.setWindowIcon(QIcon(":/icones/feed_tarsiers/tarsier_l_3.png"));

				QTextEdit showScores;
					showScores.setHtml(scores);
					showScores.setReadOnly(true);

				JungleButton closeDialog("Fermer");
					connect(&closeDialog, SIGNAL(clicked()), &newDialog, SLOT(accept()));

				QVBoxLayout layout;
					layout.addWidget(&showScores);
					layout.addWidget(new QLabel("<hr />"));
					layout.addWidget(&closeDialog);

			newDialog.setLayout(&layout);
			newDialog.exec();
		}

	private:
		int m_score;
};

class SceneIntroButton : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT

	public:
		SceneIntroButton()
		{
		}

		void setNavigationButton(int button)
		{
			if (button == 0)
				setPixmap(QPixmap(":/icones/feed_tarsiers/informations.png"));

			else if (button == 1)
				setPixmap(QPixmap(":/icones/feed_tarsiers/suivant.png"));

			else if (button == 2)
				setPixmap(QPixmap(":/icones/feed_tarsiers/precedent.png"));

			else // == 3
				setPixmap(QPixmap(":/icones/feed_tarsiers/home.png"));
		}

	signals:
		void clicked();

	private:
		void mousePressEvent(QGraphicsSceneMouseEvent *event)
		{
			if (event->button() == Qt::LeftButton)
				emit clicked();
		}
};

class SceneHome : public QGraphicsScene
{
	Q_OBJECT

	public:
		SceneHome()
		{
			setSceneRect(0, 0, 400, 510);
			setBackgroundBrush(QPixmap(":/icones/feed_tarsiers/background_rules_0.png"));

			QTransform transform;
				transform.translate(75, 275);
				transform.rotate(-20);

			SceneIntroButton *button = new SceneIntroButton;
				button->setNavigationButton(0);
				button->setTransform(transform);
				connect(button, SIGNAL(clicked()), this, SIGNAL(showInfos()));

			addItem(button);
		}

	signals:
		void showInfos();
};

class SceneIntro : public QGraphicsScene
{
	Q_OBJECT

	public:
		SceneIntro()
		{
			setSceneRect(0, 0, 400, 510);
			setBackgroundBrush(QPixmap(":/icones/feed_tarsiers/background_rules_1.png"));

			m_page = 1;

			QTransform matrix;
				matrix.translate(250, 465);

			m_button = new SceneIntroButton;
				m_button->setNavigationButton(m_page);
				m_button->setTransform(matrix);
				connect(m_button, SIGNAL(clicked()), this, SLOT(switchPage()));

			QTransform matrixHome;
				matrixHome.translate(10, 450);

			SceneIntroButton *m_goToHome = new SceneIntroButton;
				m_goToHome->setNavigationButton(3);
				m_goToHome->setTransform(matrixHome);
				connect(m_goToHome, SIGNAL(clicked()), this, SIGNAL(goToHome()));

			addItem(m_button);
			addItem(m_goToHome);
		}

	public slots:
		void switchPage()
		{
			if (m_page == 1)
				m_page = 2;

			else // == 2
				m_page = 1;

			m_button->setNavigationButton(m_page);
			setBackgroundBrush(QPixmap(":/icones/feed_tarsiers/background_rules_" + QString::number(m_page) + ".png"));
		}

	signals:
		void goToHome();

	private:
		int m_page;
		SceneIntroButton *m_button;
};

class FeedTarsiers : public QDialog
{
	Q_OBJECT

	public:
		FeedTarsiers(QWidget *parent = 0);

	public slots:
		void takeCricket()
		{
			m_crickets--;

			cricketsLabel->setCricketsCount(m_crickets);

			if (m_crickets == 0)
			{
				int score = m_score;

				slotStopGame();

				ScoreDialog dialog(this, score);
					dialog.exec();
			}
		}

		void updateScore(int plus)
		{
			m_score += plus;

			scoreLabel->setScore(m_score);
		}

		void resetScores()
		{
			m_crickets = 5;
				cricketsLabel->setCricketsCount(m_crickets);

			m_score = 0;
				scoreLabel->setScore(m_score);
		}

		void slotStartGame();
		void slotStopGame();
		void slotRestartGame();
		void slotPauseGame();
		void slotResumeGame();

		void showInfos()
		{
			view->setScene(sceneIntro);
		}

		void showHome()
		{
			view->setScene(sceneHome);
		}

		void showGame()
		{
			view->setScene(scene);
		}

	private:
		int m_crickets;
		int m_score;
		CricketsLabel *cricketsLabel;
		ScoreLabel *scoreLabel;
		SceneHome *sceneHome;
		SceneIntro *sceneIntro;
		FeedTarsiersScene *scene;
		JungleButton *buttonStartGame;
		JungleButton *buttonStopGame;
		JungleButton *buttonRestartGame;
		JungleButton *buttonPauseGame;
		JungleButton *buttonResumeGame;
		QGraphicsView *view;
};

#endif
