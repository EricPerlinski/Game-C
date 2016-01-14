#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>


#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>



#include "jeu.h"

typedef struct {
	Jeu* jeu;
	List* maps;
	int current_map_i;
	int size;
	bool success;
} Game;

/* Taille de l'ecran */


/* Periode de rafraichissement*/
#define FPS 100
#define TIME_TO_JUMP 100

Game* game_init();
void game_next_map_test(Game* game);

#endif
