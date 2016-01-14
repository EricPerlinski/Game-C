#include "projectile.h"

Projectile* projectile_init(Type type, int x0, int y0, int x1, int y1, int xmap, int ymap){
	Projectile* p = (Projectile*)malloc(sizeof(Projectile));
	p->type=type;

	/* switch sur type */
	
	p->x0=x0;
	p->y0=y0;
	p->vitesse=PROJECTILE_VITESSE;

	x1 = x1+xmap;
	y1 = y1+ymap;


	if(p->type == TYPE_BALLE){
		p->puissance=PROJECTILE_PUISSANCE;
		float hyp= sqrt((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0)) ;
		float adj=x1-x0;
		float opp=y1-y0;

		float vcos = acos(adj/hyp);
		float vsin = asin(opp/hyp);

		p->rotation= vcos;
		if(vsin<0){
			p->rotation*=-1;
		}
		p->x = x0;
		p->y = y0;
		p->x = p->x + cos(p->rotation)*32;
		p->y = p->y + sin(p->rotation)*32;
	}else if(p->type==TYPE_GRENADE){
		p->puissance=PROJECTILE_PUISSANCE_GRENADE;
		p->x_centre=x0+((x1-x0)/2);
		p->y_centre=y0;
		p->x=x0;
		p->y=y0;
		p->x0=x0;
		p->y0=y0;
		p->tombe=false;
		if(p->x_centre<p->x0){
			p->rotation=M_PI*2;
		}else{
			p->rotation=M_PI;
		}
		p->dist = sqrt(abs(p->x - p->x_centre)*abs(p->x - p->x_centre)+abs(p->y - p->y_centre)*abs(p->y - p->y_centre));
	}
	return p;
}

Projectile* projectile_boss_init(Type type, int x0, int y0, int x1, int y1, int xmap, int ymap){
	Projectile* p = (Projectile*)malloc(sizeof(Projectile));
	p->type=type;

	/* switch sur type */
	p->puissance=PROJECTILE_PUISSANCE;
	p->x0=x0;
	p->y0=y0;
	p->vitesse=PROJECTILE_VITESSE;

	x1 = x1+xmap;
	y1 = y1+ymap;

	float hyp= sqrt((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0)) ;
	float adj=x1-x0;
	float opp=y1-y0;

	float vcos = acos(adj/hyp);
	float vsin = asin(opp/hyp);
	
	p->rotation= vcos;
	if(vsin<0){
		p->rotation*=-1;
	}
	p->x = x0;
	p->y = y0;
	p->x = p->x + cos(p->rotation)*100;
	p->y = p->y + sin(p->rotation)*100;
	return p;
}


void projectile_dest(void* proj){
	Projectile* p = (Projectile*)proj;
	free(p);
}


void projectile_tire(Projectile* proj){
	if(proj->type==TYPE_BALLE){
		proj->x = proj->x + cos(proj->rotation)*PROJECTILE_VITESSE;
		proj->y = proj->y + sin(proj->rotation)*PROJECTILE_VITESSE;
	}else if(proj->type==TYPE_GRENADE){
		if(proj->rotation <= 2*M_PI+0.2 &&  proj->rotation >= M_PI && !proj->tombe){
			if(M_PI/(70/PROJECTILE_VITESSE_GRENADE)<=2*M_PI/180 && false){
				proj->tombe=true;
			}else{
				if(proj->x_centre<proj->x0){
					proj->rotation-=M_PI/(70/PROJECTILE_VITESSE_GRENADE);
				}else{
					proj->rotation+=M_PI/(70/PROJECTILE_VITESSE_GRENADE);
				}

				proj->x = proj->x_centre+cos(proj->rotation)*proj->dist;
				proj->y = proj->y_centre+sin(proj->rotation)*proj->dist;
			}
		}else{
			proj->tombe=true;
			proj->y+=PROJECTILE_VITESSE_GRENADE ;
		}


	}
}

void projectile_draw(Projectile* proj,Jeu* jeu){
	al_draw_filled_circle(proj->x-jeu->map->x,proj->y-jeu->map->y,3,al_map_rgb(94,94,94));
	
}


int projectile_degat(Projectile* proj){
	return proj->puissance;
}


bool projectile_collision_map(Projectile* proj, Map* map){
	if(proj->x<0 || proj->x > map->w*LENGTH_SPRITE || proj->y<0 || proj->y > map->h*LENGTH_SPRITE){
		if(proj->type==TYPE_GRENADE){
			if(proj->y<=0){
				proj->y=0;
				return false;
			}
		}
		return true;
	}
	int xpro = proj->x/LENGTH_SPRITE;
	int ypro = proj->y/LENGTH_SPRITE;
	if(map->map[ypro][xpro]->transparent == 0 ){
		return true;
	}
	return false;
}

bool projectile_collision_perso(Projectile* proj, Personnage* perso){
	if(proj->type==TYPE_GRENADE){
		if( (  abs(proj->x-proj->x0)>40 ) && (proj->x > perso->x) &&  (proj->x < perso->x + PERSO_LENGTH_SPRITE) && (proj->y > perso->y) && (proj->y < perso->y + PERSO_HEIGHT_SPRITE)){
			return true;
		}else{
			return false;
		}

	}else{
		if((proj->x > perso->x) &&  (proj->x < perso->x + PERSO_LENGTH_SPRITE) && (proj->y > perso->y) && (proj->y < perso->y + PERSO_HEIGHT_SPRITE)){
			return true;
		}else{
			return false;
		}
	}
}

bool projectile_collision_ennemis(Projectile* proj, List* ennemis){
	if(ennemis->size!=0){
		bool boul = true;
		Elem* e = ennemis->tete;
		while(liste_a_un_suivant(ennemis,e) || (boul)){
			if(boul){
				boul = false;
				e = ennemis->tete;
			}else{
				e = liste_suivant(ennemis,e);
			}

			if(projectile_collision_perso(proj,((Ennemi*)(e->object))->bot)){
				perso_touche(((Ennemi*)(e->object))->bot,proj->puissance);
				return true;
			}	
		}
	}
	return false;
}

bool projectile_collision_boss(Projectile* proj, List* boss){
	if(boss->size!=0){
		bool boul = true;
		Elem* e = boss->tete;
		while(liste_a_un_suivant(boss,e) || (boul)){
			if(boul){
				boul = false;
				e = boss->tete;
			}else{
				e = liste_suivant(boss,e);
			}

			if((proj->x > ((Boss*)(e->object))->x) &&  (proj->x <  ((Boss*)(e->object))->x + BOSS_LENGTH_SPRITE) && (proj->y >  ((Boss*)(e->object))->y) && (proj->y <  ((Boss*)(e->object))->y + BOSS_HEIGHT_SPRITE)){
				boss_touche((Boss*)(e->object),proj->puissance);
				return true;
			}	
		}
	}
	return false;
}





