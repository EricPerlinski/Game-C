#include "map.h"

/* 
Fonction pour initialiser une Map
sprite_src : chemin vers le BITMAP pour le sprite
map_src : chemein vers le fichier texte decrivant la map
w : largeur de la map
h : hauteur de la map
*/

Map* map_init(char* map_src, int w,int h){

	Map* map = (Map*)malloc(sizeof(Map));
	map->x=0;
	map->y=0;
	map->dx=w;
	map->dy=h;

	map->frame_x=0;
	map->frame_y=0;
	map->frame_dx=map->dx;
	map->frame_dy=map->dy;

	map->background=NULL;

	/**********************/
	/* PARSAGE A DEPLACER */
	/**********************/
	return map;
}


/*
Fonction qui permet de remplir une ligne de la map avec la chaine du fichier map
map : la map cree precedement
ligne : numero de la ligne en cours de remplissage
chaine : chaine de la ligne numero ligne pcontenant la description de la ligne de la map
*/
void map_fill(Map* map, int ligne, char* chaine){
	int i=0;
	while(i < map->w){
		int num = atoi(chaine);
		/*int dec = (int)(strchr(chaine,':')-chaine)+1;*/
		/*int flags = atoi(chaine+dec);*/

		map->map[ligne][i] = tile_init(num,i,ligne/*,flags*/);

		i++;
		if(i<map->w){
			chaine = (strchr(chaine, ','))+1;
		}
	}
}

/*
fonction pour detruire la Tile
*/
void map_dest(Map* map){
	int i,j;
	for(i=0;i<map->h;i++){
		for(j=0;j<map->w;j++){
			tile_dest(map->map[i][j]);
		}
		free(map->map[i]);
	}
	al_destroy_bitmap(map->sprite);
	al_destroy_bitmap(map->background);
	al_destroy_sample(map->musique);
	free(map);
}


/*
fonction pour dessiner la carte sur l ecran
*/
void map_draw(Map* map){
	int i,j;
	int x_case = map->x/LENGTH_SPRITE;
	int y_case = map->y/LENGTH_SPRITE;
	int dx_case = map->dx/LENGTH_SPRITE;
	int dy_case = map->dy/LENGTH_SPRITE;


	/* on verifie avant pour ne pas afficher la map sur des endroit non existant*/
	if(dx_case<0){
		dx_case=0;
	}
	if(map->x+map->dx > map->w*LENGTH_SPRITE){
		x_case=map->w-dx_case;
	}else if(map->x<0){
		x_case=0;
	}

	if(dy_case<0){
		dy_case=0;
	}
	if(map->y+map->dy > map->h*LENGTH_SPRITE){
		y_case=map->h-dy_case;
	}else if(map->y<0){
		y_case=0;
	}

	/* dessin de l arriere plan */
	if(map->background!=NULL){
		al_draw_bitmap_region(map->background,250,200,map->w*LENGTH_SPRITE,map->h*LENGTH_SPRITE,0,0,0);
	}

	/* On dessine maintenant toutes les Tile */
	for(i=0;i<dx_case+(map->x<map->dx?1:0);i++){
		for(j=0;j<dy_case+(y_case+dy_case<map->h?1:0);j++){
			if(map->map[j+y_case][i+x_case]->num!=0){
				tile_draw(map->map[j+y_case][i+x_case],map->sprite,i-((map->x%LENGTH_SPRITE)/(float)LENGTH_SPRITE),j-((map->y%LENGTH_SPRITE)/(float)LENGTH_SPRITE));
			}
		}
	}


	/* Permet de voir le cadre 'suiveur' du personnage */
	/*
	int x1 = map->frame_x;
	int x2 = map->frame_x+map->frame_dx;
	int y1 = map->frame_y;
	int y2 = map->frame_y+map->frame_dy;
	
	al_draw_line(x1,y1,x1,y2,al_map_rgb(125,125,125),1);
	al_draw_line(x1,y1,x2,y1,al_map_rgb(125,125,125),1);
	al_draw_line(x2,y2,x1,y2,al_map_rgb(125,125,125),1);
	al_draw_line(x2,y2,x2,y1,al_map_rgb(125,125,125),1);
	*/
}



/*
Fonction permettant de deplacer la vue de maniere horizontale
map : map a bouger
val : valeur du deplacament a faire (>0 vers la doite, sinon vers la gauche)
*/
void map_mouv_horiz(Map* map, int val){
	map->x+=val;
	if(map->x+map->dx > map->w*LENGTH_SPRITE){
		map->x = map->w*16 - map->dx;
	}else if(map->x<0){
		map->x=0;
	}
}

/*
Fonction permettant de deplacer la vue de maniere verticale
map : map a bouger
val : valeur du deplacament a faire (>0 vers le bas, sinon vers le haut)
*/
void map_mouv_vert(Map* map, int val){
	map->y+=val;
	if(map->y+map->dy > map->h*LENGTH_SPRITE){
		map->y = map->h*16 - map->dy;
	}else if(map->y<0){
		map->y=0;
	}
}


