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

#include "EditeurDeTexte.h"
#include "autresClasses/RechercherRemplacer.h"

EditeurDeTexte::EditeurDeTexte(QWidget *parent = 0) : QMainWindow(parent)
{
	toolBarOptions = addToolBar("Options du texte");
		toolBarOptions->setObjectName("Options du texte");


		addToolBarBreak();


	toolBarTexte = addToolBar("Options de l'éditeur de texte");
		toolBarTexte->setObjectName("Options de l'éditeur de texte");

	creerOptionsTexte();

	QToolButton *buttonNewTab = new QToolButton;
		buttonNewTab->setDefaultAction(nouveau);
		buttonNewTab->setAutoRaise(true);

	onglets = new QTabWidget;
		onglets->setMovable(true);
		onglets->setTabsClosable(true);
		onglets->setDocumentMode(true);
		onglets->setCornerWidget(buttonNewTab, Qt::TopRightCorner);
		connect(onglets, SIGNAL(tabCloseRequested(int)), this, SLOT(slotFermerOnglet(int)));
		connect(onglets, SIGNAL(currentChanged(int)), this, SLOT(slotChangementOnglet(int)));
		slotNouveau();

	setCentralWidget(onglets);

	QSettings restaurer(Multiuso::appDirPath() + "/reglages/editeur_de_texte.ini", QSettings::IniFormat);
		restoreState(restaurer.value("etat_fenetre").toByteArray());
}

