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

#include "APropos.h"

APropos::APropos(QWidget *parent = 0) : QDialog(parent)
{
	setWindowTitle("À propos de Multiuso");
	setWindowIcon(QIcon(":/icones/actions/actionAPropos.png"));

	QLabel *contenuPrincipal = new QLabel;
		contenuPrincipal->setText("<center>"
					"<p>"
						"<img src=':/icones/application/iconeApplication_162x128.png'/>"
					"</p>"
					"<h1>Multiuso " + QCoreApplication::applicationVersion() + "</h1>"
					"<p>"
						"Un programme à tout faire :<br />"
						"Du bloc-notes au calendrier, en passant par le<br />"
						"compteur de lignes et l'agenda, Multiuso<br />"
						"est le programme à avoir constamment à portée de main."
					"</p>"
					"<p>"
						"Copyright © 2009-2010 Quentin RICHERT"
					"</p>"
					"<p>"
						"<a href='" + QCoreApplication::organizationDomain() + "'>" + QCoreApplication::organizationDomain() + "</a>"
					"</p>"
				"</center>");

		contenuPrincipal->setOpenExternalLinks(true);

	contributorsModel = new QStandardItemModel;
		populateModel(contributorsModel);

	contributorsContent = new QTreeView;
		contributorsContent->setHeaderHidden(true);
		contributorsContent->setModel(contributorsModel);

	stackedContenu = new QStackedWidget;
		stackedContenu->addWidget(contenuPrincipal);
		stackedContenu->addWidget(contributorsContent);
		stackedContenu->setCurrentIndex(0);

	QPushButton *licence = new QPushButton("&Licence");
		connect(licence, SIGNAL(clicked()), this, SLOT(slotLicence()));

	QPushButton *contributeurs = new QPushButton("&Contributeurs →");
		connect(contributeurs, SIGNAL(clicked()), this, SLOT(slotSwitchView()));

	QPushButton *upstream = new QPushButton("← &Upstream");
		connect(upstream, SIGNAL(clicked()), this, SLOT(slotSwitchView()));

	stackedBoutons = new QStackedWidget;
		stackedBoutons->addWidget(contributeurs);
		stackedBoutons->addWidget(upstream);
		stackedBoutons->setCurrentIndex(0);

	QGridLayout *layoutBoutons = new QGridLayout;
		layoutBoutons->addWidget(stackedBoutons, 0, 0, 1, 2);
		layoutBoutons->addWidget(new QWidget, 0, 2, 1, 1);
		layoutBoutons->addWidget(licence, 0, 3, 1, 1);

	QGridLayout *layoutAPropos = new QGridLayout(this);
		layoutAPropos->addWidget(stackedContenu, 0, 0, 1, 2);
		layoutAPropos->addWidget(new QLabel("<hr />"), 1, 0, 1, 2);
		layoutAPropos->addLayout(layoutBoutons, 2, 0, 1, 2);

	// <Easter Egg>

	QShortcut *easterEgg = new QShortcut(QKeySequence("Ctrl+Alt+Y"), this);
		connect(easterEgg, SIGNAL(activated()), this, SLOT(slotEasterEgg()));

	// </Easter Egg>
}

