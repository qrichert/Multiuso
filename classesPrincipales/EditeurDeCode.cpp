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

#include "EditeurDeCode.h"
#include "autresClasses/RechercherRemplacer.h"
#include "classesPrincipales/NavigateurWeb.h"
#include "classesPrincipales/FenPrincipale.h"

EditeurDeCode::EditeurDeCode(QWidget *parent) : QMainWindow(parent)
{
	toolBarOptions = addToolBar("Options du code");
		toolBarOptions->setObjectName("Options du code");

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

	pointeurSurParent = parent;
	webBrowserPointer = NULL;

	setCentralWidget(onglets);

	QSettings restaurer(Multiuso::appDirPath() + "/ini/editeur_de_code.ini", QSettings::IniFormat);
		restoreState(restaurer.value("etat_fenetre").toByteArray());
}

void EditeurDeCode::creerOptionsTexte()
{
	nouveau = new QAction("Nouveau (Ctrl + N)", this);
		nouveau->setIcon(QIcon(":/icones/editeur_de_code/nouveau.png"));
		nouveau->setShortcut(QKeySequence("Ctrl+N"));
		connect(nouveau, SIGNAL(triggered()), this, SLOT(slotNouveau()));
		toolBarOptions->addAction(nouveau);

	enregistrer = new QAction("Enregistrer (Ctrl + s)", this);
		enregistrer->setIcon(QIcon(":/icones/editeur_de_code/enregistrer.png"));
		enregistrer->setShortcut(QKeySequence("Ctrl+S"));
		enregistrer->setDisabled(true);
		connect(enregistrer, SIGNAL(triggered()), this, SLOT(slotEnregistrer()));
		toolBarOptions->addAction(enregistrer);

	enregistrerSous = new QAction("Enregistrer sous... (Ctrl + Shift + S)", this);
		enregistrerSous->setIcon(QIcon(":/icones/editeur_de_code/enregistrerSous.png"));
		enregistrerSous->setShortcut(QKeySequence("Ctrl+Shift+S"));
		connect(enregistrerSous, SIGNAL(triggered()), this, SLOT(slotEnregistrerSous()));
		toolBarOptions->addAction(enregistrerSous);

	ouvrir = new QAction("Ouvrir... (Ctrl + 0)", this);
		ouvrir->setIcon(QIcon(":/icones/editeur_de_code/ouvrir.png"));
		ouvrir->setShortcut(QKeySequence("Ctrl+O"));
		connect(ouvrir, SIGNAL(triggered()), this, SLOT(slotOuvrir()));
		toolBarOptions->addAction(ouvrir);


	toolBarOptions->addSeparator();


	annuler = new QAction("Annuler (Ctrl + Z)", this);
		annuler->setIcon(QIcon(":/icones/editeur_de_code/annuler.png"));
		annuler->setShortcut(QKeySequence("Ctrl+Z"));
		annuler->setDisabled(true);
		connect(annuler, SIGNAL(triggered()), this, SLOT(slotAnnuler()));
		toolBarOptions->addAction(annuler);

	retablir = new QAction("Rétablir (Ctrl + Y)", this);
		retablir->setIcon(QIcon(":/icones/editeur_de_code/retablir.png"));
		retablir->setShortcut(QKeySequence("Ctrl+Y"));
		retablir->setDisabled(true);
		connect(retablir, SIGNAL(triggered()), this, SLOT(slotRetablir()));
		toolBarOptions->addAction(retablir);


	toolBarOptions->addSeparator();


	supprimerSelection = new QAction("Supprimer (Suppr)", this);
		supprimerSelection->setIcon(QIcon(":/icones/editeur_de_code/supprimer.png"));
		supprimerSelection->setShortcut(QKeySequence("Suppr"));
		connect(supprimerSelection, SIGNAL(triggered()), this, SLOT(slotSupprimerSelection()));
		toolBarOptions->addAction(supprimerSelection);


	toolBarOptions->addSeparator();


	rechercher = new QAction("Rechercher... (Ctrl + F)", this);
		rechercher->setIcon(QIcon(":/icones/editeur_de_code/rechercher.png"));
		rechercher->setShortcut(QKeySequence("Ctrl+F"));
		connect(rechercher, SIGNAL(triggered()), this, SLOT(slotRechercher()));
		toolBarOptions->addAction(rechercher);

	rechercherRemplacer = new QAction("Rechercher/Remplacer... (Ctrl + H)", this);
		rechercherRemplacer->setIcon(QIcon(":/icones/editeur_de_code/rechercherRemplacer.png"));
		rechercherRemplacer->setShortcut(QKeySequence("Ctrl+H"));
		connect(rechercherRemplacer, SIGNAL(triggered()), this, SLOT(slotRechercherRemplacer()));
		toolBarOptions->addAction(rechercherRemplacer);


	toolBarOptions->addSeparator();


	imprimer = new QAction("Imprimer (Ctrl + P)", this);
		imprimer->setIcon(QIcon(":/icones/editeur_de_code/imprimer.png"));
		imprimer->setShortcut(QKeySequence("Ctrl+P"));
		connect(imprimer, SIGNAL(triggered()), this, SLOT(slotImprimer()));
		toolBarOptions->addAction(imprimer);


	toolBarOptions->addSeparator();


	copier = new QAction("Copier (Ctrl + C)", this);
		copier->setIcon(QIcon(":/icones/editeur_de_code/copier.png"));
		copier->setShortcut(QKeySequence("Ctrl+C"));
		connect(copier, SIGNAL(triggered()), this, SLOT(slotCopier()));
		toolBarOptions->addAction(copier);

	couper = new QAction("Couper (Ctrl + X)", this);
		couper->setIcon(QIcon(":/icones/editeur_de_code/couper.png"));
		couper->setShortcut(QKeySequence("Ctrl+X"));
		connect(couper, SIGNAL(triggered()), this, SLOT(slotCouper()));
		toolBarOptions->addAction(couper);

	coller = new QAction("Coller (Ctrl + V)", this);
		coller->setIcon(QIcon(":/icones/editeur_de_code/coller.png"));
		coller->setShortcut(QKeySequence("Ctrl+V"));
		connect(coller, SIGNAL(triggered()), this, SLOT(slotColler()));
		toolBarOptions->addAction(coller);

	toutSelectionner = new QAction("Tout sélectionner (Ctrl + A)", this);
		toutSelectionner->setIcon(QIcon(":/icones/editeur_de_code/toutSelectionner.png"));
		toutSelectionner->setShortcut(QKeySequence("Ctrl+A"));
		connect(toutSelectionner, SIGNAL(triggered()), this, SLOT(slotToutSelectionner()));
		toolBarOptions->addAction(toutSelectionner);

	repeterTexte = new QAction("Répéter du texte", this);
		repeterTexte->setIcon(QIcon(":/icones/editeur_de_code/repeterTexte.png"));
		connect(repeterTexte, SIGNAL(triggered()), this, SLOT(slotRepeterTexte()));
		toolBarOptions->addAction(repeterTexte);


	toolBarOptions->addSeparator();


	selectionMajuscule = new QAction("Sélection en majuscule", this);
		selectionMajuscule->setIcon(QIcon(":/icones/editeur_de_code/selectionMajuscule.png"));
		connect(selectionMajuscule, SIGNAL(triggered()), this, SLOT(slotSelectionMajuscule()));
		toolBarOptions->addAction(selectionMajuscule);

	selectionMinuscule = new QAction("Sélection en minuscule", this);
		selectionMinuscule->setIcon(QIcon(":/icones/editeur_de_code/selectionMinuscule.png"));
		connect(selectionMinuscule, SIGNAL(triggered()), this, SLOT(slotSelectionMinuscule()));
		toolBarOptions->addAction(selectionMinuscule);


	toolBarOptions->addSeparator();


	colorisation = new QComboBox;
		colorisation->addItem("Texte brut");
		colorisation->addItem("ActionScript");
		colorisation->addItem("ASM");
		colorisation->addItem("AutoIT");
		colorisation->addItem("Befunge");
		colorisation->addItem("Brainfuck");
		colorisation->addItem("C");
		colorisation->addItem("Caml");
		colorisation->addItem("C++");
		colorisation->addItem("C#");
		colorisation->addItem("CSS");
		colorisation->addItem("D");
		colorisation->addItem("Erlang");
		colorisation->addItem("F#");
		colorisation->addItem("Fortran");
		colorisation->addItem("GLSL");
		colorisation->addItem("Haskell");
		colorisation->addItem("HTML / (x)HTML / XML");
		colorisation->addItem("INI");
		colorisation->addItem("Intercal");
		colorisation->addItem("Io");
		colorisation->addItem("Java");
		colorisation->addItem("JavaScript");
		colorisation->addItem("Lex et Yacc");
		colorisation->addItem("Lisp");
		colorisation->addItem("Lua");
		colorisation->addItem("Miranda");
		colorisation->addItem("Pascal");
		colorisation->addItem("Perl");
		colorisation->addItem("PHP");
		colorisation->addItem("Python");
		colorisation->addItem("Ruby");
		colorisation->addItem("SQL");
		colorisation->setCurrentIndex(0);
		toolBarOptions->addWidget(colorisation);
		connect(colorisation, SIGNAL(currentIndexChanged(int)), this, SLOT(changementHighlighter(int)));


	toolBarOptions->addSeparator();


	openInWebBrowser = new QAction("Ouvrir dans le navigateur Web", this);
		openInWebBrowser->setIcon(QIcon(":/icones/editeur_de_code/openInWebBrowser.png"));
		connect(openInWebBrowser, SIGNAL(triggered()), this, SLOT(slotOpenInWebBrowser()));
		toolBarOptions->addAction(openInWebBrowser);
}