void EditeurDeTexte::creerOptionsTexte()
{
	gras = new QAction("Gras (Ctrl + B)", this);
		gras->setIcon(QIcon(":/icones/editeur_de_texte/gras.png"));
		gras->setShortcut(QKeySequence("Ctrl+B"));
		gras->setCheckable(true);
		gras->setChecked(false);
		connect(gras, SIGNAL(triggered()), this, SLOT(slotGras()));
		toolBarTexte->addAction(gras);

	italique = new QAction("Italique (Ctrl + I)", this);
		italique->setIcon(QIcon(":/icones/editeur_de_texte/italique.png"));
		italique->setShortcut(QKeySequence("Ctrl+I"));
		italique->setCheckable(true);
		italique->setChecked(false);
		connect(italique, SIGNAL(triggered()), this, SLOT(slotItalique()));
		toolBarTexte->addAction(italique);

	souligne = new QAction("Souligné (Ctrl + U)", this);
		souligne->setIcon(QIcon(":/icones/editeur_de_texte/souligne.png"));
		souligne->setShortcut(QKeySequence("Ctrl+U"));
		souligne->setCheckable(true);
		souligne->setChecked(false);
		connect(souligne, SIGNAL(triggered()), this, SLOT(slotSouligne()));
		toolBarTexte->addAction(souligne);

	toolBarTexte->addSeparator();

	gauche = new QAction("Gauche", this);
		gauche->setIcon(QIcon(":/icones/editeur_de_texte/gauche.png"));
		gauche->setCheckable(true);
		gauche->setChecked(true);
		connect(gauche, SIGNAL(triggered()), this, SLOT(slotGauche()));
		toolBarTexte->addAction(gauche);

	centre = new QAction("Centré", this);
		centre->setIcon(QIcon(":/icones/editeur_de_texte/centre.png"));
		centre->setCheckable(true);
		centre->setChecked(false);
		connect(centre, SIGNAL(triggered()), this, SLOT(slotCentre()));
		toolBarTexte->addAction(centre);

	droite = new QAction("Droite", this);
		droite->setIcon(QIcon(":/icones/editeur_de_texte/droite.png"));
		droite->setCheckable(true);
		droite->setChecked(false);
		connect(droite, SIGNAL(triggered()), this, SLOT(slotDroite()));
		toolBarTexte->addAction(droite);

	justifie = new QAction("Justifié", this);
		justifie->setIcon(QIcon(":/icones/editeur_de_texte/justifie.png"));
		justifie->setCheckable(true);
		justifie->setChecked(false);
		connect(justifie, SIGNAL(triggered()), this, SLOT(slotJustifie()));
		toolBarTexte->addAction(justifie);


	toolBarTexte->addSeparator();


	taille = new QComboBox;
		taille->addItem("6");
		taille->addItem("7");
		taille->addItem("8");
		taille->addItem("9");
		taille->addItem("10");
		taille->addItem("11");
		taille->addItem("12");
		taille->addItem("13");
		taille->addItem("14");
		taille->addItem("15");
		taille->addItem("16");
		taille->addItem("18");
		taille->addItem("20");
		taille->addItem("22");
		taille->addItem("24");
		taille->addItem("26");
		taille->addItem("28");
		taille->addItem("32");
		taille->addItem("36");
		taille->addItem("40");
		taille->addItem("42");
		taille->addItem("44");
		taille->addItem("48");
		taille->addItem("54");
		taille->addItem("60");
		taille->addItem("66");
		taille->addItem("72");
		taille->addItem("80");
		taille->addItem("88");
		taille->addItem("96");
		taille->setCurrentIndex(5);
		connect(taille, SIGNAL(currentIndexChanged(QString)), this, SLOT(changerTaille(QString)));
		toolBarTexte->addWidget(taille);


	toolBarTexte->addSeparator();


	police = new QFontComboBox;
		connect(police, SIGNAL(currentFontChanged(QFont)), this, SLOT(changerPolice(QFont)));
		toolBarTexte->addWidget(police);


	toolBarTexte->addSeparator();


	couleur = new QAction("Couleur", this);
		couleur->setIcon(QIcon(":/icones/editeur_de_texte/couleur.png"));
		connect(couleur, SIGNAL(triggered()), this, SLOT(changerCouleur()));
		toolBarTexte->addAction(couleur);


	toolBarTexte->addSeparator();


	selectionMajuscule = new QAction("Sélection en majuscule", this);
		selectionMajuscule->setIcon(QIcon(":/icones/editeur_de_texte/selectionMajuscule.png"));
		connect(selectionMajuscule, SIGNAL(triggered()), this, SLOT(slotSelectionMajuscule()));
		toolBarTexte->addAction(selectionMajuscule);

	selectionMinuscule = new QAction("Sélection en minuscule", this);
		selectionMinuscule->setIcon(QIcon(":/icones/editeur_de_texte/selectionMinuscule.png"));
		connect(selectionMinuscule, SIGNAL(triggered()), this, SLOT(slotSelectionMinuscule()));
		toolBarTexte->addAction(selectionMinuscule);

	nouveau = new QAction("Nouveau (Ctrl + N)", this);
		nouveau->setIcon(QIcon(":/icones/editeur_de_texte/nouveau.png"));
		nouveau->setShortcut(QKeySequence("Ctrl+N"));
		connect(nouveau, SIGNAL(triggered()), this, SLOT(slotNouveau()));
		toolBarOptions->addAction(nouveau);

	enregistrer = new QAction("Enregistrer (Ctrl + s)", this);
		enregistrer->setIcon(QIcon(":/icones/editeur_de_texte/enregistrer.png"));
		enregistrer->setShortcut(QKeySequence("Ctrl+S"));
		enregistrer->setDisabled(true);
		connect(enregistrer, SIGNAL(triggered()), this, SLOT(slotEnregistrer()));
		toolBarOptions->addAction(enregistrer);

	enregistrerSous = new QAction("Enregistrer sous... (Ctrl + Shift + S)", this);
		enregistrerSous->setIcon(QIcon(":/icones/editeur_de_texte/enregistrerSous.png"));
		enregistrerSous->setShortcut(QKeySequence("Ctrl+Shift+S"));
		connect(enregistrerSous, SIGNAL(triggered()), this, SLOT(slotEnregistrerSous()));
		toolBarOptions->addAction(enregistrerSous);

	ouvrir = new QAction("Ouvrir... (Ctrl + 0)", this);
		ouvrir->setIcon(QIcon(":/icones/editeur_de_texte/ouvrir.png"));
		ouvrir->setShortcut(QKeySequence("Ctrl+O"));
		connect(ouvrir, SIGNAL(triggered()), this, SLOT(slotOuvrir()));
		toolBarOptions->addAction(ouvrir);


	toolBarOptions->addSeparator();


	annuler = new QAction("Annuler (Ctrl + Z)", this);
		annuler->setIcon(QIcon(":/icones/editeur_de_texte/annuler.png"));
		annuler->setShortcut(QKeySequence("Ctrl+Z"));
		annuler->setDisabled(true);
		connect(annuler, SIGNAL(triggered()), this, SLOT(slotAnnuler()));
		toolBarOptions->addAction(annuler);

	retablir = new QAction("Rétablir (Ctrl + Y)", this);
		retablir->setIcon(QIcon(":/icones/editeur_de_texte/retablir.png"));
		retablir->setShortcut(QKeySequence("Ctrl+Y"));
		retablir->setDisabled(true);
		connect(retablir, SIGNAL(triggered()), this, SLOT(slotRetablir()));
		toolBarOptions->addAction(retablir);


	toolBarOptions->addSeparator();


	supprimerSelection = new QAction("Supprimer (Suppr)", this);
		supprimerSelection->setIcon(QIcon(":/icones/editeur_de_texte/supprimer.png"));
		supprimerSelection->setShortcut(QKeySequence("Suppr"));
		connect(supprimerSelection, SIGNAL(triggered()), this, SLOT(slotSupprimerSelection()));
		toolBarOptions->addAction(supprimerSelection);


	toolBarOptions->addSeparator();


	rechercher = new QAction("Rechercher... (Ctrl + F)", this);
		rechercher->setIcon(QIcon(":/icones/editeur_de_texte/rechercher.png"));
		rechercher->setShortcut(QKeySequence("Ctrl+F"));
		connect(rechercher, SIGNAL(triggered()), this, SLOT(slotRechercher()));
		toolBarOptions->addAction(rechercher);

	rechercherRemplacer = new QAction("Rechercher/Remplacer... (Ctrl + H)", this);
		rechercherRemplacer->setIcon(QIcon(":/icones/editeur_de_texte/rechercherRemplacer.png"));
		rechercherRemplacer->setShortcut(QKeySequence("Ctrl+H"));
		connect(rechercherRemplacer, SIGNAL(triggered()), this, SLOT(slotRechercherRemplacer()));
		toolBarOptions->addAction(rechercherRemplacer);


	toolBarOptions->addSeparator();


	imprimer = new QAction("Imprimer (Ctrl + P)", this);
		imprimer->setIcon(QIcon(":/icones/editeur_de_texte/imprimer.png"));
		imprimer->setShortcut(QKeySequence("Ctrl+P"));
		connect(imprimer, SIGNAL(triggered()), this, SLOT(slotImprimer()));
		toolBarOptions->addAction(imprimer);


	toolBarOptions->addSeparator();


	copier = new QAction("Copier (Ctrl + C)", this);
		copier->setIcon(QIcon(":/icones/editeur_de_texte/copier.png"));
		copier->setShortcut(QKeySequence("Ctrl+C"));
		connect(copier, SIGNAL(triggered()), this, SLOT(slotCopier()));
		toolBarOptions->addAction(copier);

	couper = new QAction("Couper (Ctrl + X)", this);
		couper->setIcon(QIcon(":/icones/editeur_de_texte/couper.png"));
		couper->setShortcut(QKeySequence("Ctrl+X"));
		connect(couper, SIGNAL(triggered()), this, SLOT(slotCouper()));
		toolBarOptions->addAction(couper);

	coller = new QAction("Coller (Ctrl + V)", this);
		coller->setIcon(QIcon(":/icones/editeur_de_texte/coller.png"));
		coller->setShortcut(QKeySequence("Ctrl+V"));
		connect(coller, SIGNAL(triggered()), this, SLOT(slotColler()));
		toolBarOptions->addAction(coller);

	toutSelectionner = new QAction("Tout sélectionner (Ctrl + A)", this);
		toutSelectionner->setIcon(QIcon(":/icones/editeur_de_texte/toutSelectionner.png"));
		toutSelectionner->setShortcut(QKeySequence("Ctrl+A"));
		connect(toutSelectionner, SIGNAL(triggered()), this, SLOT(slotToutSelectionner()));
		toolBarOptions->addAction(toutSelectionner);

	insererImage = new QAction("Insérer une image", this);
		insererImage->setIcon(QIcon(":/icones/editeur_de_texte/insererImage.png"));
		connect(insererImage, SIGNAL(triggered()), this, SLOT(slotInsererImage()));
		toolBarOptions->addAction(insererImage);

	repeterTexte = new QAction("Répéter du texte", this);
		repeterTexte->setIcon(QIcon(":/icones/editeur_de_texte/repeterTexte.png"));
		connect(repeterTexte, SIGNAL(triggered()), this, SLOT(slotRepeterTexte()));
		toolBarOptions->addAction(repeterTexte);
}

