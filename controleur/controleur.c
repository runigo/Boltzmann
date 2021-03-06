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

#include "controleur.h"

int controleurEvolution(controleurT * controleur);

int controleurProjection(controleurT * controleur);
int controleurEvolutionSysteme(controleurT * controleur);
int controleurConstructionGraphique(controleurT * controleur);

int controleurTraiteEvenement(controleurT * controleur);

int controleurActionClavier(controleurT * controleur);
int controleurClavierMaj(controleurT * controleur);
int controleurClavierCtrl(controleurT * controleur);

int controleurClavier(controleurT * controleur);
int controleurClavier2(controleurT * controleur);
int controleurClavier3(controleurT * controleur);

int controleurCommandes(controleurT * controleur, int zone);

int controleurSouris(controleurT * controleur);
int controleurDefile(controleurT * controleur);
int controleurDefilePointDeVue(controleurT * controleur);
int controleurDefileCommandes(controleurT * controleur);
void controleurBoutonSouris(controleurT * controleur, int appui);
void controleurAfficheSouris(controleurT * controleur);

void controleurChangeMode(controleurT * controleur);
void controleurChangeVitesse(controleurT * controleur, float facteur);

int controleurDestruction(controleurT * control)
	{


	fprintf(stderr, "Suppression du système\n");
	systemePendulesSuppression(&(*control).systemePendules);

	fprintf(stderr, "Suppression du graphe\n");
	grapheSuppression(&(*control).graphe);

	fprintf(stderr, "Suppression de l'horloge\n");
	horlogeSuppression(&(*control).horloge);

	fprintf(stderr, "Suppression du rendu\n");
	graphiqueDestruction(&(*control).graphique);

	fprintf(stderr, "Suppression de la fenêtre\n");
	interfaceDestruction(&(*control).interface);

	fprintf(stderr, "Sortie de la SDL\n");
	interfaceQuitteSDL();

	return 0;
	}

int controleurSimulationGraphique(controleurT * controleur)
	{
	do	{
			//fprintf(stderr, "Prise en compte des actions clavier\n");
		controleurActionClavier(controleur);
		}
	while((*controleur).sortie == 0);

	return 0;
	}

int controleurActionClavier(controleurT * controleur)
	{
	if (SDL_WaitEvent(&(*controleur).interface.evenement))
		{
		controleurTraiteEvenement(controleur);
		}
	return (*controleur).sortie;
	}

int controleurEvolution(controleurT * controleur)
	{
	//printf("Entrée dans controleurEvolution, SDL_GetTicks() = %d\n",(int)(SDL_GetTicks()));

		//fprintf(stderr, "    Durée entre affichage = %d\n",horlogeChronoDuree(&(*controleur).horloge));
	//horlogeChronoDepart(&(*controleur).horloge);

		//fprintf(stderr, "Projection du système sur la représentation graphique\n");
	controleurProjection(controleur);
		//fprintf(stderr, "    Durée = %d\n",horlogeChronoDuree(&(*controleur).horloge));



	if((*controleur).options.modePause > 0)
		{
		//horlogeChronoDepart(&(*controleur).horloge);
		//fprintf(stderr, "Evolution temporelle du système\n");
		controleurEvolutionSysteme(controleur);
		//fprintf(stderr, "    Durée = %d\n",horlogeChronoDuree(&(*controleur).horloge));
		}

	//horlogeChronoDepart(&(*controleur).horloge);

		//fprintf(stderr, "Mise à jour de la fenêtre graphique\n");
	controleurConstructionGraphique(controleur);
		//fprintf(stderr, "    Durée = %d\n",horlogeChronoDuree(&(*controleur).horloge));


		//projectionChangePsi(&(*controleur).projection, -0.003);

	//fprintf(stderr, "    Durée des évolutions = %d\n",horlogeChronoDuree(&(*controleur).horloge));

	//printf("Sortie de controleurEvolution, SDL_GetTicks() = %d\n",(int)(SDL_GetTicks()));

	return (*controleur).sortie;
	}