void EditeurDeCode::slotOpenInWebBrowser()
{
	if (webBrowserPointer == NULL)
		return;

	NavigateurWeb *browser = qobject_cast<NavigateurWeb *>(webBrowserPointer);

	if (browser == 0)
		return;

	browser->openCodeEditDocument(onglets->tabText(onglets->currentIndex()), pageActuelle()->toPlainText());

	FenPrincipale *fen = qobject_cast<FenPrincipale *>(pointeurSurParent);

	if (!Multiuso::openTabsList(fen->tabWidget()).contains("Navigateur Web"))
		fen->homeTab()->openTab("Navigateur Web");

	fen->setTabIndex(Multiuso::tabIndexOf("Navigateur Web", fen->tabWidget()));
}

void EditeurDeCode::slotChangementTexte()
{
	QSettings reglages(Multiuso::appDirPath() + "/ini/editeur_de_code.ini", QSettings::IniFormat);

	if (reglages.value("enregistrement/enregistrement_automatique").toBool())
		slotEnregistrer();

	onglets->setTabIcon(onglets->currentIndex(), QIcon(":/icones/editeur_de_code/non_enregistre.png"));
	pageActuelle()->setEstEnregistre(false);
	pageActuelle()->setSauvegardeOk(true);

	enregistrer->setEnabled(true);
}

