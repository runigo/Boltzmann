
/*
Copyright février 2018, Stephan Runigo
runigo@free.fr
SiGP 1.3.7  simulateur de gaz parfait
Ce logiciel est un programme informatique servant à simuler un gaz parfait
et à en donner une représentation graphique. Il permet d'observer une détente
de Joule ainsi que des transferts thermiques avec des thermostats.
Ce logiciel est régi par la licence CeCILL soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
sur le site "http://www.cecill.info".
En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme, le
titulaire des droits patrimoniaux et les concédants successifs.
A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe à
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies. Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.
*/

#include "systemeThermo.h"

void systemeThermoIncremente(systemeThermoT * systemeThermo); // L'ancien état reçoit l'état actuel qui reçoit le nouvel état.
void systemeThermoMoteur(systemeThermoT * systemeThermo); // Calcule le nouvel état du système en fonction de l'ancien et de l'actuel

void systemeThermoParoi(systemeThermoT * systemeThermo);
void systemeThermoInertie(systemeThermoT * systemeThermo);
void systemeThermoChocsParticules(systemeThermoT * systemeThermo);

void systemeThermoInitialisePosition(systemeThermoT * systemeThermo)
	{
		// Initialisation des mobiles
	int i;
	for(i=0;i<NOMBRE_PARTICULES;i++)
		{
		if(i%2==0)
			{
			mobileInitialise(&(*systemeThermo).mobile[i], &(*systemeThermo).montage, (*systemeThermo).vitesseInitiale, i);
			}
		else		// Possibilité vitesse = 0 pour la moitié des particules
			{
			mobileInitialise(&(*systemeThermo).mobile[i], &(*systemeThermo).montage, (*systemeThermo).vitesseInitiale, i);
			//mobileInitialise(&(*systemeThermo).mobile[i], &(*systemeThermo).montage, 0, i);
			}
		}
	return;
	}

void systemeThermoInitialise(systemeThermoT * systemeThermo, int taille, float vitesse)
	{
		// Taille des particules
	(*systemeThermo).diametre = taille;

		// Conditions initiales
	(*systemeThermo).vitesseInitiale = vitesse;

		// Initialisation des mobiles
	systemeThermoInitialisePosition(systemeThermo);

	return;
	}

void systemeThermoEvolue(systemeThermoT * systemeThermo, int duree)
	{		//  Évolution du système
	int i;
		//fprintf(stderr, "Evolution temporelle du systemeThermo \n");
	for (i=0;i<duree;i++)
		{
			//fprintf(stderr, "systemeThermoMoteur, calcul nouvel état\n");
		systemeThermoMoteur(systemeThermo);
		}
	return;
	}

void systemeThermoMoteur(systemeThermoT * systemeThermo)
/*
	Calcule le nouvel état du système en fonction de l'actuel et du nouveau
*/
	{
		//fprintf(stderr, "systemeThermoIncremente, évolution ancien état\n");
	systemeThermoIncremente(systemeThermo); // Fait disparaitre l'ancien

		//fprintf(stderr, "systemeThermoInertie, Propagation inertielle\n");
	systemeThermoInertie(systemeThermo); // Calcul un nouveau

		//fprintf(stderr, "systemeThermoChocsParticules, Chocs entre particules\n");
	systemeThermoChocsParticules(systemeThermo); // Corrige les coordonnées

		//fprintf(stderr, "systemeThermoParoi, Chocs avec les parois\n");
	systemeThermoParoi(systemeThermo); // Corrige les coordonnées

	return;
	}

void systemeThermoInertie(systemeThermoT * systemeThermo)
/*
		Propagation inertielle
*/
	{
	int i;

	for(i=0;i<NOMBRE_PARTICULES;i++)			//	Application du principe d'inertie
		{
		mobileInertie(&(*systemeThermo).mobile[i]);
		}
	return;
	}

void systemeThermoParoi(systemeThermoT * systemeThermo)
/*
		Chocs avec les parois
*/
	{
	int i;

	for(i=0;i<NOMBRE_PARTICULES;i++)			//	Chocs avec les parois
		{
		mobileParoi(&(*systemeThermo).mobile[i], &(*systemeThermo).montage);
		}
	return;
	}

void systemeThermoChocsParticules(systemeThermoT * systemeThermo)
/*
		Chocs entre particules
*/
	{
	int i, j;

	for(i=0;i<NOMBRE_PARTICULES;i++)		//	Réinitialisation
		{
		(*systemeThermo).mobile[i].collision = 0;
		}

	for(i=0;i<NOMBRE_PARTICULES;i++)			//	Chocs entre particules
		{
		for(j=i+1;j<NOMBRE_PARTICULES;j++)
			{
				{
				//fprintf(stderr, "mobileCollision entre i et j %d %d\n", i, j);
				mobileCollision(&(*systemeThermo).mobile[i], &(*systemeThermo).mobile[j]);
				}
			}
		}

	return;
	}

void systemeThermoIncremente(systemeThermoT * systemeThermo)
	//	L'ancien état reçoit l'état actuel,
	//	l'état actuel reçoit le nouvel état.
	{
	int i;
	for(i=0;i<NOMBRE_PARTICULES;i++)
		{
		mobileIncremente(&(*systemeThermo).mobile[i]);
		}
	return;
	}
/*
*/

void systemeThermoChangeCloison(systemeThermoT * systemeThermo, int mode)
/*
		Changement de la paroi centrale
*/
	{
	int i;

	if(mode==0)	//	Suppression de la cloison
		{
		for(i=0;i<NOMBRE_PARTICULES;i++)
			{
			(*systemeThermo).mobile[i].droite = 1;
			}
		}
	else		//	Réinitialisation droite-gauche
		{
		for(i=0;i<NOMBRE_PARTICULES;i++)
			{
			if((*systemeThermo).mobile[i].nouveau.x > 0)
				(*systemeThermo).mobile[i].droite = 1;
			else
				(*systemeThermo).mobile[i].droite = 0;
			}
		}

		// Changement du montage
	montageChangeParoiCentrale(&(*systemeThermo).montage, mode);

	return;
	}

void systemeThermoChangeDiametre(systemeThermoT * systemeThermo, float facteur)
/*
		Changement de la surface efficace des particules
*/
	{
	int i;
	int diametre = 1;
	float diamCarre = 0.71;

	//(*mobile).diametre=TAILLE;
	//(*mobile).diamCarre=TAILLE*TAILLE*0.71;

	if( (*systemeThermo).diametre > TAILLE_PARTICULE_MAX && facteur > 1 )
		{
		fprintf(stderr, "Diamètre maximal atteint. ");
		}
	else
		{
		if( facteur > 1)
			{
			(*systemeThermo).diametre ++;
			}
		else
			{
			if( (*systemeThermo).diametre > 1 )
				{
				(*systemeThermo).diametre --;
				}
			else
				{
				fprintf(stderr, "Diamètre minimal atteint. ");
				}
			}
		}

	diametre = (*systemeThermo).diametre;
	diamCarre = diametre*diametre*0.71;

		//	Réinitialisation surface efficace
	for(i=0;i<NOMBRE_PARTICULES;i++)
		{
		(*systemeThermo).mobile[i].diametre = diametre;
		(*systemeThermo).mobile[i].diamCarre = diamCarre;
		}

	printf("Diamètre des particules = %d\n", diametre);
	return;
	}

////////////////////////////////////////////////////////////////////////////////

