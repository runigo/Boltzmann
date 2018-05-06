/*
Copyright mai 2018, Stephan Runigo
runigo@free.fr
Boltzmann 1.0 simulateur pour les sciences physiques
Ce logiciel est un programme informatique servant à simuler différents 
phénomènes physiques et à en donner une représentation graphique.
Ce logiciel est régi par la licence CeCILL soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
sur le site "http://www.cecill.info".
En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme, le
titulaire des droits patrimoniaux et les concédants successifs.
A cet égard l'attention de l'utilisateur est attirée sur les risques
associés au chargement, à l'utilisation, à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe à
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant des connaissances informatiques approfondies. Les
utilisateurs sont donc invités à charger et tester l'adéquation du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.
*/

#include "donnees.h"

	//		Valeurs initiales des données physiques du simulateur,
	//						et création du système

	//		Valeur initiale et création du graphe

	//		Initialisation de la SDL


int donneesSysteme(systemePendulesT * systemePendules, optionsT * options);
int donneesGraphe(grapheT * graphe, optionsT * options);

int donneesControleur(controleurT * controleur)
	{

	(*controleur).sortie = 0;	// Sortie de SiCP si <> 0
	(*controleur).appui = 0;	// Appuie sur la souris
	(*controleur).modeClavier = 1;	//	1 : commande de la chaîne, 2 : Graphisme, 3 : Sauvegarde
	(*controleur).modeMenu = (*controleur).options.modeMenu;		// 0 : Menu, 1 SiCP, 2 SiGP

		fprintf(stderr, " Initialisation du système\n");
	donneesSysteme(&(*controleur).systemePendules, &(*controleur).options);

		fprintf(stderr, " Création du système\n");
	systemePendulesCreation(&(*controleur).systemePendules);
	changeFormeDissipation(&(*controleur).systemePendules, 1);
		fprintf(stderr, " Initialisation du graphe\n");
	donneesGraphe(&(*controleur).graphe, &(*controleur).options);

		fprintf(stderr, " Création du graphe\n");
	grapheCreation(&(*controleur).graphe, (*controleur).options.nombre);


		//fprintf(stderr, " Initialisation de la projection\n");
		//fprintf(stderr, "projectionInitialiseCouleurs\n");
	projectionInitialiseCouleurs(&(*controleur).projection, 222, 111, 222, 255);// r, v, b, fond
		//fprintf(stderr, "projectionInitialiseLongueurs\n");
	projectionInitialiseLongueurs(&(*controleur).projection, FENETRE_Y/3, FENETRE_X*0.7, 0.57);// hauteur, largeur, ratio de distance
		//fprintf(stderr, "projectionInitialisePointDeVue\n");
	projectionInitialisePointDeVue(&(*controleur).projection, PI/2 - 0.27, PI/2 + 0.21);//r=facteur de distance, psi, phi
	//projectionInitialisePointDeVue(&(*controleur).projection, 3*FENETRE_X, 0.0, 0.0);//r, psi, phi

		fprintf(stderr, " Initialisation SDL\n");
	interfaceInitialisationSDL();
		//fprintf(stderr, " Création de l'interface SDL\n");
	interfaceInitialisation(&(*controleur).interface);
		//fprintf(stderr, " Création du rendu\n");
	graphiqueInitialisation(&(*controleur).graphique, &(*controleur).interface, TAILLE_MASSE, (*controleur).options.fond);

	int largeur;
	int hauteur;
	int x, y;
		fprintf(stderr, " Initialisation des commmandes\n");
	SDL_GetWindowSize((*controleur).interface.fenetre, &largeur, &hauteur);
	(*controleur).graphique.largeur=largeur;
	(*controleur).graphique.hauteur=hauteur;
	commandesInitialiseBoutons(&(*controleur).commandes, largeur, hauteur);

	SDL_PumpEvents();
	SDL_GetMouseState(&x,&y);
	commandesInitialiseSouris(&(*controleur).commandes, x, y);

		fprintf(stderr, " Initialisation horloge SDL\n");
	horlogeCreation(&(*controleur).horloge);

	return 0;
	}

