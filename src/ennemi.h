#ifndef ENNEMI_H
#define ENNEMI_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "jeu.h"


#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>


#define ENNEMI_RAYON 400
#define ENNEMI_ANGLE 150

/* hardcore : 1 -> facile : 100*/
#define ENNEMI_DIFFICULTE 80

#define ENNEMI_ROTATION 300

#define ENNEMI_VITESSE 1

#define ENNEMI_DISTANCE_MIN 64

typedef enum{
	CHERCHE,
	ATTAQUE
}Ennemi_etat;

typedef enum{
	DIR_DROITE,
	DIR_GAUCHE,
	DIR_COUNT
}Direction;

typedef struct{
	Personnage* bot;
	Ennemi_etat etat;
	Direction dir;
	bool change_etat;
}Ennemi;

Ennemi* ennemi_init(int x, int y,char* sprite);
List* ennemi_liste_init(char* map_src);
void ennemi_dest(void* ennemi);

void ennemi_update_etat(Ennemi* ennemi, Jeu* jeu);
bool ennemis_vision(Ennemi* ennemi, Jeu* jeu);
void ennemi_attaque(Ennemi* ennemi, Jeu* jeu);
void ennemi_cherche(Ennemi* ennemi, Personnage* perso);

void ennemi_draw(Ennemi* ennemi,Map* map);

#endif