void APropos::populateModel(QStandardItemModel *model)
{
	/*
	StandardItem *itemAmauryFender = new StandardItem("Amaury FENDER");
		itemAmauryFender->appendRow(new StandardItem("Rédaction du manuel"));
			model->appendRow(itemAmauryFender);
	(Pour une prochaine version plus "finie")
	*/

	StandardItem *itemAleksandraWolska = new StandardItem("Aleksandra Wolska (www.olawolska.com)");
		itemAleksandraWolska->appendRow(new StandardItem("Icônes \"49 hand drawing icons\""));
			model->appendRow(itemAleksandraWolska);
			
	StandardItem *itemAlessandroRei = new StandardItem("Alessandro Rei (www.mentalrey.it)");
		itemAlessandroRei->appendRow(new StandardItem("Icônes \"Dark Glass\""));
			model->appendRow(itemAlessandroRei);

	StandardItem *itemAndroidIcons = new StandardItem("Android icons (www.androidicons.com)");
		itemAndroidIcons->appendRow(new StandardItem("Icônes \"Android\""));
			model->appendRow(itemAndroidIcons);

	StandardItem *itemThemeAmbianceUbuntu = new StandardItem("Thème « Ambiance » d'Ubuntu");
		itemThemeAmbianceUbuntu->appendRow(new StandardItem("Icônes de suppression dans le Speed dial"));
			model->appendRow(itemThemeAmbianceUbuntu);

	StandardItem *itemEveraldoCoelho = new StandardItem("Everaldo COELHO (www.everaldo.com)");
		itemEveraldoCoelho->appendRow(new StandardItem("Icônes \"Crystal\""));
			model->appendRow(itemEveraldoCoelho);

	StandardItem *itemQuentinRichert = new StandardItem("Quentin RICHERT");
		itemQuentinRichert->appendRow(new StandardItem("Chef de projet (upstream)"));
			model->appendRow(itemQuentinRichert);

	StandardItem *itemShydowSdZ = new StandardItem("Shydow (www.siteduzero.com)");
		itemShydowSdZ->appendRow(new StandardItem("Highlighter ActionScript"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter ASM"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter AutoIT"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Befunge"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Brainfuck"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Caml"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter C#"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter CSS"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter D"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Erlang"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter F#"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Fortran"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter GLSL"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Haskell"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter HTML / (x)HTML / XML (CDATA)"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter INI"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Intercal"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Io"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Java"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter JavaScript"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Lex et Yacc"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Lisp"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Lua"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Miranda"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Pascal"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Perl"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter PHP"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Python"));
		itemShydowSdZ->appendRow(new StandardItem("Highlighter Ruby"));
			model->appendRow(itemShydowSdZ);

	StandardItem *itemStalkerSdZ = new StandardItem("Stalker (www.siteduzero.com)");
		itemStalkerSdZ->appendRow(new StandardItem("Gestion des serveurs proxy"));
			model->appendRow(itemStalkerSdZ);

	StandardItem *itemXavierXavi91 = new StandardItem("Xavier (Xavi91)");
		itemXavierXavi91->appendRow(new StandardItem("Icône principale"));
			model->appendRow(itemXavierXavi91);
}

void APropos::slotSwitchView()
{
	int index = stackedBoutons->currentIndex();

	if (index == 0)
		index = 1;

	else if (index == 1)
		index = 0;

	stackedContenu->setCurrentIndex(index);
	stackedBoutons->setCurrentIndex(index);
}

void APropos::slotLicence()
{
	QDialog dialogueLicence(this);
		dialogueLicence.setWindowTitle("À propos de Multiuso - Licence");
		dialogueLicence.setWindowIcon(QIcon(":/icones/actions/actionAPropos.png"));
		dialogueLicence.resize(550, 350);

	QTextEdit *texteLicence = new QTextEdit;
		texteLicence->setPlainText("Copyright © 2009-2010 Quentin RICHERT\n"
				"\n"
				"Multiuso is free software: you can redistribute it and/or modify\n"
				"it under the terms of the GNU General Public License as published by\n"
				"the Free Software Foundation, either version 3 of the License, or\n"
				"(at your option) any later version.\n"
				"\n"
				"Multiuso is distributed in the hope that it will be useful,\n"
				"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
				"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
				"GNU General Public License for more details.\n"
				"\n"
				"You should have received a copy of the GNU General Public License\n"
				"along with Multiuso.  If not, see <http://www.gnu.org/licenses/>.");
		texteLicence->setReadOnly(true);

	QVBoxLayout *layoutLicence = new QVBoxLayout(&dialogueLicence);
		layoutLicence->addWidget(texteLicence);
		layoutLicence->setContentsMargins(0, 0, 0, 0);

	dialogueLicence.exec();
}

// <Easter Egg>

void APropos::slotEasterEgg()
{
	static int passages = 0;

	passages++;

	if (passages == 1)
	{
		std::cout << "Désolé, il n'y a pas d'easter egg ici ;p\n";
	}

	else if (passages == 2)
	{
		std::cout << "Attention, je vais m'énerver, vous êtes prévenu !\n";
	}

	else if (passages == 3)
	{
		std::cout << "Puisque je vous dis qu'il n'y a pas d'easter egg ici ! :@\n";
	}

	else if (passages == 4)
	{
		std::cout << "Bon ok, vous avez gagné !\nEntrez un nombre pour voir : ";
			int nombreEntre;
			std::cin >> nombreEntre;

		if (nombreEntre == 42)
		{
			std::cout << "          „\n  _¸˛_¸˛_||_¸˛_¸˛_\n (////////////////)\n"
				<< " (- - - - - - - - )\n (////////////////)\n--------------------\n"
				<< " THE CAKE IS A LIE\n\n";
		}

		else
		{
			if (nombreEntre <= 9)
				std::cout << "\nJe n'aime pas ce chiffre !\nAvez-vous lu les astuces O.o ?\n";

			else
				std::cout << "\nJe n'aime pas ce nombre !\nAvez-vous lu les astuces O.o ?\n";

			passages = 7;
		}
	}

	else if (passages == 5)
	{
		std::cout << "C'est bon, vous l'avez vu maintenant ! Allez, du vent !\n";
	}

	else if (passages == 6)
	{
		std::cout << "Encore vous --' Bon ben je fais grève...\n";
	}

	else
	{
		std::cout << "/!\\ EN GRÈVE /!\\\n";
	}
}

// </Easter Egg>
