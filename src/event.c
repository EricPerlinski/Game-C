#include "event.h"

Event* event_init(Periph p){
	Event* ev = (Event*)malloc(sizeof(Event));
	ev->gauche=false;
	ev->droit=false;
	ev->haut=false;
	ev->bas=false;
	ev->saut=false;
	ev->menu=false;
	ev->weapon_change=false;
	ev->son=false;
	ev->change=false;
	ev->ok=false;

	ev->x=0;
	ev->y=0;
	ev->clic_gauche=false;	

	ev->periph=p;
	if(ev->periph>=EVENT_PERIPH_COUNT || ev->periph<0){
		ev->periph=EVENT_PERIPH_CLAVIER;
	}
	if(ev->periph == EVENT_PERIPH_XBOX){
		ev->joy=al_get_joystick(0);
		if(!ev->joy){
			ev->periph=EVENT_PERIPH_CLAVIER;
		}
	}



	return ev;
}

void event_dest(Event* ev){
	free(ev);
}



/* 
Permet de recupere toutes les touches/deplacement fait par l'utilisateur
Avec la gestion du clavier et de la manette XBox
*/

void event_update(Event* ev, ALLEGRO_EVENT* al_ev){
	switch(ev->periph){

		case EVENT_PERIPH_CLAVIER:

		if(al_ev->type == ALLEGRO_EVENT_KEY_DOWN) {
			ev->change=true;
			switch(al_ev->keyboard.keycode) {
				case ALLEGRO_KEY_SPACE:
				ev->saut = true;
				break;

				case ALLEGRO_KEY_UP:
				ev->haut = true;
				break;

				case ALLEGRO_KEY_DOWN:
				ev->bas = true;
				break;

				case ALLEGRO_KEY_S:
				ev->bas = true;
				break;

				case ALLEGRO_KEY_Q: 
				ev->gauche = true;
				break;

				case ALLEGRO_KEY_D:
				ev->droit = true;
				break;

				case ALLEGRO_KEY_ESCAPE:
				ev->menu = true;
				break;

				case ALLEGRO_KEY_P:
				ev->son = true;
				break;

				case ALLEGRO_KEY_ENTER:
				ev->ok = true;
				break;

				case ALLEGRO_KEY_1:
				ev->weapon_change = true;
				break;
			}
		}else if(al_ev->type == ALLEGRO_EVENT_KEY_UP) {
			ev->change=true;
			switch(al_ev->keyboard.keycode) {
				case ALLEGRO_KEY_SPACE:
				ev->saut = false;
				break;

				case ALLEGRO_KEY_UP:
				ev->haut = false;
				break;

				case ALLEGRO_KEY_DOWN:
				ev->bas = false;
				break;

				case ALLEGRO_KEY_Q: 
				ev->gauche = false;
				break;

				case ALLEGRO_KEY_D:
				ev->droit = false;
				break;

				case ALLEGRO_KEY_P:
				ev->son = false;
				break;

				case ALLEGRO_KEY_ENTER:
				ev->ok = false;
				break;

				case ALLEGRO_KEY_1:
				ev->weapon_change = false;
				break;
			}
		}

		else if(al_ev->type == ALLEGRO_EVENT_MOUSE_AXES ){
			ev->x = al_ev->mouse.x;
			ev->y = al_ev->mouse.y;
		}
		else if(al_ev->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			switch(al_ev->mouse.button){
				case 1 : 
				ev->clic_gauche=true;
			}
		}else if(al_ev->type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			switch(al_ev->mouse.button){
				case 1 : 
				ev->clic_gauche=false;
			}
		}

		break;



		case EVENT_PERIPH_XBOX:
		if(al_ev->type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN){
			ev->change=true;
			switch (al_ev->joystick.button) {
				case 0:
				ev->ok=true;
				break;
				case 1 : 
				ev->weapon_change=true;
				break;
				case 3 :
				ev->son=true;
				break;
				case 7:
				ev->menu = true;
				break;
				case 4 :
				ev->saut=true;
				break;
				case 5:
				ev->clic_gauche = true;
				break;
				case 11:
				ev->gauche = true;
				break;
				case 12:
				ev->droit = true;
				break;
				case 13:
				ev->haut = true;
				break;
				case 14:
				ev->bas = true;
				break;
			}
		}else if(al_ev->type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP){
			ev->change=true;
			switch (al_ev->joystick.button) {
				case 0:
				ev->ok=false;
				break;
				case 1 : 
				ev->weapon_change=false;
				break;
				case 3 :
				ev->son=false;
				break;
				case 7:
				ev->menu = false;
				break;
				case 4 :
				ev->saut=false;
				break;
				case 5:
				ev->clic_gauche = false;
				break;
				case 11:
				ev->gauche = false;
				break;
				case 12:
				ev->droit =false;
				break;
				case 13:
				ev->haut = false;
				break;
				case 14:
				ev->bas = false;
				break;
			}
		}else if(al_ev->type == ALLEGRO_EVENT_JOYSTICK_AXIS){
			float pos = al_ev->joystick.pos;
			ev->change=true;
			switch (al_ev->joystick.stick) {
				case 0:
				if (al_ev->joystick.axis == 0){
					if(pos<0.3 && pos > -0.3){
						ev->gauche=false;
						ev->droit=false;
					}else if(pos>=0.3){
						ev->droit=true;
						ev->gauche=false;
					}else{
						ev->gauche=true;
						ev->droit=false;
					}
				}else if (al_ev->joystick.axis == 1){
					if(pos<0.3 && pos > -0.3){
						ev->haut=false;
						ev->bas=false;
					}else if(pos>=0.3){
						ev->bas=true;
						ev->haut=false;
					}else{
						ev->haut=true;
						ev->bas=false;
					}
				}
				break;
			}
		}


		break;
	}


}



