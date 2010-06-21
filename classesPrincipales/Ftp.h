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

#ifndef HEADER_FTP
#define HEADER_FTP

#include "../CurrentIncludes.h"
#include <QFtp>

class FtpAction : public QAction
{
	Q_OBJECT

	public:
		FtpAction(QObject *parent) : QAction(parent)
		{
			initialize();
		}

		FtpAction(const QString &text, QObject *parent) : QAction(text, parent)
		{
			initialize();
		}

		FtpAction(const QIcon &icon, const QString &text, QObject *parent) : QAction(icon, text, parent)
		{
			initialize();
		}

		void initialize()
		{
			m_host = "";
			m_login = "";
			m_password = "";
			m_port = "";
		}

		void setHost(QString host)
		{
			m_host = host;
		}

		void setLogin(QString login)
		{
			m_login = login;
		}

		void setPassword(QString password)
		{
			m_password = password;
		}

		void setPort(QString port)
		{
			m_port = port;
		}
		
		QString getHost()
		{
			return m_host;
		}

		QString getLogin()
		{
			return m_login;
		}

		QString getPassword()
		{
			return m_password;
		}

		QString getPort()
		{
			return m_port;
		}

	private:
		QString m_host;
		QString m_login;
		QString m_password;
		QString m_port;
};

class Ftp : public QMainWindow
{
	Q_OBJECT

	public:
		Ftp(QWidget *parent);

		void createActions();
		void createWidgets();
		void createMenus();
		void createToolBars();
		void createViews();

		void loadHistory();

		void addError(QString msg);
		void addSuccessful(QString msg);
		void addInformation(QString msg);

		void sauvegarderEtat();

	public slots:
		void openHistory();
		void removeHistory();
	
		void listComputerFiles();
		void listComputerFiles(QTableWidgetItem *item);
		void listComputerFiles(QDir dir);

		void connectOrDisconnect();

		void cdFtp(QString dirName);
		void listFtp();
		void listFtp(QTableWidgetItem *item);
		void listFtp(QString dirName);
		void ftpCommandFinished(int, bool error);
		void addToList(QUrlInfo infos);
		void updateTransferLabel(bool minus = false);
		void continueDownloads();
		void ftpUpdateTransferProgress(qint64 dowloaded, qint64 total);

		void openComputerMenu(QPoint);
		void openFtpMenu(QPoint);

		void removeOnComputer();
		void renameOnComputer();
		void addComputerFolder();
		
		void removeOnServer();
		void renameOnServer();
		void addServerFolder();

	private: // GUI
		QString currentComputerPath;
		QString currentServerPath;
		QString fakeServerPath;
		
		QAction *actionConnect;
		QAction *actionDisconnect;
		QList<FtpAction *> history;
		QAction *actionRemoveHistory;

		QLineEdit *lineHost;
		QLineEdit *lineLogin;
		QLineEdit *linePassword;
		QLineEdit *linePort;
		QPushButton *buttonActions;
		QTextEdit *textOutput;

		QMenu *menuActions;

		QToolBar *toolBarConnection;

		QLabel *labelStatus;
		QLabel *labelLights;
		QMovie *labelLightsMovie;
		QStatusBar *displayStatus;

		QLineEdit *lineComputerPath;
		QTableWidget *tableComputer;
		QLineEdit *lineServerPath;
		QTableWidget *tableServer;
		QSplitter *splitterViews;
		QTableWidget *tableDownloads;

	private: // Non-GUI
		QFtp *ftp;

		QFile *fileToPut;
		QFile *fileToGet;

		enum TransferType
		{
			PUT_FILE, GET_FILE
		};

		struct Transfer
		{
			QString firstFile;
			QString secondFile;
			TransferType type;
		};

		QQueue<Transfer> transferQueue;
		bool downloadIsInProgress;
};

#endif
