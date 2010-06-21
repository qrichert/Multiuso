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

#include "SpeedDial.h"

SpeedDial::SpeedDial()
{
	loadSpeedDial();

	screen = new WebPageScreenshot;
		connect(screen, SIGNAL(screenshotTaken(QString, QString, QPixmap)), this, SLOT(addNewLink(QString, QString, QPixmap)));
		connect(screen, SIGNAL(screenshotNotTaken()), this, SLOT(downloadError()));

	connect(this, SIGNAL(linkClicked(QUrl)), this, SLOT(openLink(QUrl)));

	addingNew = false;
	currentDial = 0;
}

void SpeedDial::loadSpeedDial()
{
	titleContent.clear();
	linksContent.clear();
	imgContent.clear();
	
	QString slashToAdd = "";

		if (Multiuso::currentOS() == "windows")
			slashToAdd = "/"; // Windows needs a third slash : file:///C:\Users\foobar\icon.png

	QFile speedDialContent(Multiuso::appDirPath() + "/navigateurWeb/speedDial/speedDial.html");

	if (speedDialContent.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		
		QString content = speedDialContent.readAll();
				
			content.replace("${PATH}", "file://" + slashToAdd + Multiuso::appDirPath());	
			content.replace("${WEBSITE}", QCoreApplication::organizationDomain());	
			content.replace("${DELETE}", "file://" + slashToAdd + Multiuso::appDirPath() + "/navigateurWeb/speedDial/pictures/delete.png");	
			content.replace("${DELETEHOVER}", "file://" + slashToAdd + Multiuso::appDirPath() + "/navigateurWeb/speedDial/pictures/delete_hover.png");	
			content.replace("${DELETEACTIVE}", "file://" + slashToAdd + Multiuso::appDirPath() + "/navigateurWeb/speedDial/pictures/delete_active.png");	
			content.replace("${DELETEALPHA}", "file://" + slashToAdd + Multiuso::appDirPath() + "/navigateurWeb/speedDial/pictures/delete_alpha.png");	
	
			QSettings links(Multiuso::appDirPath() + "/reglages/navigateur.ini", QSettings::IniFormat);

			for (int i = 1; i <= 8; i++)
			{
				QStringList infos = links.value("speedDial/dial" + QString::number(i)).value<QStringList >();

				content.replace("${TABTITLE_" + QString::number(i) + "}", infos.value(0));	
				content.replace("${TABLINK_" + QString::number(i) + "}", infos.value(1));

				if (infos.value(2) == "${EMPTYIMG}")
					content.replace("${TABIMG_" + QString::number(i) + "}", "file://" + slashToAdd + Multiuso::appDirPath() + "/navigateurWeb/speedDial/pictures/empty.png");	
			
				else if (infos.value(2) == "${WAIT}")
					content.replace("${TABIMG_" + QString::number(i) + "}", "file://" + slashToAdd + Multiuso::appDirPath() + "/navigateurWeb/speedDial/pictures/wait.png");	
			
				else
					content.replace("${TABIMG_" + QString::number(i) + "}", "file://" + slashToAdd + Multiuso::appDirPath() + "/navigateurWeb/speedDial/" + infos.value(2));

				titleContent << infos.value(0);
				linksContent << infos.value(1);
				imgContent << infos.value(2);
			}

		setHtml(content);
	}

	else
	{
		setHtml("<span style='color:red'><strong>Erreur à l'ouverture du speed dial</strong></span>");
	}
}

void SpeedDial::addNewLink(QString title, QString url, QPixmap pixmap)
{
	pixmap = pixmap.scaledToWidth(180, Qt::SmoothTransformation);
	pixmap = pixmap.copy(0, 0, 180, 135);

	QString pixmapName = QString::number(currentDial) + "_" + QString::number(Multiuso::randomNumber(1000, 9999)) + ".png"; // In order to prevent similar names (because of the network cache).

	pixmap.save(Multiuso::appDirPath() + "/navigateurWeb/speedDial/" + pixmapName);

	if (title.size() > 18)
		title = title.left(15) + "...";
	
	QStringList newItems;
		newItems << title << url << pixmapName;

	QSettings links(Multiuso::appDirPath() + "/reglages/navigateur.ini", QSettings::IniFormat);
		links.setValue("speedDial/dial" + QString::number(currentDial), newItems);

	loadSpeedDial();
		
	addingNew = false;
	currentDial = 0;
}

void SpeedDial::downloadError()
{
	QStringList emptyItems;
		emptyItems << "(vide)" << "Ajouter un site Web" << "${EMPTYIMG}";

	QSettings links(Multiuso::appDirPath() + "/reglages/navigateur.ini", QSettings::IniFormat);
		links.setValue("speedDial/dial" + QString::number(currentDial), emptyItems);

	loadSpeedDial();

	addingNew = false;
	currentDial = 0;

	QString js = "document.getElementById('downloadError').style.display='block';"
		"document.getElementById('downloadError').style.left=((" + QString::number(width()) + " / 2) - 200) + 'px';"
		"document.getElementById('downloadError').style.top=((" + QString::number(height()) + " / 2) - 100) + 'px';";

	page()->mainFrame()->evaluateJavaScript(js);
}

void SpeedDial::openLink(QUrl urlClicked)
{
	if (addingNew)
		return;

	QString url = urlClicked.toString();

	if (url.startsWith("link"))
	{
		int link = url.right(1).toInt();

		if (linksContent.value(link - 1) == "Ajouter un site Web")
		{
			while (true)
			{
				GetPageUrl getPageUrl;

				QString linkToAdd = getPageUrl.getUrl();

				if (linkToAdd.isEmpty())
				{
					return;
				}
	
				else if (linkToAdd.contains(QRegExp("https?://(.+).(.+)")))
				{
					screen->takeScreenshotOf(linkToAdd);
					
					QStringList emptyItems;
						emptyItems << "(vide)" << "Ajouter un site Web" << "${WAIT}";

					QSettings links(Multiuso::appDirPath() + "/reglages/navigateur.ini", QSettings::IniFormat);
						links.setValue("speedDial/dial" + QString::number(link), emptyItems);

					loadSpeedDial();

					addingNew = true;
					currentDial = link;

					return;
				}

				else
				{
					int answer = QMessageBox::question(this, "Multiuso", "Le lien saisi n'est pas valide, voulez-vous réessayer ?",
							QMessageBox::Yes | QMessageBox::No);

					if (answer == QMessageBox::No)
						return;
				}
			}
		}
			
		else
		{
			emit openUrlRequested(linksContent.value(link - 1));
		}
	}

	else if (url.startsWith("delete"))
	{
		int link = url.right(1).toInt();

		QStringList emptyItems;
			emptyItems << "(vide)" << "Ajouter un site Web" << "${EMPTYIMG}";

		QSettings links(Multiuso::appDirPath() + "/reglages/navigateur.ini", QSettings::IniFormat);
			links.setValue("speedDial/dial" + QString::number(link), emptyItems);

			QFile::remove(Multiuso::appDirPath() + "/navigateurWeb/speedDial/" + imgContent.value(link - 1));

		loadSpeedDial();
	}
	
	else if (url == "close_download_error")
	{
		page()->mainFrame()->evaluateJavaScript("document.getElementById('downloadError').style.display='none';");
	}


	else
	{
		emit openUrlRequested(url);
	}
}
