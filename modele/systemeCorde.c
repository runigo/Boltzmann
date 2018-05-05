/*
Copyright février 2018, Stephan Runigo
runigo@free.fr
SiCF 1.2.1  simulateur de corde vibrante et spectre
Ce logiciel est un programme informatique servant à simuler l'équation
d'une corde vibrante, à calculer sa transformée de fourier, et à donner
une représentation graphique de ces fonctions. 
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

#include "systemeCorde.h"

void systemeCordeInitialisePendul(systemeCordeT * systemeCorde);
void systemeCordeCouplage(systemeCordeT * systemeCorde);
void systemeCordeInertie(systemeCordeT * systemeCorde);
void systemeCordeIncremente(systemeCordeT * systemeCorde);
void systemeCordeInitialiseLimiteInfini(systemeCordeT * systemeCorde);
void systemeCordeChangeLimite(systemeCordeT * systemeCorde);
void systemeCordeFormeDioptre(systemeCordeT * systemeCorde, float facteur);
float systemeCordeMoteur(systemeCordeT * systemeCorde);

// normalise le premier pendule
void systemeCordeJaugeZero(systemeCordeT * systemeCorde);
// normalise la moyenne du système à zéro
void systemeCordeJauge(systemeCordeT * systemeCorde);
double systemeCordeMoyenne(systemeCordeT * systemeCorde);



/*----------------JAUGE ET NORMALISATION-------------------*/

// normalise le premier pendule
void systemeCordeJaugeZero(systemeCordeT * systemeCorde)
	{
	float position = (*systemeCorde).pendule[0].nouveau;
	float jauge;

	//fprintf(stderr, "\nEntrée systemeCordeJaugeZero, position = %f\n", position);
	//fprintf(stderr, "\nEntrée systemeCordeJaugeZero, position/DEUXPI = %f\n", position/DEUXPI);

	jauge = -DEUXPI*(int)(position/DEUXPI);
	if(jauge>PI || jauge<PI)
		{
		int i;
		for(i=1;i<N;i++)
			{
			penduleJauge(&(*systemeCorde).pendule[i], jauge);
			}
		}


	//fprintf(stderr, "Moyenne = %f\n", systemeCordeMoyenne(systemeCorde));
	//fprintf(stderr, "systemeCordeJaugeZero, sortie\n\n");
	return;
	}


// normalise la moyenne du système à zéro
void systemeCordeJauge(systemeCordeT * systemeCorde)
	{
	double moyenne = systemeCordeMoyenne(systemeCorde);
	if(moyenne>(MOYENNE_MAX))
		{
		int i;
		for(i=0;i<N;i++)
			{
			penduleJauge(&(*systemeCorde).pendule[i], -MOYENNE_MAX);
			}
		fprintf(stderr, "Moyenne = %f\n", moyenne);
		}
	if(moyenne<(-MOYENNE_MAX))
		{
		int i;
		for(i=0;i<N;i++)
			{
			penduleJauge(&(*systemeCorde).pendule[i], MOYENNE_MAX);
			}
		fprintf(stderr, "Moyenne = %f\n", moyenne);
		}
	return;
	}

double systemeCordeMoyenne(systemeCordeT * systemeCorde)
	{
	double moyenne = 0.0;

	int i;
	for(i=0;i<N;i++)
		{
		moyenne = moyenne + (*systemeCorde).pendule[i].nouveau;
		}

	return ( moyenne / N );
	}

void systemeCordeInitialise(systemeCordeT * systemeCorde)
	{
	// Initialisation des pendules
	systemeCordeInitialisePendul(systemeCorde);

	// Initialise les conditions aux limites
	//systemeCordeChangeLimite(systemeCorde);

	// Extrémité absorbante
	systemeCordeInitialiseLimiteInfini(systemeCorde);
/*
	// Dissipation initiale Corde
	if((*systemeCorde).equation == 3 || (*systemeCorde).equation == 4)
		{
		changeFormeDissipation(systemeCorde, 2);
		}
*/

	// Masse initiale Dioptre
	if((*systemeCorde).equation == 4)
		{
	int i;
			for(i=N/2;i<N;i++)
				{
				penduleReinitialiseMasse(&(*systemeCorde).pendule[i], (*systemeCorde).masseDroite, (*systemeCorde).moteur.dt);
				}
		}
	//printf("Systeme initialisé\n");

	return;
	}

void systemeCordeEvolution(systemeCordeT * systemeCorde, int duree)
	{//	Fait évoluer le système pendant duree * dt
	int i;
	for(i=0;i<duree;i++)
		{
		systemeCordeCouplage(systemeCorde);
		systemeCordeInertie(systemeCorde);
		systemeCordeIncremente(systemeCorde);
		}
	systemeCordeJaugeZero(systemeCorde);
	return;
	}

void systemeCordeInitialisePendul(systemeCordeT * systemeCorde)
	{
	float m=(*systemeCorde).masseGauche;
	float l=(*systemeCorde).longueur;
	float d=(*systemeCorde).dissipation;
	float c=(*systemeCorde).couplage;
	float g=(*systemeCorde).gravitation;
	float t=(*systemeCorde).moteur.dt;

	int i;

	for(i=0;i<N;i++)
		{
		penduleInitialiseParametre(&(*systemeCorde).pendule[i], m, l, d);
		penduleInitialiseExterieur(&(*systemeCorde).pendule[i], c, g, t);
		penduleInitialisePosition(&(*systemeCorde).pendule[i], 0.0, 0.0);
		penduleInitialiseDephasage(&(*systemeCorde).pendule[i], 0.0);
		}

	penduleInitialiseDephasage(&(*systemeCorde).pendule[0], (*systemeCorde).dephasage);

	return;
	}

