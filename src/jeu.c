#include "jeu.h"

Jeu* jeu_init(char* map_src){
	Jeu* jeu = (Jeu*)malloc(sizeof(Jeu));
	jeu->map = map_init(map_src,DISPLAY_W,DISPLAY_H);
	jeu->ennemis = liste_init();
	jeu->boss = liste_init();
	jeu->medikit = liste_init();
	jeu->munition = liste_init();
	jeu->perso = perso_init(30,30,"img/sprite_perso.png");
	if(jeu_parse(jeu,map_src) == 0){
	}
	jeu->cursor = al_load_bitmap("img/viseur.png");
	if(!jeu->cursor){
		fprintf(stderr, "Erreur curseur\n");

	}

	jeu->font = al_load_ttf_font("font/font_menu.ttf",24,0);
	if(!jeu->font){
		fprintf(stderr, "Erreur lors du chargement du font\n");
		return NULL;
	}

	jeu->font_jeu = al_load_ttf_font("font/Minecraftia.ttf",16,0);
	if(!jeu->font_jeu){
		fprintf(stderr, "Erreur lors du chargement du font\n");
		return NULL;
	}

	if(al_is_audio_installed()){
		jeu->mixer = al_get_default_mixer();
		jeu->Gun_sound = al_load_sample("sounds/Handgun.ogg");
		jeu->Gun_sound_empty = al_load_sample("sounds/dry_fire.ogg");
		jeu->Rifle_sound = al_load_sample("sounds/Rifle.ogg");
		jeu->Grenade_sound = al_load_sample("sounds/grenade.ogg");
		if (!jeu->Gun_sound){
			fprintf(stderr, "Erreur lors du chargement du gun_sound\n");
			return NULL;
		}
		if(!jeu->Gun_sound_empty){
			fprintf(stderr, "Erreur lors du chargement du gun_sound_empty\n");
			return NULL;
		}
		if(!jeu->Rifle_sound){
			fprintf(stderr, "Erreur lors du chargement du Rifle_sound\n");
			return NULL;
		}
		if(!jeu->Grenade_sound){
			fprintf(stderr, "Erreur lors du chargement du Grenade_sound\n");
			return NULL;
		}

	}else{
		jeu->Gun_sound=NULL;
		jeu->Gun_sound_empty=NULL;
		jeu->Rifle_sound=NULL;
	}



	jeu->projectiles = liste_init();
	jeu->menu = menu_init();
	jeu->ev = event_init(EVENT_PERIPH_XBOX);
	jeu->boucle_jeu=true;
	return jeu;
}

void jeu_dest(Jeu* jeu){
	map_dest(jeu->map);
	perso_dest(jeu->perso);
	liste_dest(jeu->ennemis,ennemi_dest);
	liste_dest(jeu->projectiles,projectile_dest);
	menu_dest(jeu->menu);
	free(jeu);
}

