#include "boss.h"

Boss* boss_init(int x, int y,char* sprite_src){
	Boss* boss = (Boss*)malloc(sizeof(Boss));
	boss->x=x;
	boss->y=y;
	boss->width=0;
	boss->height=0;
	boss->point_vie_initial=BOSS_HP;
	boss->point_vie=boss->point_vie_initial;
	boss->sprite = al_load_bitmap(sprite_src);
	if(boss->sprite==NULL){
		perror("Perso_init:");
		fprintf(stderr, "Erreur lors du chargement du sprite du personnage\n");
		return NULL;
	}
	return boss;
}
void boss_update_etat(Boss* boss, Jeu* jeu){
	int distx =  jeu->perso->x - boss->x;
	/*int disty = abs(boss->y - perso->y);*/
	if(abs(distx) <= BOSS_RAYON  && !perso_est_mort(jeu->perso)){
		boss->etat=BOSS_ATTAQUE;
		boss_attaque(boss, jeu);
	}else{
		boss->etat=BOSS_CHERCHE;
		boss_arret(boss);
		boss_cherche(boss,jeu);
	}
}

bool boss_mouv_horizontal(Boss* boss,Map* map,int xtemp){
	bool res=true;
	if(boss->x+xtemp+BOSS_LENGTH_SPRITE<map->w*LENGTH_SPRITE && boss->x >= 0){
		int i;
		int mult = (xtemp<0?-1:1);
		for(i=1;i<=xtemp*mult;i++){
			boss->x +=1*mult;

			if(boss_collision(boss,map)){
				boss->x -= 1*mult;
				res=false;
				break;
			}
		}
	}else{
		if(boss->x<0){
			boss->x=0;
		}
	}
	return res;
}

bool boss_mouv_vertical(Boss* boss,Map* map,int ytemp){
	bool res=true;
	if(boss->y+ytemp+BOSS_HEIGHT_SPRITE<map->w*LENGTH_SPRITE && boss->y >= 0){
		int i;
		int mult = (ytemp<0?-1:1);
		for(i=1;i<=ytemp*mult;i++){
			boss->x +=1*mult;

			if(boss_collision(boss,map)){
				boss->y -= 1*mult;
				res=false;
				break;
			}
		}

	}else{
		if(boss->y<0){
			boss->y=0;
		}
	}
	return res;
}

void boss_cherche(Boss* boss, Jeu* jeu){
	static int rotation = 0;
	/* la rotation est effectué suivant un compteur */
	if(rotation==BOSS_ROTATION){
		boss->dir=(boss->dir+1)%BOSS_DIR_COUNT;
		rotation=0;
	}else if(rotation == BOSS_DISTANCE_CHERCHE){
		boss->dir = (boss->dir+1)%BOSS_DIR_COUNT;
	}else{
		rotation++;
		boss_mouv_horizontal(boss, jeu->map, (BOSS_VITESSE*(boss->dir==BOSS_DIR_DROITE?1:-1)) );
	}
}