void map_play_musique(Map* map){
	al_play_sample(map->musique, 0.1, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
	map->is_playing=true;
}


void map_stop_musique(Map* map){
	al_stop_samples();
	map->is_playing=false;
}

void map_toggle_musique(Map* map){
	if(al_is_audio_installed() && map->musique!=NULL){
		if(map->is_playing){
			map_stop_musique(map);
		}else{
			map_play_musique(map);
		}
	}
}


void map_set_frame(Map* map, int x, int y, int dx, int dy){
	map->frame_x=x;
	map->frame_y=y;
	map->frame_dx=dx;
	map->frame_dy=dy;
}


void map_update_frame_player(Map* map, int x, int y){
	if(x-map->x < map->frame_x){
		map_mouv_horiz(map,(x-map->x)-map->frame_x);
	}
	if(x-map->x > map->frame_x+map->frame_dx){
		map_mouv_horiz(map,(x-map->x)-(map->frame_x+map->frame_dx));
	}
	if(y-map->y < map->frame_y){
		map_mouv_vert(map,(y-map->y)-map->frame_y);
	}
	if(y-map->y > map->frame_y+map->frame_dy){
		map_mouv_vert(map,(y-map->y)-(map->frame_y+map->frame_dy));
	}
}



void map_aleatoire(char* dest, int w, int h){
	int** map = (int**)malloc(sizeof(int*) * h);

	int i=0;
	int j=0;
	for(i=0;i<h;i++){
		map[i]=(int*)malloc(sizeof(int)*w);
	}

	FILE* file = fopen(dest,"w");
	fprintf(file, "#ennemies\n");

	srand(time(NULL));
	int sol = (h/3)+rand()%(h/3);
	int y_perso = (sol-5)*LENGTH_SPRITE;

	int range = sol-6;
	bool rangeB=false;
	bool eau=false;
	int prof_eau=5;
	bool petrol=false;
	bool pop_ennnemis=false;
	int dist_eau=0;
	int r=0;

	for(i=0;i<w;i++){

		r = rand()%RAND;
		for(j=0;j<h;j++){
			if(j==sol-6 && rangeB){
				map[j][i]=0;
			}else{
				if(j<sol){
					map[j][i]=0;
				}else if(j==sol){
					if(eau){
						map[j][i]=8;
					}else if(petrol){
						map[j][i]=11;
					}else{
						map[j][i]=102;
					}
				}else{
					if(eau){
						if(j<sol+prof_eau ){
							map[j][i]=30;
						}else if(j==sol+prof_eau){
							map[j][i]=33;
						}else{
							map[j][i]=14;
						}
					}else if(petrol){
						if(j<sol+prof_eau ){
							map[j][i]=38;
						}else if(j==sol+prof_eau){
							map[j][i]=41;
						}else{
							map[j][i]=14;
						}
					}else{
						map[j][i]=14;
					}
				}
			}
		}

		if(i%50==0 && i > 10){
			pop_ennnemis=true;
		}
		if(pop_ennnemis && !eau && !petrol){
			fprintf(file, "BASE1,%d,%d\n",i*LENGTH_SPRITE,(sol-5)*LENGTH_SPRITE);
			pop_ennnemis=false;
		}



		if(i%10 == 0){
			switch(r){

				case 0:
				if(sol>5 ){
					sol--;
					eau=false;
				}
				break;


				case 1:
				if(sol>6 && !eau){
					sol-=2;
					eau=false;
				}
				break;


				case 2:
				if(sol<h-7){
					sol++;
					if(eau){
						sol--;
						eau=false;
					}
				}
				break;


				case 3 :
				if(sol<h-8 && !eau){
					sol+=2;
					if(eau){
						sol-=2;;
						eau=false;
					}
				}
				break;


				case 4:
				if(!eau && i>100){
					eau=!eau;
				}
				break;


				case 5:
				if(sol<h-15 && !eau){
				sol+=8; /* Assassina aerien */
					
				}
				break;


				case 6 :
				if(i>100){
					petrol=true;
				}
				break;
				

				case 7:
				if(range<sol-15){
					range--;
				}
				break;
				

				case 8:
				if(range<sol-5){
					range++;
				}
				break;
				

				case 9:
				rangeB=!rangeB;
				break;
			}

		}else if(i%5==0){
			petrol=false;
		}
		if(eau && dist_eau==MAP_MAX_EAU){
			dist_eau=0;
			eau=false;
		}else{
			dist_eau++;
		}

	}

	fprintf(file, "BOSS,%d,%d\n", (i-100)*LENGTH_SPRITE, (sol-10)*LENGTH_SPRITE);



	/*fprintf(file, "BASE1,300,30\n");
	fprintf(file, "BASE1,600,30\n");
	fprintf(file, "BASE1,700,30\n");
	fprintf(file, "BOSS,900,30\n");
*/
	fprintf(file, "$perso\n");
	fprintf(file, "x:%d\n",80);
	fprintf(file, "y:%d\n",y_perso);

	fprintf(file, "@map\n");
	fprintf(file, "w:%d\n", w);
	fprintf(file, "h:%d\n", h);
	fprintf(file, "t:2\n");
	fprintf(file, "s:img/sprite_map.png\n");
	fprintf(file, "b:img/background.png\n");
	fprintf(file, "m:musique/musique.ogg\n");

	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			fprintf(file, "%d%c", map[i][j],(j<w-1?',':'\n'));
		}
	}

	fclose(file);
}