int jeu_parse(Jeu* jeu, char* map_src){	
	
	FILE* src = fopen(map_src,"r");
	char* ligne=NULL;
	int x,y,i;
	int x_perso;
	int y_perso;
	char* type_ennemi=NULL;
	Ennemi* e = NULL;
	Boss* b = NULL;

	if(src==NULL){
		fprintf(stderr, "Erreur lors du chargement du sprite\n");
		return -1;
	}

	ligne = (char*)malloc((MAX_W_LINE_MAP)*sizeof(char));

	jeu->map->h=-1;
	jeu->map->w=-1;
	jeu->map->type_map=-1;
	jeu->map->sprite=NULL;
	jeu->map->background=NULL;
	jeu->map->musique=NULL;


	while(fgets(ligne,1000*sizeof(char),src)!= NULL && ligne[0] != '#'){
		continue;
	}
	while(fgets(ligne,1000*sizeof(char),src)!= NULL && ligne[0] != '$'){
		if(strlen(ligne)>0){
			char* token;
			const char* sep = ",";
			token = strtok(ligne,sep);
			i = 0;
			
			type_ennemi = (char*)malloc(10*sizeof(char));
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


			if(strncmp(type_ennemi,"BASE1",5) == 0){
				e = ennemi_init(x,y,"img/sprite_ennemi.png");
			}else if (strncmp(type_ennemi,"BASE2",5) == 0){
				e = ennemi_init(x,y,"img/sprite_ennemi.png");
			}else{
				b = boss_init(x,y,"img/sprite_boss.png");
			}

			if(e!=NULL){
				liste_add_last(jeu->ennemis,(void*)e);
				e=NULL;
			}else if(b!=NULL){
				liste_add_last(jeu->boss,(void*)b);
				b=NULL;
			}
		}
	}

	while(fgets(ligne,1000*sizeof(char),src)!= NULL && ligne[0] != '@'){
		if(ligne[0]=='x'){
			x_perso=atoi((char*)(ligne+2));
			continue;
		}
		if(ligne[0]=='y'){
			y_perso=atoi((char*)(ligne+2));
			continue;
		}
	}
	jeu->perso->x = x_perso;
	jeu->perso->y = y_perso; 


	int ligne_courante=0;
	int init=0;
	while(fgets(ligne,MAX_W_LINE_MAP,src)!= NULL){
		if(strlen(ligne)>0){
			
			if(jeu->map->h!=-1 && jeu->map->w != -1 && jeu->map->musique != NULL && jeu->map->sprite !=NULL && jeu->map->background !=NULL && jeu->map->type_map != -1 && init==0){
				jeu->map->map = (Tile***)malloc(jeu->map->h*sizeof(Tile**));
				int i;
				for(i=0;i<jeu->map->h;i++){
					jeu->map->map[i] = (Tile**)malloc(jeu->map->w*sizeof(Tile*));
				}
				init=1;
			}

			if(ligne[0]=='h'){
				jeu->map->h=atoi((char*)(ligne+2));
				continue;
			}
			if(ligne[0]=='w'){
				jeu->map->w=atoi((char*)(ligne+2));
				continue;
			}
			if(ligne[0]=='t'){
				jeu->map->type_map = atoi((char*)(ligne+2));
				continue;
			}
			if(ligne[0]=='b'){
				if(ligne[strlen(ligne)-1] == '\n'){
					ligne[strlen(ligne)-1] = '\0';
				}	
				jeu->map->background = al_load_bitmap(ligne+2);
				if(jeu->map->background==NULL){
					fprintf(stderr, "Erreur lors du chargement du background\n");
					return -1;
				}
				continue;
			}

			if(ligne[0]=='s'){
				if(ligne[strlen(ligne)-1] == '\n'){
					ligne[strlen(ligne)-1] = '\0';
				}	
				jeu->map->sprite = al_load_bitmap(ligne+2);
				if(jeu->map->sprite==NULL){
					fprintf(stderr, "Erreur lors du chargement du sprite\n");
					return -1;
				}
				continue;
			}

			if(ligne[0]=='m'){
				if(ligne[strlen(ligne)-1] == '\n'){
					ligne[strlen(ligne)-1] = '\0';
				}	
				if(al_is_audio_installed()){
					jeu->map->musique = al_load_sample(ligne+2);
					if (!jeu->map->musique){
						fprintf(stderr, "Erreur lors du chargement de la musique\n");
						return -1;
					}
				}else{
					jeu->map->musique=NULL;
				}
				jeu->map->is_playing=false;
				continue;
			}

			if(ligne[0]=='@'){
				continue;
			}

			
			
			map_fill(jeu->map,ligne_courante, ligne);
			ligne_courante++;

			if(ligne_courante==jeu->map->h){
				break;
			}

		}
	}



	fclose(src);
	free(ligne);
	return 0;
}


