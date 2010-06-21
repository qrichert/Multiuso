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

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	
	QCoreApplication::setApplicationName("Multiuso");
	QCoreApplication::setApplicationVersion("2.2.0");
	QCoreApplication::setOrganizationName("Quentin RICHERT");
	QCoreApplication::setOrganizationDomain("http://multiuso.sourceforge.net/");

	bool showTips = false;
	bool showMainWindow = true;

	QStringList arguments = QCoreApplication::arguments();
	
	if (arguments.count() > 1)
	{
		for (int i = 0; i < arguments.count(); i++)
		{
			if (arguments.value(i) == "-v" || arguments.value(i) == "--version")
			{
				qDebug() << "Multiuso version " << QCoreApplication::applicationVersion() << " for " << Multiuso::currentOSCompleteStr();
				qDebug() << "Using Qt version " << qVersion();

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

	QApplication::setQuitOnLastWindowClosed(false);

	QString locale = QLocale::system().name();

	QTranslator translator;
		translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));

		app.installTranslator(&translator);

	QSettings config(Multiuso::appDirPath() + "/reglages/config.ini", QSettings::IniFormat);

	if (config.value("ouverture/crash").toBool())
	{	
	}

	config.setValue("ouverture/crash", true);

	config.setValue("reseau/internet", true);

	if (config.value("last_path").toString().isEmpty())
		config.setValue("last_path", QDir::homePath());
		
	if (config.value("telechargements/dossier").toString().isEmpty())
		config.setValue("telechargements/dossier", QDir::homePath());
	
	QSettings fileBrowserSettings(Multiuso::appDirPath() + "/reglages/nav_fichiers.ini", QSettings::IniFormat);
	
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