void EditeurDeTexte::slotChangementTexte()
{
	QSettings reglagesSauvegarde(Multiuso::appDirPath() + "/reglages/editeur_de_texte.ini", QSettings::IniFormat);

	if (reglagesSauvegarde.value("enregistrement/enregistrement_automatique").toBool())
	{
		slotEnregistrer();

		return;
	}

	onglets->setTabIcon(onglets->currentIndex(), QIcon(":/icones/editeur_de_texte/non_enregistre.png"));
	pageActuelle()->setEstEnregistre(false);
	pageActuelle()->setSauvegardeOk(true);

	enregistrer->setEnabled(true);
}

void EditeurDeTexte::slotFermerOnglet(int onglet)
{
	onglets->setCurrentIndex(onglet);

	int reponse = QMessageBox::Yes;

	if (!pageActuelle()->estEnregistre())
	{
		reponse = QMessageBox::question(this, "Multiuso", "Toutes les modifications non enregistrées seront perdues<br />"
				"Voulez-vous continuer ?", QMessageBox::Yes | QMessageBox::No);
	}

	if (reponse == QMessageBox::Yes)
	{
		if (onglets->count() - 1 == 0)
			slotNouveau();

		onglets->removeTab(onglet);
	}
}

void EditeurDeTexte::sauvegarderEtat()
{
	QSettings enregistrer(Multiuso::appDirPath() + "/reglages/editeur_de_texte.ini", QSettings::IniFormat);
		enregistrer.setValue("etat_fenetre", saveState());
}

