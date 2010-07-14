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

#include "TelechargerFichier.h"

TelechargerFichier::TelechargerFichier(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("Multiuso - Téléchargements");
	setWindowIcon(QIcon(":/icones/actions/actionTelechargements.png"));
	resize(490, 315);

	QLabel *infos = new QLabel("Entrez l'URL du fichier à télécharger :");

	lineLien = new QLineEdit;

	QPushButton *telecharger = new QPushButton("&Télécharger");
		connect(telecharger, SIGNAL(clicked()), this, SLOT(ajouterTelechargement()));

	QStringList entetesTelechargements;
		entetesTelechargements << "×" << "Nom" << "Progression" << "Statut";

	listeTelechargements = new QTableWidget(0, entetesTelechargements.size());
		listeTelechargements->setShowGrid(false);
		listeTelechargements->verticalHeader()->hide();
		listeTelechargements->horizontalHeader()->setStretchLastSection(true);
		listeTelechargements->setHorizontalHeaderLabels(entetesTelechargements);
		listeTelechargements->setSelectionBehavior(QAbstractItemView::SelectRows);
		listeTelechargements->setSelectionMode(QAbstractItemView::SingleSelection);

	QVBoxLayout *layout = new QVBoxLayout(this);
		layout->addWidget(infos);
		layout->addWidget(lineLien);
		layout->addWidget(telecharger);
		layout->addWidget(new QLabel("<hr />"));
		layout->addWidget(listeTelechargements);
		layout->addWidget(Multiuso::closeButton(this));
}

void TelechargerFichier::ajouterTelechargement()
{
	nouveauTelechargement("");
}

void TelechargerFichier::nouveauTelechargement(const QString lien)
{
	show();
	emit DesactiverIcone();

	if (lien.isEmpty() && lineLien->text().isEmpty())
		return;

	QUrl urlFichier = lineLien->text();

	if (!lien.isEmpty())
		urlFichier = lien;

	QUrlInfo infosUrl;
		infosUrl.setName(lineLien->text());

	if (!lien.isEmpty())
		infosUrl.setName(lien);


	if (infosUrl.isFile())
	{
		QFileInfo infosDL(infosUrl.name());

		if (QMessageBox::question(this, "Multiuso", "Voulez-vous enregistrer <em>« " + infosDL.fileName() + " »</em> ?",
					QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
			return;


		QSettings emplacementDossier(Multiuso::appDirPath() + "/reglages/config.ini", QSettings::IniFormat);

		QDir dir;
			dir.mkpath(emplacementDossier.value("telechargements/dossier").toString() + "/Multiuso - Téléchargements");

		QFile fileTmp(emplacementDossier.value("telechargements/dossier").toString() + "/Multiuso - Téléchargements/" + infosDL.fileName());

		if (fileTmp.exists())
		{
			int reponse = QMessageBox::question(this, "Multiuso", "Le fichier <em>« " + infosDL.fileName() + " »</em> existe déjà !<br />"
					"Voulez-vous le remplacer ?", QMessageBox::Yes | QMessageBox::No);

			if (reponse == QMessageBox::Yes)
				fileTmp.remove();

			else
				return;
		}

		QNetworkRequest requete(urlFichier);

		QNetworkAccessManager *manager = new QNetworkAccessManager;

		QNetworkReply *reponse = manager->get(requete);

		connect(reponse, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(telechargementContinue(qint64, qint64)));
		connect(reponse, SIGNAL(finished()), this, SLOT(finTelechargement()));

		reponses << reponse;

		QProgressBar *progression = new QProgressBar;

		progressionsTelechargements << progression;

		nomsFichiers << infosDL.fileName();

		QString nomDuFichierActuel = infosDL.fileName();
			nomDuFichierActuel = Multiuso::htmlspecialchars(nomDuFichierActuel);

		QTableWidgetItem *nom = new QTableWidgetItem(nomDuFichierActuel);
			nom->setFlags(nom->flags() & ~Qt::ItemIsEditable);

		QTableWidgetItem *statut = new QTableWidgetItem("En attente...");
			statut->setFlags(statut->flags() & ~Qt::ItemIsEditable);

		QPushButton *annuler = new QPushButton;
			annuler->setToolTip("Téléchargement n°" + QString::number(listeTelechargements->rowCount() + 1));
			annuler->setIcon(QIcon(":/icones/telechargements/annuler.png"));
			connect(annuler, SIGNAL(clicked()), this, SLOT(annulerTelechargement()));

		int ligne = listeTelechargements->rowCount();

		listeTelechargements->setRowCount(listeTelechargements->rowCount() + 1);
		listeTelechargements->setCellWidget(ligne, 0, annuler);
		listeTelechargements->setItem(ligne, 1, nom);
		listeTelechargements->setCellWidget(ligne, 2, progression);
		listeTelechargements->setItem(ligne, 3, statut);

		listeTelechargements->resizeColumnsToContents();
		listeTelechargements->horizontalHeader()->setStretchLastSection(true);
	}
}

void TelechargerFichier::telechargementContinue(qint64 recu, qint64 total)
{
	QNetworkReply *reponse;
		reponse = qobject_cast<QNetworkReply *>(sender());

	QProgressBar *progression = progressionsTelechargements.value(reponses.indexOf(reponse));

	listeTelechargements->item(reponses.indexOf(reponse), 3)->setText("Transfert en cours...");

	if (total != -1)
	{
		progression->setMaximum(total);
		progression->setValue(recu);
	}
}

void TelechargerFichier::annulerTelechargement()
{
	QPushButton *bouton;
		bouton = qobject_cast<QPushButton *>(sender());
		bouton->setDisabled(true);

	QString toolTip = bouton->toolTip();
		toolTip = toolTip.replace(QRegExp("Téléchargement n°(.+)"), "\\1");

	int ligne = toolTip.toInt() - 1;

	QNetworkReply *reponse = reponses.value(ligne);
		reponse->abort();

	QProgressBar *progression = progressionsTelechargements.value(ligne);
		progression->setValue(progression->maximum());

	listeTelechargements->item(ligne, 3)->setText("Annulé");
}

void TelechargerFichier::finTelechargement()
{
	QNetworkReply *reponse;
		reponse = qobject_cast<QNetworkReply *>(sender());

	QPushButton *bouton = qobject_cast<QPushButton *>(listeTelechargements->cellWidget(reponses.indexOf(reponse), 0));
		bouton->setDisabled(true);

	QProgressBar *progression = progressionsTelechargements.value(reponses.indexOf(reponse));
		progression->setMaximum(100);
		progression->setValue(100);

	listeTelechargements->item(reponses.indexOf(reponse), 3)->setText("Transfert terminé");

	QString nomFichier = nomsFichiers.value(reponses.indexOf(reponse));

	QSettings emplacementDossier(Multiuso::appDirPath() + "/reglages/config.ini", QSettings::IniFormat);

	QDir dir;
		dir.mkpath(emplacementDossier.value("telechargements/dossier").toString() + "/Multiuso - Téléchargements");

	QFile fichier(emplacementDossier.value("telechargements/dossier").toString() + "/Multiuso - Téléchargements/" + nomFichier);
		fichier.open(QIODevice::WriteOnly | QIODevice::Truncate);
		fichier.write(reponse->readAll());
		fichier.close();
}

void TelechargerFichier::closeEvent(QCloseEvent *event)
{
	accept();
	event->accept();
}

void TelechargerFichier::accept()
{
	emit MajEtat();

	QDialog::accept();
}

void TelechargerFichier::reject()
{
	accept();
}
