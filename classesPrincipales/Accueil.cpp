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

#include "Accueil.h"
#include "FenPrincipale.h"

Accueil::Accueil(QWidget *parent = 0) : QWidget(parent)
{
	parentPointer = parent;

	setAcceptDrops(true);

// Picture
	HomePicture *labelAccueil = new HomePicture;
		connect(labelAccueil, SIGNAL(clicked()), this, SLOT(showButtons()));

	QVBoxLayout *layoutImage = new QVBoxLayout;
		layoutImage->addWidget(labelAccueil);
		layoutImage->setContentsMargins(0, 0, 0, 0);

	widgetImage = new QWidget;
		widgetImage->setLayout(layoutImage);

// Label(s)
	buttonsLabel = new ButtonLabel;

	QHBoxLayout *layoutButtonsLabel = new QHBoxLayout;
		layoutButtonsLabel->addWidget(buttonsLabel);
		layoutButtonsLabel->setContentsMargins(0, 0, 0, 25);

// Buttons
	fileBrowser = new OpenButton;
		fileBrowser->setIcon(QIcon(":/icones/accueil/filebrowser.png"));
		fileBrowser->setObjectName("filebrowser");
		connect(fileBrowser, SIGNAL(clicked()), this, SLOT(buttonClicked()));
		connect(fileBrowser, SIGNAL(activeHover(QString)), this, SLOT(slotActiveHover(QString)));
		connect(fileBrowser, SIGNAL(inactiveHover()), this, SLOT(slotInactiveHover()));

	widgets = new OpenButton;
		widgets->setIcon(QIcon(":/icones/accueil/widgets.png"));
		widgets->setObjectName("widgets");
		connect(widgets, SIGNAL(clicked()), this, SLOT(buttonClicked()));
		connect(widgets, SIGNAL(activeHover(QString)), this, SLOT(slotActiveHover(QString)));
		connect(widgets, SIGNAL(inactiveHover()), this, SLOT(slotInactiveHover()));


	pictureViewer = new OpenButton;
		pictureViewer->setIcon(QIcon(":/icones/accueil/pictureviewer.png"));
		pictureViewer->setObjectName("pictureviewer");
		connect(pictureViewer, SIGNAL(clicked()), this, SLOT(buttonClicked()));
		connect(pictureViewer, SIGNAL(activeHover(QString)), this, SLOT(slotActiveHover(QString)));
		connect(pictureViewer, SIGNAL(inactiveHover()), this, SLOT(slotInactiveHover()));


	textEdit = new OpenButton;
		textEdit->setIcon(QIcon(":/icones/accueil/textedit.png"));
		textEdit->setObjectName("textedit");
		connect(textEdit, SIGNAL(clicked()), this, SLOT(buttonClicked()));
		connect(textEdit, SIGNAL(activeHover(QString)), this, SLOT(slotActiveHover(QString)));
		connect(textEdit, SIGNAL(inactiveHover()), this, SLOT(slotInactiveHover()));


	codeEdit = new OpenButton;
		codeEdit->setIcon(QIcon(":/icones/accueil/codeedit.png"));
		codeEdit->setObjectName("codeedit");
		connect(codeEdit, SIGNAL(clicked()), this, SLOT(buttonClicked()));
		connect(codeEdit, SIGNAL(activeHover(QString)), this, SLOT(slotActiveHover(QString)));
		connect(codeEdit, SIGNAL(inactiveHover()), this, SLOT(slotInactiveHover()));


	ftp = new OpenButton;
		ftp->setIcon(QIcon(":/icones/accueil/ftp.png"));
		ftp->setObjectName("ftp");
		connect(ftp, SIGNAL(clicked()), this, SLOT(buttonClicked()));
		connect(ftp, SIGNAL(activeHover(QString)), this, SLOT(slotActiveHover(QString)));
		connect(ftp, SIGNAL(inactiveHover()), this, SLOT(slotInactiveHover()));


	webBrowser = new OpenButton;
		webBrowser->setIcon(QIcon(":/icones/accueil/webbrowser.png"));
		webBrowser->setObjectName("webbrowser");
		connect(webBrowser, SIGNAL(clicked()), this, SLOT(buttonClicked()));
		connect(webBrowser, SIGNAL(activeHover(QString)), this, SLOT(slotActiveHover(QString)));
		connect(webBrowser, SIGNAL(inactiveHover()), this, SLOT(slotInactiveHover()));


	QHBoxLayout *layoutOpen1l = new QHBoxLayout; // 1st line
		layoutOpen1l->addWidget(fileBrowser);
		layoutOpen1l->addWidget(widgets);
		layoutOpen1l->addWidget(pictureViewer);
		layoutOpen1l->addWidget(textEdit);

	QHBoxLayout *layoutOpen2l = new QHBoxLayout; // 2nd line
		layoutOpen2l->addWidget(codeEdit);
		layoutOpen2l->addWidget(ftp);
		layoutOpen2l->addWidget(webBrowser);

	QGridLayout *layoutOpen = new QGridLayout;
		layoutOpen->addLayout(layoutButtonsLabel, 0, 0);
		layoutOpen->addLayout(layoutOpen1l, 1, 0);
		layoutOpen->addLayout(layoutOpen2l, 2, 0);
		layoutOpen->setContentsMargins(0, 0, 0, 0);

	widgetOpen = new QWidget;
		widgetOpen->setLayout(layoutOpen);

// Central area
	widgetCentralArea = new QStackedWidget;
		widgetCentralArea->addWidget(widgetImage);
		widgetCentralArea->addWidget(widgetOpen);

	oldCentralWidgetPos = widgetCentralArea->pos();

	QGridLayout *layoutCentralArea = new QGridLayout;
		layoutCentralArea->addWidget(widgetCentralArea, 0, 0, 1, 2, Qt::AlignCenter);

	aireCentrale = new QMdiArea;
		aireCentrale->setLayout(layoutCentralArea);

	QSettings optionsAccueil(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);

		if (optionsAccueil.value("accueil/fond_screenshot").toBool())
		{
			aireCentrale->setBackground(QPixmap::grabWindow(QApplication::desktop()->winId()));
		}

		else
		{
			if (QFile::exists(Multiuso::appDirPath() + "/extensions/images/fond_accueil.png"))
				aireCentrale->setBackground(QPixmap(Multiuso::appDirPath() + "/extensions/images/fond_accueil.png"));

			else
				aireCentrale->setBackground(Qt::black);
		}

	showPictureButton = new QPushButton(aireCentrale);
		showPictureButton->move(10, 10);
		showPictureButton->resize(50, 50);
		showPictureButton->setIcon(QIcon(":/icones/accueil/home.png"));
		showPictureButton->setIconSize(QSize(48, 48));
		connect(showPictureButton, SIGNAL(clicked()), this, SLOT(showPicture()));
		showPictureButton->hide();

	QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);
		layoutPrincipal->addWidget(aireCentrale);
		layoutPrincipal->setContentsMargins(0, 0, 0, 0);

	transitionInProgress = false;

	if (optionsAccueil.value("ouverture/page").toInt() == 0)
	{
		widgetCentralArea->setCurrentIndex(0);
		showPictureButton->hide();
	}

	else
	{
		widgetCentralArea->setCurrentIndex(1);
		showPictureButton->show();
	}
}