bool EditeurDeTexte::tousLesDocumentsEnregistres()
{
	for (int i = 0; i < onglets->count(); i++)
	{
		onglets->setCurrentIndex(i);

		TextEdit *textEdit = onglets->currentWidget()->findChild<TextEdit *>();

		if (!textEdit->estEnregistre())
			return false;
	}

	return true;
}

void EditeurDeTexte::slotChangementOnglet(int onglet)
{
	onglet = 0;

	slotRemettreValeurs();
}

QWidget *EditeurDeTexte::nouvelOnglet()
{
	TextEdit *champDeSaisie = new TextEdit;
		champDeSaisie->setAcceptRichText(true);
		champDeSaisie->setFontPointSize(11);
		connect(champDeSaisie, SIGNAL(textChanged()), this, SLOT(slotChangementTexte()));
		connect(champDeSaisie, SIGNAL(redoAvailable(bool)), this, SLOT(slotAMRedo(bool)));
		connect(champDeSaisie, SIGNAL(undoAvailable(bool)), this, SLOT(slotAMUndo(bool)));
		connect(champDeSaisie, SIGNAL(ouvrirFichier(QString)), this, SLOT(slotOuvrirFichier(QString)));

	QVBoxLayout *layoutChampDeSaisie = new QVBoxLayout;
		layoutChampDeSaisie->addWidget(champDeSaisie);
		layoutChampDeSaisie->setContentsMargins(0, 0, 0, 0);

	QWidget *widgetConteneur = new QWidget;
		widgetConteneur->setLayout(layoutChampDeSaisie);

	return widgetConteneur;
}

TextEdit *EditeurDeTexte::pageActuelle()
{
	return onglets->currentWidget()->findChild<TextEdit *>();
}

QString EditeurDeTexte::titreTabCourrant()
{
	return onglets->tabText(onglets->currentIndex());
}

void EditeurDeTexte::slotAMRedo(bool available)
{
	retablir->setEnabled(available);
	pageActuelle()->setRedoOk(available);
}

void EditeurDeTexte::slotAMUndo(bool available)
{
	annuler->setEnabled(available);
	pageActuelle()->setUndoOk(available);
}

void EditeurDeTexte::slotRemettreValeurs()
{
	annuler->setEnabled(pageActuelle()->undoOk());
	retablir->setEnabled(pageActuelle()->redoOk());

	gras->setChecked(pageActuelle()->estGras());
	italique->setChecked(pageActuelle()->estItalique());
	souligne->setChecked(pageActuelle()->estSouligne());

	gauche->setChecked(pageActuelle()->estAligneGauche());
	centre->setChecked(pageActuelle()->estAligneCentre());
	droite->setChecked(pageActuelle()->estAligneDroite());
	justifie->setChecked(pageActuelle()->estAligneJustifie());

	taille->setCurrentIndex(pageActuelle()->tailleDuTexte());
	police->setCurrentFont(pageActuelle()->policeDuTexte());
}

