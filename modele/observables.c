/*
Copyright novembre 2017, Stephan Runigo
runigo@free.fr
SiCP 1.4.1 simulateur de chaîne de pendules
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

#include "observables.h"

double observablesEnergieCinetiquePendule(penduleT * pendule, float dt)
	{
	double vitesse = (*pendule).actuel - (*pendule).ancien;

	return (0.5 * (*pendule).masse * (*pendule).longueur * (*pendule).longueur * vitesse * vitesse / dt / dt);
	}

double observablesEnergieGravitationPendule(penduleT * pendule, float dt)
	{	//(*pendule).gamma est négatif et gamma = dt.dt.g/l
	double h=(*pendule).longueur*(1.0-cos((*pendule).actuel));

	return ( - (*pendule).masse * (*pendule).gamma * h / dt / dt);
	}

double observablesEnergieHarmoniquePendule(penduleT * pendule, float dt)
	{	//	k(ressort) = masse.raideur.longueur
	double h = (*pendule).longueur * (*pendule).actuel * (*pendule).actuel;
	return ( - 0.5 * (*pendule).masse * (*pendule).gamma * h / dt / dt);
	}

double observablesEnergieCouplagePendule(penduleT * pendule, penduleT * suivant, float dt)
	{	//(*pendule).kapa est négatif
	double x, y;
	x=(*pendule).actuel;
	y=(*suivant).actuel - (*pendule).dephasage;
	return ( - 0.5 * (*pendule).longueur * (*pendule).longueur * (*pendule).masse * (*pendule).kapa *  (x-y) * (x-y) / dt / dt);
	}

double observablesEnergieCinetiqueSystemePendules(systemePendulesT * systemePendules)
	{
	chaineT *iter=(*systemePendules).premier;
	double ectotal=0.0;
	do
		{
		ectotal=ectotal+observablesEnergieCinetiquePendule(&(*iter).pendule, (*systemePendules).moteurs.dt);
		iter=iter->suivant;
		}
	while(iter!=(*systemePendules).premier);
	return ectotal;
	}

double observablesEnergieCouplageSystemePendules(systemePendulesT * systemePendules)
	{
	chaineT *iter=(*systemePendules).premier;
	double epctotal=0.0;
	do
		{
		epctotal=epctotal + observablesEnergieCouplagePendule(&(*iter).pendule, &(*iter->suivant).pendule, (*systemePendules).moteurs.dt);
		iter=iter->suivant;
		}
	while(iter!=(*systemePendules).premier);
	return epctotal;
	}

double observablesEnergieGravitationSystemePendules(systemePendulesT * systemePendules)
	{
	chaineT *iter=(*systemePendules).premier;
	double epptotal=0.0;
	do
		{
		epptotal=epptotal + observablesEnergieGravitationPendule(&(*iter).pendule, (*systemePendules).moteurs.dt);
		iter=iter->suivant;
		}
	while(iter!=(*systemePendules).premier);
	return epptotal;
	}

double observablesEnergieHarmoniqueSystemePendules(systemePendulesT * systemePendules)
	{
	chaineT *iter=(*systemePendules).premier;
	double ephtotal=0.0;
	do
		{
		ephtotal=ephtotal + observablesEnergieHarmoniquePendule(&(*iter).pendule, (*systemePendules).moteurs.dt);
		iter=iter->suivant;
		}
	while(iter!=(*systemePendules).premier);
	return ephtotal;
	}

double observablesEnergieCinetiqueSystemeCorde(systemeCordeT * systeme)
	{
	int i;
	double ectotal=0.0;
	float dt = (*systeme).moteur.dt;
	for(i=0;i<N;i++)
		{
		ectotal=ectotal+observablesEnergieCinetiquePendule(&((*systeme).pendule[i]), dt);
		}
	return ectotal;
	}

double observablesEnergieCouplageSystemeCorde(systemeCordeT * systeme)
	{
	int i;
	double ectotal=0.0;
	float dt = (*systeme).moteur.dt;
	for(i=1;i<N;i++)
		{
		ectotal=ectotal+observablesEnergieCouplagePendule(&((*systeme).pendule[i-1]), &((*systeme).pendule[i]), dt);
		}
	if((*systeme).libreFixe==0)
		ectotal=ectotal+observablesEnergieCouplagePendule(&((*systeme).pendule[N-1]), &((*systeme).pendule[0]), dt);
	return ectotal;
	}

double observablesEnergieGravitationSystemeCorde(systemeCordeT * systeme)
	{
	int i;
	double ectotal=0.0;
	float dt = (*systeme).moteur.dt;
	for(i=0;i<N;i++)
		{
		ectotal=ectotal+observablesEnergieGravitationPendule(&((*systeme).pendule[i]), dt);
		}
	return ectotal;
	}

double observablesEnergieHarmoniqueSystemeCorde(systemeCordeT * systeme)
	{
	int i;
	double ectotal=0.0;
	float dt = (*systeme).moteur.dt;
	for(i=0;i<N;i++)
		{
		ectotal=ectotal+observablesEnergieHarmoniquePendule(&((*systeme).pendule[i]), dt);
		}
	return ectotal;
	}

void  observablesAfficheEnergie(systemePendulesT * systemePendules)
	{
	double eCouplage, eGravitation, eCinetique;

		//printf("\nCalcul de l'énergie cinétique\n");
	eCinetique=observablesEnergieCinetiqueSystemePendules(systemePendules);
	printf("\n  Energie cinétique   %12.9f\n", eCinetique);

		//printf("Calcul de l'énergie de couplage\n");
	eCouplage=observablesEnergieCouplageSystemePendules(systemePendules);
	printf("  Energie de couplage  %12.9f\n", eCouplage);

		//	Pendule=1, Harmonique=2, Corde=3, Dioptre=4
	if((*systemePendules).equation == 1)
		{
		//printf("Calcul de l'énergie de gravitation\n");
		eGravitation=observablesEnergieGravitationSystemePendules(systemePendules);
		printf("  Energie de gravitation  %12.9f\n", eGravitation);
		}

	if((*systemePendules).equation == 2)
		{
		//printf("Calcul de l'énergie de raideur harmonique\n");
		eGravitation=observablesEnergieHarmoniqueSystemePendules(systemePendules);
		printf("  Energie  de raideur harmonique  %12.9f\n", eGravitation);
		}

	if((*systemePendules).equation == 3 || (*systemePendules).equation == 4)
		{
		//printf("Calcul de l'énergie de raideur harmonique\n");
		eGravitation=0.0;
		}

	printf("\nEnergie totale      %12.9f\n\n", eCouplage + eGravitation + eCinetique);
	return;
	}
///////////////////////////////////////////////////////////////////
