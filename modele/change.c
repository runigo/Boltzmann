/*
Copyright mars 2018, Stephan Runigo
runigo@free.fr
SiCP 1.5  simulateur de chaîne de pendules
Ce logiciel est un programme informatique servant à simuler l'équation
d'une chaîne de pendules et à en donner une représentation graphique.
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
associés au chargement, à l'utilisation, à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe à
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant des connaissances informatiques approfondies. Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.
*/

#include "change.h"

void changeLimite(systemePendulesT * systemePendules);

/*--------------------------------------------------------------*/

void changeCouplage(systemePendulesT * systemePendules, float facteur)
	{// Multiplie le couplage par facteur
	float maximum;
	chaineT *iter=(*systemePendules).premier;

	maximum = (*systemePendules).couplage * facteur / (*systemePendules).nombre;

	if(maximum < COUPLAGE_MAX)
		{
		(*systemePendules).couplage=(*systemePendules).couplage*facteur;
		do
			{
			penduleChangeCouplage(&(iter->pendule), facteur);
			iter=iter->suivant;
			}
		while(iter!=(*systemePendules).premier);
		}
	else
		{
		printf("Maximum du couplage = %6.3f\n", (*systemePendules).couplage);
		}

	printf("Couplage = %6.3f\n", (*systemePendules).couplage);

	return;
	}

void changeGravitation(systemePendulesT * systemePendules, float facteur)
	{// Multiplie la gravitation par facteur
	chaineT *iter=(*systemePendules).premier;
	float gravitation;

	gravitation = (*systemePendules).gravitation * facteur;

	if(gravitation < GRAVITATION_MAX && gravitation > GRAVITATION_MIN)
		{
		(*systemePendules).gravitation = gravitation;
		do
			{
			penduleChangeGravitation(&(iter->pendule), facteur);
			iter=iter->suivant;
			}
		while(iter!=(*systemePendules).premier);
		}
	else
		{
		printf("Maximum de la gravité = %6.3f\n", (*systemePendules).gravitation);
		}

	printf("Gravitation = %6.3f\n", (*systemePendules).gravitation);

	return;
	}

void changeMasse(systemePendulesT * systemePendules, float facteur)
	{// Multiplie la masse par facteur
	chaineT *iter=(*systemePendules).premier;
	float masse;

	masse = (*systemePendules).masse * facteur;

	if(masse < MASSE_MAX && masse > MASSE_MIN)
		{
		(*systemePendules).masse = masse;
		do
			{
			penduleChangeMasse(&(iter->pendule), facteur);
			iter=iter->suivant;
			}
		while(iter!=(*systemePendules).premier);
		}
	else
		{
		printf("Masse limite atteinte\n");
		}

	printf("Masse = %6.3f\n", (*systemePendules).masse);

	return;
	}

void changeDissipation(systemePendulesT * systemePendules, float facteur)
	{// Multiplie la dissipation par facteur
	chaineT *iter;
	iter = (*systemePendules).premier;

	float dissipation = (*systemePendules).dissipation * facteur;
	//float dissipationMaximale = DISSIPATION_MAX_DT/(*systemePendules).moteurs.dt;

	if(dissipation < DISSIPATION_MAX && dissipation > DISSIPATION_MIN)
		{
		if(facteur!=0.0)
			{
			(*systemePendules).dissipation = (*systemePendules).dissipation * facteur;
			}

		do
			{
			penduleChangeDissipation(&(iter->pendule), facteur);
			iter=iter->suivant;
			}
		while(iter!=(*systemePendules).premier);
		}
	else
		{
		printf("dissipation limite atteinte\n");
		}


	printf("Dissipation = %6.3f\n", (*systemePendules).dissipation);

	return;
	}

void changeFormeDissipation(systemePendulesT * systemePendules, int forme)
	{// initialise une dissipation nulle(0), uniforme(1) ou extremite absorbante(2)
	chaineT *iter;
	iter=(*systemePendules).premier;
	float dissipation = 0.99;

	if ( forme == 0 )
		dissipation = 0.0;

	if (  forme == 1 )
		{
		if ( (*systemePendules).dissipation != 0.0 )
			{
			dissipation = (*systemePendules).dissipation;
			}
		else
			{
			(*systemePendules).dissipation = dissipation;
			}
		}

	do
		{
		if ( forme == 2 )
			{
			dissipation = iter->pendule.dissipation * (*systemePendules).dissipation;
			}
		penduleInitialiseAlpha(&(iter->pendule), dissipation, (*systemePendules).moteurs.dt);
		iter=iter->suivant;
		}
	while(iter!=(*systemePendules).premier);

	if ( forme == 2 )
		{
		printf("Dissipation premier= %6.3f\n", iter->pendule.dissipation);
		printf("Dissipation dernier= %6.3f\n", iter->precedent->pendule.dissipation);
		}
	else
		{ // Cas uniforme (forme=1)
		printf("Dissipation = %6.3f\n", dissipation);
		}

	return;
	}

void changeConditionsLimites(systemePendulesT * systemePendules, int libreFixe)
	{
	(*systemePendules).libreFixe=libreFixe;
	printf("libreFixe = %d, ", (*systemePendules).libreFixe);

	switch(libreFixe)
		{
		case 0:
			printf(": conditions aux limites périodiques\n");
			break;
		case 1:
			printf(": extrémités libres\n");
			break;
		case 2:
			printf(": extrémités fixes\n");
			break;
		case 3:
			printf(": premier libre, dernier fixe\n");
			break;
		case 4:
			printf(": premier fixe, dernier libre\n");
			break;
		}

	changeLimite(systemePendules);

	return;
	}

void changeLimite(systemePendulesT * systemePendules)
	{// Change le couplage du dernier pendule
	float couplage=0.0;
	if ((*systemePendules).libreFixe==0 || (*systemePendules).libreFixe==2)
		{
		couplage=(*systemePendules).couplage;
		}

	penduleInitialiseKapa(&(*systemePendules).premier->precedent->pendule, couplage, (*systemePendules).moteurs.dt);

	printf("Couplage dernier = %6.3f\n", couplage);

	return;
	}

void changeDephasage(systemePendulesT * systemePendules, float dephasage)
	{
	float nouveau = (*systemePendules).premier->pendule.dephasage + dephasage;

	if(nouveau>(-DEPHASAGE_MAX) && nouveau<DEPHASAGE_MAX)
		{
		penduleAjouteDephasage(&(*systemePendules).premier->pendule, dephasage);
		}
	else
		{
		printf("dissipation limite atteinte\n");
		}
	
	
	printf("Dephasage premier = %6.3f\n", (*systemePendules).premier->pendule.dephasage);

	return;
	}

//////////////////////////////////////////////////////////////////////////
