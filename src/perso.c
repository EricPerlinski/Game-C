#include "perso.h"
#include "tile.h"

Personnage* perso_init(int x, int y,char* sprite_src){
	Personnage* perso = (Personnage*)malloc(sizeof(Personnage));
	perso->x=x;
	perso->y=y;
	perso->width=0;
	perso->height=0;
	perso->nbvie = 3;
	perso->point_vie_initial=PERSO_VIE;
	perso->point_vie=perso->point_vie_initial;
	perso->sprite = al_load_bitmap(sprite_src);
	perso->balles_handgun = 15;
	perso->balles_rifle = 30;
	perso->grenades=5;
	perso->sprite_arme_interface = al_load_bitmap("img/sprite_arme.png");
	perso->sprite_vie = al_load_bitmap("img/sprite_vie.png");
	perso->arme_courante = WEAPON_LESS;
	if(perso->sprite==NULL){
		fprintf(stderr, "Erreur lors du chargement du sprite du personnage\n");
		return NULL;
	}
	if(perso->sprite_vie==NULL){
		fprintf(stderr, "Erreur lors du chargement du sprite des vies\n");
		return NULL;
	}
	if(perso->sprite_arme_interface==NULL){
		fprintf(stderr, "Erreur lors du chargement du sprite des armes\n");
		return NULL;
	}

	return perso;
}


void perso_update_frame_player(Personnage* perso,Map* map, int x, int y){
	perso_mouv_horizontal(perso,map,x);
}


void perso_mouv_sauter(Personnage* perso,Map* map){
	int i=0;
	if( perso->sol==0 && !perso_est_dans_eau(perso,map)){
		for(i=0;i<PERSO_JUMP+16;i++){
			perso->y -= 1;
			if(perso->sol==0 && !perso_est_dans_eau(perso,map)){
				perso->y+=1;
				break;
			}
		}
	}else if(perso_est_dans_eau(perso,map)){
		for(i=0;i<PERSO_JUMP+16;i++){
			perso->y -= 1;
			if(perso_collision(perso,map)){
				perso->y+=1;
				break;
			}
		}
	}else{
		for(i=0;i<PERSO_JUMP;i++){
			perso->y -= 1;
			if(perso_collision(perso,map)){
				perso->y+=1;
				break;
			}
		}
		perso->sol = 0;
	}
}

void perso_gravite(Personnage* perso, Map* map){	
	perso->y += PERSO_GRAVITE;
	if(perso_collision(perso,map) ){
		perso->y -= PERSO_GRAVITE;
		if(!perso_est_dans_eau(perso,map)){
			perso->sol=1;	
		}else{
			perso->sol=0;
			/*perso->y += PERSO_GRAVITE;*/ 
		}
		
	}else{
		perso->sol=0;
	}
}



void perso_arret(Personnage* perso){
	switch(perso->arme_courante){
		case WEAPON_LESS: 
		perso->height = 0;
		perso->width = 0;
		break;
		case HANDGUN:
		perso->height = 2;
		perso->width = 1;
		break;
		case RIFLE:
		perso->height = 5;
		perso->width = 1;
		break; 
		default :
		perso->height = 0;
		perso->width = 0;
		break;
	}
}

void perso_change_arme(Personnage* perso){
	perso->arme_courante = (perso->arme_courante + 1)%WEAPON_COUNT;
}

bool perso_mouv_horizontal(Personnage* perso,Map* map,int xtemp){
	bool res=true;
	if(perso->x+xtemp+PERSO_LENGTH_SPRITE<map->w*LENGTH_SPRITE && perso->x >= 0){

		int i;
		int mult = (xtemp<0?-1:1);
		for(i=1;i<=xtemp*mult;i++){
			perso->x +=1*mult;

			if(perso_collision(perso,map)){
				perso->x -= 1*mult;
				res=false;
				break;
			}
		}


		switch(perso->arme_courante){
			case WEAPON_LESS: 
			perso->height = 1;
			break;
			case HANDGUN:
			perso->height = 6;
			break;
			case RIFLE:
			perso->height = 4;
			break; 
			default :
			perso->height = 1;
			break;
		}
		/*
		if(perso->arme_courante == WEAPON_LESS){
			perso->height = 1;
		}else if(perso->arme_courante == HANDGUN){
			perso->height = 6;
		}
		*/

		static int cnt=0;
		if(cnt==PERSO_UPDATE_CHG_SPRITE){
			cnt=0;
			perso->width = (perso->width +1) % 6;
		}else{
			cnt++;
		}
	}else{
		if(perso->x<0){
			perso->x=0;
		}
	}
	return res;
}