/* Prmet d'emuler la position de la souris avec le joystick de la manette*/
void event_update_x_y(Event* ev, int vitesse){
	if(ev->periph == EVENT_PERIPH_XBOX){
		ALLEGRO_JOYSTICK_STATE ret_state;
		al_get_joystick_state(ev->joy, &ret_state);

		float axex = ret_state.stick[0].axis[4];
		float axey = ret_state.stick[0].axis[6];

		if(axex>0.3){
			ev->x+=vitesse;
		}else if(axex<-0.3){
			ev->x-=vitesse;
		}

		if(axey>0.3){
			ev->y+=vitesse;
		}else if(axey<-0.3){
			ev->y-=vitesse;
		}
	}
}


void event_toggle_periph(Event* ev){
	ev->periph = (ev->periph+1)%EVENT_PERIPH_COUNT;
	if(ev->periph == EVENT_PERIPH_XBOX){
		al_reconfigure_joysticks();
		ev->joy = al_get_joystick(0);
		if(!ev->joy){
			ev->periph=EVENT_PERIPH_CLAVIER;
		}
		
		
	}
}


void event_reset(Event* ev){
/*
ev->gauche=false;
ev->droit=false;
ev->haut=false;
ev->bas=false;
*/
ev->saut=false;
ev->menu=false;
ev->son=false;
}

Periph event_get_periph(Event* ev){
	return ev->periph;
}



bool event_get_gauche(Event* ev){
	return ev->gauche;
}

bool event_get_droit(Event* ev){
	return ev->droit;
}



bool event_get_saut(Event* ev){
	return ev->saut;
}

bool event_get_weapon_change(Event* ev){
	return ev->weapon_change;
}

bool event_get_menu(Event* ev){
	return ev->menu;
}

bool event_get_son(Event* ev){
	return ev->son;
}



bool event_get_clic_gauche(Event* ev){
	return ev->clic_gauche;
}

int event_get_x(Event* ev){
	return ev->x;
}

int event_get_y(Event* ev){
	return ev->y;
}



/* 
les fonctions de type event_get_XXX_court
utilisent une petite astuce pour la sauvegarde d'état
cela permet de ne pas avoir le menu par exemple defiler trop vite
Tant que la touche n'a pas été relaché, le get retourne faux
*/
bool event_get_weapon_change_court(Event* ev){
	static bool actif=false;
	if(actif){
		if(!ev->weapon_change){
			actif=false;
		}
		return false;
	}else if(ev->weapon_change && !actif){
		actif = true;
		return true;
	}
	return false;
}

bool event_get_clic_gauche_court(Event* ev){
	static bool actif=false;
	if(actif){
		if(!ev->clic_gauche){
			actif=false;
		}
		return false;
	}else if(ev->clic_gauche && !actif){
		actif = true;
		return true;
	}
	return false;
}
bool event_get_haut_court(Event* ev){
	static bool actif=false;
	if(actif){
		if(!ev->haut){
			actif=false;
		}
		return false;
	}else if(ev->haut && !actif){
		actif = true;
		return true;
	}
	return false;
}

bool event_get_bas_court(Event* ev){
	static bool actif=false;
	if(actif){
		if(!ev->bas){
			actif=false;
		}
		return false;
	}else if(ev->bas && !actif){
		actif = true;
		return true;
	}
	return false;
}

bool event_get_ok_court(Event* ev){
	static bool actif=false;
	if(actif){
		if(!ev->ok){
			actif=false;
		}
		return false;
	}else if(ev->ok && !actif){
		actif = true;
		return true;
	}
	return false;
}


bool event_get_menu_court(Event* ev){
	static bool actif=false;
	if(actif){
		if(!ev->menu){
			actif=false;
		}
		return false;
	}else if(ev->menu && !actif){
		actif = true;
		return true;
	}
	return false;
}