void EditeurDeTexte::slotGras()
{
	if (gras->isChecked())
	{
		pageActuelle()->setFontWeight(QFont::Bold);
		pageActuelle()->setEstGras(true);
	}

	else
	{
		pageActuelle()->setFontWeight(QFont::Normal);
		pageActuelle()->setEstGras(false);
	}
}

void EditeurDeTexte::slotItalique()
{
	if (italique->isChecked())
	{
		pageActuelle()->setFontItalic(true);
		pageActuelle()->setEstItalique(true);
	}

	else
	{
		pageActuelle()->setFontItalic(false);
		pageActuelle()->setEstItalique(false);
	}
}

void EditeurDeTexte::slotSouligne()
{
	if (souligne->isChecked())
	{
		pageActuelle()->setFontUnderline(true);
		pageActuelle()->setEstSouligne(true);
	}

	else
	{
		pageActuelle()->setFontUnderline(false);
		pageActuelle()->setEstSouligne(false);
	}
}

void EditeurDeTexte::slotGauche()
{
	pageActuelle()->setAlignment(Qt::AlignLeft);

	pageActuelle()->setEstAligneGauche(true);
	pageActuelle()->setEstAligneCentre(false);
	pageActuelle()->setEstAligneDroite(false);
	pageActuelle()->setEstAligneJustifie(false);

	centre->setChecked(false);
	droite->setChecked(false);
	justifie->setChecked(false);
}

void EditeurDeTexte::slotCentre()
{
	pageActuelle()->setAlignment(Qt::AlignCenter);

	pageActuelle()->setEstAligneGauche(false);
	pageActuelle()->setEstAligneCentre(true);
	pageActuelle()->setEstAligneDroite(false);
	pageActuelle()->setEstAligneJustifie(false);

	gauche->setChecked(false);
	droite->setChecked(false);
	justifie->setChecked(false);
}

void EditeurDeTexte::slotDroite()
{
	pageActuelle()->setAlignment(Qt::AlignRight);

	pageActuelle()->setEstAligneGauche(false);
	pageActuelle()->setEstAligneCentre(false);
	pageActuelle()->setEstAligneDroite(true);
	pageActuelle()->setEstAligneJustifie(false);

	gauche->setChecked(false);
	centre->setChecked(false);
	justifie->setChecked(false);
}

void EditeurDeTexte::slotJustifie()
{
	pageActuelle()->setAlignment(Qt::AlignJustify);

	pageActuelle()->setEstAligneGauche(false);
	pageActuelle()->setEstAligneCentre(false);
	pageActuelle()->setEstAligneDroite(false);
	pageActuelle()->setEstAligneJustifie(true);

	gauche->setChecked(false);
	centre->setChecked(false);
	droite->setChecked(false);
}

void EditeurDeTexte::changerTaille(QString)
{
	pageActuelle()->setFontPointSize(taille->currentText().toInt());
	pageActuelle()->setTailleDuTexte(taille->currentIndex());
}

void EditeurDeTexte::changerPolice(QFont)
{
	pageActuelle()->setCurrentFont(police->currentFont());
	pageActuelle()->setPoliceDuTexte(police->currentFont());
}

void EditeurDeTexte::changerCouleur()
{
	QColor nouvelleCouleur = QColorDialog::getColor(pageActuelle()->textColor(), this);

	if (nouvelleCouleur.isValid())
    		pageActuelle()->setTextColor(nouvelleCouleur);
}

void EditeurDeTexte::slotSelectionMajuscule()
{
	QString texte = pageActuelle()->textCursor().selectedText();
		texte = texte.toUpper();

	pageActuelle()->textCursor().removeSelectedText();
	pageActuelle()->textCursor().insertText(texte);
}

void EditeurDeTexte::slotSelectionMinuscule()
{
	QString texte = pageActuelle()->textCursor().selectedText();
		texte = texte.toLower();

	pageActuelle()->textCursor().removeSelectedText();
	pageActuelle()->textCursor().insertText(texte);
}

void EditeurDeTexte::slotNouveau()
{
	onglets->addTab(nouvelOnglet(), QIcon(":/icones/editeur_de_texte/enregistre.png"), "Nouveau document");
	onglets->setCurrentIndex(onglets->count() - 1);
	taille->setCurrentIndex(5);
}

