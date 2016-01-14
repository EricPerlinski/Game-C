#ifndef JEU_H
#define JEU_H

#include <stdio.h>
#include <stdlib.h>

#define DISPLAY_W (50*LENGTH_SPRITE)
#define DISPLAY_H (23*LENGTH_SPRITE)

#define VITESSE 4

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

typedef struct jeu Jeu;

#include <string.h>
#include "customlinkedlist.h"
#include "tile.h"
#include "map.h"
#include "perso.h"
#include "boss.h"
#include "projectile.h"
#include "event.h"
#include "menu.h"
#include "ennemi.h"
#include "medikit.h"
#include "munition.h"


struct jeu{
	Map* map;
	Personnage* perso;
	List* ennemis;
	List* projectiles;
	List* boss;
	List* medikit;
	List* munition;
	Menu* menu;
	ALLEGRO_BITMAP* cursor;
	ALLEGRO_SAMPLE* Gun_sound;
	ALLEGRO_SAMPLE* Gun_sound_empty;
	ALLEGRO_SAMPLE* Rifle_sound;
	ALLEGRO_SAMPLE* Grenade_sound;
	ALLEGRO_MIXER* mixer;
	Event* ev;
	ALLEGRO_FONT* font;
	ALLEGRO_FONT* font_jeu;
	bool boucle_jeu;
};



Jeu* jeu_init(char* map_src);
void jeu_dest(Jeu* jeu);
int jeu_parse(Jeu* jeu,char* map_src);
void jeu_draw(Jeu* jeu);
bool jeu_update_event(Jeu* jeu);
void jeu_parcours_collision_ennemis(Elem* e,void* jeu);
void jeu_parcours_collision_boss(Elem* e,void* jeu);
void jeu_parcours_collision_medikit(Elem* e,void* jeu);
void jeu_parcours_collision_munition(Elem* e,void* jeu);
void jeu_parcours_collision_projectiles(Elem* e,void* jeu);
int jeu_fin_jeu(Jeu* jeu);

#endif