void jeu_draw(Jeu* jeu){
	
	al_clear_to_color(al_map_rgb(255,255,255));



	/* mise a jour de la map avec la position du joueur */
	map_update_frame_player(jeu->map,jeu->perso->x,jeu->perso->y);

	/* dessin de la map */
	map_draw(jeu->map);
	
	/* Petit menu lors d'un GAME OVER */
	if(perso_est_mort(jeu->perso)){
		menu_affiche_mort(jeu->menu,jeu);
		if(event_get_ok_court(jeu->ev)){
			perso_ressusciter(jeu->perso);
			if(jeu->perso->nbvie <= 0){
				jeu->boucle_jeu = false;
			}
		}
		if(perso_mourir(jeu->perso,jeu->map) == false){
			perso_draw(jeu->perso,jeu->map,300.0,300.0);
		}
		perso_draw(jeu->perso,jeu->map,300.0,300.0);
	}else{	
		/* s'il est pas mort on affiche le perso */
		if(!menu_est_actif(jeu->menu)){
			perso_gravite(jeu->perso,jeu->map);
		}
		perso_draw(jeu->perso,jeu->map,event_get_x(jeu->ev),event_get_y(jeu->ev));
	}
	

	/* Projectiles */ 
	liste_traiter_foncteur(jeu->projectiles,((void*)jeu),jeu_parcours_collision_projectiles);
	
	/* Ennemis */
	liste_traiter_foncteur(jeu->ennemis,(void*)jeu,jeu_parcours_collision_ennemis);
	
	/* Medikit */
	liste_traiter_foncteur(jeu->medikit,(void*)jeu,jeu_parcours_collision_medikit);
	
	/* Munition */
	liste_traiter_foncteur(jeu->munition,(void*)jeu,jeu_parcours_collision_munition);

	/* Boss */
	liste_traiter_foncteur(jeu->boss,(void*)jeu,jeu_parcours_collision_boss);
	

	/* HUD */

	/* points de vie */
	int x0=10;
	float x1=x0+200*((float)(jeu->perso->point_vie)/(float)(jeu->perso->point_vie_initial));
	int x2=210;
	al_draw_line(x0,10,x1,10,al_map_rgb(0,255,0),10);
	al_draw_line(x1,10,x2,10,al_map_rgb(255,0,0),10);

	/* avancée dans la map */
	/*
	float x1=x0+200*((float)(jeu->perso->point_vie)/(float)(jeu->perso->point_vie_initial));
	int x2=210;
	al_draw_line(x0,10,x1,10,al_map_rgb(0,255,0),10);
	al_draw_line(x1,10,x2,10,al_map_rgb(255,0,0),10);
	*/
	/* Ennemis restants */ 
	if(jeu->map->type_map == 1){	
		char ennemies_left[3];
		sprintf(ennemies_left,"%d",jeu->ennemis->size + jeu->boss->size);
		al_draw_text(jeu->font_jeu,al_map_rgb(0,0,0),300,10,ALLEGRO_ALIGN_LEFT,"ennemies left : ");
		al_draw_text(jeu->font_jeu,al_map_rgb(0,0,0),450,10,ALLEGRO_ALIGN_LEFT,ennemies_left);
	} else {
		al_draw_text(jeu->font_jeu,al_map_rgb(0,0,0),300,10,ALLEGRO_ALIGN_LEFT,"Go to the end!");
	}
	
	char bullet_left[4];
	switch(jeu->perso->arme_courante){
		case HANDGUN:
		sprintf(bullet_left,"%d",jeu->perso->balles_handgun);
		al_draw_text(jeu->font_jeu,al_map_rgb(0,0,0),50,20,ALLEGRO_ALIGN_LEFT,bullet_left);
		break;
		case RIFLE:
		sprintf(bullet_left,"%d",jeu->perso->balles_rifle);
		al_draw_text(jeu->font_jeu,al_map_rgb(0,0,0),50,20,ALLEGRO_ALIGN_LEFT,bullet_left);
		break;
		case GRENADE:
		sprintf(bullet_left,"%d",jeu->perso->grenades);
		al_draw_text(jeu->font_jeu,al_map_rgb(0,0,0),50,20,ALLEGRO_ALIGN_LEFT,bullet_left);
		break;
		default :

		break;
	}
	
	int k;
	for(k=0;k<jeu->perso->nbvie;k++){
		al_draw_bitmap_region(jeu->perso->sprite_vie,0,0,12,12,215+(10*k),5,0);
	}

	al_draw_bitmap_region(jeu->perso->sprite_arme_interface,(37)*jeu->perso->arme_courante,0,36,17,10,25,0);

	/* viseur de souris */
	al_draw_bitmap(jeu->cursor,event_get_x(jeu->ev)-8,event_get_y(jeu->ev)-8,0);

	/* Menu */
	if(menu_est_actif(jeu->menu)){
		menu_draw(jeu->menu,jeu);
	}

	
	event_reset(jeu->ev);
	al_flip_display();

}