void EditeurDeCode::slotFermerOnglet(int onglet)
{
	onglets->setCurrentIndex(onglet);

	int reponse = QMessageBox::Yes;

	if (!pageActuelle()->estEnregistre())
		reponse = QMessageBox::question(this, "Multiuso", "Toutes les modifications non enregistrées seront perdues<br />"
				"Voulez-vous continuer ?", QMessageBox::Yes | QMessageBox::No);

	if (reponse == QMessageBox::Yes)
	{
		if (onglets->count() - 1 == 0)
			slotNouveau();

		onglets->removeTab(onglet);
	}
}

void EditeurDeCode::highlighterFor(QString extension)
{
	QStringList highlightersC;
		highlightersC << "c" << "h";

	QStringList highlightersCaml;
		highlightersCaml << "ml" << "mli";

	QStringList highlightersCPP;
		highlightersCPP << "cpp" << "c++" << "cc" << "cxx" << "hpp" << "h++";

	QStringList highlightersCSharp;
		highlightersCSharp << "cs";

	QStringList highlightersD;
		highlightersD << "d" << "di";

	QStringList highlightersErlang;
		highlightersErlang << "erl";

	QStringList highlightersFSharp;
		highlightersFSharp << "fs";

	QStringList highlightersHaskell;
		highlightersHaskell << "hs";

	QStringList highlightersHTML;
		highlightersHTML << "htm" << "html" << "cfm" << "chm" << "xml";

	QStringList highlightersJava;
		highlightersJava << "java" << "jar" << "class";

	QStringList highlightersJavaScript;
		highlightersJavaScript << "js";

	QStringList highlightersPerl;
		highlightersPerl << "p" << "pl";

	QStringList highlightersPHP;
		highlightersPHP << "php";

	QStringList highlightersPython;
		highlightersPython << "py" << "pyw";

	QStringList highlightersRuby;
		highlightersRuby << "rb";

	QStringList highlightersSQL;
		highlightersSQL << "sql" << "rsd" << "trn" << "fdb" << "ldf" << "mdf" << "ndf";

	if (highlightersC.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("C"));

	else if (highlightersCaml.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("Caml"));

	else if (highlightersCPP.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("C++"));

	else if (highlightersCSharp.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("C#"));

	else if (highlightersD.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("D"));

	else if (highlightersErlang.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("Erlang"));

	else if (highlightersFSharp.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("F#"));

	else if (highlightersHaskell.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("Haskell"));

	else if (highlightersHTML.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("HTML / (x)HTML"));

	else if (highlightersJava.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("Java"));

	else if (highlightersJavaScript.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("JavaScript"));

	else if (highlightersPerl.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("Perl"));

	else if (highlightersPHP.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("PHP"));

	else if (highlightersPython.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("Python"));

	else if (highlightersRuby.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("Ruby"));

	else if (highlightersSQL.contains(extension))
		colorisation->setCurrentIndex(colorisation->findText("SQL"));

	else
		colorisation->setCurrentIndex(colorisation->findText("Texte brut"));
}