int controleurProjection(controleurT * controleur)
	{
	int largeur;
	int hauteur;
	int x, y;

		//void SDL_GetWindowSize(SDL_Window* window, int* w, int* h)
	SDL_GetWindowSize((*controleur).interface.fenetre, &largeur, &hauteur);

		// Réinitialisation des commandes si la fenetre change de taille
	if((*controleur).graphique.largeur!=largeur || (*controleur).graphique.hauteur!=hauteur)
		{
		(*controleur).graphique.largeur=largeur;
		(*controleur).graphique.hauteur=hauteur;
		commandesInitialiseBoutons(&(*controleur).commandes, largeur, hauteur);
		}

		// Réinitialisation des commandes de la souris
	SDL_PumpEvents();
	SDL_GetMouseState(&x,&y);
	commandesInitialiseSouris(&(*controleur).commandes, x, y);

		//fprintf(stderr, "projectionInitialiseLongueurs\n");
	projectionInitialiseLongueurs(&(*controleur).projection, hauteur*RATIO_H_L, largeur, (*controleur).projection.pointDeVue.r);

	projectionSystemeChaineDePendule(&(*controleur).systemePendules, &(*controleur).projection, &(*controleur).graphe);
	projectionSystemeCommandes(&(*controleur).systemePendules, &(*controleur).projection, &(*controleur).commandes);

	return (*controleur).sortie;
	}

int controleurEvolutionSysteme(controleurT * controleur)
	{
		//fprintf(stderr, "Evolution temporelle du système\n");
	systemePendulesEvolution(&(*controleur).systemePendules, (*controleur).options.duree);

	return 0;
	}

int controleurConstructionGraphique(controleurT * controleur)
	{

		//fprintf(stderr, "Nettoyage de l'affichage\n");
	graphiqueNettoyage(&(*controleur).graphique);

		//fprintf(stderr, "Dessin des Commandes\n");
	graphiqueCommandes(&(*controleur).graphique, &(*controleur).commandes);

		//fprintf(stderr, "Dessin des graphes\n");
	if((*controleur).graphe.support==0)
		{
		graphiquePendule(&(*controleur).graphique, &(*controleur).graphe);
		}
	else
		{
		graphiquePenduleSupport(&(*controleur).graphique, &(*controleur).graphe);
		}

		//fprintf(stderr, "Mise à jour de l'affichage\n");
	graphiqueMiseAJour(&(*controleur).graphique);

	return (*controleur).sortie;
	}

int controleurTraiteEvenement(controleurT * controleur)
	{
	int sortie = 0;
	switch((*controleur).interface.evenement.type)
		{
		case SDL_QUIT:
			(*controleur).sortie = 1;break;
		case SDL_MOUSEWHEEL:
			controleurDefile(controleur);break;
		case SDL_MOUSEMOTION:
			controleurSouris(controleur);break;
		case SDL_MOUSEBUTTONDOWN:
			controleurBoutonSouris(controleur, 1);break;
		case SDL_MOUSEBUTTONUP:
			controleurBoutonSouris(controleur, 0);break;
		case SDL_USEREVENT:
			controleurEvolution(controleur);break;
	  case SDL_KEYDOWN:
			{
			if ((((*controleur).interface.evenement.key.keysym.mod & KMOD_LSHIFT) == KMOD_LSHIFT)
			|| (((*controleur).interface.evenement.key.keysym.mod & KMOD_RSHIFT) == KMOD_RSHIFT))
				{
				sortie = controleurClavierMaj(controleur);break;
				}
			else
				{
				if ((((*controleur).interface.evenement.key.keysym.mod & KMOD_LCTRL) == KMOD_LCTRL)
				|| (((*controleur).interface.evenement.key.keysym.mod & KMOD_RCTRL) == KMOD_RCTRL))
					{
					sortie = controleurClavierCtrl(controleur);break;
					}
				else
					{	//	1 : commande de la chaîne, 2 : Graphisme, 3 : Sauvegarde
					if((*controleur).modeClavier == 3)
						{
						sortie = controleurClavier3(controleur);break;
						}
					if((*controleur).modeClavier == 2)
						{
						sortie = controleurClavier2(controleur);break;
						}
					else
						{
						sortie = controleurClavier(controleur);break;
						}
					}
				}
			}
		default:
			;
		}
	if(sortie!=0) (*controleur).sortie = 1;
	return (*controleur).sortie;
	}

void controleurChangeMode(controleurT * controleur)
	{
	(*controleur).options.modePause=-(*controleur).options.modePause;

	return;
	}

