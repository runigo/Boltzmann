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

#include "projection.h"

float projectionAbsolue(float valeur);

int projectionPerspectiveChaine(projectionT * projection, grapheT * graphe);
int projectionSystemeChaine3D(systemePendulesT * systemePendules, projectionT * projection, grapheT * graphe);

int projectionInitialiseSupport(projectionT * projection, int nombre);
int projectionPerspectiveSupport(projectionT * projection, grapheT * graphe);

float projectionAbsolue(float valeur)
	{
	if(valeur<0) return -valeur;
	return valeur;
	}

int projectionSystemeCommandes(systemePendulesT * systemePendules, projectionT * projection, commandesT * commandes)
	{		// Projette le système sur les commandes
	(void)projection;
	float theta;
	float ratioRotatif = 0.9;

				//	Projection sur les boutons rotatifs
	theta = DEUXPI * (*systemePendules).couplage/(COUPLAGE_MAX * (*systemePendules).nombre);
	(*commandes).rotatifPositionX[0]=(int)(-ratioRotatif*(*commandes).rotatifX*sin(theta)); //	Couplage
	(*commandes).rotatifPositionY[0]=(int)(ratioRotatif*(*commandes).rotatifY*cos(theta)); //	Couplage

	theta = DEUXPI * (*systemePendules).dissipation/DISSIPATION_MAX;
	(*commandes).rotatifPositionX[1]=(int)(-ratioRotatif*(*commandes).rotatifX*sin(theta));
	(*commandes).rotatifPositionY[1]=(int)(ratioRotatif*(*commandes).rotatifY*cos(theta));

	theta = DEUXPI * projectionAbsolue((*systemePendules).moteurs.courant/JOSEPHSON_MAX);	//	Amplitude du moteur josephson
	(*commandes).rotatifPositionX[2]=(int)(-ratioRotatif*(*commandes).rotatifX*sin(theta));
	(*commandes).rotatifPositionY[2]=(int)(ratioRotatif*(*commandes).rotatifY*cos(theta));

	theta = DEUXPI * log(1+(*systemePendules).moteurs.amplitude/AMPLITUDE_MAX);	//	Amplitude du moteur périodique
	(*commandes).rotatifPositionX[3]=(int)(-ratioRotatif*(*commandes).rotatifX*sin(theta));
	(*commandes).rotatifPositionY[3]=(int)(ratioRotatif*(*commandes).rotatifY*cos(theta));

	theta = DEUXPI * log(1+(*systemePendules).moteurs.frequence/FREQUENCE_MAX);	//	Fréquence du moteurs
	(*commandes).rotatifPositionX[4]=(int)(-ratioRotatif*(*commandes).rotatifX*sin(theta));
	(*commandes).rotatifPositionY[4]=(int)(ratioRotatif*(*commandes).rotatifY*cos(theta));

		//int rotatifPositionX[ROTATIF_COMMANDES]; // Position du bouton rotatif
		//int rotatifPositionY[ROTATIF_COMMANDES];


	int i;
	for(i=0;i<BOUTON_COMMANDES;i++) (*commandes).boutonEtat[i]=0;

		//int libreFixe;		//	0 : périodiques 1 : libres, 2 : fixes, 
							//		3 libre-fixe, 4 fixe-libre
	switch((*systemePendules).libreFixe)	//	
		{
		case 0:
			(*commandes).boutonEtat[0]=1;break; // 32	Périodique
		case 1:
			(*commandes).boutonEtat[1]=1;break; // 62	Libre
		case 2:
			(*commandes).boutonEtat[2]=1;break; // 88 	Fixe
		case 3:
			(*commandes).boutonEtat[3]=1;break; // 115	Mixte
		case 4:
			(*commandes).boutonEtat[3]=1;break; // 115	Mixte
		default:
			;
		}
/*
	(*commandes).boutonEtat[4]=1; // 167	Uniforme
	(*commandes).boutonEtat[5]=1; // 198	Nulle
	(*commandes).boutonEtat[6]=1; // 230	Extrémité
*/
	if((*systemePendules).moteurs.josephson > 0.0)
		{
		(*commandes).boutonEtat[7]=1; // 284	Marche
		(*commandes).boutonEtat[9]=1; // 339	Droite
		}
	else
		{
		if((*systemePendules).moteurs.josephson < 0.0)
			{
			(*commandes).boutonEtat[7]=1; // 284	Marche
			(*commandes).boutonEtat[10]=1; // 367	Gauche
			}
		else
			{
			(*commandes).boutonEtat[8]=1; // 311	Arrêt
			}
		}

	switch((*systemePendules).moteurs.generateur)	//	0:eteint, 1:sinus, 2:carre, 3:impulsion
		{
		case 0:
			(*commandes).boutonEtat[11]=1;break; // 421	Arrêt
		case 1:
			(*commandes).boutonEtat[12]=1;break; // 449	Sinus
		case 2:
			(*commandes).boutonEtat[13]=1;break; // 481	Carré
		case 3:
			(*commandes).boutonEtat[14]=1;break; // 509	Impulsion
		default:
			;
		}

	/*	float (*systemePendules).moteurs.phi;			//	Phase
		float (*systemePendules).dephasage;	//	déphasage entre les limites
	(*commandes).boutonEtat[15]=1; // 536	Fluxon
	(*commandes).boutonEtat[16]=1; // 563	Anti F.
	*/
	return 0;
	}

