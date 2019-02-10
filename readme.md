# Perlin Vertex

## Installation

Utilise la commande `$ make` dans le repertoire contenant le fichier main.cpp pour compiler le jeu

Pour lancer le jeu : `$ ./executable`


## Objectif

  Survit le plus longtemps possible en évitant les obstacles qui se dirige vers toi.
  Attention : tu ne possède que 8 vies.


## Commandes

Flèches du clavier:
  - Gauche : Tourne le vaisseau à gauche
  - Droite : Tourne le vaisseau à droite
  - Haut : Accélère
  - Bas : Ralentit


## Attention

Lorsque tu n'as plus de points de vie : le jeu se ferme 'brutalement'*


## Problèmes

Si un problème apparaît durant la compilation : pas de panique !
- Bien supprimer tous les fichiers .o dans tous les sous-répertoires (et vérifier même après un `$ make clean`)
- Le programme a été testé avec l'installation des packages suivants : make, g++, libglew-dev, freeglut3-dev
  Veuillez les installer si ce n'est pas déjà fait