void EditeurDeTexte::slotEnregistrer()
{
	QFile fichier(pageActuelle()->fichierOuvert());

	if (fichier.exists() && fichier.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QFileInfo infosFichier(fichier);

		if (infosFichier.fileName().contains(QRegExp("(.){1,}.(mlts)?html?$")))
		{
			fichier.write(pageActuelle()->toHtml().toAscii());
		}

		else
		{
			fichier.write(pageActuelle()->toPlainText().toAscii());
		}

		pageActuelle()->setEstEnregistre(true);
		pageActuelle()->setSauvegardeOk(false);

		enregistrer->setDisabled(true);

		onglets->setTabIcon(onglets->currentIndex(), QIcon(":/icones/editeur_de_texte/enregistre.png"));
	}

	else
	{
		slotEnregistrerSous();
	}

	fichier.close();
}

void EditeurDeTexte::slotEnregistrerSous()
{
	QString cheminDuFichier = QFileDialog::getSaveFileName(this, "Multiuso", Multiuso::lastPath() + "/Nouveau document.mltshtml", "Texte (*)");
	
	Multiuso::setLastPath(cheminDuFichier);

	QFile fichier(cheminDuFichier);

	if (fichier.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QFileInfo infosFichier(fichier);

		if (infosFichier.fileName().contains(QRegExp("(.){1,}.(mlts)?html?$")))
			fichier.write(pageActuelle()->toHtml().toAscii());

		else
			fichier.write(pageActuelle()->toPlainText().toAscii());

		QString titre = infosFichier.fileName();

		onglets->setTabText(onglets->currentIndex(), titre);
		onglets->setTabIcon(onglets->currentIndex(), QIcon(":/icones/editeur_de_texte/enregistre.png"));

		pageActuelle()->setEstEnregistre(true);
		pageActuelle()->setSauvegardeOk(false);
		pageActuelle()->setFichierOuvert(cheminDuFichier);

		enregistrer->setDisabled(true);
	}

	fichier.close();
}

void EditeurDeTexte::slotOuvrir()
{
	QString cheminFichier = QFileDialog::getOpenFileName(this, "Multiuso", Multiuso::lastPath(), "Texte (*)");
	
	Multiuso::setLastPath(cheminFichier);
		
	if (!cheminFichier.isEmpty())
		slotOuvrirFichier(cheminFichier);
}

void EditeurDeTexte::slotOuvrirFichier(QString cheminFichier)
{
	QFile fichier(cheminFichier);

	if (fichier.exists() && fichier.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		if (onglets->count() == 1 && onglets->tabText(onglets->currentIndex()) == "Nouveau document" && pageActuelle()->estEnregistre())
		{
		}

		else
		{
			slotNouveau();
		}

		onglets->setCurrentIndex(onglets->count());

		QFileInfo infosFichier(fichier);

		if (infosFichier.fileName().contains(QRegExp("(.){1,}.(mlts)?html?$")))
			pageActuelle()->setHtml(fichier.readAll());

		else
			pageActuelle()->setPlainText(fichier.readAll());

		onglets->setTabText(onglets->currentIndex(), infosFichier.fileName());
		onglets->setTabIcon(onglets->currentIndex(), QIcon(":/icones/editeur_de_texte/enregistre.png"));

		pageActuelle()->setEstEnregistre(true);
		pageActuelle()->setSauvegardeOk(false);
		pageActuelle()->setFichierOuvert(cheminFichier);

		enregistrer->setDisabled(true);
	}

	fichier.close();
}


void EditeurDeTexte::slotAnnuler()
{
	pageActuelle()->undo();
}

void EditeurDeTexte::slotRetablir()
{
	pageActuelle()->redo();
}

void EditeurDeTexte::slotSupprimerSelection()
{
	pageActuelle()->textCursor().removeSelectedText();
}