bool jeu_update_event(Jeu* jeu){
	if(menu_root(jeu->menu)){
		if(event_get_menu_court(jeu->ev)){
			menu_toggle(jeu->menu);
		}
	}

	/* Menu actif */
	if(!menu_est_actif(jeu->menu)){

		/* On execute les events */

		/* on active/desactive le son */
		if(event_get_son(jeu->ev)){
			map_toggle_musique(jeu->map);
		}

		/* deplacement vers la gauche */
		if(event_get_gauche(jeu->ev) && !perso_est_mort(jeu->perso)){
			perso_update_frame_player(jeu->perso,jeu->map,-VITESSE,0); 
			perso_collision(jeu->perso, jeu->map);
		}

		/* deplacement vers la droite*/
		if(event_get_droit(jeu->ev) && !perso_est_mort(jeu->perso)){
			perso_update_frame_player(jeu->perso,jeu->map,VITESSE,0); 
			perso_collision(jeu->perso, jeu->map);
		}

		/* aucun deplacement -> remise en etat 'debout' du perso*/
		if(!event_get_gauche(jeu->ev) && !event_get_droit(jeu->ev) && !perso_est_mort(jeu->perso)){
			perso_arret(jeu->perso);
		}

		/* saut du perso */
		if(event_get_saut(jeu->ev) && !perso_est_mort(jeu->perso)){
			perso_mouv_sauter(jeu->perso,jeu->map);	
			perso_update_frame_player(jeu->perso,jeu->map,0,-VITESSE);
		}

		/* tire du perso */
		if(event_get_clic_gauche_court(jeu->ev) && !perso_est_mort(jeu->perso)){

			bool tire_ok = false;
			switch(jeu->perso->arme_courante){
				case HANDGUN:
				if(jeu->perso->balles_handgun > 0){
					tire_ok = true;
					jeu->perso->balles_handgun -= 1;
				}
				break;
				case RIFLE:
				if(jeu->perso->balles_rifle > 0){
					tire_ok = true;
					jeu->perso->balles_rifle -= 1;
				}
				break; 
				case GRENADE:
				if(jeu->perso->grenades > 0){
					tire_ok = true;
					jeu->perso->grenades -= 1;
				}
				break; 
				default :
				tire_ok = false;
				break;
			}

			if(tire_ok){

				if(jeu->Gun_sound != NULL){
					ALLEGRO_SAMPLE_INSTANCE* Gun_sound_instance;
					ALLEGRO_SAMPLE_INSTANCE* Rifle_sound_instance;
					switch(jeu->perso->arme_courante){
						case HANDGUN:

						Gun_sound_instance = al_create_sample_instance(jeu->Gun_sound);
						al_set_sample_instance_playmode(Gun_sound_instance,ALLEGRO_PLAYMODE_ONCE);
						al_set_sample_instance_gain(Gun_sound_instance,0.5);
						al_set_sample_instance_speed(Gun_sound_instance,1.5);
						al_attach_sample_instance_to_mixer(Gun_sound_instance,jeu->mixer);
						al_play_sample_instance(Gun_sound_instance);

						Projectile* proj = projectile_init(TYPE_BALLE,jeu->perso->x+PERSO_LENGTH_SPRITE/2,jeu->perso->y+PERSO_HEIGHT_SPRITE/2,event_get_x(jeu->ev),event_get_y(jeu->ev),jeu->map->x,jeu->map->y);
						liste_add_last(jeu->projectiles,(void*)proj);
						break;
						case RIFLE:

						Rifle_sound_instance = al_create_sample_instance(jeu->Rifle_sound);
						al_set_sample_instance_playmode(Rifle_sound_instance,ALLEGRO_PLAYMODE_ONCE);
						al_set_sample_instance_gain(Rifle_sound_instance,0.5);
						al_set_sample_instance_speed(Rifle_sound_instance,1.5);
						al_attach_sample_instance_to_mixer(Rifle_sound_instance,jeu->mixer);
						al_play_sample_instance(Rifle_sound_instance);

						Projectile* proj1 = projectile_init(TYPE_BALLE,jeu->perso->x+PERSO_LENGTH_SPRITE/2,jeu->perso->y+PERSO_HEIGHT_SPRITE/2,event_get_x(jeu->ev),event_get_y(jeu->ev)+5,jeu->map->x,jeu->map->y);
						liste_add_last(jeu->projectiles,(void*)proj1);
						Projectile* proj2 = projectile_init(TYPE_BALLE,jeu->perso->x+PERSO_LENGTH_SPRITE/2,jeu->perso->y+PERSO_HEIGHT_SPRITE/2,event_get_x(jeu->ev),event_get_y(jeu->ev),jeu->map->x,jeu->map->y);
						liste_add_last(jeu->projectiles,(void*)proj2);
						Projectile* proj3 = projectile_init(TYPE_BALLE,jeu->perso->x+PERSO_LENGTH_SPRITE/2,jeu->perso->y+PERSO_HEIGHT_SPRITE/2,event_get_x(jeu->ev),event_get_y(jeu->ev)-5,jeu->map->x,jeu->map->y);
						liste_add_last(jeu->projectiles,(void*)proj3);
						break;

						case GRENADE:
						liste_add_last(jeu->projectiles,(void*)projectile_init(TYPE_GRENADE,jeu->perso->x+PERSO_LENGTH_SPRITE/2,jeu->perso->y+PERSO_HEIGHT_SPRITE/2,event_get_x(jeu->ev),event_get_y(jeu->ev),jeu->map->x,jeu->map->y));

						break;
						default :
						break;
					}
				}

				

			}else{

				if(jeu->Gun_sound_empty != NULL){
					ALLEGRO_SAMPLE_INSTANCE* Gun_sound_empty_instance;
					Gun_sound_empty_instance = al_create_sample_instance(jeu->Gun_sound_empty);
					al_set_sample_instance_playmode(Gun_sound_empty_instance,ALLEGRO_PLAYMODE_ONCE);
					al_set_sample_instance_gain(Gun_sound_empty_instance,0.5);
					al_set_sample_instance_speed(Gun_sound_empty_instance,1.5);
					al_attach_sample_instance_to_mixer(Gun_sound_empty_instance,jeu->mixer);
					al_play_sample_instance(Gun_sound_empty_instance);
				}




			}
		}

		/* Changement d'arme */
		if(event_get_weapon_change_court(jeu->ev) && !perso_est_mort(jeu->perso)){
			perso_change_arme(jeu->perso);
		}

	}else{
		/* Si le menu est actif, on navigue dedans*/

		/* vers le haut */
		if(event_get_haut_court(jeu->ev)){
			menu_monter(jeu->menu);
		}
		/* vers le bas */
		if(event_get_bas_court(jeu->ev)){
			menu_descendre(jeu->menu);
		}
		/* on valide le menu et execute l'action suivant le cas*/
		if(event_get_ok_court(jeu->ev)){
			menu_activer(jeu->menu,jeu);
		}
		if(event_get_menu_court(jeu->ev)){
			menu_retour(jeu->menu);
		}
	}

	/* on recupere les coordonné de la souris/joystick pour le viseur*/
	event_update_x_y(jeu->ev,VITESSE);
	return jeu->boucle_jeu;

}