int projectionInitialiseCouleurs(projectionT * projection, int r, int v, int b, int fond)
	{		// Initialise la couleur du graphe
	(*projection).rouge = r;
	(*projection).vert = v;
	(*projection).bleu = b;
	(*projection).fond = fond;
	return 0;
	}
int projectionInitialiseLongueurs(projectionT * projection, int hauteur, int largeur, float  perspective)
	{		// Fixe la taille de la chaîne et l'effet de perspective
	(*projection).hauteur = hauteur;
	(*projection).largeur = largeur;
	(*projection).perspective = perspective;
	return 0;
	}
int projectionInitialisePointDeVue(projectionT * projection, float psi, float phi)
	{		// Initialise la position de l'observateur et calcul les vecteurs perpendiculaires
	vecteurInitialisePolaire(&(*projection).pointDeVue, (*projection).perspective, psi, phi);
	vecteurInitialiseVecteurPhi(&(*projection).pointDeVue, &(*projection).vecteurPhi, (*projection).perspective);
	vecteurInitialiseVecteurPsi(&(*projection).pointDeVue, &(*projection).vecteurPsi, (*projection).perspective);
	return 0;
	}

int projectionChangePhi(projectionT * projection, float x)
	{		// Change la position de l'observateur suivant phi
	float r, psi, phi;

	r = (*projection).pointDeVue.r;
	psi = (*projection).pointDeVue.psi;
	phi = (*projection).pointDeVue.phi + x;

		// phi reste inférieur à PI
	if(phi > PI)
		{
		phi = PI;
		}

		// phi reste supérieur à zéro
	if(phi < 0.0)
		{
		phi = 0.0;
		}

	vecteurInitialisePolaire(&(*projection).pointDeVue, r, psi, phi);
	vecteurInitialiseVecteurPhi(&(*projection).pointDeVue, &(*projection).vecteurPhi, (*projection).perspective);
	vecteurInitialiseVecteurPsi(&(*projection).pointDeVue, &(*projection).vecteurPsi, (*projection).perspective);
	return 0;
	}

int projectionChangePsi(projectionT * projection, float x)
	{		// Change la position de l'observateur suivant psi
	float r, psi, phi;

	r = (*projection).pointDeVue.r;
	psi = (*projection).pointDeVue.psi + x;
	phi = (*projection).pointDeVue.phi;

	if(psi > PI)
		{
		psi = psi - DEUXPI;
		}

	if(psi < -PI)
		{
		psi = psi + DEUXPI;
		}

	vecteurInitialisePolaire(&(*projection).pointDeVue, r, psi, phi);
	vecteurInitialiseVecteurPhi(&(*projection).pointDeVue, &(*projection).vecteurPhi, (*projection).perspective);
	vecteurInitialiseVecteurPsi(&(*projection).pointDeVue, &(*projection).vecteurPsi, (*projection).perspective);
	return 0;
	}

int projectionAffichePointDeVue(projectionT * projection)
	{		// Affiche les valeurs de psi et phi
	float r, psi, phi;

	r = (*projection).pointDeVue.r;
	psi = (*projection).pointDeVue.psi;
	phi = (*projection).pointDeVue.phi;

	printf("(*projection).pointDeVue.r = %f\n", r);
	printf("(*projection).pointDeVue.psi = %f\n", psi);
	printf("(*projection).pointDeVue.phi = %f\n", phi);

	return 0;
	}
/*

		Projection du système sur le rendu en perspective

*/
int projectionSystemeChaineDePendule(systemePendulesT * systemePendules, projectionT * projection, grapheT * graphe)
	{
		// Projection du système sur la chaîne de pendule 3D
	projectionSystemeChaine3D(systemePendules, projection, graphe);

		// Initialisation des points du support
	projectionInitialiseSupport(projection, (*systemePendules).nombre);

		// Projection en 2D de la représentation 3D
	projectionPerspectiveChaine(projection, graphe);
	projectionPerspectiveSupport(projection, graphe);

	return 0;
	}

int projectionInitialiseSupport(projectionT * projection, int nombre)
//
//                                                L   K
//                                             H   G
//                                               M
//                                               
//                                                J   I
//                                             F   E
//               N


