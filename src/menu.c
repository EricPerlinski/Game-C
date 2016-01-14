#include "menu.h"

Menu* menu_init(){
	Menu* m = (Menu*)malloc(sizeof(Menu));



	m->liste[MENU_JOUER] = "Jouer";
	m->liste[MENU_CH_PERIPH] = "Peripherique";
	m->liste[MENU_CREDIT] = "Credits";
	m->liste[MENU_QUITTER] = "Quitter";
	m->actif=false;
	m->selected=MENU_JOUER;
	m->menu_actif=MENU_NB_MENU;
	return m;
}


void menu_dest(Menu* menu){
	free(menu);
}


void menu_draw(Menu* menu, Jeu* jeu){
	int x=400;
	int y=100;
	int pas= 30;
	int i=0;
	Periph per = event_get_periph(jeu->ev);
	if(menu->menu_actif==MENU_CREDIT){
		menu_affiche_credits(menu,jeu);
		return;
	}


	al_draw_text(jeu->font,al_map_rgb(0,0,0), x,y-40,ALLEGRO_ALIGN_CENTRE,"MENU" );
	ALLEGRO_COLOR color;
	for(i=0;i<MENU_NB_MENU;i++){
		if(i==menu->selected){
			color = al_map_rgb(255,0,0);
		}else{
			color = al_map_rgb(0,0,0);
		}

		al_draw_text(jeu->font,color, x,y+(i*pas),ALLEGRO_ALIGN_CENTRE,menu->liste[i] );
		
		if(i==MENU_CH_PERIPH){
			char* p;
			switch(per){
				case EVENT_PERIPH_CLAVIER :
				p = "Clavier";
				break;
				case EVENT_PERIPH_XBOX :
				p = "XBox";
				break;
				case EVENT_PERIPH_COUNT:
				break;
			}
			al_draw_text(jeu->font,color,x+200,y+(i*pas),ALLEGRO_ALIGN_LEFT,p);
		}

	}
}

void menu_afficher(Menu* menu){
	menu->actif=true;
}

void menu_cacher(Menu* menu){
	menu->actif=false;
}

void menu_descendre(Menu* menu){
	if(menu->selected<MENU_NB_MENU-1){
		menu->selected++;
	}
}

void menu_monter(Menu* menu){
	if(menu->selected>0){
		menu->selected--;
	}
}

Menu_action menu_selection(Menu* menu){
	return menu->selected;
}

bool menu_est_actif(Menu* m){
	return m->actif;
}

void menu_toggle(Menu* menu){
	if(menu->actif){
		menu_cacher(menu);
	}else{
		menu_afficher(menu);
	}
}


void menu_affiche_mort(Menu* menu, Jeu* jeu){
	al_draw_text(jeu->font,al_map_rgb(0,0,0), 400,100,ALLEGRO_ALIGN_CENTRE,"GAME OVER" );
	al_draw_text(jeu->font,al_map_rgb(0,0,0), 400,140,ALLEGRO_ALIGN_CENTRE,"Entrer pour recommencer" );

}

void menu_activer(Menu* menu,Jeu* jeu){
	
	switch(menu->selected){
		case MENU_JOUER :
		menu_cacher(menu);
		break;
		case MENU_CH_PERIPH :
		event_toggle_periph(jeu->ev);
		break; 
		case MENU_CREDIT :
		menu->menu_actif=menu->selected;
		menu_affiche_credits(jeu->menu,jeu);
		break;
		case MENU_QUITTER : 
		jeu->boucle_jeu=false;
		break;
		case MENU_NB_MENU : 
		break;
	}
}

void menu_affiche_credits(Menu* menu, Jeu* jeu){
	al_clear_to_color(al_map_rgb(255,255,255));
	int x=50;
	int pas=30;
	al_draw_text(jeu->font,al_map_rgb(0,0,0), 400,x,ALLEGRO_ALIGN_CENTRE,"Developeurs : " );
	al_draw_text(jeu->font,al_map_rgb(0,0,0), 400,x+=pas,ALLEGRO_ALIGN_CENTRE,"Valentin GIANNINI" );
	al_draw_text(jeu->font,al_map_rgb(0,0,0), 400,x+=pas,ALLEGRO_ALIGN_CENTRE,"Eric PERLINSKI" );
	al_draw_text(jeu->font,al_map_rgb(0,0,0), 400,x+=pas,ALLEGRO_ALIGN_CENTRE,"Sprite map : " );
	al_draw_text(jeu->font,al_map_rgb(0,0,0), 400,x+=pas,ALLEGRO_ALIGN_CENTRE,"Lars Doucet, Sean Choate, and Megan Bednarz." );
	al_draw_text(jeu->font,al_map_rgb(0,0,0), 400,x+=pas,ALLEGRO_ALIGN_CENTRE,"Sprite personnage" );
	al_draw_text(jeu->font,al_map_rgb(0,0,0), 400,x+=pas,ALLEGRO_ALIGN_CENTRE,"?UNKNOWN?" );
	al_draw_text(jeu->font,al_map_rgb(0,0,0), 400,x+=pas,ALLEGRO_ALIGN_CENTRE,"Musique" );
	al_draw_text(jeu->font,al_map_rgb(0,0,0), 400,x+=pas,ALLEGRO_ALIGN_CENTRE,"a venir" );
	al_flip_display();
}

void menu_retour(Menu* menu){
	if(menu->menu_actif!=MENU_NB_MENU){
		menu->menu_actif=MENU_NB_MENU;
	}else{
		menu_toggle(menu);
	}
}


bool menu_root(Menu* menu){
	return menu->menu_actif==MENU_NB_MENU;
}
