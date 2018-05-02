
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

#include "thermostat.h"

void thermostatChangeTemperatureMoyenne(thermostatT * thermostat, float facteur);
void thermostatChangeSymetrie(thermostatT * thermostat, int uniforme);
void thermostatChangeTemperatureMoyenne(thermostatT * thermostat, float facteur);
void thermostatChangeTemperatureGauche(thermostatT * thermostat, float facteur);
void thermostatChangeTemperatureDroite(thermostatT * thermostat, float facteur);

void thermostatAfficheTemperature(thermostatT * thermostat);

void thermostatInitialise(thermostatT * thermostat)

			//	Initialisation du thermostat
	{
	
	(*thermostat).temperature = 7.0;//	Température moyenne du thermostat
	(*thermostat).gauche = 0.0071;	//	Température gauche
	(*thermostat).droite = 1111.0;	//	Température droite
	(*thermostat).actif = 0;	//	0 : Système isolé, 1:température uniforme, 2:active gauche-droite
	return;
	}

void thermostatChangeEtat(thermostatT * thermostat, int mode)
	{
	//		Change l'activité des thermostats

	(*thermostat).actif = mode;

	switch( (*thermostat).actif )
		{
		case 0:
			printf("(*thermostat).actif = %d : système isolé\n", (*thermostat).actif);break;
		case 1:
			printf("(*thermostat).actif = %d : thermostat uniforme\n", (*thermostat).actif);break;
		case 2:
			printf("(*thermostat).actif = %d : thermostats gauche-droite\n", (*thermostat).actif);break;
		default:
			(*thermostat).actif = 0;
			fprintf(stderr, "Err : thermostatChangeSymetrie, (*thermostat).actif = %d \n", (*thermostat).actif);break;
		}
	return;
	}

void thermostatChangeTemperature(thermostatT * thermostat, float facteur)

			//	Change température moyenne
	{
	float temperature = (*thermostat).temperature * facteur;
	if (temperature < TEMPERATURE_MAX && temperature > TEMPERATURE_MIN)
		{
		(*thermostat).temperature = (*thermostat).temperature * facteur;
		printf("Température = %f\n", (*thermostat).temperature);
		}
	else
		{
		printf("Température limite atteinte\n");
		printf("Température = %f\n", (*thermostat).temperature);
		}
	return;
	}
void thermostatChangeTemperatureDroite(thermostatT * thermostat, float facteur)

			//	Change température droite
	{
	float temperature = (*thermostat).droite * facteur;
	if (temperature < TEMPERATURE_MAX && temperature > TEMPERATURE_MIN)
		{
		(*thermostat).droite = temperature ;
		printf("Température droite = %f\n", (*thermostat).droite);
		}
	else
		{
		printf("Température limite atteinte\n");
		printf("Température droite = %f\n", (*thermostat).droite);
		}
	return;
	}
void thermostatChangeTemperatureGauche(thermostatT * thermostat, float facteur)

			//	Change température gauche
	{
	float temperature = (*thermostat).gauche * facteur;
	if (temperature < TEMPERATURE_MAX && temperature > TEMPERATURE_MIN)
		{
		(*thermostat).gauche = temperature;
		printf("Température gauche = %f\n", (*thermostat).gauche);
		}
	else
		{
		printf("Température limite atteinte\n");
		printf("Température gauche = %f\n", (*thermostat).gauche);
		}
	return;
	}

void thermostatInverseTemperature(thermostatT * thermostat)
	{
	float temperature;
	temperature = (*thermostat).droite;
	(*thermostat).droite = (*thermostat).gauche;
	(*thermostat).gauche = temperature;
	printf("(*thermostat).gauche = %f\n", (*thermostat).gauche);
	printf("(*thermostat).droite = %f\n", (*thermostat).droite);
	return;
	}

void thermostatAfficheTemperature(thermostatT * thermostat)

			// 	Affiche la valeur des paramètres du thermostat
	{
	printf("(*thermostat).temperature = %f\n", (*thermostat).temperature);
	printf("(*thermostat).gauche = %f\n", (*thermostat).gauche);
	printf("(*thermostat).droite = %f\n", (*thermostat).droite);
	return;
	}

void thermostatAfficheThermostat(thermostatT * thermostat)

			// 	Affiche la valeur des paramètres du thermostat
	{
	printf("\nÉTATS DES THERMOSTATS\n");
	switch( (*thermostat).actif )
		{
		case 0:
			printf("	(*thermostat).actif = %d : système isolé\n", (*thermostat).actif);break;
		case 1:
			printf("	(*thermostat).actif = %d : thermostat uniforme\n", (*thermostat).actif);
			printf("	Température indicative = %f\n", (*thermostat).temperature);
			break;
		case 2:
			printf("	(*thermostat).actif = %d : thermostats gauche-droite\n", (*thermostat).actif);
			printf("	Température indicative à gauche = %f\n", (*thermostat).gauche);
			printf("	Température indicative à droite = %f\n", (*thermostat).droite);
			break;
		default:
			break;
		}
	return;
	}

//////////////////////////////////////////////////////////////////////////