void EditeurDeTexte::slotRechercher()
{
	l_rechercher = new QLineEdit;

	if (pageActuelle()->textCursor().hasSelection())
		l_rechercher->setText(pageActuelle()->textCursor().selectedText());

	QLabel *l_motARechercher = new QLabel("Mot à rechercher :");

	QPushButton *b_rechercher = new QPushButton("&Rechercher");
	QPushButton *b_ok = new QPushButton("&OK");

	QGridLayout *layoutPrincipal = new QGridLayout;
		layoutPrincipal->addWidget(l_motARechercher, 0, 0, 1, 1);
		layoutPrincipal->addWidget(l_rechercher, 0, 1, 1, 1);
		layoutPrincipal->addWidget(b_rechercher, 1, 0, 1, 1, Qt::AlignRight);
		layoutPrincipal->addWidget(b_ok, 1, 1, 1, 1, Qt::AlignRight);

	QDialog *d_rechercher = new QDialog(this);
		d_rechercher->setLayout(layoutPrincipal);
		d_rechercher->setWindowTitle("Éditeur de texte - Rechercher");
		d_rechercher->setWindowIcon(QIcon(":/icones/editeur_de_texte/rechercher.png"));

	connect(b_rechercher, SIGNAL(clicked()), this, SLOT(slotRechercher2()));
	connect(b_ok, SIGNAL(clicked()), d_rechercher, SLOT(accept()));

	d_rechercher->exec();
}

void EditeurDeTexte::slotRechercher2()
{
	QTextCursor curseurActuel = pageActuelle()->textCursor();

	if (pageActuelle()->textCursor().selectedText() == l_rechercher->text())
		pageActuelle()->textCursor().setPosition(pageActuelle()->textCursor().position() + pageActuelle()->textCursor().selectedText().length());

	else
		pageActuelle()->moveCursor(QTextCursor::Start);

	bool trouve = pageActuelle()->find(l_rechercher->text());

	if (!trouve)
	{
		QMessageBox::critical(this, "Multiuso", "Impossible de trouver :<br /><strong>" + l_rechercher->text() + "</strong>");
		pageActuelle()->setTextCursor(curseurActuel);
	}
}

void EditeurDeTexte::slotRechercherRemplacer()
{
	l_rechercher = new QLineEdit;
	l_remplacer = new QLineEdit;

	if (pageActuelle()->textCursor().hasSelection())
		l_rechercher->setText(pageActuelle()->textCursor().selectedText());

	QLabel *l_motARechercher = new QLabel("Mot à rechercher :");
	QLabel *l_motARemplacer = new QLabel("Remplacer ce mot par :");

	QPushButton *b_rechercher = new QPushButton("&Rechercher");
	QPushButton *b_remplacer = new QPushButton("Rem&placer");
	QPushButton *b_ok = new QPushButton("&OK");

	QGridLayout *layoutPrincipal = new QGridLayout;
		layoutPrincipal->addWidget(l_motARechercher, 0, 0, 1, 1);
		layoutPrincipal->addWidget(l_rechercher, 0, 1, 1, 1);
		layoutPrincipal->addWidget(l_motARemplacer, 1, 0, 1, 1);
		layoutPrincipal->addWidget(l_remplacer, 1, 1, 1, 1);
		layoutPrincipal->addWidget(b_rechercher, 2, 0, 1, 1);
		layoutPrincipal->addWidget(b_remplacer, 2, 1, 1, 1);
		layoutPrincipal->addWidget(b_ok, 3, 0, 1, 2);

	QDialog *d_rechercher = new QDialog(this);
		d_rechercher->setLayout(layoutPrincipal);
		d_rechercher->setWindowTitle("Éditeur de texte - Rechercher/Remplacer");
		d_rechercher->setWindowIcon(QIcon(":/icones/editeur_de_texte/rechercherRemplacer.png"));

	connect(b_rechercher, SIGNAL(clicked()), this, SLOT(slotRechercher2()));
	connect(b_remplacer, SIGNAL(clicked()), this, SLOT(slotRechercherRemplacer2()));
	connect(b_ok, SIGNAL(clicked()), d_rechercher, SLOT(accept()));

	d_rechercher->exec();
}

void EditeurDeTexte::slotRechercherRemplacer2()
{
	if (pageActuelle()->textCursor().hasSelection())
	{
		pageActuelle()->textCursor().removeSelectedText();
		pageActuelle()->textCursor().insertText(l_remplacer->text());
	}
}