void jeu_parcours_collision_ennemis(Elem* e,void* jeu){
	Jeu* j = (Jeu*)jeu;
	if(perso_collision_dessous(j->perso, ((Ennemi*)(e->object))->bot)){
		if(((Ennemi*)(e->object))->bot->width > 5){
			int chance_medikit = rand()%4;
			if(chance_medikit == 0){
				Medikit* medikit = medikit_init(((Ennemi*)(e->object))->bot->x,((Ennemi*)(e->object))->bot->y,"img/sprite_medikit.png");
				liste_add_last(j->medikit,(void*)medikit);
			}
			int chance_munition = rand()%4;
			if(chance_munition == 0){
				Munition* munition = munition_init(((Projectile*)(e->object))->x,((Projectile*)(e->object))->y,"img/sprite_munition.png");
				liste_add_last(j->munition,(void*)munition);
			}
			liste_supprime(j->ennemis,e);
		}
		if(perso_mourir(((Ennemi*)(e->object))->bot,j->map)) {
			int chance_medikit = rand()%4;
			if(chance_medikit == 0){
				Medikit* medikit = medikit_init(((Ennemi*)(e->object))->bot->x,((Ennemi*)(e->object))->bot->y,"img/sprite_medikit.png");
				liste_add_last(j->medikit,(void*)medikit);
			}
			int chance_munition = rand()%4;
			if(chance_munition == 0){
				Munition* munition = munition_init(((Projectile*)(e->object))->x,((Projectile*)(e->object))->y,"img/sprite_munition.png");
				liste_add_last(j->munition,(void*)munition);
			}
			liste_supprime(j->ennemis,e);

		}else{
			ennemi_draw((Ennemi*)(e->object),j->map);
		}
	}
	if(perso_est_mort(((Ennemi*)(e->object))->bot)){
		if(((Ennemi*)(e->object))->bot->width > 5){
			int chance_medikit = rand()%4;
			if(chance_medikit == 0){
				Medikit* medikit = medikit_init(((Ennemi*)(e->object))->bot->x,((Ennemi*)(e->object))->bot->y,"img/sprite_medikit.png");
				liste_add_last(j->medikit,(void*)medikit);
			}
			int chance_munition = rand()%4;
			if(chance_munition == 0){
				Munition* munition = munition_init(((Ennemi*)(e->object))->bot->x-10,((Ennemi*)(e->object))->bot->y,"img/sprite_munition.png");
				liste_add_last(j->munition,(void*)munition);
			}
			liste_supprime(j->ennemis,e);
		}
		if(perso_mourir(((Ennemi*)(e->object))->bot,j->map)){
			int chance_medikit = rand()%4;
			if(chance_medikit == 0){
				Medikit* medikit = medikit_init(((Ennemi*)(e->object))->bot->x,((Ennemi*)(e->object))->bot->y,"img/sprite_medikit.png");
				liste_add_last(j->medikit,(void*)medikit);
			}
			int chance_munition = rand()%4;
			if(chance_munition == 0){
				Munition* munition = munition_init(((Ennemi*)(e->object))->bot->x-10,((Ennemi*)(e->object))->bot->y,"img/sprite_munition.png");
				liste_add_last(j->munition,(void*)munition);
			}
			liste_supprime(j->ennemis,e);
		}else{
			ennemi_draw((Ennemi*)(e->object),j->map);
		}
		
	}else{
		if(!menu_est_actif(((Jeu*)jeu)->menu)){
			perso_gravite(((Ennemi*)(e->object))->bot,((Jeu*)jeu)->map);
		}
		ennemi_draw((Ennemi*)(e->object),j->map);
		if(!menu_est_actif(((Jeu*)jeu)->menu)){
			ennemi_update_etat((Ennemi*)(e->object),j);
		}
	}
}