void Accueil::restoreOpenedTabs()
{
	QSettings optionsAccueil(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);

	QStringList openedTabs = optionsAccueil.value("ouverture/onglets").value<QStringList>();

	foreach (QString tabName, openedTabs)
	{
		if (tabName == "Navigateur de fichiers")
			fileBrowser->emitClicked();

		else if (tabName == "Widgets")
			widgets->emitClicked();

		else if (tabName == "Visionneur d'images")
			pictureViewer->emitClicked();

		else if (tabName == "Éditeur de texte")
			textEdit->emitClicked();

		else if (tabName == "Éditeur de code")
			codeEdit->emitClicked();

		else if (tabName == "Client FTP")
			ftp->emitClicked();

		else if (tabName == "Navigateur Web")
			webBrowser->emitClicked();
	}
}

void Accueil::openTab(QString tabName)
{
	if (tabName == "Navigateur de fichiers")
		fileBrowser->emitClicked();

	else if (tabName == "Widgets")
		widgets->emitClicked();

	else if (tabName == "Visionneur d'images")
		pictureViewer->emitClicked();

	else if (tabName == "Éditeur de texte")
		textEdit->emitClicked();

	else if (tabName == "Éditeur de code")
		codeEdit->emitClicked();

	else if (tabName == "Client FTP")
		ftp->emitClicked();

	else if (tabName == "Navigateur Web")
		webBrowser->emitClicked();
}

int Accueil::openedPage()
{
	return widgetCentralArea->currentIndex();
}