void systemeCordeInitialiseLimiteInfini(systemeCordeT * systemeCorde)
	{
	int i;

	for(i=0;i<N;i++)
		{
		(*systemeCorde).pendule[i].dissipation = 0.0;
		}

	for(i=1;i<N/6;i++)
		{
		(*systemeCorde).pendule[N-i].dissipation = 10*(1.0-i/(float)(N/6));
		}


	return;
	}

void systemeCordeCouplage(systemeCordeT * systemeCorde)
	{//	Calcul des forces de couplage

  // SiCF-1.0 inaugure penduleCouplage(m0, m0, m1)
  // nécessitant un dephasage nul et traitant l'extrémité libre
	int libreFixe = (*systemeCorde).libreFixe;

	int i;
	for(i=1;i<(N-1);i++)
		{
		penduleCouplage(&(*systemeCorde).pendule[i-1], &(*systemeCorde).pendule[i], &(*systemeCorde).pendule[i+1]);
		}

  if(libreFixe!=0)
    {
    //penduleInitialiseDephasage(&(*systemeCorde).pendule[1], 0.0);
  	penduleCouplage(&(*systemeCorde).pendule[0], &(*systemeCorde).pendule[0], &(*systemeCorde).pendule[1]);
	  penduleCouplage(&(*systemeCorde).pendule[N-2], &(*systemeCorde).pendule[N-1], &(*systemeCorde).pendule[N-1]);
	  }
  else // Limites périodiques
    {
  	penduleCouplage(&(*systemeCorde).pendule[N-1], &(*systemeCorde).pendule[0], &(*systemeCorde).pendule[1]);
	  penduleCouplage(&(*systemeCorde).pendule[N-2], &(*systemeCorde).pendule[N-1], &(*systemeCorde).pendule[0]);
	  }

	return;
	}

void systemeCordeInertie(systemeCordeT * systemeCorde)
	{//	Principe d'inertie applique au système

  // Version SiCF-1.0, extrémité moteur fixe.

	int equation = (*systemeCorde).equation;
	int libreFixe = (*systemeCorde).libreFixe;
	int etatMoteur = (*systemeCorde).moteur.generateur;
	float courantJosephson = (*systemeCorde).moteur.josephson;

	float moteur = moteursGenerateur(&(*systemeCorde).moteur);

				// Cas du premier et du dernier pendule

			//  0 : periodiques 1 : libres, 2 : fixes, 3 libre-fixe, 4 fixe-libre

	if (libreFixe==0 || libreFixe==1 || libreFixe==3)
		{
		penduleInertie(&(*systemeCorde).pendule[0], equation, courantJosephson);
		}
	else
		{
		penduleInitialisePosition(&(*systemeCorde).pendule[0], 0.0, 0.0);
		}
	if (libreFixe==0 || libreFixe==1 || libreFixe==4)
		{
		penduleInertie(&(*systemeCorde).pendule[N-1], equation, courantJosephson);
		}
	else
		{
		penduleInitialisePosition(&(*systemeCorde).pendule[N-1], (*systemeCorde).pendule[N-1].dephasage, (*systemeCorde).pendule[N-1].dephasage);
		}

	if(etatMoteur!=0) // moteur allumé
		{
		penduleInitialisePosition(&(*systemeCorde).pendule[0], moteur, moteur);
		}
	/*else // SiCF-1.0 : extrémité moteur fixe
		{
		penduleInitialisePosition(&(*systemeCorde).pendule[0], 0, 0);
		}*/

						// Cas des autres pendules
	int i;
	for(i=1;i<(N-1);i++)
		{
		penduleInertie(&((*systemeCorde).pendule[i]), (*systemeCorde).equation, courantJosephson);
		}
	return;
	}

void systemeCordeIncremente(systemeCordeT * systemeCorde)
	{//	incremente l'horloge, l'ancien et l'actuel etat du système

	//(*systemeCorde).moteur.horloge = (*systemeCorde).moteur.horloge + (*systemeCorde).moteur.dt;
	(*systemeCorde).moteur.chrono = (*systemeCorde).moteur.chrono + (*systemeCorde).moteur.dt;

	int i;
	for(i=0;i<N;i++)
		{
		penduleIncremente(&((*systemeCorde).pendule[i]));
		}

	return;
	}

void systemeCordeAffiche(systemeCordeT * systemeCorde)
	{// Affichage de la position et des parametres
	printf("\nParamètres système\n");
	printf("	Couplage entre les pendules	%4.3f\n", (*systemeCorde).couplage);
	printf("	Longueur des pendules		%4.3f\n",(*systemeCorde).longueur);
	printf("	Intensité de la gravitation	%4.3f\n",(*systemeCorde).gravitation);
	printf("	Masse des pendules à droite : %4.3f, à gauche : %4.3f\n",(*systemeCorde).masseDroite,(*systemeCorde).masseGauche);
	printf("	Masse des pendules \n");
	/*	int equation;		//	Pendule=1, Harmonique=2, Corde=3, Dioptre=4
		float dephasage;	//	déphasage entre les limites
		int libreFixe;		// 0 : périodiques 1 : libres, 2 : fixes, 
	*/					//		3 libre-fixe, 4 fixe-libre
	return;
	}

//////////////////////////////////////////////////////////////////////