void jeu_parcours_collision_medikit(Elem* e,void* jeu){
	Jeu* j = (Jeu*)jeu;
	medikit_update_etat((Medikit*)(e->object),j);
	if(((((Medikit*)(e->object))->x) > j->perso->x) &&  ((((Medikit*)(e->object))->x) < j->perso->x + PERSO_LENGTH_SPRITE) && ((((Medikit*)(e->object))->y) > j->perso->y) && ((((Medikit*)(e->object))->y) < j->perso->y + PERSO_HEIGHT_SPRITE)){
		perso_rend_vie(j->perso,(((Medikit*)(e->object))->point_vie_rendu));
		liste_supprime(j->medikit,e);
	}else{
		medikit_draw((Medikit*)(e->object),j->map);
	}
}

void jeu_parcours_collision_munition(Elem* e,void* jeu){
	Jeu* j = (Jeu*)jeu;
	munition_update_etat((Munition*)(e->object),j);
	if(((((Munition*)(e->object))->x) > j->perso->x) &&  ((((Munition*)(e->object))->x) < j->perso->x + PERSO_LENGTH_SPRITE) && ((((Munition*)(e->object))->y) > j->perso->y) && ((((Munition*)(e->object))->y) < j->perso->y + PERSO_HEIGHT_SPRITE)){
		perso_rend_munition(j->perso);
		liste_supprime(j->munition,e);
	}else{
		munition_draw((Munition*)(e->object),j->map);
	}
}


void jeu_parcours_collision_boss(Elem* e,void* jeu){
	Jeu* j = (Jeu*)jeu;
	if(boss_est_mort((Boss*)(e->object))){
		if(((Boss*)(e->object))->width > 5){
			liste_supprime(j->boss,e);
		}
		if(boss_mourir(((Boss*)(e->object)),j->map)){
			liste_supprime(j->boss,e);
		}else{
			boss_draw((Boss*)(e->object),j->map);
		}
		
	}else{
		boss_draw((Boss*)(e->object),j->map);
		if(!menu_est_actif(((Jeu*)jeu)->menu)){
			boss_update_etat((Boss*)(e->object),j);
		}
	}
}