void Accueil::dropEvent(QDropEvent *event)
{
	FenPrincipale *fenP = qobject_cast<FenPrincipale *>(parentPointer);

	if (fenP == 0)
		return;

	const QMimeData *dropData = event->mimeData();

	if (!dropData->hasUrls())
		return;

	foreach (QUrl url, dropData->urls())
	{
		QString urlPath = url.path();

		if (Multiuso::currentOS() == "windows")
			urlPath = urlPath.right(urlPath.length() - 1);

		if (QFileInfo(urlPath).exists())
			fenP->ouvrirFichier(urlPath);
	}
}

void Accueil::actualiserFond()
{
	QSettings optionsAccueil(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);

		if (optionsAccueil.value("accueil/fond_screenshot").toBool())
		{
			aireCentrale->setBackground(QPixmap::grabWindow(QApplication::desktop()->winId()));
		}

		else
		{
			if (QFile::exists(Multiuso::appDirPath() + "/extensions/images/fond_accueil.png"))
				aireCentrale->setBackground(QPixmap(Multiuso::appDirPath() + "/extensions/images/fond_accueil.png"));

			else
				aireCentrale->setBackground(Qt::black);
		}
}

void Accueil::buttonClicked()
{
	if (transitionInProgress)
		return;

	FenPrincipale *fenP = qobject_cast<FenPrincipale *>(parentPointer);

	if (fenP == 0)
		return;

	OpenButton *button = qobject_cast<OpenButton *>(sender());

	if (button == 0)
		return;

	QString objtName = button->objectName();

	if (button->isActive())
	{
		button->setActive(false);
		button->setIcon(QIcon(":/icones/accueil/" + objtName + ".png"));
	}

	else
	{
		button->setActive(true);
		button->setIcon(QIcon(":/icones/accueil/activated_" + objtName + ".png"));
	}

	fenP->showTab(objtName, button->isActive());
}

void Accueil::showButtons()
{
	if (transitionInProgress)
		return;

	transitionInProgress = true;

	oldCentralWidgetPos = widgetCentralArea->pos();

	QPropertyAnimation *anim = new QPropertyAnimation(widgetCentralArea, "pos");
		anim->setDuration(ANIMATION_DURATION);
		anim->setKeyValueAt(0, oldCentralWidgetPos);
		anim->setKeyValueAt(1, QPoint(-(widgetCentralArea->width()), widgetCentralArea->pos().y()));
		anim->start();

	QTimer::singleShot(ANIMATION_DURATION, this, SLOT(showButtons2()));
}

void Accueil::showButtons2()
{
	widgetCentralArea->setCurrentIndex(1);

	QPropertyAnimation *anim = new QPropertyAnimation(widgetCentralArea, "pos");
		anim->setDuration(ANIMATION_DURATION);
		anim->setKeyValueAt(0, QPoint(oldCentralWidgetPos.x(), -(widgetCentralArea->height())));
		anim->setKeyValueAt(1, oldCentralWidgetPos);
		anim->start();

	QTimer::singleShot(ANIMATION_DURATION, this, SLOT(showButtons3()));
}

void Accueil::showButtons3()
{
	showPictureButton->show();

	transitionInProgress = false;
}

void Accueil::showPicture()
{
	if (transitionInProgress)
		return;

	transitionInProgress = true;

	oldCentralWidgetPos = widgetCentralArea->pos();

	showPictureButton->hide();

	QPropertyAnimation *anim = new QPropertyAnimation(widgetCentralArea, "pos");
		anim->setDuration(ANIMATION_DURATION);
		anim->setKeyValueAt(0, oldCentralWidgetPos);
		anim->setKeyValueAt(1, QPoint(oldCentralWidgetPos.y(), -(widgetCentralArea->height())));
		anim->start();

	QTimer::singleShot(ANIMATION_DURATION, this, SLOT(showPicture2()));
}

void Accueil::showPicture2()
{
	widgetCentralArea->setCurrentIndex(0);

	QPropertyAnimation *anim = new QPropertyAnimation(widgetCentralArea, "pos");
		anim->setDuration(ANIMATION_DURATION);
		anim->setKeyValueAt(0, QPoint(-(widgetCentralArea->width()), oldCentralWidgetPos.y()));
		anim->setKeyValueAt(1, oldCentralWidgetPos);
		anim->start();

	QTimer::singleShot(ANIMATION_DURATION, this, SLOT(showPicture3()));
}

void Accueil::showPicture3()
{
	transitionInProgress = false;
}

void Accueil::slotActiveHover(QString buttonName)
{
	if (transitionInProgress)
		return;

	buttonsLabel->setLabelText(Multiuso::getFullNameOf(buttonName));
}

void Accueil::slotInactiveHover()
{
	if (transitionInProgress)
		return;

	buttonsLabel->noLabelText();
}