void controleurChangeVitesse(controleurT * controleur, float facteur)
	{
	if( (*controleur).options.duree > 11 )
		{
		(*controleur).options.duree = (*controleur).options.duree * facteur;
		}
	else
		{
		if( facteur > 1)
			{
			(*controleur).options.duree ++;
			}
		else
			{
			if( (*controleur).options.duree > 1 )
				{
				(*controleur).options.duree --;
				}
			else
				{
				fprintf(stderr, "duree minimale atteinte, ");
				}
			}
		}

	if( (*controleur).options.duree > DUREE_MAX)
		{
		fprintf(stderr, "duree maximale atteinte, ");
		(*controleur).options.duree = DUREE_MAX;
		}
	fprintf(stderr, "duree = %d\n", (*controleur).options.duree);
	return;
	}

int controleurClavier(controleurT * controleur)
	{
	switch ((*controleur).interface.evenement.key.keysym.sym)
		{
	// Mode : évolution du système en pause
		case SDLK_RETURN:
			controleurChangeMode(controleur);break;
		//case SDLK_BACKSPACE:
			//controleurChangeMode(controleur);break;

	// Vitesse de la simulation
		case SDLK_KP_PLUS:
			controleurChangeVitesse(controleur, 1.1);break;
		case SDLK_KP_MINUS:
			controleurChangeVitesse(controleur, 0.91);break;
		case SDLK_F9:
			controleurChangeVitesse(controleur, 0.32);break;
		case SDLK_F10:
			controleurChangeVitesse(controleur, 0.91);break;
		case SDLK_F11:
			controleurChangeVitesse(controleur, 1.1);break;
		case SDLK_F12:
			controleurChangeVitesse(controleur, 3.1);break;

	// Conditions aux limites
		case SDLK_y:
			changeDephasage(&(*controleur).systemePendules, 2*PI);break;
		case SDLK_h:
			changeDephasage(&(*controleur).systemePendules, -2*PI);break;
		case SDLK_w:
			changeConditionsLimites(&(*controleur).systemePendules, 0); // periodiques
			break;
		case SDLK_x:
			changeConditionsLimites(&(*controleur).systemePendules, 1); // libres
			break;
		case SDLK_c:
			changeConditionsLimites(&(*controleur).systemePendules, 2); // fixes
			break;
		case SDLK_b:
			fprintf(stderr, "Commande désactivée depuis SiCP 1.4.1");
			//changeConditionsLimites(&(*controleur).systemePendules, 3); // libre fixe
			break;
		case SDLK_n:
			changeConditionsLimites(&(*controleur).systemePendules, 3); // fixe libre
			break;


	// Dissipation
		case SDLK_e:
			changeDissipation(&(*controleur).systemePendules, 0.77);break;
		case SDLK_d:
			changeDissipation(&(*controleur).systemePendules, 1.3);break;
		case SDLK_r:
			changeFormeDissipation(&(*controleur).systemePendules, 0);break;
		case SDLK_f:
			changeFormeDissipation(&(*controleur).systemePendules, 1);break;
		case SDLK_v:
			changeFormeDissipation(&(*controleur).systemePendules, 2);break;

	// Couplage
		case SDLK_a:
			changeCouplage(&(*controleur).systemePendules, 1.1);break;
		case SDLK_q:
			changeCouplage(&(*controleur).systemePendules, 0.91);break;

	// Masse
		case SDLK_z:
			changeMasse(&(*controleur).systemePendules, 1.7);break;
		case SDLK_s:
			changeMasse(&(*controleur).systemePendules, 0.6);break;

	// Gravitation
		case SDLK_t:
			changeGravitation(&(*controleur).systemePendules, 1.3);break;
		case SDLK_g:
			changeGravitation(&(*controleur).systemePendules, 0.77);break;

	// Moteur jonction Josephson
		case SDLK_UP:
			moteursChangeJosephson(&(*controleur).systemePendules.moteurs,1.1);break;
		case SDLK_DOWN:
			moteursChangeJosephson(&(*controleur).systemePendules.moteurs,0.91);break;
		case SDLK_LEFT:
			moteursChangeJosephson(&(*controleur).systemePendules.moteurs,-1.0);break;
		case SDLK_RIGHT:
			moteursChangeJosephson(&(*controleur).systemePendules.moteurs,0.0);break;

	// Moteur générateur de signaux
		case SDLK_p:
			moteursChangeFrequence(&(*controleur).systemePendules.moteurs,1.1);break;
		case SDLK_m:
			moteursChangeFrequence(&(*controleur).systemePendules.moteurs,0.91);break;
		case SDLK_u:
			moteursChangeAmplitude(&(*controleur).systemePendules.moteurs,1.1);break;
		case SDLK_j:
			moteursChangeAmplitude(&(*controleur).systemePendules.moteurs,0.91);break;
		case SDLK_o:
			moteursChangeGenerateur(&(*controleur).systemePendules.moteurs, -1);break;
		case SDLK_i:
			moteursChangeGenerateur(&(*controleur).systemePendules.moteurs, 3);break;
		case SDLK_l:
			moteursChangeGenerateur(&(*controleur).systemePendules.moteurs, 2);break;


	// Choix de la simulation
/*
		case SDLK_F1: // Pendules
			(*controleur).systemePendules.equation = 1;break;
		case SDLK_F2: // Harmoniques
			(*controleur).systemePendules.equation = 2;break;
		case SDLK_F3: // Corde
			(*controleur).systemePendules.equation = 3;break;
*/
  // Afficher les observables

		case SDLK_F4: // Corde asymétrique
			controleurAfficheSouris(controleur);
			break;
		case SDLK_F5:
			observablesAfficheEnergie(&(*controleur).systemePendules);
			break;
		case SDLK_F6:
			moteursAfficheHorloge(&(*controleur).systemePendules.moteurs);
			break;
		case SDLK_F7:
			projectionAffichePointDeVue(&(*controleur).projection);
			break;
	// Support
		case SDLK_F8:
			grapheChangeSupport(&(*controleur).graphe);break;


		default:
			;
		}
	return (*controleur).sortie;
	}