int donneesOptions(optionsT * options)
	{
		// Préréglage des valeurs optionnelles

	(*options).modeDemo = 1;		// 0 : SiCP, 1 Graphique démo, 2 Commande démo
	(*options).modeClavier = 1;		// 0 : SiCP, 1 Graphique démo, 2 Commande démo
	(*options).modeMenu = 0;		// 0 : SiCP, 1 Graphique démo, 2 Commande démo

	(*options).modePause = 1;		// avec ou sans attente
	(*options).duree = 91;		// 100 : temps réèl.
	(*options).fond=240;		// couleur du fond de l'affichage

			// OPTIONS SiCP
	(*options).dt=0.0003;		// discrétisation du temps
							// 25 images par seconde, SDL_Delay(30);
							// dt*duree = 0.004
	(*options).soliton=3;
	(*options).support=1;		// Support de la chaîne
	(*options).nombre=133;		// nombre de pendule
	(*options).equation=1;		// 1 : pendule, 2 : linéarisation,
							//	 3 : corde, 4 : dioptre

			// OPTIONS SiGP
	(*options).temperature = 8.9;	// Température initiale
	(*options).gauche = 0.00333;	// Température thermostat gauche
	(*options).droite = 33.3;	// Température thermostat droit

	(*options).thermostat=0;	// 0 : système isolé, 1 : système thermostaté.
	(*options).cloison = 1;		// 0 : pas de paroi centrale. 1 : détente de joule, 2 : cloison fermé,
					//	 -1, -2 : démon de maxwell.
	(*options).trou = DEMI_TROU;

	return 0;
	}

int donneesSysteme(systemePendulesT * systemePendules, optionsT * options)
	{

		// Initialisation du moteurs

	(*systemePendules).moteurs.dt = (*options).dt;	// discrétisation du temps

	(*systemePendules).moteurs.chrono = 0.0;

	(*systemePendules).moteurs.courant=3.0;		// Mémoire courant Josephson si = 0
	(*systemePendules).moteurs.josephson=-3*(*options).dt*(*options).dt;

	(*systemePendules).moteurs.generateur = 0;	// éteint, sinus, carre, impulsion
	(*systemePendules).moteurs.amplitude=0.3;		// Amplitude du générateur de signaux
	(*systemePendules).moteurs.frequence=5.0;	// Fréquence du générateur de signaux
	(*systemePendules).moteurs.phi=0;

		// Caractéristique de la chaîne

	(*systemePendules).libreFixe = 0;	// 0 periodique, 1 libre, 2 fixe
	(*systemePendules).nombre = (*options).nombre;		// nombre de pendule
	(*systemePendules).equation = (*options).equation;	// 1 : pendule pesant, 2 : linéarisation
												// 3 : corde, 4 : dioptre

		// Paramètres physiques

	(*systemePendules).gravitation = 9.81;
	(*systemePendules).masse = 1.0;
	(*systemePendules).longueur = 9.81/4/PI/PI; // = 25 cm => période = 1 s
	(*systemePendules).dissipation = 0.17;
	(*systemePendules).couplage = 11.1 * (*systemePendules).nombre;
	(*systemePendules).dephasage = (*options).soliton * 2 * PI;

/*
	if((*systemePendules).equation == 3 || (*systemePendules).equation == 4)
		{		 // donneeCorde;
		(*systemePendules).couplage = (*systemePendules).couplage * 10.0;
		(*systemePendules).gravitation = 0.0;
		(*systemePendules).libreFixe = 2;
		(*systemePendules).moteurs.josephson=0.0;
		}
*/
	return 0;
	}

int donneesGraphe(grapheT * graphe, optionsT * options)
	{
	(*graphe).rouge=20;
	(*graphe).bleu=150;
	(*graphe).vert=200;
	(*graphe).fond=(*options).fond;	//	Couleur du fond
	(*graphe).support=(*options).support;	//	Couleur du fond
	(*graphe).dessous = 0;	// Vue de dessous
	(*graphe).gauche = 0;		// Vue de gauche
	return 0;
	}

//////////////////////////////////////////////////////////////////////////
