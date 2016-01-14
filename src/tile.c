#include "tile.h"

/* 
Fonction pour initialiser une Tile
num : numero de la tile (correspond au sprite recherche)
x : position en x sur la map
y : position en y sur la map
flags : rotation verticale et/ou hotizontale
*/

Tile* tile_init(int num, int x, int y/*, int flags*/){
	srand(time(NULL));

	Tile* t = (Tile*)malloc(sizeof(Tile));
	t->num = num;
	t->x = x;
	t->y = y;

	int lvl = num/100;
	switch(lvl){
		/* premiere ligne du sprite */
		case 0:
			t->w=16;
			t->h=16;
			break;
		/* deuxieme ligne du sprite */
		case 1:
			t->w=16;
			t->h=18;
			break;
		/* troisieme ligne du sprite */
		case 2:
			t->w=18;
			t->h=16;
			break;
		/* quatrieme ligne du sprite */
		case 3:
			t->w=18;
			t->h=18;
			break;
		/* au cas ou :D */
		default:
			t->w=16;
			t->h=16;
			break;
	}
	if( num==0 || (num >= 8 && num <= 13) || num ==30 || num ==38 ){
		t->transparent = true;	
	}else{
		t->transparent = false;
	}

	if((num >= 8 && num <= 10) || (num >= 29 && num <= 33) || num==38 || num==41){
		t->liquide=true;
	}else{
		t->liquide=false;
	}

	if(num==11 || num==12 || num==13 || num==38 || num==41){
		t->tueur=true;
	}else{
		t->tueur = false;
	}

	t->flags=0;
	return t;
}

/*
fonction pour detruire la Tile
*/
void tile_dest(Tile* t){
	free(t);
}

/*
fonction pour dessiner la Tile
t : Tile a dessiner
sprite : BITMAP a utiliser comme sprite
x : coordonne en x pour dessiner sur l ecran
y : coordonne en y pour dessiner sur l ecran
*/
void tile_draw(Tile* t,ALLEGRO_BITMAP *sprite,float x, float y){
	int posy = t->num/100;
	int posx = t->num%100;

	/* annimation de l eau */
	/* inutile donc indispensable */
	int eau=0;
	static int mouv = 0;
	static double f = -1;
	if(f==-1){
		f=time(NULL);
	}
	if((t->num == 8 || t->num == 11)){
		if((time(NULL)-f)>=1){
			mouv = (mouv+1)%3;
			f=time(NULL);
		}
		eau = ((t->num == 8 || t->num == 11)?((mouv+t->x)%3):0);
	}
	

	if(t->num==0){
		al_draw_tinted_bitmap_region(sprite,al_map_rgba(255, 255,255, 0.5), 1+(t->w+1)*posx,1+(t->h+1)*posy, t->w,t->h,(x)*16,(y)*16,t->flags);
		return;
	}
	switch(posy){
		case 0:
			al_draw_bitmap_region(sprite, 1+(t->w+1)*(posx+eau),1+(t->h+1)*posy, t->w,t->h,(x)*16,(y)*16,t->flags);
			break;
		case 1:
			al_draw_bitmap_region(sprite, 1+(t->w+1)*posx,18, t->w,18,(x)*16,(y)*16-2,t->flags);
			break;
		case 2:
			al_draw_bitmap_region(sprite, 1+(t->w+1)*posx,37, t->w,t->h,(x)*16-2,(y)*16,t->flags);
			break;
		case 3:
			al_draw_bitmap_region(sprite, 1+(t->w+1)*posx,54, t->w,t->h,(x)*16-2,(y)*16-2,t->flags);
			break;
	}
}

/*
Fonction pour le debug
*/
void tile_affiche_console(Tile* t){
	printf("[%d : %d:%d , %d:%d]\n",t->num,t->x,t->y,t->w,t->h );
}
