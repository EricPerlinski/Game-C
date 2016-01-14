#include "main.h"

Game* game_init(){

	Game* game = (Game*)malloc(sizeof(Game));
	game->maps = liste_init();
	game->size = 0;
	game->success = false;
	game->current_map_i = 1;
	char* ligne = (char*)malloc(MAX_TILE_W*sizeof(char));
	FILE* src = fopen("maps.txt","r");
	if(src==NULL){
		fprintf(stderr, "Erreur lors du chargement du fichier maps\n");
		return NULL;
	}

	while(fgets(ligne,MAX_TILE_W,src)!= NULL){
		if(ligne[strlen(ligne)-1] == '\n'){
			ligne[strlen(ligne)-1] = '\0';
		}
		char* game_map_src = (char*)malloc(sizeof(ligne));
		strcpy(game_map_src,ligne);
		liste_add_last(game->maps,(void*)game_map_src);
		game->size += 1;
	}
	game->jeu = jeu_init((char*)(game->maps->tete->object));
	return game;
}

void game_next_map_test(Game* game){
	if(jeu_fin_jeu(game->jeu) == 1){
		int i = 1;
		Elem* map_i = game->maps->tete;
		while(i <= game->current_map_i){
			map_i = liste_suivant(game->maps,map_i);
			i++;
		}

		if(i == game->size +1){
			game->jeu->boucle_jeu = false;
			game->success = true;
		}else{
			ALLEGRO_SAMPLE* current_musique = game->jeu->map->musique;
			game->jeu = jeu_init((char*)(map_i->object));
			ALLEGRO_SAMPLE* next_musique = game->jeu->map->musique;
			if(current_musique!=next_musique){
				al_stop_samples();
				map_play_musique(game->jeu->map);
			}
			game->current_map_i += 1;
			char num_niveau[4];
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),300,150,ALLEGRO_ALIGN_LEFT,"STAGE CLEAR !");
			if(game->size == game->current_map_i){
				al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),280,190,ALLEGRO_ALIGN_LEFT,"Loading final stage");
			}else{
				al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),300,190,ALLEGRO_ALIGN_LEFT,"Loading level ");
				al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),440,190,ALLEGRO_ALIGN_LEFT,num_niveau);
			}
			al_flip_display();
			sleep(2);
			al_clear_to_color(al_map_rgb(0,0,0));
			al_flip_display();
			map_set_frame(game->jeu->map,DISPLAY_W/2-200,100,200,DISPLAY_H-300);	
		}
	}
}