int EditeurDeCode::highlighterIndex(QString highlighterName)
{
	return colorisation->findText(highlighterName);
}

void EditeurDeCode::openContent(QString content, QString title)
{
	if (onglets->count() == 1 && onglets->tabText(onglets->currentIndex()) == "Nouveau document" && pageActuelle()->estEnregistre())
	{
	}

	else
	{
		slotNouveau();
	}

	onglets->setCurrentIndex(onglets->count());

	pageActuelle()->setPlainText(content);

	onglets->setTabText(onglets->currentIndex(), title);
	onglets->setTabIcon(onglets->currentIndex(), QIcon(":/icones/editeur_de_code/non_enregistre.png"));

	pageActuelle()->setEstEnregistre(false);
	pageActuelle()->setSauvegardeOk(true);

	enregistrer->setDisabled(false);

	highlighterFor("html");
}

void EditeurDeCode::setWebBrowser(QMainWindow *browser)
{
	webBrowserPointer = browser;
}

void EditeurDeCode::changementHighlighter(int highlighter)
{
	pageActuelle()->changeHighlighter(highlighter);
	pageActuelle()->setColorisation(highlighter);
}

void EditeurDeCode::sauvegarderEtat()
{
	QSettings enregistrer(Multiuso::appDirPath() + "/ini/editeur_de_code.ini", QSettings::IniFormat);
		enregistrer.setValue("etat_fenetre", saveState());
}

bool EditeurDeCode::tousLesDocumentsEnregistres()
{
	for (int i = 0; i < onglets->count(); i++)
	{
		onglets->setCurrentIndex(i);

		CodeEdit *codeEdit = onglets->currentWidget()->findChild<CodeEdit *>();

		if (!codeEdit->estEnregistre())
			return false;
	}

	return true;
}

void EditeurDeCode::slotChangementOnglet(int onglet)
{
	onglet = 0;

	slotRemettreValeurs();
}

