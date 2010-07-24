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

#ifndef HEADER_MULTIUSO
#define HEADER_MULTIUSO

#include <QtGui>
#include "time.h"

class Multiuso
{
	public:
		static QString appDirPath(QString program = "")
		{
			if (program.isEmpty())
				program = "Multiuso";

			#ifdef Q_WS_MAC
				return QCoreApplication::applicationDirPath().remove("/" + program + ".app/Contents/MacOS");
			#else
				return QCoreApplication::applicationDirPath();
			#endif
		}
		
		static QString addSlash(QString str)
		{
			if (!str.endsWith("/"))
				str += "/";
				
			return str;
	
		}

		static QString cdUp(QString str)
		{
			QDir dir(str);
				dir.cdUp();

			return dir.path();
		}
		
		static QString cleanStr(QString str)
		{
			str.replace(QRegExp("[ëËéÉèÈêÊäÄàÀâÂüÜûÛùÙ]"), "");
			str.replace("/", "");
			str.replace("\\", "");
			str.replace("=", "");

			return str;
		}

		static QWidget *closeButton(QDialog *dialog, QString label = "")
		{
			if (label.isEmpty())
				label = "Fermer";

			QPushButton *close = new QPushButton(label);
				QObject::connect(close, SIGNAL(clicked()), dialog, SLOT(accept()));

			QVBoxLayout *layout = new QVBoxLayout;
				layout->addWidget(close);
				layout->setAlignment(Qt::AlignRight);
				layout->setContentsMargins(0, 0, 0, 0);

			QWidget *widget = new QWidget;
				widget->setLayout(layout);

			return widget;
		}
		
		static bool copyDirectory(QString from, QString to)
		{
			return copyDirectory(QDir(from), QDir(to)); // See "copyDirectory(QDir from, QDir to)".
		}
		
		static bool copyDirectory(QDir from, QDir to) // Copies "from" in "to"
		{
			if (!from.exists())
				return false;
				
			if (!to.exists())
			{
				if (!to.mkpath(to.absolutePath()))
					return false;
			}

			QString fromPath = addSlash(from.absolutePath());
			QString toPath = addSlash(to.absolutePath());
			
			QString copyPath = addSlash(toPath + from.dirName());

			if (!QDir(copyPath).exists())
			{
				if (!to.mkpath(copyPath))
					return false;
			}
			
			foreach (QString fileName, from.entryList())
			{
				if (fileName == "." || fileName == "..")
					continue;

				if (QFileInfo(fromPath + fileName).isDir())
				{
					if (!copyDirectory(QDir(fromPath + fileName), QDir(copyPath)))
						return false;
				}
				
				else
				{
					if (!QFile::copy(fromPath + fileName, copyPath + fileName))
						return false;
				}
			}
			
			return true;
		}
		
		static QString currentOS()
		{
			QString os = "";

			#ifdef Q_WS_X11
				os = "linux";
			#endif
			
			#ifdef Q_WS_MAC
				os = "mac";
			#endif
			
			#ifdef Q_WS_WIN
				os = "windows";
			#endif
			
			return os;
		}
		
		static QString currentOSCompleteStr()
		{
			QString os = "";

			#ifdef Q_WS_X11
				os = "GNU/Linux";
			#endif
			
			#ifdef Q_WS_MAC
				os = "Mac OS";
			#endif
			
			#ifdef Q_WS_WIN
				os = "Microsoft® Windows®";
			#endif
			
			return os;
		}

		static QString currentSuffix()
		{
			QString suffix = "";

			#ifdef Q_WS_X11
				suffix = "";
			#endif

			#ifdef Q_WS_MAC
				suffix = ".app";
			#endif
	
			#ifdef Q_WS_WIN
				suffix = ".exe";
			#endif
			
			return suffix;
		}
		
		static QByteArray decrypt(QByteArray ba)
		{
			ba = ba.remove(0, 9);
			ba = QByteArray::fromBase64(ba);

			return QByteArray::fromHex(ba);
		}

		static QAction *emptyAction(QWidget *parent)
		{
			QAction *action = new QAction("(vide)", parent);
				action->setDisabled(true);

			return action;
		}

		static QByteArray encrypt(QByteArray ba)
		{
			ba = ba.toHex();

			return QByteArray("ENCRYPTED") + ba.toBase64();
		}

		static QString firstLetterToUpper(QString str)
		{
			QString newStr = str.toLower();
				newStr = newStr.left(1).toUpper() + newStr.right(newStr.length() - 1);

			return newStr;
		}
		
		static QFont fontForCode()
		{
			return QFont("Courrier");
		}
		
		static QString getFullNameOf(QString shortName)
		{
			if (shortName == "home")
				return QString("Accueil");
			
			else if (shortName == "filebrowser")
				return QString("Navigateur de fichiers");

			else if (shortName == "widgets")
				return QString("Widgets");

			else if (shortName == "pictureviewer")
				return QString("Visionneur d'images");

			else if (shortName == "textedit")
				return QString("Éditeur de texte");

			else if (shortName == "codeedit")
				return QString("Éditeur de code");

			else if (shortName == "ftp")
				return QString("Client FTP");

			else if (shortName == "webbrowser")
				return QString("Navigateur Web");

			else
				return QString("");
		}
		
