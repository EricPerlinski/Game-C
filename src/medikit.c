#include "medikit.h"
#include "tile.h"

Medikit* medikit_init(int x, int y,char* sprite_src){
	Medikit* medikit = (Medikit*)malloc(sizeof(Medikit));
	medikit->x=x;
	medikit->y=y;
	medikit->point_vie_rendu=25;
	medikit->sprite = al_load_bitmap(sprite_src);
	if(medikit->sprite==NULL){
		fprintf(stderr, "Erreur lors du chargement du sprite du medikit\n");
		return NULL;
	}
	medikit->sol = 0;

	return medikit;
}

void medikit_update_etat(Medikit* medikit,Jeu* jeu){
	medikit_gravite(medikit,jeu->map);
}

void medikit_update_frame_player(Medikit* medikit,Map* map, int x, int y){
	medikit_mouv_horizontal(medikit,map,x);
}

void medikit_gravite(Medikit* medikit, Map* map){	
	medikit->y += MEDIKIT_GRAVITE;
	if(medikit_collision(medikit,map) ){
		medikit->y -= MEDIKIT_GRAVITE;
		if(!medikit_est_dans_eau(medikit,map)){
			medikit->sol=1;	
		}else{
			medikit->sol=0;
		}
		
	}else{
		medikit->sol=0;
	}
}

bool medikit_mouv_horizontal(Medikit* medikit,Map* map,int xtemp){
	bool res=true;
	if(medikit->x+xtemp+MEDIKIT_LENGTH_SPRITE<map->w*LENGTH_SPRITE && medikit->x >= 0){

		int i;
		int mult = (xtemp<0?-1:1);
		for(i=1;i<=xtemp*mult;i++){
			medikit->x +=1*mult;

			if(medikit_collision(medikit,map)){
				medikit->x -= 1*mult;
				res=false;
				break;
			}
		}
	}else{
		if(medikit->x<0){
			medikit->x=0;
		}
	}
	return res;
}

bool medikit_collision(Medikit* medikit, Map* map){
	int x = medikit->x/LENGTH_SPRITE;
	int y = medikit->y/LENGTH_SPRITE;

	int largeur = MEDIKIT_LENGTH_SPRITE/LENGTH_SPRITE+1;
	int hauteur = MEDIKIT_HEIGHT_SPRITE/LENGTH_SPRITE+1;

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

bool medikit_est_dans_eau(Medikit* medikit, Map* map){
	int x = medikit->x/LENGTH_SPRITE;
	int y = medikit->y/LENGTH_SPRITE;

	int largeur = MEDIKIT_LENGTH_SPRITE/LENGTH_SPRITE+1;
	int hauteur = MEDIKIT_HEIGHT_SPRITE/LENGTH_SPRITE+1;

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

bool medikit_est_dans_liquide_tueur(Medikit* medikit, Map* map){
	int x = medikit->x/LENGTH_SPRITE;
	int y = medikit->y/LENGTH_SPRITE;

	int largeur = MEDIKIT_LENGTH_SPRITE/LENGTH_SPRITE+1;
	int hauteur = MEDIKIT_HEIGHT_SPRITE/LENGTH_SPRITE+1;

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

void medikit_draw(Medikit* medikit,Map* map){
	
	int xdest = medikit->x-map->x;
	int ydest = medikit->y-map->y;
	
	al_draw_bitmap_region(medikit->sprite,0,0,MEDIKIT_LENGTH_SPRITE,MEDIKIT_HEIGHT_SPRITE,xdest,ydest,0);
}

void medikit_dest(Medikit* medikit){
	al_destroy_bitmap(medikit->sprite);
	free(medikit);
}