void boss_attaque(Boss* boss, Jeu* jeu){
	int attaque = rand()%BOSS_DIFFICULTE;
	int distx =  abs(jeu->perso->x - boss->x);
	/*si le perso est assez proche mais pas trop, il avance vers lui*/
	if( distx > BOSS_DISTANCE_MIN ){
		if(!boss_mouv_horizontal(boss,jeu->map,(BOSS_VITESSE*(boss->dir==BOSS_DIR_DROITE?1:-1)))){
		}
	}else{
		/* si le perso est trop proche, il s'arrete*/
		boss_arret(boss);
	}
	/* il attaque de maniere aleatoire suiant la difficulté*/
	if(attaque==0){
		if(jeu->Gun_sound != NULL){
			ALLEGRO_SAMPLE_INSTANCE* Gun_sound_instance;
			Gun_sound_instance = al_create_sample_instance(jeu->Gun_sound);
			al_set_sample_instance_playmode(Gun_sound_instance,ALLEGRO_PLAYMODE_ONCE);
			al_set_sample_instance_gain(Gun_sound_instance,0.5);
			al_set_sample_instance_speed(Gun_sound_instance,1.5);
			al_attach_sample_instance_to_mixer(Gun_sound_instance,jeu->mixer);
			al_play_sample_instance(Gun_sound_instance);
		}
		Projectile* proj1 = projectile_boss_init(TYPE_BALLE,boss->x + BOSS_LENGTH_SPRITE/2, boss->y + BOSS_HEIGHT_SPRITE/2,jeu->perso->x+ PERSO_LENGTH_SPRITE/2 + 50,jeu->perso->y+ PERSO_HEIGHT_SPRITE/2,0,0);
		liste_add_last(jeu->projectiles,(void*)proj1);
		Projectile* proj2 = projectile_boss_init(TYPE_BALLE,boss->x + BOSS_LENGTH_SPRITE/2, boss->y + BOSS_HEIGHT_SPRITE/2,jeu->perso->x+ PERSO_LENGTH_SPRITE/2 +25,jeu->perso->y+ PERSO_HEIGHT_SPRITE/2,0,0);
		liste_add_last(jeu->projectiles,(void*)proj2);
		Projectile* proj3 = projectile_boss_init(TYPE_BALLE,boss->x + BOSS_LENGTH_SPRITE/2, boss->y + BOSS_HEIGHT_SPRITE/2,jeu->perso->x+ PERSO_LENGTH_SPRITE/2,jeu->perso->y+ PERSO_HEIGHT_SPRITE/2,0,0);
		liste_add_last(jeu->projectiles,(void*)proj3);
		Projectile* proj4 = projectile_boss_init(TYPE_BALLE,boss->x + BOSS_LENGTH_SPRITE/2, boss->y + BOSS_HEIGHT_SPRITE/2,jeu->perso->x+ PERSO_LENGTH_SPRITE/2 -25,jeu->perso->y+ PERSO_HEIGHT_SPRITE/2,0,0);
		liste_add_last(jeu->projectiles,(void*)proj4);
		Projectile* proj5 = projectile_boss_init(TYPE_BALLE,boss->x + BOSS_LENGTH_SPRITE/2, boss->y + BOSS_HEIGHT_SPRITE/2,jeu->perso->x+ PERSO_LENGTH_SPRITE/2 -50,jeu->perso->y+ PERSO_HEIGHT_SPRITE/2,0,0);
		liste_add_last(jeu->projectiles,(void*)proj5);
	}
}


void boss_arret(Boss* boss){
	boss->width = 0;
	boss->height= 0;
}



bool boss_collision(Boss* boss, Map* map){
	int x = boss->x/LENGTH_SPRITE;
	int y = boss->y/LENGTH_SPRITE;

	int largeur = BOSS_LENGTH_SPRITE/LENGTH_SPRITE+1;
	int hauteur = BOSS_HEIGHT_SPRITE/LENGTH_SPRITE+1;

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


void boss_draw(Boss* boss,Map* map){
	int xdest = boss->x-map->x;
	int ydest = boss->y-map->y;

	int xsprite = 1+(boss->width * (BOSS_LENGTH_SPRITE+1) );
	int ysprite = 1+(boss->height * (BOSS_HEIGHT_SPRITE+1) );
	float pourcent = (float)(boss->point_vie)/(float)boss->point_vie_initial;
	al_draw_line(xdest,ydest,xdest+(pourcent*BOSS_LENGTH_SPRITE),ydest,al_map_rgb(0,255,0),1);
	al_draw_line(xdest+(pourcent*BOSS_LENGTH_SPRITE),ydest,xdest+BOSS_LENGTH_SPRITE,ydest,al_map_rgb(255,0,0),1);

	al_draw_bitmap_region(boss->sprite,xsprite,ysprite,BOSS_LENGTH_SPRITE,BOSS_HEIGHT_SPRITE,xdest,ydest,0);
}

void boss_dest(Boss* boss){
	al_destroy_bitmap(boss->sprite);
	free(boss);
}

void boss_touche(Boss* boss, int degats){
	boss->point_vie -= degats;
}

bool boss_est_mort(Boss* boss){
	return boss->point_vie<=0;
}

bool boss_mourir(Boss* boss,Map* map){
	static int tempo = 2;
	if(boss->width == 5 && boss->height == 0 && boss_est_mort(boss)){
		return true;
	}else{
		tempo-=1;
		if(tempo < 0){
			boss->height = 0;
			boss->width = boss->width+1;
			tempo = 2;
		}
		return false;
	}
}

