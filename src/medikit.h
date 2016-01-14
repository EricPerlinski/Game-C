#ifndef MEDIKIT_H
#define MEDIKIT_H

#include <stdio.h>
#include <stdlib.h>

#include "jeu.h" 

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#define MEDIKIT_LENGTH_SPRITE 20
#define MEDIKIT_HEIGHT_SPRITE 19

#define MEDIKIT_GRAVITE 3

typedef struct {
	ALLEGRO_BITMAP *sprite;
	int sol;
	int x;
	int y;
	int point_vie_rendu;
}Medikit;


Medikit* medikit_init(int x, int y, char* sprite_src);
void medikit_dest(Medikit* medikit);
void medikit_update_etat(Medikit* medikit,Jeu* jeu);
void medikit_draw(Medikit* medikit,Map* map);
void medikit_gravite(Medikit* medikit, Map* map);
void medikit_update_frame_player(Medikit* medikit, Map* map, int x, int y);
bool medikit_mouv_horizontal(Medikit* medikit,Map* map,int x);
bool medikit_collision(Medikit* medikit, Map* map);
bool medikit_est_dans_eau(Medikit* medikit, Map* map);
bool medikit_est_dans_liquide_tueur(Medikit* medikit, Map* map);
#endif
