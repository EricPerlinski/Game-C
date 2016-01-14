#ifndef PERSO_H
#define PERSO_H

#include <stdio.h>
#include <stdlib.h>

#include "map.h" 

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#define PERSO_LENGTH_SPRITE 39
#define PERSO_HEIGHT_SPRITE 49

#define PERSO_UPDATE_CHG_SPRITE 4

#define PERSO_JUMP 80

#define PERSO_GRAVITE 3

#define PERSO_VIE 10



typedef enum{
	WEAPON_LESS,
	HANDGUN,
	RIFLE,
	GRENADE,
	WEAPON_COUNT
}WEAPONS;

typedef struct {
	ALLEGRO_BITMAP *sprite;
	int sprite_x;
	int nbvie;
	int sprite_y;
	int width;
	int height;
	int sol;
	int x;
	int y;
	int point_vie;
	int point_vie_initial;
	int balles_handgun;
	int balles_rifle;
	int grenades;
	WEAPONS arme_courante;
	ALLEGRO_BITMAP* sprite_vie;
	ALLEGRO_BITMAP* sprite_arme_interface;
}Personnage;


Personnage* perso_init(int x, int y, char* sprite_src);
void perso_dest(Personnage* perso);
void perso_draw(Personnage* perso,Map* map, float xsouris, float ysouris);
bool perso_mouv_horizontal(Personnage* perso,Map* map, int x);
void perso_mouv_vertical(Personnage* perso, int y);
void perso_gravite(Personnage* perso, Map* map);
void perso_update_frame_player(Personnage* perso, Map* map, int x, int y);
void perso_mouv_sauter(Personnage* perso,Map* map);
void perso_tomber(Personnage* perso);
void perso_rotation_sprite(Personnage* perso);
bool perso_mourir(Personnage* perso,Map* map);
void perso_arret(Personnage* perso);
void perso_touche(Personnage* perso, int degats);
void perso_rend_vie(Personnage* perso, int nbsoin);
void perso_rend_munition(Personnage* perso);
bool perso_collision(Personnage* perso, Map* map);
bool perso_est_mort(Personnage* perso);
void perso_ressusciter(Personnage* perso);
bool perso_est_dans_eau(Personnage* perso, Map* map);
bool perso_collision_dessous(Personnage* perso, Personnage* ennemi);
void perso_change_arme(Personnage* perso);
void perso_est_dans_liquide_tueur(Personnage* perso, Map* map);

#endif