void jeu_parcours_collision_projectiles(Elem* e,void* jeu){
	Jeu* j = (Jeu*)jeu;


	if(projectile_collision_map((Projectile*)(e->object),j->map)){
		if(((Projectile*)(e->object))->type==TYPE_GRENADE){
			ALLEGRO_SAMPLE_INSTANCE* Grenade_sound_instance = al_create_sample_instance(j->Grenade_sound);
			al_set_sample_instance_playmode(Grenade_sound_instance,ALLEGRO_PLAYMODE_ONCE);
			al_set_sample_instance_gain(Grenade_sound_instance,0.5);
			al_set_sample_instance_speed(Grenade_sound_instance,1.5);
			al_attach_sample_instance_to_mixer(Grenade_sound_instance,j->mixer);
			al_play_sample_instance(Grenade_sound_instance);
		}
		projectile_dest((Projectile*)(e->object));
		liste_supprime(j->projectiles,e);
	}else{
		if(projectile_collision_perso((Projectile*)(e->object),j->perso)){
			if(((Projectile*)(e->object))->type==TYPE_GRENADE){
				ALLEGRO_SAMPLE_INSTANCE* Grenade_sound_instance = al_create_sample_instance(j->Grenade_sound);
				al_set_sample_instance_playmode(Grenade_sound_instance,ALLEGRO_PLAYMODE_ONCE);
				al_set_sample_instance_gain(Grenade_sound_instance,0.5);
				al_set_sample_instance_speed(Grenade_sound_instance,1.5);
				al_attach_sample_instance_to_mixer(Grenade_sound_instance,j->mixer);
				al_play_sample_instance(Grenade_sound_instance);
			}
			perso_touche(j->perso,((Projectile*)(e->object))->puissance);
			if(perso_est_mort(j->perso)){
				if(perso_mourir(j->perso,j->map)){
				}
				perso_draw(j->perso,j->map,300.0,300.0);
			}
			projectile_dest((Projectile*)(e->object));
			liste_supprime(j->projectiles,e);
		}else if(projectile_collision_ennemis((Projectile*)(e->object),j->ennemis)){
			if(((Projectile*)(e->object))->type==TYPE_GRENADE){
				ALLEGRO_SAMPLE_INSTANCE* Grenade_sound_instance = al_create_sample_instance(j->Grenade_sound);
				al_set_sample_instance_playmode(Grenade_sound_instance,ALLEGRO_PLAYMODE_ONCE);
				al_set_sample_instance_gain(Grenade_sound_instance,0.5);
				al_set_sample_instance_speed(Grenade_sound_instance,1.5);
				al_attach_sample_instance_to_mixer(Grenade_sound_instance,j->mixer);
				al_play_sample_instance(Grenade_sound_instance);
			}
			projectile_dest((Projectile*)(e->object));
			liste_supprime(j->projectiles,e);
		}else if(projectile_collision_boss((Projectile*)(e->object),j->boss)){
			if(((Projectile*)(e->object))->type==TYPE_GRENADE){
				ALLEGRO_SAMPLE_INSTANCE* Grenade_sound_instance = al_create_sample_instance(j->Grenade_sound);
				al_set_sample_instance_playmode(Grenade_sound_instance,ALLEGRO_PLAYMODE_ONCE);
				al_set_sample_instance_gain(Grenade_sound_instance,0.5);
				al_set_sample_instance_speed(Grenade_sound_instance,1.5);
				al_attach_sample_instance_to_mixer(Grenade_sound_instance,j->mixer);
				al_play_sample_instance(Grenade_sound_instance);
			}	
			int chance_medikit = rand()%32;
			if(chance_medikit == 0){
				Medikit* medikit = medikit_init(((Projectile*)(e->object))->x,((Projectile*)(e->object))->y,"img/sprite_medikit.png");
				liste_add_last(j->medikit,(void*)medikit);
			}
			int chance_munition = rand()%32;
			if(chance_munition == 0){
				Munition* munition = munition_init(((Projectile*)(e->object))->x,((Projectile*)(e->object))->y,"img/sprite_munition.png");
				liste_add_last(j->munition,(void*)munition);
			}
			projectile_dest((Projectile*)(e->object));
			liste_supprime(j->projectiles,e);
		}else{
			projectile_draw((Projectile*)(e->object),j);
			if(!menu_est_actif(((Jeu*)jeu)->menu)){
				projectile_tire((Projectile*)(e->object));
			}
		}
	}


}

int jeu_fin_jeu(Jeu* jeu){

	if(jeu->map->type_map == 1){
		if(jeu->ennemis->size == 0 && jeu->boss->size ==0)
			return 1;
	}else{
		if(jeu->perso->x > (jeu->map->w*LENGTH_SPRITE - 10*LENGTH_SPRITE)){
			return 1;
		}
	}
	return 0;

}


