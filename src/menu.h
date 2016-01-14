#ifndef MENU_H
#define MENU_H


#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "map.h"
#include "event.h"

typedef enum{
	MENU_JOUER,
	MENU_CH_PERIPH,
	MENU_CREDIT,
	MENU_QUITTER,
	MENU_NB_MENU
}Menu_action;

typedef struct{
	bool actif;
	char* liste[MENU_NB_MENU];
	Menu_action selected;
	Menu_action menu_actif;
}Menu;


#include "jeu.h"

Menu* menu_init();
void menu_dest(Menu* menu);

void menu_draw(Menu* menu, Jeu* jeu);

void menu_afficher(Menu* menu);
void menu_cacher(Menu* menu);

void menu_descendre(Menu* menu);
void menu_monter(Menu* menu);
Menu_action menu_selection(Menu* menu);

void menu_toggle(Menu* menu);

bool menu_est_actif(Menu* m);

void menu_affiche_mort(Menu* menu,Jeu* jeu);

void menu_activer(Menu* menu,Jeu* jeu);

void menu_affiche_credits(Menu* menu,Jeu* jeu);

void menu_retour(Menu* menu);

bool menu_root(Menu* menu);


#endif
