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

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QIcon>
#include <QSplashScreen>
#include <QSettings>
#include "CurrentIncludes.h"
#include "classesPrincipales/FenPrincipale.h"
#include "autresClasses/Astuces.h"

void checkFiles();

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	
	QCoreApplication::setApplicationName("Multiuso");
	QCoreApplication::setApplicationVersion("2.2.0");
	//QCoreApplication::setApplicationVersion("2.2.1");
	QCoreApplication::setOrganizationName("Quentin RICHERT");
	QCoreApplication::setOrganizationDomain("http://multiuso.sourceforge.net/");

	QApplication::setQuitOnLastWindowClosed(false);

	QString locale = QLocale::system().name();

	QTranslator translator;
		translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
			app.installTranslator(&translator);	

	// Reading config file
		bool usePassword = false;

		QFile configFile(Multiuso::appDirPath() + "/ini/config.ini");
			configFile.open(QIODevice::ReadOnly | QIODevice::Text);

				if (configFile.readAll().startsWith("ENCRYPTED"))
					usePassword = true;

			configFile.close();

	if (usePassword)
	{
		bool retry = true;
		int steps = 0;

		while (retry)
		{
			steps++;

			bool ok;

			QString mdp = QInputDialog::getText(new QWidget, "Multiuso", "Veuillez saisir le mot de passe :", QLineEdit::Password, "", &ok);

			if (!ok)
				return EXIT_SUCCESS;

			QSettings reglagesPassword(Multiuso::appDirPath() + "/ini/PWD.ini", QSettings::IniFormat);
				QByteArray verif = reglagesPassword.value("pwd").toByteArray();


			mdp = "ér97&_Èhz" + mdp + "~odE987sDe!";

			QByteArray ba = mdp.toAscii();
				ba = QCryptographicHash::hash(ba, QCryptographicHash::Sha1).toHex();

			if (ba != verif)
			{
				if (steps == 3)
					return EXIT_SUCCESS;

				int answer = QMessageBox::critical(NULL, "Multiuso", "<strong>Mot de passe incorrect !</strong><br />"
						"<em>Voulez-vous réessayer ?</em>", QMessageBox::Yes | QMessageBox::No);

				if (answer == QMessageBox::No)
					return EXIT_SUCCESS;
			}

			else
			{
				retry = false;
			}
		}

		QStringList iniFiles = QDir(Multiuso::appDirPath() + "/ini").entryList();
			iniFiles.removeOne(".");
			iniFiles.removeOne("..");
			iniFiles.removeOne("PWD.ini");
			
		foreach(QString iniFile, iniFiles)
		{
			QFile file(Multiuso::appDirPath() + "/ini/" + iniFile);
				file.open(QIODevice::ReadOnly | QIODevice::Text);
					QString content = Multiuso::decrypt(file.readAll());
				file.close();
				file.open(QIODevice::WriteOnly | QIODevice::Text);
					file.write(content.toAscii());
				file.close();
		}

		QStringList webBrowserFiles1 = QDir(Multiuso::appDirPath() + "/navigateurWeb/autre").entryList();
				webBrowserFiles1.removeOne(".");
				webBrowserFiles1.removeOne("..");
				webBrowserFiles1.removeOne("cookies.mltscookie");
				
		foreach(QString webBrowserFile, webBrowserFiles1)
		{
			QFile file(Multiuso::appDirPath() + "/navigateurWeb/autre/" + webBrowserFile);
				file.open(QIODevice::ReadOnly | QIODevice::Text);
					QString content = Multiuso::decrypt(file.readAll());
				file.close();
				file.open(QIODevice::WriteOnly | QIODevice::Text);
					file.write(content.toAscii());
				file.close();
		}

		QStringList webBrowserFiles2 = QDir(Multiuso::appDirPath() + "/navigateurWeb/favoris").entryList();
				webBrowserFiles2.removeOne(".");
				webBrowserFiles2.removeOne("..");

		foreach(QString webBrowserFile, webBrowserFiles2)
		{
			QFile file(Multiuso::appDirPath() + "/navigateurWeb/favoris/" + webBrowserFile);
				file.open(QIODevice::ReadOnly | QIODevice::Text);
					QString content = Multiuso::decrypt(file.readAll());
				file.close();
				file.open(QIODevice::WriteOnly | QIODevice::Text);
					file.write(content.toAscii());
				file.close();
		}
	}

	checkFiles();

	bool showTips = false;
	bool showMainWindow = true;

	QStringList arguments = QCoreApplication::arguments();
	
	if (arguments.count() > 1)
	{
		for (int i = 0; i < arguments.count(); i++)
		{
			if (arguments.value(i) == "-v" || arguments.value(i) == "--version")
			{
				std::cout << "Multiuso version " << QCoreApplication::applicationVersion().toStdString()
					<< " for " << Multiuso::currentOSCompleteStr().toStdString() << "\n"
					<< "Using Qt version " << qVersion() << "\n";

				return EXIT_SUCCESS;
			}
			
			else if (arguments.value(i) == "-sti" || arguments.value(i) == "--systemtrayicon")
			{
				showMainWindow = false;
			}

			else if (arguments.value(i) == "-st" || arguments.value(i) == "--showtips")
			{
				showTips = true;
			}
		}
	}

	QSettings config(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);

	if (config.value("ouverture/crash").toBool()){}

	config.setValue("ouverture/crash", true);

	config.setValue("reseau/internet", true);

	if (config.value("last_path").toString().isEmpty())
		config.setValue("last_path", QDir::homePath());
		
	if (config.value("telechargements/dossier").toString().isEmpty())
		config.setValue("telechargements/dossier", QDir::homePath());
	
	QSettings fileBrowserSettings(Multiuso::appDirPath() + "/ini/nav_fichiers.ini", QSettings::IniFormat);
	
	if (fileBrowserSettings.value("dossier_accueil").toString().isEmpty())
		fileBrowserSettings.setValue("dossier_accueil", QDir::homePath());

	FenPrincipale fenetre;
		QApplication::instance()->installEventFilter(&fenetre);

		if (showMainWindow)
			fenetre.show();

	if (config.value("ouverture/astuces").toBool() || showTips)
	{
		Astuces astuces(&fenetre);
			astuces.exec();
	}
	
	return app.exec();
}

