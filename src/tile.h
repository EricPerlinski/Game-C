#ifndef TILE_H
#define TILE_H

#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>



#define TILE_DEGAT 1
/*
Structure d une Tile
*/

typedef struct{
	int num;
	int x;
	int y;
	int w;
	int h;
	bool transparent;
	bool liquide;
	bool tueur;
	int flags;
}Tile;



Tile* tile_init(int num, int x, int y/*, int flags*/);
void tile_dest(Tile* t);
void tile_draw(Tile* t,ALLEGRO_BITMAP *sprite, float x, float y);

#endif