int controleurClavier2(controleurT * controleur)
	{
	switch ((*controleur).interface.evenement.key.keysym.sym)
		{
	// Mode : évolution du système en pause
		case SDLK_RETURN:
			controleurChangeMode(controleur);break;
		//case SDLK_BACKSPACE:
			//controleurChangeMode(controleur);break;

	// Déplacement du point de vue
		case SDLK_a:
			projectionChangePhi(&(*controleur).projection, 0.01);break;
		case SDLK_z:
			projectionChangePhi(&(*controleur).projection, 0.01);break;
		case SDLK_e:
			projectionChangePhi(&(*controleur).projection, 0.09);break;
		case SDLK_r:
			projectionChangePhi(&(*controleur).projection, 0.09);break;

		case SDLK_q:
			projectionChangePsi(&(*controleur).projection, -0.09);break;
		case SDLK_s:
			projectionChangePsi(&(*controleur).projection, -0.01);break;
		case SDLK_d:
			projectionChangePsi(&(*controleur).projection, 0.01);break;
		case SDLK_f:
			projectionChangePsi(&(*controleur).projection, 0.09);break;

		case SDLK_w:
			projectionChangePhi(&(*controleur).projection, -0.01);break;
		case SDLK_x:
			projectionChangePhi(&(*controleur).projection, -0.01);break;
		case SDLK_c:
			projectionChangePhi(&(*controleur).projection, -0.09);break;
		case SDLK_v:
			projectionChangePhi(&(*controleur).projection, -0.09);break;

		default:
			;
		}
	return (*controleur).sortie;
	}

int controleurClavier3(controleurT * controleur)
	{
	switch ((*controleur).interface.evenement.key.keysym.sym)
		{

	   		 // Mode : évolution du système en pause
		case SDLK_RETURN:
			controleurChangeMode(controleur);break;
		//case SDLK_BACKSPACE:
			//controleurChangeMode(controleur);break;


			// Réinitialisation du système
		case SDLK_a:
			fprintf(stderr, "Réinitialisation du système\n");
			systemePendulesInitialisePosition(&(*controleur).systemePendules);break;

		default:
			;
		}
	return (*controleur).sortie;
	}

int controleurClavierMaj(controleurT * controleur)
	{
	switch ((*controleur).interface.evenement.key.keysym.sym)
		{

	// Sortie

		case SDLK_ESCAPE:
			(*controleur).sortie = 1;break;

    // Mode : évolution du système en pause

		case SDLK_RETURN:
			controleurChangeMode(controleur);break;
		//case SDLK_BACKSPACE:
			//controleurChangeMode(controleur);break;


	// Réinitialisation du système
		// Lecture des fichier
		case SDLK_a:
			fprintf(stderr, "Réinitialisation du système\n");
			systemePendulesInitialisePosition(&(*controleur).systemePendules);break;

		default:
			;
		}
	return (*controleur).sortie;
	}

