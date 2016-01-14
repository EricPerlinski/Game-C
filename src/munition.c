#include "munition.h"
#include "tile.h"

Munition* munition_init(int x, int y,char* sprite_src){
	Munition* munition = (Munition*)malloc(sizeof(Munition));
	munition->x=x;
	munition->y=y;
	munition->sprite = al_load_bitmap(sprite_src);
	if(munition->sprite==NULL){
		fprintf(stderr, "Erreur lors du chargement du sprite du munition\n");
		return NULL;
	}
	munition->sol = 0;

	return munition;
}

void munition_update_etat(Munition* munition, Jeu* jeu){
	munition_gravite(munition,jeu->map);
}

void munition_update_frame_player(Munition* munition,Map* map, int x, int y){
	munition_mouv_horizontal(munition,map,x);
}

void munition_gravite(Munition* munition, Map* map){	
	munition->y += MUNITION_GRAVITE;
	if(munition_collision(munition,map) ){
		munition->y -= MUNITION_GRAVITE;
		if(!munition_est_dans_eau(munition,map)){
			munition->sol=1;	
		}else{
			munition->sol=0;
		}
		
	}else{
		munition->sol=0;
	}
}

bool munition_mouv_horizontal(Munition* munition,Map* map,int xtemp){
	bool res=true;
	if(munition->x+xtemp+MUNITION_LENGTH_SPRITE<map->w*LENGTH_SPRITE && munition->x >= 0){

		int i;
		int mult = (xtemp<0?-1:1);
		for(i=1;i<=xtemp*mult;i++){
			munition->x +=1*mult;

			if(munition_collision(munition,map)){
				munition->x -= 1*mult;
				res=false;
				break;
			}
		}
	}else{
		if(munition->x<0){
			munition->x=0;
		}
	}
	return res;
}

bool munition_collision(Munition* munition, Map* map){
	int x = munition->x/LENGTH_SPRITE;
	int y = munition->y/LENGTH_SPRITE;

	int largeur = MUNITION_LENGTH_SPRITE/LENGTH_SPRITE+1;
	int hauteur = MUNITION_HEIGHT_SPRITE/LENGTH_SPRITE+1;

	int i=x;
	int j=y;


	while(i<x+largeur && i<map->w){
		j=y;
		while(j<y+hauteur && j<map->h){
			if(i>=0 && j>=0 && i<map->w && j<map->h){
				if(map->map[j][i]->transparent == 0){
					return true;
				}
			}
			j++;
		}
		i++;
	}
	return false;
}

bool munition_est_dans_eau(Munition* munition, Map* map){
	int x = munition->x/LENGTH_SPRITE;
	int y = munition->y/LENGTH_SPRITE;

	int largeur = MUNITION_LENGTH_SPRITE/LENGTH_SPRITE+1;
	int hauteur = MUNITION_HEIGHT_SPRITE/LENGTH_SPRITE+1;

	int i=x;
	int j=y+hauteur-1;

	while(i<x+largeur && i<map->w){
		if(i>=0 && j>=0 && i<map->w && j<map->h){
			if(map->map[j][i]->liquide == 1){
				return true;
			}
		}
		i++;
	}
	return false;
}

bool munition_est_dans_liquide_tueur(Munition* munition, Map* map){
	int x = munition->x/LENGTH_SPRITE;
	int y = munition->y/LENGTH_SPRITE;

	int largeur = MUNITION_LENGTH_SPRITE/LENGTH_SPRITE+1;
	int hauteur = MUNITION_HEIGHT_SPRITE/LENGTH_SPRITE+1;

	int i=x;
	int j=y+hauteur-1;

	while(i<x+largeur && i<map->w){
		if(i>=0 && j>=0 && i<map->w && j<map->h){
			if(map->map[j][i]->tueur == 1){
				return true;
			}
		}
		i++;
	}
	return false;
}

void munition_draw(Munition* munition,Map* map){
	
	int xdest = munition->x-map->x;
	int ydest = munition->y-map->y;
	
	al_draw_bitmap_region(munition->sprite,0,0,MUNITION_LENGTH_SPRITE,MUNITION_HEIGHT_SPRITE,xdest,ydest,0);
}

void munition_dest(Munition* munition){
	al_destroy_bitmap(munition->sprite);
	free(munition);
}