		static QString htmlspecialchars(QString str)
		{
			str.replace("&", "&&");

			return str;
		}
		
		static QIcon iconForFile(QString file, QString type)
		{
			QString extension = QFileInfo(file).suffix().toLower();

			QIcon icon;

			if (type.toLower() == "dossier")
			{
				icon.addFile(":/icones/mime/Dossier.png");

				return icon;
			}

			QStringList pictures;
				pictures << "png" << "jpg" << "jpeg" << "bmp" << "gif" << "pbm" << "pgm" << "ppm" << "xbm" << "xpm" << "tiff";

			QStringList movies;
				movies << "avi" << "mp4" << "mpg" << "mpeg" << "3gp" << "mov" << "mkv" << "flv" << "ogv";

			QStringList sounds;
				sounds << "mp3" << "flac" << "aac" << "wav" << "wma";

			QStringList web;
				web << "html" << "htm" << "js" << "aspx" << "xml" << "sql";

			QStringList texts;
				texts << "mltstxt" << "mltshtml" << "txt" << "odt" << "odf" << "rtf" << "docx";

			QStringList compressed;
				compressed << "ar" << "bz2" << "cbz" << "gz" << "lzma" << "tar" << "bz2" << "zip" << "7z" << "gzip" << "bzip2" << "rar";

			QStringList shell;
				shell << "sh" << "bat" << "cmd";

			if (pictures.contains(extension)) { icon.addFile(":/icones/mime/img.png"); }
			else if (movies.contains(extension)) { icon.addFile(":/icones/mime/video.png"); }
			else if (sounds.contains(extension)) { icon.addFile(":/icones/mime/sound.png"); }
			else if (web.contains(extension)) { icon.addFile(":/icones/mime/web.png"); }
			else if (texts.contains(extension)) { icon.addFile(":/icones/mime/texte.png"); }
			else if (shell.contains(extension)) { icon.addFile(":/icones/mime/shell.png"); }
			else if (compressed.contains(extension)) { icon.addFile(":/icones/mime/tar.png"); }
			else if (extension == "axx") { icon.addFile(":/icones/mime/axx.png"); }
			else if (extension == "c") { icon.addFile(":/icones/mime/c.png"); }
			else if (extension == "cpp") { icon.addFile(":/icones/mime/cpp.png"); }
			else if (extension == "deb") { icon.addFile(":/icones/mime/deb.png"); }
			else if (extension == "f") { icon.addFile(":/icones/mime/f.png"); }
			else if (extension == "h") { icon.addFile(":/icones/mime/h.png"); }
			else if (extension == "hpp") { icon.addFile(":/icones/mime/hpp.png"); }
			else if (extension == "iso") { icon.addFile(":/icones/mime/iso.png"); }
			else if (extension == "j") { icon.addFile(":/icones/mime/j.png"); }
			else if (extension == "jar") { icon.addFile(":/icones/mime/jar.png"); }
			else if (extension == "java") { icon.addFile(":/icones/mime/java.png"); }
			else if (extension == "l") { icon.addFile(":/icones/mime/l.png"); }
			else if (extension == "log") { icon.addFile(":/icones/mime/log.png"); }
			else if (extension == "midi") { icon.addFile(":/icones/mime/midi.png"); }
			else if (extension == "nfo") { icon.addFile(":/icones/mime/nfo.png"); }
			else if (extension == "o") { icon.addFile(":/icones/mime/o.png"); }
			else if (extension == "p") { icon.addFile(":/icones/mime/p.png"); }
			else if (extension == "php") { icon.addFile(":/icones/mime/php.png"); }
			else if (extension == "pl") { icon.addFile(":/icones/mime/pl.png"); }
			else if (extension == "pdf") { icon.addFile(":/icones/mime/pdf.png"); }
			else if (extension == "ps") { icon.addFile(":/icones/mime/ps.png"); }
			else if (extension == "py") { icon.addFile(":/icones/mime/py.png"); }
			else if (extension == "rc") { icon.addFile(":/icones/mime/rc.png"); }
			else if (extension == "s") { icon.addFile(":/icones/mime/s.png"); }
			else if (extension == "tar") { icon.addFile(":/icones/mime/tar.png"); }
			else if (extension == "tex") { icon.addFile(":/icones/mime/tex.png"); }
			else if (extension == "tmp") { icon.addFile(":/icones/mime/tmp.png"); }
			else if (extension == "ttf") { icon.addFile(":/icones/mime/ttfpng"); }
			else if (extension == "y") { icon.addFile(":/icones/mime/y.png"); }
			else { icon.addFile(":/icones/mime/fichier.png"); }

			return icon;
		}

