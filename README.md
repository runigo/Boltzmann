# Boltzmann
Simulateur pour les Sciences Physiques

Ce programme sert à simuler différents phénomènes physiques
et à en donner une représentation graphique.

Il est écrit en C et nécessite l'installation de la librairie graphique SDL 2


INSTALLATION

Pour installer ce programme sur votre ordinateur, vous devez avoir
installé la librairie graphique SDL ainsi que les outils permettant
la compilation :
Par exemple, avec une distribution basée sur debian, dans un terminal avec les
droits de l'administrateur :

	# apt-get install gcc make libsdl2-dev

Placez-vous dans le répertoire des sources, effectuez la compilation,

	$ make

Lancer l'exécutable créé :

	$ ./Boltzmann

Lancer l'exécutable avec d'éventuelles options :

	$ ./Boltzmann nombre 398


OPTION DE LA LIGNE DE COMMANDE

	aide			Affiche l'aide de SiCP

	fond			Couleur du fond de l'affichage
	dt			Discrétisation du temps 
	pause			Pause entre les affichages en ms
	soliton			Nombre de solitons initiales
	nombre			Nombre de pendules

UTILISATION

Le terminal dans lequel est lancé l'exécutable affiche des informations
lors de l'exécution, la fenêtre graphique doit être active pour piloter
la chaîne.

	a, q : augmenter, diminuer le couplage
	z, s : augmenter, diminuer la masse

	e, d : diminuer, augmenter la dissipation
	r, f : retire, forme la dissipation
	v : forme l'extrémité absorbante

	t, g : augmenter, diminuer la gravitation
	y, h : ajoute, enlève un déphasage de 2pi

	w : conditions aux limites périodique
	x : extrémités libres
	c : extrémités fixe
	b, n : fixe une extrémité, libère l'autre

	flêche droite : allume, éteint le courant Josephson
	flêches haut, bas : augmente diminue l'intensité
	flêche gauche : change le sens du courant

	u, j : augmenter, diminuer l'amplitude du générateur
	i : démarre une impulsion
	o : démarre, éteint le générateur
	l : démarre le signal carrée
	p, m : augmenter, diminuer la fréquence

	F5 : affiche les observables
	F8 : Graphique avec ou sans support
	Entrée : change le mode temporel

	F9, F10 : diminuent la vitesse de la simulation
	F11, F12 : augmentent la vitesse de la simulation

Lorsque le bouton de la souris est maintenu, les mouvements de celle-ci
permettent la rotation du point de vue de l'observateur.

LICENCE

Copyright mai 2018, Stephan Runigo
runigo@free.fr
Boltzmann 1.0 simulateur pour les sciences physiques
Ce logiciel est un programme informatique servant à simuler différents 
phénomènes physiques et à en donner une représentation graphique.
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
utilisateurs sont donc invités à charger et tester l'adéquation du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.
////////////////////////////////////////////////////////////////
