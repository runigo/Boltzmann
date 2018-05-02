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

#ifndef _CONSTANTES_
#define _CONSTANTES_

//#include "unistd.h"

#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include <assert.h>

//#define PI 3.14159265358979323846264338327950288419716939937510582
//#define PI 3.1415926535897932384626433832795
#define PI 3.14159265359
#define DEUXPI 6.28318530718



	//	PARAMÈTRES GRAPHIQUES

#define LARGEUR 958 // Largeur de la fenêtre
#define FENETRE_X LARGEUR // Largeur de la fenêtre
#define HAUTEUR 589 // Hauteur de la fenêtre
#define FENETRE_Y HAUTEUR // Hauteur de la fenêtre

#define BOUTON_COMMANDES 17 // Nombres de petits boutons
#define ROTATIF_COMMANDES 5 // Nombre de boutons rotatifs

#define RATIO_H_L 0.436 // Ratio Hauteur sur Largeur de la chaîne
//#define RATIO_C_X 0.491 // Ratio Centrage X Sans Menu
//#define RATIO_C_Y 0.751 // Ratio Centrage Y Sans Menu
#define RATIO_C_X 0.391 // Ratio Centrage X Avec Menu
#define RATIO_C_Y 0.851 // Ratio Centrage Y Avec Menu
#define RATIO_R_MIN 0.21 // Ratio Distance minimal
#define RATIO_R_MAX 0.99 // Ratio Distance maximal

#define TAILLE 8 // Diamètre des masses



	//	PARAMÈTRES DISCRÉTISATION

#define TEMPS_AFFICHAGE 110 // Durée entre les affichages en ms

#define DUREE_MAX 599 // Nombre maximal d'incrémentation du système entre deux affichages.

#define DT_MIN 0.000003 // Discrétisation minimal du temps
#define DT 0.0003
#define DT_MAX 0.003		//	discrétisation du temps
//#define DT_MIN 0.000000003	//	discrétisation du temps


	//	PARAMÈTRES DE LA CHAINE

#define NOMBRE_MIN 2 // Nombre minimal de pendules
#define NOMBRE_MAX 399 // Nombre maximal de pendules

//#define SOLITON_MAX 19 // Nombre maximal de soliton
#define DEPHASAGE_MAX 49*DEUXPI // Déphasage maximal


	//	PARAMÈTRES PHYSIQUES

#define COUPLAGE_MAX 99 // Couplage maximal / nombre de pendule
#define COUPLAGE_MIN 0.003

#define GRAVITATION_MAX 7777 // Gravitation maximale.
#define GRAVITATION_MIN 0.00007 // Gravitation minimale.

#define MASSE_MAX 7777
#define MASSE_MIN 0.7

#define DISSIPATION_MAX 333	//	Coeficient de frottement visqueux
#define DISSIPATION_MAX_DT 0.00999	//	Valeur maximale
#define DISSIPATION_MIN 0.00003	//	Valeur minimale

#define FREQUENCE_MAX 9123	//	Valeur maximale de la fréquence du moteur
#define FREQUENCE_MIN 0.007	//	Valeur minimale de la fréquence du moteur

#define AMPLITUDE_MAX 91	//	Valeur maximale de l'amplitude du moteur
#define AMPLITUDE_MIN 0.007	//	Valeur minimale de l'amplitude du moteur

#define JOSEPHSON_MAX 91	//	Valeur maximale de l'amplitude du courant josephson
#define JOSEPHSON_MIN 0.00007	//	Valeur minimale de l'amplitude du courant josephson

#endif
