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

#ifndef HEADER_ACCUEIL
#define HEADER_ACCUEIL

#include "../CurrentIncludes.h"

class HomePicture : public QLabel
{
	Q_OBJECT

	public:
		HomePicture(QWidget *parent = 0, Qt::WindowFlags f = 0) : QLabel(parent, f)
		{
			setProperties();
		}

		HomePicture(const QString &text, QWidget *parent = 0, Qt::WindowFlags f = 0) : QLabel(text, parent, f)
		{
			setProperties();
		}

	public:
		void setProperties()
		{
			setPixmap(QPixmap(":/images/accueil.png"));
			setFixedSize(660, 275);
		}
	
	protected:
		void enterEvent(QEvent *)
		{
			setPixmap(QPixmap(":/images/activated_accueil.png"));
		}

		void leaveEvent(QEvent *)
		{
			setPixmap(QPixmap(":/images/accueil.png"));
		}

		void mousePressEvent(QMouseEvent *ev)
		{
			if (ev->button() == Qt::LeftButton)
				emit clicked();

			else
				QLabel::mousePressEvent(ev);
		}

	signals:
		void clicked();
};

class OpenButton : public QPushButton
{
	Q_OBJECT

	public:
		OpenButton(QWidget *parent = 0) : QPushButton(parent)
		{
			setProperties();
		}

		OpenButton(const QString &text, QWidget *parent = 0) : QPushButton(text, parent)
		{
			setProperties();
		}

		OpenButton(const QIcon &icon, const QString &text, QWidget *parent = 0) : QPushButton(icon, text, parent)
		{
			setProperties();
		}

		void setProperties()
		{
			setFixedSize(135, 135);
			setIconSize(QSize(113, 113));
			setStyleSheet("background-color: transparent; border: 0; outline: 0;");
			m_active = false;
		}

		void setActive(bool active)
		{
			m_active = active;
		}

		bool isActive()
		{
			return m_active;
		}

		void emitClicked()
		{
			emit clicked();
		}

	signals:
		void activeHover(QString name);
		void inactiveHover();

	protected:
		void enterEvent(QEvent *)
		{
			setIconSize(QSize(128, 128));

			emit activeHover(objectName());
		}

		void leaveEvent(QEvent *)
		{
			setIconSize(QSize(113, 113));

			emit inactiveHover();
		}
	
	private:
		bool m_active;
};

class ButtonLabel : public QLabel
{
	Q_OBJECT

	public:
		ButtonLabel(QWidget *parent = 0) : QLabel(parent)
		{
			setFixedHeight(50);
			setText("");
		
			setStyleSheet("background-color: transparent;");
		}

	public slots:
		void setLabelText(QString text)
		{
			setText("<center>"
					"<span style='font-weight:bold;color:#cfcfcf;'>"
						+ text +
					"</span>"
				"</center>");
		}

		void noLabelText()
		{
			setText("");
		}
};

class Accueil : public QWidget
{
	#define ANIMATION_DURATION 250

	Q_OBJECT

	public:
		Accueil(QWidget *parent);

		void restoreOpenedTabs();
		void openTab(QString tabName);
		int openedPage();

	protected:
		void dragEnterEvent(QDragEnterEvent *event)
		{
			if (event->mimeData()->hasFormat("text/uri-list"))
				event->acceptProposedAction();
		}

		void dropEvent(QDropEvent *event);

	public slots:
		void actualiserFond();
		void buttonClicked();
		void showButtons();
		void showButtons2();
		void showButtons3();
		void showPicture();
		void showPicture2();
		void showPicture3();

		void slotActiveHover(QString buttonName);
		void slotInactiveHover();

	private:
		QWidget *parentPointer;
		QMdiArea *aireCentrale;
		QStackedWidget *widgetCentralArea;
		QPushButton *showPictureButton;

		QWidget *widgetImage;
		QWidget *widgetOpen;

		ButtonLabel *buttonsLabel;

		OpenButton *fileBrowser;
		OpenButton *widgets;
		OpenButton *pictureViewer;
		OpenButton *textEdit;
		OpenButton *codeEdit;
		OpenButton *ftp;
		OpenButton *webBrowser;

		QPoint oldCentralWidgetPos;
		bool transitionInProgress;
};

#endif
