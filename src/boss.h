#ifndef BOSS_H
#define BOSS_H

#include <stdio.h>
#include <stdlib.h>

#include "jeu.h" 

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#define BOSS_UPDATE_CHG_SPRITE 4
#define BOSS_LENGTH_SPRITE 90
#define BOSS_HEIGHT_SPRITE 55
#define BOSS_HP 20
#define BOSS_RAYON 400
#define BOSS_ANGLE 360

/* hardcore : 1 -> facile : 100*/
#define BOSS_DIFFICULTE 80

#define BOSS_ROTATION 300

#define BOSS_VITESSE 1

#define BOSS_DISTANCE_MIN 300

#define BOSS_DISTANCE_CHERCHE 100


typedef enum{
	BOSS_CHERCHE,
	BOSS_ATTAQUE
}Boss_etat;

typedef enum{
	BOSS_DIR_DROITE,
	BOSS_DIR_GAUCHE,
	BOSS_DIR_COUNT
}Boss_direction;

typedef struct {
	ALLEGRO_BITMAP *sprite;
	int sprite_x;
	int sprite_y;
	int width;
	int height;
	int x;
	int y;
	int point_vie;
	int point_vie_initial;
	Boss_etat etat;
	Boss_direction dir;
}Boss;


Boss* boss_init(int x, int y,char* sprite_src);
void boss_dest(Boss* boss);
void boss_draw(Boss* boss,Map* map);
bool boss_mouv_horizontal(Boss* boss,Map* map, int xtemp);
bool boss_mouv_vertical(Boss* boss,Map* map, int ytemp);
void boss_mouv_sauter(Boss* boss,Map* map);
void boss_rotation_sprite(Boss* boss);
bool boss_mourir(Boss* boss,Map* map);
void boss_arret(Boss* boss);
void boss_touche(Boss* boss, int degats);
bool boss_collision(Boss* boss, Map* map);
bool boss_est_mort(Boss* boss);
void boss_update_etat(Boss* boss,Jeu* jeu);
void boss_attaque(Boss* boss, Jeu* jeu);
void boss_cherche(Boss* boss, Jeu* jeu);
#endif