int main(int argc, char **argv){

	if(argc==3){
		if(strcmp(argv[1],"rand")==0 && strcmp(argv[1],"map/maps.txt")!=0){
			map_aleatoire(argv[2], 1000,100);
			printf("map cree\n");
			return 0;
		}
	}

	/* Creation de l ecran */
	ALLEGRO_DISPLAY *display = NULL;
	/* Creation du timer */
	ALLEGRO_TIMER *timer;
	/* Creation de la liste d'evenement */
	ALLEGRO_EVENT_QUEUE *queue;
	bool redraw = true;
	/*bool boucle_jeu=true;*/


	/* Initialisation d allegro */
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	if(!al_init_image_addon()) {
		fprintf(stderr, "failed to initialize allegro image addon!\n");
		return -1;
	}
	if(!al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize allegro primitive addon!\n");
		return -1;
	}
	if(!al_install_mouse()) {
		fprintf(stderr, "failed to initialize allegro mouse !\n");
		return -1;
	}
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize allegro keyboard!\n");
		return -1;
	}
	if(!al_install_joystick()){
		fprintf(stderr, "failed to initialize allegro joystick!\n");
		return -1;
	}
	if(!al_install_audio()) {
		fprintf(stderr, "failed to initialize allegro audio!\n");
		/*return -1;*/
	}
	if(!al_init_acodec_addon()){
		fprintf(stderr, "failed to initialize allegro audio codec!\n");
		/*return -1;*/
	}
	if (!al_reserve_samples(2)){
		fprintf(stderr, "failed to initialize allegro reserve sample 1!\n");
		/*return -1;*/
	}

	al_init_font_addon();

	if(!al_init_ttf_addon()){
		fprintf(stderr, "failed to initialize allegro ttf addon!\n");
		return -1;
	}

	Game* game;
	/* Instantiation de l ecran */
	display = al_create_display(DISPLAY_W,DISPLAY_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	bool recommencer = true;
	while(recommencer){

		recommencer = false;
		game = game_init();

		al_hide_mouse_cursor(display);

		/* Instantiation de la liste d evenement */
		queue = al_create_event_queue();

		/* Instantiation du timer de jeu */
		timer = al_create_timer(1.0/FPS);
		al_start_timer(timer);

		/* Enregistrement des evenement */
		al_register_event_source(queue, al_get_keyboard_event_source());
		al_register_event_source(queue, al_get_mouse_event_source());
		al_register_event_source(queue, al_get_joystick_event_source());
		al_register_event_source(queue, al_get_display_event_source(display));
		al_register_event_source(queue, al_get_timer_event_source(timer));


		/* ECRAN TITRE */

		al_clear_to_color(al_map_rgb(0,0,0));
		al_draw_bitmap_region(game->jeu->map->background,250,200,game->jeu->map->w*LENGTH_SPRITE,game->jeu->map->h*LENGTH_SPRITE,0,0,0);
		al_draw_text(game->jeu->font_jeu,al_map_rgb(0,0,0),280,150,ALLEGRO_ALIGN_LEFT,"Titre du jeu");
		al_draw_text(game->jeu->font_jeu,al_map_rgb(0,0,0),280,190,ALLEGRO_ALIGN_LEFT,"Press enter");
		if(al_is_audio_installed()){
			ALLEGRO_SAMPLE* main_musique;
			main_musique = al_load_sample("musique/musique_titre.ogg");
			if (!main_musique){
				fprintf(stderr, "Erreur lors du chargement musique d'intro\n");
			}

			al_play_sample(main_musique, 0.1, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
		}
		al_flip_display();
		bool debut = false;
		Event* event_titre;
		ALLEGRO_EVENT event_titre_event;
		event_titre = event_init(EVENT_PERIPH_XBOX);
		while(debut != true){
			al_wait_for_event(queue, &event_titre_event);
			if(event_get_ok_court(event_titre)){
				debut = true;
			}
			event_update(event_titre,&event_titre_event);
		}

		if(al_is_audio_installed()){
			al_stop_samples();
		}
		/* Ecran de chargement du premier niveau */

		char num_niveau[4];
		sprintf(num_niveau,"%d",game->current_map_i);
		al_clear_to_color(al_map_rgb(0,0,0));
		if(game->size == game->current_map_i){
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),280,190,ALLEGRO_ALIGN_LEFT,"Loading final stage");
		}else{
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),300,190,ALLEGRO_ALIGN_LEFT,"Loading level ");
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),440,190,ALLEGRO_ALIGN_LEFT,num_niveau);
		}
		al_flip_display();
		sleep(2);
		al_clear_to_color(al_map_rgb(0,0,0));
		al_flip_display();

		/* Début de la boucle de jeu */
		game->jeu->ev->son = true;
		map_set_frame(game->jeu->map,DISPLAY_W/2-200,100,200,DISPLAY_H-300);	

		while(game->jeu->boucle_jeu){

			

			game_next_map_test(game);
			
			ALLEGRO_EVENT event;
			al_wait_for_event(queue, &event);

				/* On demande a quitter le jeu */
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE ){
				game->jeu->boucle_jeu=false;
			}
			event_update(game->jeu->ev,&event);

			/* Evenement timer */
			if(event.type == ALLEGRO_EVENT_TIMER){
				redraw=true;
			}

			/*
			if(event.type == ALLEGRO_EVENT_DISPLAY_RESIZE){
				al_acknowledge_resize(display);
				redraw = true;
			}
			*/
				/* affichage */

			if(redraw && al_is_event_queue_empty(queue)){
				redraw=false; 
				game->jeu->boucle_jeu = jeu_update_event(game->jeu);
				jeu_draw(game->jeu);
			}

		}



		/* Ecran de fin du jeu */
		if(game->success == true){
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),400,100,ALLEGRO_ALIGN_CENTER,"Félicitation, vous avez terminés les premiers niveaux du jeu !");
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),400,120,ALLEGRO_ALIGN_CENTER,"Ce jeu à été créé à l'occasion du premier projet de C de");
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),400,140,ALLEGRO_ALIGN_CENTER,"la promotion 2016 de Télécom Nancy");
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),400,160,ALLEGRO_ALIGN_CENTER,"par");
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),400,180,ALLEGRO_ALIGN_CENTER,"Giannini Valentin et Eric Perlinski");
			al_flip_display();
			sleep(5);
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),400,150,ALLEGRO_ALIGN_CENTER,"Appuyez sur [ESC] pour quitter, ou sur [Entrer] pour recommencer");
			al_flip_display();
		}else{
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),400,100,ALLEGRO_ALIGN_CENTER,"Vous avez épuisés toutes les vies de votre personnage !");
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),400,120,ALLEGRO_ALIGN_CENTER,"Ce jeu à été créé à l'occasion du premier projet de C de");
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),400,140,ALLEGRO_ALIGN_CENTER,"la promotion 2016 de Télécom Nancy");
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),400,160,ALLEGRO_ALIGN_CENTER,"par");
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),400,180,ALLEGRO_ALIGN_CENTER,"Gianini Valentin et Eric Perlinski");
			al_flip_display();
			sleep(5);
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_text(game->jeu->font_jeu,al_map_rgb(255,255,255),400,150,ALLEGRO_ALIGN_CENTER,"Appuyez sur [ESC] pour quitter, ou sur [Entrer] pour recommencer");
			al_flip_display();
		}
		/* TODO Chargement écran de fin */
		bool action = false;
		Event* event_credits;
		ALLEGRO_EVENT event_credits_event;
		
		event_credits = event_init(EVENT_PERIPH_XBOX);
		while(action != true){
			al_wait_for_event(queue, &event_credits_event);
			if(event_get_ok_court(event_credits)){
				action = true;
				recommencer = true;
			}else if (event_get_menu(event_credits)){
				action = true;
				recommencer = false;
			}
			event_update(event_credits,&event_credits_event);
		}

	}

	jeu_dest(game->jeu);
	al_destroy_display(display);

	return 0;
}



