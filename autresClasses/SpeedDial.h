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

#ifndef HEADER_SPEEDDIAL
#define HEADER_SPEEDDIAL

#include "../CurrentIncludes.h"
#include "classesPrincipales/NavigateurWeb.h"
#include "autresClasses/WebPageScreenshot.h"

class GetPageUrl : public QDialog
{
	Q_OBJECT

	public:
		GetPageUrl(QWidget *parent = 0, Qt::WindowFlags flags = 0) : QDialog(parent, flags)
		{
			setWindowTitle("Multiuso");
			setWindowIcon(QIcon(":/icones/navigateur_web/actionSpeedDial.png"));
			
			url = new QLineEdit;
				url->setText("http://");
				connect(url, SIGNAL(textChanged(QString)), this, SLOT(urlChanged(QString)));

			okButton = new QPushButton("Annuler");
				connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

			QGridLayout *mainLayout = new QGridLayout(this);
				mainLayout->addWidget(new QLabel("<img src=':/icones/navigateur_web/actionSpeedDial.png'/>"), 0, 0, 1, 1);
				mainLayout->addWidget(new QLabel("Veuillez saisir l'URL de la page à ajouter :"), 0, 1, 1, 2);
				mainLayout->addWidget(url, 1, 0, 1, 3);
				mainLayout->addWidget(new QLabel("<hr />"), 2, 0, 1, 3);
				mainLayout->addWidget(okButton, 3, 2, 1, 1);
		}

		QString getUrl()
		{
			QStringList links;

			QFile completer(Multiuso::appDirPath() + "/navigateurWeb/autre/completer.mltscompleter");

			if (completer.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				QTextStream in(&completer);
	
				while (!in.atEnd())
					links << in.readLine();
			}	
	
			completer.close();
	
			QCompleter *completerUrl = new QCompleter(links, this);
				completerUrl->setCaseSensitivity(Qt::CaseInsensitive);
						
			url->setCompleter(completerUrl);

			if (exec() == QDialog::Rejected)
			{
				url->setText("http://");

				return QString("");
			}

			QString urlEntered = url->text();
				
				if (urlEntered.contains(QRegExp("^(h|ht|htt|http|http:|http:/|http://)$")))
					urlEntered = "";

			url->setText("http://");

			return urlEntered;
		}

	public slots:
		void urlChanged(QString url)
		{
			if (url.isEmpty() || url.contains(QRegExp("^(h|ht|htt|http|http:|http:/|http://)$")))
					okButton->setText("Annuler");

			else
				okButton->setText("Valider");
		}

	private:
		QLineEdit *url;
		QPushButton *okButton;
};

class SpeedDial : public QWebView
{
	Q_OBJECT

	public:
		SpeedDial();

		void loadSpeedDial();

	public slots:
		void addNewLink(QString title, QString url, QPixmap pixmap);
		void downloadError();
		void openLink(QUrl urlClicked);

	signals:
		void openUrlRequested(QString url);

	private:
		QStringList titleContent;
		QStringList linksContent;
		QStringList imgContent;
		WebPageScreenshot *screen;
		bool addingNew;
		int currentDial;
};

#endif
