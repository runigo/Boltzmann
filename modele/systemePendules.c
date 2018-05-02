/*
Copyright mars 2018, Stephan Runigo
runigo@free.fr
SiCP 2.0.1 simulateur de chaîne de pendules
Ce logiciel est un programme informatique servant à simuler l'équation
d'une chaîne de pendules et à en donner une représentation graphique.
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
A cet égard  l'attention de l'utilisateur est attirée sur les risques
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

#include "systemePendules.h"

void systemePendulesInitialisePendule(systemePendulesT * systemePendules);
void systemePendulesInitialiseLimiteInfini(systemePendulesT * systemePendules);
void systemePendulesIncremente(systemePendulesT * systemePendules);
void systemePendulesCouplage(systemePendulesT * systemePendules);
void systemePendulesInertie(systemePendulesT * systemePendules);
void systemePendulesCourantLimite(systemePendulesT * systemePendules);

void systemePendulesJaugeZero(systemePendulesT * systemePendules);

/*--------------------------------------------------------------*/

int systemePendulesCreation(systemePendulesT * systemePendules)
	{
	(*systemePendules).premier = chaineCreation((*systemePendules).nombre);

	systemePendulesInitialisePendule(systemePendules);

	//systemeChangeLimite(systemePendules);

	systemePendulesInitialiseLimiteInfini(systemePendules);

	return 0;
	}

void systemePendulesSuppression(systemePendulesT * systemePendules)
	{
	chaineSupprime(&(*systemePendules).premier);
	printf("Système supprimé\n");
	return;
	}

/*------------------------------------------------------------------*/

void systemePendulesInitialisePosition(systemePendulesT * systemePendules)
	{
	chaineT *iter=(*systemePendules).premier;

	do
		{
		penduleInitialisePosition(&iter->pendule, 0.0, 0.0);
		iter=iter->suivant;
		}
	while(iter != (*systemePendules).premier);

	return;
	}

void systemePendulesInitialisePendule(systemePendulesT * systemePendules)
	{
	float m=(*systemePendules).masse;
	float l=(*systemePendules).longueur;
	float d=(*systemePendules).dissipation;
	float c=(*systemePendules).couplage;
	float g=(*systemePendules).gravitation;
	float t=(*systemePendules).moteurs.dt;
	chaineT *iter=(*systemePendules).premier;

	do
		{
		penduleInitialiseParametre(&iter->pendule, m, l, d);
		penduleInitialiseExterieur(&iter->pendule, c, g, t);
		penduleInitialisePosition(&iter->pendule, 0.0, 0.0);
		penduleInitialiseDephasage(&iter->pendule, 0.0);
		iter=iter->suivant;
		}
	while(iter != (*systemePendules).premier);

	penduleInitialiseDephasage(&(*systemePendules).premier->pendule, (*systemePendules).dephasage);

	return;
	}

void systemePendulesInitialiseLimiteInfini(systemePendulesT * systemePendules)
	{
	int nombre = (*systemePendules).nombre;
	int i=1;

	chaineT *iter=(*systemePendules).premier->precedent;

	for(i=0;i<nombre/6;i++)
		{
		iter->pendule.dissipation = 10*((*systemePendules).dissipation)*(1.1-i/(float)(nombre/6));
		iter=iter->precedent;
		}

	do
		{
		iter->pendule.dissipation = 0.0;
		iter=iter->precedent;
		}
	while(iter!=systemePendules->premier->precedent);

	return;
	}

/*----------------JAUGE ET NORMALISATION-------------------*/

void systemePendulesJaugeZero(systemePendulesT * systemePendules)
	{//	Rapproche la position du premier pendule de zéro
	chaineT *iter;
	iter=(*systemePendules).premier;
	float position = (*systemePendules).premier->pendule.nouveau;
	float jauge;

	jauge = -DEUXPI*(int)(position/DEUXPI);
	if(jauge>PI || jauge<PI)
		{
		do
			{
			penduleJauge(&(iter->pendule), jauge);
			iter=iter->suivant;
			}
		while(iter!=(*systemePendules).premier);
		}

	return;
	}

