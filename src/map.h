#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tile.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

/* Maximum de largeur en Tile de la map*/
#define MAX_TILE_W 2000
/* Maximum de la hauteur en Tile de la map */
#define MAX_TILE_H 100

/* Taille d'un sprite */
#define LENGTH_SPRITE 16
/* Taille de la definition d'un tile -> num:rotaion, */
#define LENGTH_DEF_SPRITE 6

/* caracter de separation des argument d'une Tile pour le fichier texte decrivant la map*/
#define SEPARATOR_ARGS :
/* caractere de separation des Tile pour le fichier map decrivant la map*/
#define SEPARATOR_TILE ,

/* Taille maximal pour une ligne du fichier map*/
#define MAX_W_LINE_MAP (MAX_TILE_W*LENGTH_DEF_SPRITE)
 
#define RAND 10

#define MAP_MAX_EAU 10


typedef struct{
	ALLEGRO_BITMAP* sprite;
	ALLEGRO_BITMAP* background;
	ALLEGRO_SAMPLE* musique;
	bool is_playing;
	/* lageur de la map */
	int w;
	/* hauteur de la map */
	int h;
	/* coordonne sup gauche d affichage */
	int x;
	int y;
	/* largeur et hauteur d affichage */
	int dx;
	int dy;

	/* cadre contenant les deplacement du joueur */
	int frame_x;
	int frame_y;
	int frame_dx;
	int frame_dy;
	int type_map;
	Tile*** map;
}Map;

Map* map_init(char* map_src, int w,int h);
void map_dest(Map* map);

void map_draw(Map*map);

void map_fill(Map* map, int ligne, char* chaine);

void map_mouv_horiz(Map* map, int val);
void map_mouv_vert(Map* map, int val);

void map_play_musique(Map* map);
void map_stop_musique(Map* map);

void map_toggle_musique(Map* map);

void map_set_frame(Map* map, int x, int y, int dx, int dy);
void map_update_frame_player(Map* map, int x, int y);

void map_aleatoire(char* dest, int w, int h);


#endif