QWidget *EditeurDeCode::nouvelOnglet()
{
	CodeEdit *champDeSaisie = new CodeEdit(this);
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

CodeEdit *EditeurDeCode::pageActuelle()
{
	return onglets->currentWidget()->findChild<CodeEdit *>();
}

QString EditeurDeCode::titreTabCourrant()
{
	return onglets->tabText(onglets->currentIndex());
}

void EditeurDeCode::slotAMRedo(bool available)
{
	retablir->setEnabled(available);
	pageActuelle()->setRedoOk(available);
}

void EditeurDeCode::slotAMUndo(bool available)
{
	annuler->setEnabled(available);
	pageActuelle()->setUndoOk(available);
}

void EditeurDeCode::slotRemettreValeurs()
{
	annuler->setEnabled(pageActuelle()->undoOk());
	retablir->setEnabled(pageActuelle()->redoOk());

	colorisation->setCurrentIndex(pageActuelle()->colorisationCourante());
}


void EditeurDeCode::slotSelectionMajuscule()
{
	QString texte = pageActuelle()->textCursor().selectedText();
		texte = texte.toUpper();

	pageActuelle()->textCursor().removeSelectedText();
	pageActuelle()->textCursor().insertText(texte);
}

void EditeurDeCode::slotSelectionMinuscule()
{
	QString texte = pageActuelle()->textCursor().selectedText();
		texte = texte.toLower();

	pageActuelle()->textCursor().removeSelectedText();
	pageActuelle()->textCursor().insertText(texte);
}

void EditeurDeCode::slotNouveau()
{
	onglets->addTab(nouvelOnglet(), QIcon(":/icones/editeur_de_code/enregistre.png"), "Nouveau document");
	onglets->setCurrentIndex(onglets->count() - 1);
}

void EditeurDeCode::setTextActuel(QString texte)
{
	pageActuelle()->setPlainText(texte);
}

void EditeurDeCode::slotEnregistrer()
{
	QFile fichier(pageActuelle()->fichierOuvert());

	if (fichier.exists() && fichier.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		fichier.write(pageActuelle()->toPlainText().toAscii());

		pageActuelle()->setEstEnregistre(true);
		pageActuelle()->setSauvegardeOk(false);

		enregistrer->setDisabled(true);

		onglets->setTabIcon(onglets->currentIndex(), QIcon(":/icones/editeur_de_code/enregistre.png"));
	}

	else
	{
		slotEnregistrerSous();
	}

	fichier.close();
}

void EditeurDeCode::slotEnregistrerSous()
{
	QString cheminDuFichier = QFileDialog::getSaveFileName(this, "Multiuso", Multiuso::lastPath() + "/Nouveau document.mltstxt", "Texte (*)");

	Multiuso::setLastPath(cheminDuFichier);

	QFile fichier(cheminDuFichier);

	if (fichier.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QFileInfo infosFichier(fichier);

		fichier.write(pageActuelle()->toPlainText().toAscii());

		QString titre = infosFichier.fileName();

		onglets->setTabText(onglets->currentIndex(), titre);
		onglets->setTabIcon(onglets->currentIndex(), QIcon(":/icones/editeur_de_code/enregistre.png"));

		pageActuelle()->setEstEnregistre(true);
		pageActuelle()->setSauvegardeOk(false);
		pageActuelle()->setFichierOuvert(cheminDuFichier);

		enregistrer->setDisabled(true);
	}

	fichier.close();
}

void EditeurDeCode::slotOuvrir()
{
	QString cheminFichier = QFileDialog::getOpenFileName(this, "Multiuso", Multiuso::lastPath(), "Texte (*)");

	Multiuso::setLastPath(cheminFichier);

	if (!cheminFichier.isEmpty())
		slotOuvrirFichier(cheminFichier);
}

void EditeurDeCode::slotOuvrirFichier(QString cheminFichier)
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

		pageActuelle()->setPlainText(fichier.readAll());

		onglets->setTabText(onglets->currentIndex(), infosFichier.fileName());
		onglets->setTabIcon(onglets->currentIndex(), QIcon(":/icones/editeur_de_code/enregistre.png"));

		pageActuelle()->setEstEnregistre(true);
		pageActuelle()->setSauvegardeOk(false);
		pageActuelle()->setFichierOuvert(cheminFichier);

		enregistrer->setDisabled(true);
	}

	fichier.close();

	highlighterFor(QFileInfo(fichier).suffix());
}


