#ifndef EVENT_H
#define EVENT_H

#include <stdlib.h>
#include <stdio.h>


#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>


typedef enum{
	EVENT_PERIPH_CLAVIER,
	EVENT_PERIPH_XBOX,
	EVENT_PERIPH_COUNT
}Periph;


typedef struct{
	bool gauche;
	bool droit;
	bool haut;
	bool bas;
	bool saut;
	bool weapon_change;
	bool menu;
	bool ok;

	int x;
	int y;
	bool clic_gauche;

	int son;

	int periph;

	bool change;

	ALLEGRO_JOYSTICK *joy;
}Event;





Event* event_init(Periph p);
void event_dest(Event* ev);

void event_update(Event* ev, ALLEGRO_EVENT* al_ev);

void event_update_x_y(Event* ev, int vitesse);

void event_reset(Event* ev);

Periph event_get_periph(Event* ev);

void event_toggle_periph(Event* ev);

bool event_get_gauche(Event* ev);
bool event_get_droit(Event* ev);
bool event_get_weapon_change(Event* ev);
bool event_get_weapon_change_court(Event* ev);

bool event_get_saut(Event* ev);
bool event_get_menu(Event* ev);
bool event_get_son(Event* ev);

bool event_get_clic_gauche(Event* ev);
bool event_get_clic_gauche_court(Event* ev);
bool event_get_ok_court(Event* ev);
bool event_get_menu_court(Event* ev);

bool event_get_haut_court(Event* ev);
bool event_get_bas_court(Event* ev);

int event_get_x(Event* ev);
int event_get_y(Event* ev);

#endif