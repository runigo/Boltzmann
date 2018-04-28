/*
Copyright avril 2018, Stephan Runigo
runigo@free.fr
Boltzmann 2.0 simulateur pour les sciences physiques
Ce logiciel est un programme informatique servant à simuler différents 
phénomènes physiques et à en donner une représentation graphique.
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

#include "commandes.h"

int commandesInitialise(commandesT * commandes, int largeur, int hauteur)
	{
	int i;
		//		Initialisation des commandes
	(*commandes).rotatifsCentre=(int)(0.893528183716*largeur); // 856
	(*commandes).rotatifsGauche=(int)(0.859081419624*largeur); // 823
	(*commandes).rotatifsDroite=(int)(0.927974947808*largeur); // 889 / 958

	(*commandes).boutonsCentre=(int)(0.969728601253*largeur); // 920
	(*commandes).boutonsGauche=(int)(0.9509394572*largeur); // 911
	(*commandes).boutonsDroite=(int)(0.971816283925*largeur); // 931

	(*commandes).rotatifCentre[0]=(int)(0.1324278438*hauteur); // 78  / 589
	(*commandes).rotatifCentre[1]=(int)(0.344651952462*hauteur); // 203
	(*commandes).rotatifCentre[2]=(int)(0.555178268251*hauteur); // 327
	(*commandes).rotatifCentre[3]=(int)(0.765704584041*hauteur); // 451
	(*commandes).rotatifCentre[4]=(int)(0.920203735144*hauteur); // 542

	(*commandes).boutonCentre[0]=(int)(0.0543293718166*hauteur); // 32	Périodique
	(*commandes).boutonCentre[1]=(int)(0.105263157895*hauteur); // 62	Libre
	(*commandes).boutonCentre[2]=(int)(0.1494057725*hauteur); // 88 	Fixe
	(*commandes).boutonCentre[3]=(int)(0.195246179966*hauteur); // 115	Mixte
	(*commandes).boutonCentre[4]=(int)(0.283531409168*hauteur); // 167	Uniforme
	(*commandes).boutonCentre[5]=(int)(0.336162988115*hauteur); // 198	Nulle
	(*commandes).boutonCentre[6]=(int)(0.390492359932*hauteur); // 230	Extrémité
	(*commandes).boutonCentre[7]=(int)(0.482173174873*hauteur); // 284	Marche
	(*commandes).boutonCentre[8]=(int)(0.528013582343*hauteur); // 311	Arrêt
	(*commandes).boutonCentre[9]=(int)(0.575551782683*hauteur); // 339	Droite
	(*commandes).boutonCentre[10]=(int)(0.623089983022*hauteur); // 367	Gauche
	(*commandes).boutonCentre[11]=(int)(0.714770797963*hauteur); // 421	Arrêt
	(*commandes).boutonCentre[12]=(int)(0.7623089983*hauteur); // 449	Sinus
	(*commandes).boutonCentre[13]=(int)(0.816638370119*hauteur); // 481	Carré
	(*commandes).boutonCentre[14]=(int)(0.864176570458*hauteur); // 509	Impulsion
	(*commandes).boutonCentre[15]=(int)(0.910016977929*hauteur); // 536	Fluxon
	(*commandes).boutonCentre[16]=(int)(0.9558573854*hauteur); // 563	Anti F.

	int demiBouton=(int)(0.0208768267223*largeur); // 20 / 958
	int demiRotatif=(int)(0.0344467640919*largeur); // 33
	for(i=0;i<BOUTON_COMMANDES;i++)
		{
		(*commandes).boutonSup[i]=(*commandes).boutonCentre[i]+demiBouton;
		(*commandes).boutonInf[BOUTON_COMMANDES]=(*commandes).boutonCentre[i]+demiBouton;
		}
	for(i=0;i<ROTATIF_COMMANDES;i++)
		{
		(*commandes).rotatifSup[i]=(*commandes).rotatifCentre[i]+demiRotatif;
		(*commandes).rotatifInf[i]=(*commandes).rotatifCentre[i]+demiRotatif;
		}

	return 0;
	}

//////////////////////////////////////////////////////////////////
