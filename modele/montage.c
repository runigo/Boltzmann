
/*
Copyright novembre 2017, Stephan Runigo
runigo@free.fr
SiGP 1.3.4  simulateur de gaz parfait
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

#include "montage.h"

void montageChangeParoiCentrale(montageT * montage, int mode)
		// 0 : Pas de cloison, 1 : Cloison fermée, 2 : Cloison percée, 3 : Maxwell
	{
	(*montage).paroiCentrale = mode;

	//if(mode==1 || mode==-1 )
	if(mode==1)
			(*montage).trou = 0;
	if(mode==2 || mode==-1)
			(*montage).trou = DEMI_TROU;
	if(mode==-2)
			(*montage).trou = (*montage).demiHauteur;
		

	fprintf(stderr, "Paroi centrale = %d, ", (*montage).paroiCentrale);
	fprintf(stderr, "trou = %d\n", (*montage).trou);
	if(mode<0)fprintf(stderr, "Démon de Maxwell activé\n");

	return;
	}

void montageChangeTrou(montageT * montage, float facteur)
	{
	if(facteur<0)
		{
		if(facteur == -1)
			(*montage).trou = DEMI_TROU;
		else
			(*montage).trou = (*montage).demiHauteur;
		}
	else
		{
		if( (*montage).trou > (*montage).demiHauteur && facteur > 1 )
			{
			fprintf(stderr, "Maximum atteint, ");
			}
		else
			{
			if( (*montage).trou > 11 ){
				(*montage).trou = (*montage).trou * facteur;}
			else{
				if( facteur > 1){
					(*montage).trou ++;}
				else{
					if( (*montage).trou >= 1 ){
						(*montage).trou --;}
					else{
						fprintf(stderr, "Minimum atteint, ");}
					}
				}
			}
		}

	fprintf(stderr, "trou = %d\n", (*montage).trou);

	return;
	}

/////////////////////////////////////////////////////
/*
void montageInitialise(montageT * montage)
	{
	thermostatInitialise(&(*montage).thermostat);

	(*montage).largeur = (LARGEUR-MARGE); // Largeur
	(*montage).hauteur = (HAUTEUR_THERMO-MARGE);// Hauteur
	(*montage).demiLargeur = (LARGEUR-MARGE)/2; // Demi largeur
	(*montage).demiHauteur = (HAUTEUR_THERMO-MARGE)/2;// Demi hauteur

	(*montage).trou = DEMI_TROU;// Taille du trou
	//(*montage).epaisseur = 3*TAILLE;// Epaisseur de la cloison

	(*montage).paroiCentrale = 0;// 0 : pas de paroi centrale. 
	(*montage).pDROITE = 2*LARGEUR; // Paroi droite
	(*montage).pINFERIEUR = 2*HAUTEUR_THERMO;// Paroi inférieur

	(*montage).pCENTRE = LARGEUR/2 ;//Paroi centrale
	(*montage).pCENTREdroit = LARGEUR/2 + TAILLE ;//Paroi centrale droite
	(*montage).pCENTREgauche = LARGEUR/2 - TAILLE ;//Paroi centrale gauche
	(*montage).pCENTREdroitDouble = 2*(*montage).pCENTREdroit ;//Paroi centrale droit
	(*montage).pCENTREgaucheDouble = 2*(*montage).pCENTREgauche ;//Paroi centrale gauche

	(*montage).pTROUplus = (HAUTEUR_THERMO+TROU)/2;// Parti bas
	(*montage).pTROUmoins = (HAUTEUR_THERMO-TROU)/2;// Parti haut



	return;
	}

*/