void checkFiles()
{
	QDir().mkpath(Multiuso::appDirPath() + "/extensions/images"); // empty
	QDir().mkpath(Multiuso::appDirPath() + "/extensions/plugins"); // empty
	QDir().mkpath(Multiuso::appDirPath() + "/extensions/styles"); // empty

	// INI
	QDir().mkpath(Multiuso::appDirPath() + "/ini"); // empty

	// Web browser
	QDir().mkpath(Multiuso::appDirPath() + "/navigateurWeb/autre"); // empty
	QDir().mkpath(Multiuso::appDirPath() + "/navigateurWeb/cache"); // empty
	QDir().mkpath(Multiuso::appDirPath() + "/navigateurWeb/favoris"); // empty
	QDir().mkpath(Multiuso::appDirPath() + "/navigateurWeb/html");
	QDir().mkpath(Multiuso::appDirPath() + "/navigateurWeb/speedDial");
		QDir().mkpath(Multiuso::appDirPath() + "/navigateurWeb/speedDial/pictures");

	QStringList allFiles;
	// INI
		allFiles << ":/default_files/ini/apparence.ini"
			<< ":/default_files/ini/chronometre.ini"
			<< ":/default_files/ini/config.ini"
			<< ":/default_files/ini/creer_readme.ini"
			<< ":/default_files/ini/editeur_de_code.ini"
			<< ":/default_files/ini/editeur_de_texte.ini"
			<< ":/default_files/ini/ftp.ini"
			<< ":/default_files/ini/generer.ini"
			<< ":/default_files/ini/nav_fichiers.ini"
			<< ":/default_files/ini/navigateur.ini"
			<< ":/default_files/ini/preferences.ini"
			<< ":/default_files/ini/proxy.ini"
			<< ":/default_files/ini/visionneur_images.ini"
			<< ":/default_files/ini/widgets.ini";
	// Web browser
		allFiles << ":/default_files/navigateurWeb/html/page_vierge.html"
			<< ":/default_files/navigateurWeb/html/page_vierge.png"
			<< ":/default_files/navigateurWeb/html/pas_de_connexion.html"
			<< ":/default_files/navigateurWeb/html/pas_de_connexion.png"

			<< ":/default_files/navigateurWeb/speedDial/speedDial.html"

			<< ":/default_files/navigateurWeb/speedDial/pictures/actionSpeedDial.png"
			<< ":/default_files/navigateurWeb/speedDial/pictures/background_gradient.png"
			<< ":/default_files/navigateurWeb/speedDial/pictures/background_link_head.png"
			<< ":/default_files/navigateurWeb/speedDial/pictures/delete_active.png"
			<< ":/default_files/navigateurWeb/speedDial/pictures/delete_alpha.png"
			<< ":/default_files/navigateurWeb/speedDial/pictures/delete_hover.png"
			<< ":/default_files/navigateurWeb/speedDial/pictures/delete.png"
			<< ":/default_files/navigateurWeb/speedDial/pictures/empty.png"
			<< ":/default_files/navigateurWeb/speedDial/pictures/error.png"
			<< ":/default_files/navigateurWeb/speedDial/pictures/logo.png"
			<< ":/default_files/navigateurWeb/speedDial/pictures/wait.gif";

	foreach (QString fileName, allFiles)
	{
		QFile file(fileName);
		QFileInfo fileInfo(fileName);
		QFile newFile(Multiuso::appDirPath() + fileInfo.filePath().remove(":/default_files"));

		if (!newFile.exists())
		{
			if (fileInfo.suffix().toLower() == "png")
			{
				QPixmap pixmap(fileInfo.filePath());
					pixmap.save(QFileInfo(newFile).filePath());
			}

			else if (fileInfo.suffix().toLower() == "gif")
			{
				file.copy(QFileInfo(newFile).filePath());
			}

			else
			{
				file.open(QIODevice::ReadOnly | QIODevice::Text);
					newFile.open(QIODevice::WriteOnly | QIODevice::Text);
						newFile.write(file.readAll());
					newFile.close();
				file.close();

				if (fileInfo.fileName() == "navigateur.ini")
				{
					QSettings homePage(Multiuso::appDirPath() + "/ini/navigateur.ini", QSettings::IniFormat);

					if (Multiuso::currentOS() == "mac")
						homePage.setValue("page_accueil", "http://www.lovelymac.fr/");

					else
						homePage.setValue("page_accueil", QCoreApplication::organizationDomain());
				}
			}
		}
	}
}
