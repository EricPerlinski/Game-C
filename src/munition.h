#ifndef MUNITION_H
#define MUNITION_H

#include <stdio.h>
#include <stdlib.h>

#include "jeu.h" 

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#define MUNITION_LENGTH_SPRITE 20
#define MUNITION_HEIGHT_SPRITE 19

#define MUNITION_GRAVITE 3
typedef struct {
	ALLEGRO_BITMAP *sprite;
	int sol;
	int x;
	int y;
}Munition;


Munition* munition_init(int x, int y, char* sprite_src);
void munition_dest(Munition* munition);
void munition_update_etat(Munition* munition,Jeu* jeu);
void munition_draw(Munition* munition,Map* map);
void munition_gravite(Munition* munition, Map* map);
void munition_update_frame_player(Munition* munition, Map* map, int x, int y);
bool munition_mouv_horizontal(Munition* munition,Map* map,int x);
bool munition_collision(Munition* munition, Map* map);
bool munition_est_dans_eau(Munition* munition, Map* map);
bool munition_est_dans_liquide_tueur(Munition* munition, Map* map);
#endif
