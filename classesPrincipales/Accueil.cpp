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

// Picture
	HomePicture *labelAccueil = new HomePicture;
		connect(labelAccueil, SIGNAL(clicked()), this, SLOT(showButtons()));

	QVBoxLayout *layoutImage = new QVBoxLayout;
		layoutImage->addWidget(labelAccueil);
		layoutImage->setContentsMargins(0, 0, 0, 0);

	QWidget *widgetImage = new QWidget;
		widgetImage->setLayout(layoutImage);

// Buttons	
	fileBrowser = new OpenButton;
		fileBrowser->setIcon(QIcon(":/icones/accueil/filebrowser.png"));
		fileBrowser->setObjectName("filebrowser");
		connect(fileBrowser, SIGNAL(clicked()), this, SLOT(buttonClicked()));

	widgets = new OpenButton;
		widgets->setIcon(QIcon(":/icones/accueil/widgets.png"));
		widgets->setObjectName("widgets");
		connect(widgets, SIGNAL(clicked()), this, SLOT(buttonClicked()));

	pictureViewer = new OpenButton;
		pictureViewer->setIcon(QIcon(":/icones/accueil/pictureviewer.png"));
		pictureViewer->setObjectName("pictureviewer");
		connect(pictureViewer, SIGNAL(clicked()), this, SLOT(buttonClicked()));

	textEdit = new OpenButton;
		textEdit->setIcon(QIcon(":/icones/accueil/textedit.png"));
		textEdit->setObjectName("textedit");
		connect(textEdit, SIGNAL(clicked()), this, SLOT(buttonClicked()));

	codeEdit = new OpenButton;
		codeEdit->setIcon(QIcon(":/icones/accueil/codeedit.png"));
		codeEdit->setObjectName("codeedit");
		connect(codeEdit, SIGNAL(clicked()), this, SLOT(buttonClicked()));

	ftp = new OpenButton;
		ftp->setIcon(QIcon(":/icones/accueil/ftp.png"));
		ftp->setObjectName("ftp");
		connect(ftp, SIGNAL(clicked()), this, SLOT(buttonClicked()));

	webBrowser = new OpenButton;
		webBrowser->setIcon(QIcon(":/icones/accueil/webbrowser.png"));
		webBrowser->setObjectName("webbrowser");
		connect(webBrowser, SIGNAL(clicked()), this, SLOT(buttonClicked()));

	QGridLayout *layoutOpen = new QGridLayout;
		layoutOpen->addWidget(fileBrowser, 0, 0, 1, 1);
		layoutOpen->addWidget(widgets, 0, 1, 1, 1);
		layoutOpen->addWidget(pictureViewer, 0, 2, 1, 1);
		layoutOpen->addWidget(textEdit, 0, 3, 1, 1);
		layoutOpen->addWidget(codeEdit, 1, 0, 1, 1);
		layoutOpen->addWidget(ftp, 1, 1, 1, 1);
		layoutOpen->addWidget(webBrowser, 1, 2, 1, 1);
		layoutOpen->setContentsMargins(0, 0, 0, 0);

	QWidget *widgetOpen = new QWidget;
		widgetOpen->setLayout(layoutOpen);

// Central area
	widgetCentralArea = new QStackedWidget;
		widgetCentralArea->addWidget(widgetImage);
		widgetCentralArea->addWidget(widgetOpen);

	QGridLayout *layoutCentralArea = new QGridLayout;
		layoutCentralArea->addWidget(widgetCentralArea, 0, 0, 1, 2, Qt::AlignCenter);

	aireCentrale = new QMdiArea;
		aireCentrale->setLayout(layoutCentralArea);

	QSettings optionsAccueil(Multiuso::appDirPath() + "/reglages/config.ini", QSettings::IniFormat);

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

	if (optionsAccueil.value("ouverture/page").toInt() == 0)
		showPicture();

	else
		showButtons();
}

void Accueil::restoreOpenedTabs()
{
	QSettings optionsAccueil(Multiuso::appDirPath() + "/reglages/config.ini", QSettings::IniFormat);

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

void Accueil::actualiserFond()
{
	QSettings optionsAccueil(Multiuso::appDirPath() + "/reglages/config.ini", QSettings::IniFormat);

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
	widgetCentralArea->setCurrentIndex(1);
	showPictureButton->show();
}

void Accueil::showPicture()
{
	widgetCentralArea->setCurrentIndex(0);
	showPictureButton->hide();
}