int controleurClavierCtrl(controleurT * controleur)
	{
	switch ((*controleur).interface.evenement.key.keysym.sym)
		{
	// Sortie
		case SDLK_ESCAPE:
			(*controleur).sortie = 1;break;
	// Mode : évolution du système en pause
		case SDLK_RETURN:
			controleurChangeMode(controleur);break;
		case SDLK_BACKSPACE:
			controleurChangeMode(controleur);break;

	// Choix du modeClavier
		case SDLK_F1: // Commande de la chaîne
			(*controleur).modeClavier = 1;break;
		case SDLK_F2: // Graphisme
			(*controleur).modeClavier = 2;break;
		case SDLK_F3: // Sauvegarde
			(*controleur).modeClavier = 3;break;
		case SDLK_F4: // 
			(*controleur).modeClavier = 4;break;

		default:
			;
		}
	return (*controleur).sortie;
	}

int controleurCommandes(controleurT * controleur, int zone)
	{
	int commande;
	if(zone==2)
		{
		commande = commandeBoutons(&(*controleur).commandes);
		switch(commande)	//	
			{
			case 0:
				changeConditionsLimites(&(*controleur).systemePendules, 0);break; // 32	Périodique
			case 1:
				changeConditionsLimites(&(*controleur).systemePendules, 1);break; // 62	Libre
			case 2:
				changeConditionsLimites(&(*controleur).systemePendules, 2);break; // 88 	Fixe
			case 3:
				changeConditionsLimites(&(*controleur).systemePendules, 4);break; // 115	Mixte
			case 4:
				changeFormeDissipation(&(*controleur).systemePendules, 1);break; // 167	Uniforme
			case 5:
				changeFormeDissipation(&(*controleur).systemePendules, 0);break; // 198	Nulle
			case 6:
				changeFormeDissipation(&(*controleur).systemePendules, 2);break; // 230	Extrémité
			case 7:
				moteursChangeJosephson(&(*controleur).systemePendules.moteurs,0.0);break; // 284	Marche
			case 8:
				moteursChangeJosephson(&(*controleur).systemePendules.moteurs,0.0);break; // 311	Arrêt
			case 9:
				moteursChangeJosephson(&(*controleur).systemePendules.moteurs,-1.0);break; // 339	Droite
			case 10:
				moteursChangeJosephson(&(*controleur).systemePendules.moteurs,-1.0);break; // 367	Gauche
			case 11:
				moteursChangeGenerateur(&(*controleur).systemePendules.moteurs, 0);break; // 421	Arrêt
			case 12:
				moteursChangeGenerateur(&(*controleur).systemePendules.moteurs, 1);break; // 449	Sinus
			case 13:
				moteursChangeGenerateur(&(*controleur).systemePendules.moteurs, 2);break; // 481	Carré
			case 14:
				moteursChangeGenerateur(&(*controleur).systemePendules.moteurs, 3);break; // 509	Impulsion
			case 15:
				changeDephasage(&(*controleur).systemePendules, 2*PI);break; // 536	Fluxon
			case 16:
				changeDephasage(&(*controleur).systemePendules, -2*PI);break; // 563	Anti F.
			default:
				;
			}
		}
	return 0;
	}

int controleurSouris(controleurT * controleur)
	{
	float x, y;
	if((*controleur).appui==1)
		{
			//fprintf(stderr, "controleurSouris xrel = %d\n", (*controleur).interface.evenement.motion.xrel);
		x=-0.0031*(float)((*controleur).interface.evenement.motion.xrel);
		y=0.0031*(float)((*controleur).interface.evenement.motion.yrel);
			//fprintf(stderr, "controleurSouris yrel = %d\n", (*controleur).interface.evenement.motion.yrel);
			//fprintf(stderr, "controleurSouris yrel = %d\n", (*controleur).interface.evenement.motion.yrel);
		projectionChangePsi(&(*controleur).projection, x);
		projectionChangePhi(&(*controleur).projection, y);
		}
	return (*controleur).sortie;
	}

