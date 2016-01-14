 #include "ennemi.h"


Ennemi* ennemi_init(int x, int y,char* sprite){
	Ennemi* ennemi = (Ennemi*)malloc(sizeof(Ennemi));
	ennemi->bot = perso_init(x, y,sprite);
	ennemi->bot->point_vie_initial=10;
	ennemi->bot->point_vie=ennemi->bot->point_vie_initial;
	ennemi->etat=CHERCHE;
	ennemi->dir=DIR_GAUCHE;
	ennemi->change_etat=false;
	return ennemi;
}

void ennemi_dest(void* ennemi){
	Ennemi* e = (Ennemi*)ennemi;
	free(e->bot);
	free(e);
}

/* 
IA de l'ennemi
s'il voit le perso dans son champ de vision, alors il passe en mode attaque
sinon il reste en mode cherche avec une rotation sur lui meme
*/
void ennemi_update_etat(Ennemi* ennemi,Jeu* jeu){
	if(perso_collision_dessous(jeu->perso,ennemi->bot)){
		perso_touche(ennemi->bot,10);
	}
	if( ennemis_vision(ennemi, jeu) && !perso_est_mort(jeu->perso)){
		ennemi->change_etat=true;
		ennemi->etat=ATTAQUE;
		ennemi_attaque(ennemi,jeu);
	}else{
		if(ennemi->change_etat){
			ennemi->dir=(ennemi->dir+1)%DIR_COUNT;
			ennemi->change_etat=false;
		}else{
			ennemi->etat=CHERCHE;
			perso_arret(ennemi->bot);
			ennemi_cherche(ennemi,jeu->perso);
		}
	}
}

bool ennemis_vision(Ennemi* ennemi, Jeu* jeu){

	int distx =  jeu->perso->x - ennemi->bot->x;
	int disty = abs(ennemi->bot->y - jeu->perso->y);
	bool vue = (distx<=0 && ennemi->dir==DIR_GAUCHE) || (distx>=0 && ennemi->dir==DIR_DROITE);

	
	int xe = (ennemi->bot->x/LENGTH_SPRITE)+1;
	int ye = ennemi->bot->y/LENGTH_SPRITE+1;

	int xp = (jeu->perso->x/LENGTH_SPRITE)+1;
	int yp = jeu->perso->y/LENGTH_SPRITE+1;
	

	float xy = sqrt (abs((xe-xp)*(xe-xp)) + abs((ye-yp)+(ye-yp)));

	float dx = (xp-xe)/xy;
	float dy = (yp-ye)/xy;


	float i;


	if(abs(distx) <= ENNEMI_RAYON && disty <= ENNEMI_ANGLE && vue){
		for(i=0;i<xy;i+=1){
			if((int)(ye+(i*dy))>=0 && (int)(ye+(i*dy)) < jeu->map->h && (int)(xe+(i*dx))>=0 && (int)(xe+(i*dx)) < jeu->map->w){
				if(jeu->map->map[(int)(ye+(i*dy))][(int)(xe+(i*dx))]->transparent==false){		
					return false;
				}
			}
		}
		return true;
	}
	return false;
}


void ennemi_attaque(Ennemi* ennemi, Jeu* jeu){
	int attaque = rand()%ENNEMI_DIFFICULTE;
	int distx =  abs(jeu->perso->x - ennemi->bot->x);
	/*si le perso est assez proche mais pas trop, il avance vers lui*/
	if( distx > ENNEMI_DISTANCE_MIN ){
		if(!perso_mouv_horizontal(ennemi->bot,jeu->map,(ENNEMI_VITESSE*(ennemi->dir==DIR_DROITE?1:-1)))){
			perso_mouv_sauter(ennemi->bot,jeu->map);
		}
	}else{
		/* si le perso est trop proche, il s'arrete*/
		perso_arret(ennemi->bot);
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
		Projectile* proj = projectile_init(TYPE_BALLE,ennemi->bot->x + PERSO_LENGTH_SPRITE/2, ennemi->bot->y + PERSO_HEIGHT_SPRITE/2,jeu->perso->x+ PERSO_LENGTH_SPRITE/2,jeu->perso->y+ PERSO_HEIGHT_SPRITE/2,0,0);
		liste_add_last(jeu->projectiles,(void*)proj);
	}
}

void ennemi_cherche(Ennemi* ennemi, Personnage* perso){
	static int rotation = 0;
	/* la rotation est effectué suivant un compteur */
	if(rotation==ENNEMI_ROTATION){
		ennemi->dir=(ennemi->dir+1)%DIR_COUNT;
		rotation=0;
	}else{
		rotation++;
	}
}

void ennemi_draw(Ennemi* ennemi,Map* map){


	int xdest = ennemi->bot->x-map->x;
	int ydest = ennemi->bot->y-map->y;

	int xsprite = 1+(ennemi->bot->width * PERSO_LENGTH_SPRITE);
	int ysprite = 1+(ennemi->bot->height * PERSO_HEIGHT_SPRITE);
	perso_est_dans_liquide_tueur(ennemi->bot,map);
	float pourcent = (float)(ennemi->bot->point_vie)/(float)ennemi->bot->point_vie_initial;
	al_draw_line(xdest,ydest,xdest+(pourcent*PERSO_LENGTH_SPRITE),ydest,al_map_rgb(0,255,0),1);
	al_draw_line(xdest+(pourcent*PERSO_LENGTH_SPRITE),ydest,xdest+PERSO_LENGTH_SPRITE,ydest,al_map_rgb(255,0,0),1);


	al_draw_bitmap_region(ennemi->bot->sprite,xsprite,ysprite,PERSO_LENGTH_SPRITE-1,PERSO_HEIGHT_SPRITE-1,xdest,ydest,ennemi->dir);	

}

List* ennemi_liste_init(char* map_src){

	List* l_ennemis = liste_init();

	FILE* src = fopen(map_src,"r");
	
	if(src==NULL){
		fprintf(stderr, "Erreur lors du chargement des ennemis\n");
		return NULL;
	}
	char* ennemies = "#ennemies";
	char* ligne = (char*)malloc(1000*sizeof(char));

	while((fgets(ligne,1000*sizeof(char),src)!= NULL) && (strncmp(ligne,ennemies,8) != 0)){
	}

	/* TODO ameliorer ça ! */

	while(fgets(ligne,1000*sizeof(char),src)!= NULL && ligne[0] != '$'){

		if(strlen(ligne)>0){
			char* token;
			const char* sep = ",";
			token = strtok(ligne,sep);
			int i = 0;
			int x,y;
			char* type_ennemi = (char*)malloc(10*sizeof(char));
			type_ennemi = token;

			while(token!=NULL){
				token = strtok(NULL,",");
				switch(i) {
					case 0 : x = atoi(token);
					break;
					case 1 : y = atoi(token);
					break;
				}
				i++;
			}

			Ennemi* e = NULL;

			if(strncmp(type_ennemi,"BASE1",5) == 0){
				e = ennemi_init(x,y,"img/sprite_perso.png");
			}else if (strncmp(type_ennemi,"BASE2",5 == 0)){
				e = ennemi_init(x,y,"img/sprite_perso.png");
			}else{
				e = ennemi_init(x,y,"img/sprite_perso.png");
			}

			liste_add_last(l_ennemis,(void*)e);
		}
	}
	fclose(src);
	return l_ennemis;

}