void EditeurDeTexte::slotImprimer()
{
	QPrinter *imprimante = new QPrinter(QPrinter::ScreenResolution);
		imprimante->setDocName(titreTabCourrant());
		imprimante->setPaperSize(QPrinter::A4);
		imprimante->setOrientation(QPrinter::Portrait);

	QPrintDialog *dialogueImpression = new QPrintDialog(imprimante, this);
		dialogueImpression->setWindowTitle("Éditeur de texte - Imprimer");
		dialogueImpression->setWindowIcon(QIcon(":/icones/editeur_de_texte/editeur_de_texte.png"));

		if (pageActuelle()->textCursor().hasSelection())
			dialogueImpression->setOption(QAbstractPrintDialog::PrintSelection);

		connect(dialogueImpression, SIGNAL(accepted(QPrinter *)), this, SLOT(imprimerTexte(QPrinter *)));

		dialogueImpression->exec();
}

void EditeurDeTexte::imprimerTexte(QPrinter *imprimante)
{
	pageActuelle()->print(imprimante);
}

void EditeurDeTexte::slotCopier()
{
	pageActuelle()->copy();
}

void EditeurDeTexte::slotCouper()
{
	pageActuelle()->cut();
}

void EditeurDeTexte::slotColler()
{
	pageActuelle()->paste();
}

void EditeurDeTexte::slotToutSelectionner()
{
	pageActuelle()->selectAll();
}

void EditeurDeTexte::slotInsererImage()
{
	QFile fichierInsertion(pageActuelle()->fichierOuvert());

	QFileInfo infosFichierOuvert(fichierInsertion);

	if (infosFichierOuvert.fileName().contains(QRegExp("(.){1,}.(mlts)?html?$")))
	{
		QString image = QFileDialog::getOpenFileName(this, "Multiuso", Multiuso::lastPath(), "Image (*)");
		
		Multiuso::setLastPath(image);

		if (!image.isEmpty())
		{
			QFile imageAInserer(image);

			QFileInfo infosImage(imageAInserer);

			imageAInserer.copy(infosFichierOuvert.path() + "/" + infosImage.fileName());

			pageActuelle()->textCursor().insertHtml("<img src=\"" + infosImage.fileName()  + "\" />");
		}
	}

	else
	{
		QMessageBox::critical(this, "Multiuso", "Multiuso ne peut insérer des images que dans les formats de fichier :"
				"<br /><strong>*.mltshtml</strong>, <strong>*.htm</strong> et <strong>*.html</strong>");
	}
}

void EditeurDeTexte::slotRepeterTexte()
{
	QDialog *dialogue = new QDialog(this);

	texteARepeter = new QTextEdit;
		texteARepeter->setWordWrapMode(QTextOption::NoWrap);

		if (pageActuelle()->textCursor().hasSelection())
			texteARepeter->setPlainText(pageActuelle()->textCursor().selectedText());

	nombreDeFois = new QLineEdit;

	QPushButton *repeter = new QPushButton("&Répéter !");
		connect(repeter, SIGNAL(clicked()), this, SLOT(slotRepeterTexte2()));
		connect(repeter, SIGNAL(clicked()), dialogue, SLOT(accept()));

	QFormLayout *layoutRepeter = new QFormLayout;
		layoutRepeter->addRow("Texte à répéter :", texteARepeter);
		layoutRepeter->addRow("Nombre de fois :", nombreDeFois);
		layoutRepeter->addWidget(repeter);

	dialogue->setWindowTitle("Éditeur de texte - Répéter du texte");
	dialogue->setWindowIcon(QIcon(":/icones/editeur_de_texte/repeterTexte.png"));
	dialogue->resize(600, 260);
	dialogue->setLayout(layoutRepeter);
	dialogue->exec();
}

void EditeurDeTexte::slotRepeterTexte2()
{
	slotRepeter(texteARepeter->toPlainText(), nombreDeFois->text().toInt());
}

void EditeurDeTexte::slotRepeter(QString texte, int nombreDeFois)
{
	if (nombreDeFois > 2500)
	{
		nombreDeFois = 2500;

		QMessageBox::warning(this, "Multiuso", "Le nombre de répétitions est limité à 2500, ce texte ne sera donc répété que 2500 fois.");
	}


	QFile fichier(pageActuelle()->fichierOuvert());

	QFileInfo infosFichier(fichier);

	for (int i = 0; i < nombreDeFois; i++)
	{
		if (infosFichier.fileName().contains(QRegExp("(.){1,}.(mlts)?html?$")))
		{
			texte = texte.replace("\n", "<br />");
			pageActuelle()->insertHtml(texte);
		}

		else
		{
			pageActuelle()->insertPlainText(texte);
		}
	}
}
