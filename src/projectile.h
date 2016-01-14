#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "jeu.h"


#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#define PROJECTILE_PUISSANCE 1
#define PROJECTILE_PUISSANCE_GRENADE 10
#define PROJECTILE_VITESSE 10
#define PROJECTILE_VITESSE_GRENADE 4

#define M_PI 3.14159265358979323846


typedef enum{
	TYPE_BALLE,
	TYPE_GRENADE,
	TYPECOUNT
}Type;

typedef struct{
	Type type;
	int puissance;
	int x0;
	int y0;
	float x;
	float y;
	float x_centre;
	float y_centre;
	bool tombe;
	float rotation;
	float dist;
	int vitesse;
}Projectile;

Projectile* projectile_init(Type type, int x0, int y0, int x1, int y1, int xmap, int ymap);
Projectile* projectile_boss_init(Type type, int x0, int y0, int x1, int y1, int xmap, int ymap);
void projectile_dest(void* proj);


void projectile_tire(Projectile* proj);
void projectile_draw(Projectile* proj,Jeu* jeu);

int projectile_degat(Projectile* proj);

bool projectile_collision_map(Projectile* proj, Map* map);
bool projectile_collision_perso(Projectile* proj,Personnage* perso);
bool projectile_collision_ennemis(Projectile* proj, List* ennemis);
bool projectile_collision_boss(Projectile* proj, List* boss);

#endif