//             D   C
//         B   A
//
	{
	int i;
	float xyz;

					// AXE Oy
	xyz = 0.4*(*projection).hauteur;
	for(i=0;i<12;i+=2)
		{
		(*projection).support[i].y = xyz;
		}

	xyz = -0.4*(*projection).hauteur;
	for(i=1;i<12;i+=2)
		{
		(*projection).support[i].y = xyz;
		}
	(*projection).support[13].y = 0.0;
	(*projection).support[12].y = 0.0;

					// AXE Oz
	xyz = 1.2 * (*projection).hauteur;
	for(i=0;i<8;i++)
		{
		(*projection).support[i].z = xyz;
		}

	xyz = -0.2 * (*projection).hauteur;
	for(i=8;i<12;i++)
		{
		(*projection).support[i].z = xyz;
		}
	(*projection).support[13].z = 0.0;
	(*projection).support[12].z = 0.0;

					// AXE Ox
	xyz = 0.5 * (*projection).largeur;
	for(i=0;i<4;i++)
		{
		(*projection).support[i].x = xyz;
		}
	(*projection).support[13].x = xyz;
	(*projection).support[12].x = -xyz;
	(*projection).support[0].x += 0.5*(*projection).hauteur;
	(*projection).support[1].x += 0.5*(*projection).hauteur;

	xyz = (-0.5-1.0/nombre) * (*projection).largeur;
	for(i=4;i<13;i++)
		{
		(*projection).support[i].x = xyz;
		}
	for(i=6;i<10;i++)
		{
		(*projection).support[i].x += -0.6*(*projection).hauteur;
		}

	return 0;
	}

int projectionPerspectiveSupport(projectionT * projection, grapheT * graphe)
	{
			//	Projette le support 3D sur le rendu en perspective

	vecteurT v;
	int i;
	int centrageX = (int)( (*projection).largeur * RATIO_C_X );
	int centrageY = (int)( (*projection).hauteur * RATIO_C_Y );

	for(i=0;i<14;i++)
		{
			// Coordonnees 2D des points du support
		vecteurDifferenceCartesien(&(*projection).support[i], &(*projection).pointDeVue, &v);
		(*graphe).supporX[i] = centrageX + vecteurScalaireCartesien(&v, &(*projection).vecteurPsi);
		(*graphe).supporY[i] = centrageY + vecteurScalaireCartesien(&v, &(*projection).vecteurPhi);
		}
	if((*projection).pointDeVue.psi<0)
		{
		(*graphe).gauche=1;
		if((*projection).pointDeVue.psi < -PI/2)
			{
			(*graphe).arriere=1;
			}
		else
			{
			(*graphe).arriere=0;
			}
		}
	else
		{
		(*graphe).gauche=0;
		if((*projection).pointDeVue.psi > PI/2)
			{
			(*graphe).arriere=1;
			}
		else
			{
			(*graphe).arriere=0;
			}
		}

	if((*projection).pointDeVue.phi<PI/2)
		{
		(*graphe).dessous=1;
		}
	else
		{
		(*graphe).dessous=0;
		}

	return 0;
	}

int projectionPerspectiveChaine(projectionT * projection, grapheT * graphe)
	{
			//	Projette la chaine 3D sur le rendu en perspective

	pointsT *iterGraph=(*graphe).premier;

	vecteurT v;
	int centrageX = (int)( (*projection).largeur * RATIO_C_X );
	int centrageY = (int)( (*projection).hauteur * RATIO_C_Y );

	do
		{
			// Coordonnees 2D de la masse et centrage du graphe
		vecteurDifferenceCartesien(&(iterGraph->masse), &(*projection).pointDeVue, &v);
		iterGraph->xm = centrageX + vecteurScalaireCartesien(&v, &(*projection).vecteurPsi);
		iterGraph->ym = centrageY + vecteurScalaireCartesien(&v, &(*projection).vecteurPhi);

			// Position avant ou arrière de la masse SUPPRIMÉ DANS SiCP 1.5.1
		//iterGraph->position = iterGraph->masse.y * (*projection).pointDeVue.y;

			// Coordonnees 2D de l'axe
		vecteurDifferenceCartesien(&(iterGraph->axe), &(*projection).pointDeVue, &v);
		iterGraph->xa = centrageX + vecteurScalaireCartesien(&v, &(*projection).vecteurPsi);
		iterGraph->ya = centrageY + vecteurScalaireCartesien(&v, &(*projection).vecteurPhi);



		iterGraph = iterGraph->suivant;
		}
	while(iterGraph!=(*graphe).premier);

	return 0;
	}

int projectionSystemeChaine3D(systemePendulesT * systemePendules, projectionT * projection, grapheT * graphe)
	{	//	Projette le système sur une chaîne de pendule en 3 D
	//float x=0;
	float i = -(*systemePendules).nombre/2;

	chaineT *iterSystem=(*systemePendules).premier;
	pointsT *iterGraph=(*graphe).premier;

	do
		{
			// Axe fixe
		iterGraph->axe.x = (*projection).largeur*(i/(*systemePendules).nombre);
		iterGraph->axe.y = 0;
		iterGraph->axe.z = 0;

			// Masse
		iterGraph->masse.x = iterGraph->axe.x;
		iterGraph->masse.y = (*projection).hauteur * sin(iterSystem->pendule.nouveau);
		iterGraph->masse.z = (*projection).hauteur * cos(iterSystem->pendule.nouveau);

		//if(x>0) iterGraph->position=1;
		//else iterGraph->position=0;

		iterGraph = iterGraph->suivant;
		iterSystem = iterSystem->suivant;

		i = i + 1.0;

		}
	while(iterGraph!=(*graphe).premier);
	return 0;
	}

//////////////////////////////////////////////////////////////////////////////////////