void EditeurDeCode::slotAnnuler()
{
	pageActuelle()->undo();
}

void EditeurDeCode::slotRetablir()
{
	pageActuelle()->redo();
}

void EditeurDeCode::slotSupprimerSelection()
{
	pageActuelle()->textCursor().removeSelectedText();
}

void EditeurDeCode::slotRechercher()
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
		d_rechercher->setWindowTitle("Éditeur de code - Rechercher");
		d_rechercher->setWindowIcon(QIcon(":/icones/editeur_de_code/rechercher.png"));

	connect(b_rechercher, SIGNAL(clicked()), this, SLOT(slotRechercher2()));
	connect(b_ok, SIGNAL(clicked()), d_rechercher, SLOT(accept()));

	d_rechercher->exec();
}

void EditeurDeCode::slotRechercher2()
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

void EditeurDeCode::slotRechercherRemplacer()
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
		d_rechercher->setWindowTitle("Éditeur de code - Rechercher/Remplacer");
		d_rechercher->setWindowIcon(QIcon(":/icones/editeur_de_code/rechercherRemplacer.png"));

	connect(b_rechercher, SIGNAL(clicked()), this, SLOT(slotRechercher2()));
	connect(b_remplacer, SIGNAL(clicked()), this, SLOT(slotRechercherRemplacer2()));
	connect(b_ok, SIGNAL(clicked()), d_rechercher, SLOT(accept()));

	d_rechercher->exec();
}

void EditeurDeCode::slotRechercherRemplacer2()
{
	if (pageActuelle()->textCursor().hasSelection())
	{
		pageActuelle()->textCursor().removeSelectedText();
		pageActuelle()->textCursor().insertText(l_remplacer->text());
	}
}

void EditeurDeCode::slotImprimer()
{
	QPrinter *imprimante = new QPrinter(QPrinter::ScreenResolution);
		imprimante->setDocName(titreTabCourrant());
		imprimante->setPaperSize(QPrinter::A4);
		imprimante->setOrientation(QPrinter::Portrait);

	QPrintDialog *dialogueImpression = new QPrintDialog(imprimante, this);
		dialogueImpression->setWindowTitle("Éditeur de code - Imprimer");
		dialogueImpression->setWindowIcon(QIcon(":/icones/editeur_de_code/editeur_de_code.png"));

		if (pageActuelle()->textCursor().hasSelection())
			dialogueImpression->setOption(QAbstractPrintDialog::PrintSelection);

		connect(dialogueImpression, SIGNAL(accepted(QPrinter *)), this, SLOT(imprimerTexte(QPrinter *)));

		dialogueImpression->exec();
}

void EditeurDeCode::imprimerTexte(QPrinter *imprimante)
{
	pageActuelle()->print(imprimante);
}

void EditeurDeCode::slotCopier()
{
	pageActuelle()->copy();
}

void EditeurDeCode::slotCouper()
{
	pageActuelle()->cut();
}

void EditeurDeCode::slotColler()
{
	pageActuelle()->paste();
}

void EditeurDeCode::slotToutSelectionner()
{
	pageActuelle()->selectAll();
}

void EditeurDeCode::slotRepeterTexte()
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

	dialogue->setWindowTitle("Éditeur de code - Répéter du texte");
	dialogue->setWindowIcon(QIcon(":/icones/editeur_de_code/repeterTexte.png"));
	dialogue->resize(600, 260);
	dialogue->setLayout(layoutRepeter);
	dialogue->exec();
}

void EditeurDeCode::slotRepeterTexte2()
{
	slotRepeter(texteARepeter->toPlainText(), nombreDeFois->text().toInt());
}

void EditeurDeCode::slotRepeter(QString texte, int nombreDeFois)
{
	if (nombreDeFois > 950)
	{
		nombreDeFois = 950;

		QMessageBox::warning(this, "Multiuso", "Le nombre de répétitions est limité à 2500, ce texte ne sera donc répété que 950 fois.");
	}

	QFile fichier(pageActuelle()->fichierOuvert());

	for (int i = 0; i < nombreDeFois; i++)
		pageActuelle()->insertPlainText(texte);
}
