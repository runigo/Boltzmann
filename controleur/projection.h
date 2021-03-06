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

#ifndef _PROJECTION_
#define _PROJECTION_

#include "../modele/systemePendules.h"
#include "../graphique/graphe.h"
#include "../graphique/commandes.h"

typedef struct ProjectionT projectionT;
	struct ProjectionT
		{
		// Support fixe
		vecteurT support[14];

		// Position observateur
		vecteurT pointDeVue;
		// Vecteurs perpendiculaires
		vecteurT vecteurPhi;
		vecteurT vecteurPsi;

		int rouge;
		int vert;
		int bleu;
		int fond;

		int hauteur;
		int largeur;

		float perspective;
		};

int projectionInitialiseCouleurs(projectionT * projection, int r, int v, int b, int fond);
int projectionInitialiseLongueurs(projectionT * projection, int hauteur, int largeur, float  perspective);
int projectionInitialisePointDeVue(projectionT * projection, float psi, float phi);

int projectionSystemeChaineDePendule(systemePendulesT * systemePendules, projectionT * projection, grapheT * graphe);
int projectionSystemeCommandes(systemePendulesT * systemePendules, projectionT * projection, commandesT * commandes);

void projectionInitialiseAxeFixe(grapheT * fixe, int nombre);

int projectionChangePhi(projectionT * projection, float x);
int projectionChangePsi(projectionT * projection, float x);

int projectionAffichePointDeVue(projectionT * projection);
#endif