int controleurDefileCommandes(controleurT * controleur)
	{
	int commande;
	commande = commandeRotatifs(&(*controleur).commandes);

	if((*controleur).interface.evenement.wheel.y > 0) // scroll up
		{
		switch(commande)
			{
			case 0:
				changeCouplage(&(*controleur).systemePendules, 1.1);break;
			case 1:
				changeDissipation(&(*controleur).systemePendules, 1.1);break;
			case 2:
				moteursChangeJosephson(&(*controleur).systemePendules.moteurs, 1.1);break;
			case 3:
				moteursChangeAmplitude(&(*controleur).systemePendules.moteurs, 1.1);break;
			case 4:
				moteursChangeFrequence(&(*controleur).systemePendules.moteurs, 1.1);break;
			default:
				;
			}
		}
	else if((*controleur).interface.evenement.wheel.y < 0) // scroll down
		{
		switch(commande)	
			{
			case 0:
				changeCouplage(&(*controleur).systemePendules, 0.91);break;
			case 1:
				changeDissipation(&(*controleur).systemePendules, 0.91);break;
			case 2:
				moteursChangeJosephson(&(*controleur).systemePendules.moteurs, 0.91);break;
			case 3:
				moteursChangeAmplitude(&(*controleur).systemePendules.moteurs, 0.91);break;
			case 4:
				moteursChangeFrequence(&(*controleur).systemePendules.moteurs, 0.91);break;
			default:
				;
			}
		}

	return 0;
	}

int controleurDefile(controleurT * controleur)
	{
	if((*controleur).commandes.sourisX>(*controleur).commandes.rotatifs)
		{
		controleurDefileCommandes(controleur);
		}
	else
		{
		controleurDefilePointDeVue(controleur);
		}
	return 0;
	}

int controleurDefilePointDeVue(controleurT * controleur)
	{
	if((*controleur).interface.evenement.wheel.y > 0) // scroll up
		{
		(*controleur).projection.pointDeVue.r += 0.011;
		//fprintf(stderr, "evenement.wheel.y = %d\n", (*controleur).interface.evenement.wheel.y);
		//fprintf(stderr, "Distance = %f\n", (*controleur).projection.pointDeVue.r);
		}
	else if((*controleur).interface.evenement.wheel.y < 0) // scroll down
		{
		(*controleur).projection.pointDeVue.r -= 0.011;
		//fprintf(stderr, "evenement.wheel.y = %d\n", (*controleur).interface.evenement.wheel.y);
		//fprintf(stderr, "Distance = %f\n", (*controleur).projection.pointDeVue.r);
		}

	if((*controleur).projection.pointDeVue.r < RATIO_R_MIN)
		{
		(*controleur).projection.pointDeVue.r = RATIO_R_MIN;
		fprintf(stderr, "Distance limite = %f\n", (*controleur).projection.pointDeVue.r);
		}
	if((*controleur).projection.pointDeVue.r > RATIO_R_MAX)
		{
		(*controleur).projection.pointDeVue.r = RATIO_R_MAX;
		fprintf(stderr, "Distance limite = %f\n", (*controleur).projection.pointDeVue.r);
		}

	//if(event.wheel.x > 0) // scroll right{}
	//else if(event.wheel.x < 0) // scroll left{}

	projectionChangePsi(&(*controleur).projection, 0);
	projectionChangePhi(&(*controleur).projection, 0);

	return 0;
	}

void controleurBoutonSouris(controleurT * controleur, int appui)
	{

	(*controleur).appui=appui;
	
	if(appui==1)
		{
		if((*controleur).commandes.sourisX>(*controleur).commandes.rotatifs)
			{
			if((*controleur).commandes.sourisX>(*controleur).commandes.boutons)
				{
				controleurCommandes(controleur, 2);
				}
			else
				{
				controleurCommandes(controleur, 1);
				}
			}
		else
			{
			controleurCommandes(controleur, 0);
			}
		}
	return;
	}

void controleurAfficheSouris(controleurT * controleur)
	{
	fprintf(stderr, "(*controleur).graphique.hauteur = %d\n", (*controleur).graphique.hauteur);
	fprintf(stderr, "(*controleur).commandes.sourisY = %d\n", (*controleur).commandes.sourisY);
	fprintf(stderr, "(*controleur).graphique.largeur = %d\n", (*controleur).graphique.largeur);
	fprintf(stderr, "(*controleur).commandes.sourisX = %d\n", (*controleur).commandes.sourisX);

	return ;
	}

//////////////////////////////////////////////////////////////////////////////////////