bool perso_collision(Personnage* perso, Map* map){
	int x = perso->x/LENGTH_SPRITE;
	int y = perso->y/LENGTH_SPRITE;

	int largeur = PERSO_LENGTH_SPRITE/LENGTH_SPRITE+1;
	int hauteur = PERSO_HEIGHT_SPRITE/LENGTH_SPRITE+1;

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

bool perso_est_dans_eau(Personnage* perso, Map* map){
	int x = perso->x/LENGTH_SPRITE;
	int y = perso->y/LENGTH_SPRITE;

	int largeur = PERSO_LENGTH_SPRITE/LENGTH_SPRITE+1;
	int hauteur = PERSO_HEIGHT_SPRITE/LENGTH_SPRITE+1;

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

void perso_est_dans_liquide_tueur(Personnage* perso, Map* map){
	int x = perso->x/LENGTH_SPRITE;
	int y = perso->y/LENGTH_SPRITE;

	int largeur = PERSO_LENGTH_SPRITE/LENGTH_SPRITE+1;
	int hauteur = PERSO_HEIGHT_SPRITE/LENGTH_SPRITE+1;

	int i=x;
	int j=y+hauteur-1;

	while(i<x+largeur && i<map->w){
		if(i>=0 && j>=0 && i<map->w && j<map->h){
			if(map->map[j][i]->tueur == 1){
				perso_touche(perso, TILE_DEGAT);
				return ;
			}
		}
		i++;
	}
}

bool perso_collision_dessous(Personnage* perso, Personnage* ennemi){
	bool condy = (perso->y + PERSO_HEIGHT_SPRITE == ennemi->y+10);
	bool condx1 = perso->x>ennemi->x && perso->x<ennemi->x+PERSO_LENGTH_SPRITE;
	bool condx2 = perso->x+PERSO_LENGTH_SPRITE>ennemi->x && perso->x+PERSO_LENGTH_SPRITE<ennemi->x+PERSO_LENGTH_SPRITE;
	if (condy && (condx1 || condx2 )){
	}
	return  condy && (condx1 || condx2 );
}


void perso_draw(Personnage* perso,Map* map, float xsouris, float ysouris){
	
	int xdest = perso->x-map->x;
	int ydest = perso->y-map->y;

	int xsprite = 1+(perso->width * (PERSO_LENGTH_SPRITE+1) );
	int ysprite = 1+(perso->height * (PERSO_HEIGHT_SPRITE+1) );
	perso_est_dans_liquide_tueur(perso,map);
	if (perso->x > xsouris+map->x) {
		al_draw_bitmap_region(perso->sprite,xsprite,ysprite,PERSO_LENGTH_SPRITE,PERSO_HEIGHT_SPRITE,xdest,ydest,ALLEGRO_FLIP_HORIZONTAL);
	}else{
		al_draw_bitmap_region(perso->sprite,xsprite,ysprite,PERSO_LENGTH_SPRITE,PERSO_HEIGHT_SPRITE,xdest,ydest,0);
	}
}

void perso_dest(Personnage* perso){
	al_destroy_bitmap(perso->sprite);
	free(perso);
}

void perso_touche(Personnage* perso, int degats){
	perso->point_vie -= degats;
}

void perso_rend_vie(Personnage* perso, int nbsoin){
	if((perso->point_vie + nbsoin) > perso->point_vie_initial){
		perso->point_vie = perso->point_vie_initial; 
	}else{
		perso->point_vie += nbsoin;
	}
}

void perso_rend_munition(Personnage* perso){
	perso->balles_handgun = 10;
	perso->balles_rifle = 15;
	perso->grenades = 5;
}

bool perso_est_mort(Personnage* perso){
	return perso->point_vie<=0;
}

bool perso_mourir(Personnage* perso,Map* map){
	static int tempo = 2;
	if(perso->width == 5 && perso->height == 0 && perso_est_mort(perso)){
		return true;
	}else{
		tempo-=1;
		if(tempo < 0){
			perso->height = 0;
			perso->width = perso->width+1;
			tempo = 2;
		}
		return false;
	}
}

void perso_ressusciter(Personnage* perso){
	perso->point_vie_initial=PERSO_VIE;
	perso->point_vie=perso->point_vie_initial;
	perso->x = perso->x -200;
	perso->y = perso->y -200;
	perso->nbvie -= 1;
	perso->balles_rifle = 30;
	perso->balles_handgun = 15;
	perso->grenades = 5;
}