/*------------------------  ÉVOLUTION TEMPORELLE  -------------------------*/

void systemePendulesEvolution(systemePendulesT * systemePendules, int duree)
	{
	int i;

	//	Fait évoluer le système pendant duree*dt
	for(i=0;i<duree;i++)
		{
		//	Évolution élémentaire
		systemePendulesCouplage(systemePendules);
		systemePendulesInertie(systemePendules);
		systemePendulesIncremente(systemePendules);
		}

	//	Limite la valeur des paramètres croissants
	if((*systemePendules).moteurs.generateur==0)
		{
		//	Rapproche la position du premier pendule de zéro
		systemePendulesJaugeZero(systemePendules);
		}
	else
		{
		//	Rapproche les compteurs des moteurs de zéro
		moteurJaugeZero(&(*systemePendules).moteurs);
		}
	return;
	}

void systemePendulesCouplage(systemePendulesT * systemePendules)
	{//	Calcul les forces de couplage,
	chaineT *iter;
	iter=(*systemePendules).premier;

	do
		{
		penduleCouplage(&(iter->precedent->pendule), &(iter->pendule), &(iter->suivant->pendule));
		iter=iter->suivant;
		}
	while(iter!=(*systemePendules).premier);

	return;
	}

void systemePendulesInertie(systemePendulesT * systemePendules)
	{//	Principe d'inertie appliqué au systemePendules
	float courantJosephson = (*systemePendules).moteurs.josephson;
	float generateur = moteursGenerateur(&(*systemePendules).moteurs);

			//	Cas des extrémitées
			//  0 : periodiques 1 : libres, 2 : fixes, 3 libre-fixe, 4 fixe-libre
		// Cas du premier pendule
	if ((*systemePendules).moteurs.generateur != 0)
		{
		penduleInitialisePosition(&((*systemePendules).premier->pendule), generateur, generateur);
		}
	else
		{
		if ((*systemePendules).libreFixe==0 || (*systemePendules).libreFixe==1 || (*systemePendules).libreFixe==3 )
			{
			penduleInertie(&((*systemePendules).premier->pendule), (*systemePendules).equation, courantJosephson);
			}
		else	//	premier fixe
			{
			penduleInitialisePosition(&((*systemePendules).premier->pendule), 0.0, 0.0);
			}
		}

		// Cas du dernier pendule
	if ((*systemePendules).libreFixe==0 || (*systemePendules).libreFixe==1 || (*systemePendules).libreFixe==4 )
		{
		penduleInertie(&((*systemePendules).premier->precedent->pendule), (*systemePendules).equation, courantJosephson);
		}
	else	//	dernier fixe
		{
		penduleInitialisePosition(&((*systemePendules).premier->precedent->pendule), 0.0, 0.0);
		//penduleInitialisePosition(&((*systemePendules).premier->precedent->pendule), -(*systemePendules).premier->pendule.dephasage, -(*systemePendules).premier->pendule.dephasage);
		}


		// Reste de la chaîne

	chaineT *iter;
	iter = (*systemePendules).premier->suivant;

	do
		{
		penduleInertie(&(iter->pendule), (*systemePendules).equation, courantJosephson);
		iter=iter->suivant;
		}
	while(iter!=(*systemePendules).premier->precedent);


	return;
	}

void systemePendulesIncremente(systemePendulesT * systemePendules)
	{//	incremente l'horloge, l'ancien et l'actuel etat du systemePendules

	//(*systemePendules).moteurs.horloge=(*systemePendules).moteurs.horloge+(*systemePendules).moteurs.dt;
	(*systemePendules).moteurs.chrono=(*systemePendules).moteurs.chrono+(*systemePendules).moteurs.dt;

	chaineT *iter;
	iter=(*systemePendules).premier;

	do
		{
		penduleIncremente(&(iter->pendule));
		iter=iter->suivant;
		}
	while (iter!=(*systemePendules).premier);

	return;
	}

//////////////////////////////////////////////////////////////////////////