		static QString lastPath()
		{
			QSettings settings(appDirPath() + "/ini/config.ini", QSettings::IniFormat);
		
			QString pathToReturn = takeSlash(settings.value("last_path").toString());

			if (pathToReturn.isEmpty() || !QDir(pathToReturn).exists())
				return QDir::homePath();

			return pathToReturn;
		}

		static QString openCommand()
		{
			#ifdef Q_WS_MAC
				return QString("open ");
			#else
				return QString("");
			#endif
		}
		
		static QStringList openTabsList(QTabWidget *tabWidget)
		{
			QStringList tabs;

			for (int i = 0; i < tabWidget->count(); i++)
				tabs << tabWidget->tabText(i);

			return tabs;
		}

		static QString pluginsDirPath()
		{
			return appDirPath() + "/extensions/plugins/";
		}

		static void quit()
		{
			QSettings config(Multiuso::appDirPath() + "/ini/config.ini", QSettings::IniFormat);

			if (config.value("mot_de_passe").toBool())
			{
				QStringList iniFiles = QDir(Multiuso::appDirPath() + "/ini").entryList();
					iniFiles.removeOne(".");
					iniFiles.removeOne("..");
					iniFiles.removeOne("PWD.ini");

				foreach(QString iniFile, iniFiles)
				{
					QFile file(Multiuso::appDirPath() + "/ini/" + iniFile);
						file.open(QIODevice::ReadOnly | QIODevice::Text);
							QString content = Multiuso::encrypt(file.readAll());
						file.close();
						file.open(QIODevice::WriteOnly | QIODevice::Text);
							file.write(content.toAscii());
						file.close();
				}
			}
			
			removeDirectory(tempPath());

			qApp->quit();
		}

		static int randomNumber(int min, int max)
		{
			srand(time(NULL));

			return (rand() % (max - min + 1)) + min;
		}
		
		static bool removeDirectory(QString dir)
		{
			return removeDirectory(QDir(dir)); // See "removeDirectory(QDir)".
		}
		
		static bool removeDirectory(QDir dir)
		{
			QStringList filesList = dir.entryList();
				filesList.removeOne(".");
				filesList.removeOne("..");

			for (int i = 0; i < filesList.size(); i++)
			{
				if (QFileInfo(dir.absolutePath() + "/" + filesList.value(i)).isFile())
					QFile::remove(dir.absolutePath() + "/" + filesList.value(i));

				else if (QFileInfo(dir.absolutePath() + "/" + filesList.value(i)).isDir())
					removeDirectory(QDir(dir.absolutePath() + "/" + filesList.value(i)));
			}

			return dir.rmdir(dir.absolutePath()); // If "dir" is empty (the operation was successfull) it will return true, otherwise it will return false.
		}
		
		static int screenHeight()
		{
			QDesktopWidget desktop;
			
			return desktop.height();
		}

		static int screenWidth()
		{
			QDesktopWidget desktop;
			
			return desktop.width();
		}
		
		static void setLastPath(QString lastPath)
		{
			QString dirToSave = "";

			QFileInfo pathInfo(lastPath);

			if (pathInfo.isFile())
				dirToSave = pathInfo.absolutePath();
			
			else if (pathInfo.isDir())
				dirToSave = lastPath;

			if (dirToSave.isEmpty() || !QDir(dirToSave).exists())
				return;

			QSettings settings(appDirPath() + "/ini/config.ini", QSettings::IniFormat);
				settings.setValue("last_path", dirToSave);
		}

		static int tabIndexOf(QString text, QTabWidget *tabWidget)
		{
			for (int i = 0; i < tabWidget->count(); i++)
			{
				if (tabWidget->tabText(i) == text)
					return i;
			}

			return -1;
		}

		static QStringList tabsList()
		{
			QStringList tabs;
				tabs << "Accueil" << "Navigateur de fichiers" << "Widgets" << "Visionneur d'images" << "Éditeur de texte" <<
					"Éditeur de code" << "Client FTP" << "Navigateur Web";

			return tabs;
		}
				
		static QString takeSlash(QString str)
		{
			while (str.endsWith("/"))
				str = str.left(str.size() - 1);
				
			return str;
		}

		static QString tempPath()
		{
			QString tmp = appDirPath() + "/tmp";

			QDir().mkpath(tmp); // In case of "tmp" doesn't exist.

			return tmp;
		}

		static QString toSize(double size)
		{
			QString type = "Gio";
			size = ((size / 1024) / 1024) / 1024;

			if (size < 1)
			{
				type = "Mio";
				size *= 1024;
				
				if (size < 1)
				{
					type = "Kio";
					size *= 1024;

					if (size > 0 && size < 1)
					{
						type = "octets";
						size *= 1024;

						if (size > 0 && size < 1)
							type = "octet";
					}
				}
			}

			QString stringSize = QString::number(size);

			if (stringSize.contains("."))
				stringSize = stringSize.left(stringSize.indexOf(".") + 2);

			return QString(stringSize + " " + type);
		}
};

#endif
